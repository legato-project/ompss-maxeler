/*--------------------------------------------------------------------
  (C) Copyright 2006-2014 Barcelona Supercomputing Center
                          Centro Nacional de Supercomputacion

  This file is part of Mercurium C/C++ source-to-source compiler.

  See AUTHORS file in the top level directory for information
  regarding developers and contributors.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 3 of the License, or (at your option) any later version.

  Mercurium C/C++ source-to-source compiler is distributed in the hope
  that it will be useful, but WITHOUT ANY WARRANTY; without even the
  implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.  See the GNU Lesser General Public License for more
  details.

  You should have received a copy of the GNU Lesser General Public
  License along with Mercurium C/C++ source-to-source compiler; if
  not, write to the Free Software Foundation, Inc., 675 Mass Ave,
  Cambridge, MA 02139, USA.
--------------------------------------------------------------------*/

#include "tl-nanox-nodecl.hpp"
#include "tl-nanos.hpp"
#include "tl-nodecl-utils.hpp"
#include "tl-lowering-visitor.hpp"

#include "tl-omp-lowering-final-stmts-generator.hpp"
#include "tl-omp-lowering-utils.hpp"

#include "tl-compilerpipeline.hpp"
#include "codegen-phase.hpp"
#include "cxx-profile.h"
#include "cxx-driver-utils.h"

#include <stdio.h>
#include <errno.h>

namespace TL { namespace Nanox {

    class TaskCreationContextVisitor : public Nodecl::ExhaustiveVisitor<void>
    {
        private:
            class TargetDevicesVisitor : public Nodecl::ExhaustiveVisitor<void>
            {
                private:
                    ObjectList<std::string> _devices;

                public:
                    TargetDevicesVisitor() : _devices()
                    {}

                    void visit(const Nodecl::OmpSs::Implements& implements)
                    {
                        //TODO: Check if something has to be done here
                    }

                    void visit(const Nodecl::OmpSs::Target& target)
                    {
                        Nodecl::List devices = target.get_devices().as<Nodecl::List>();

                        for (Nodecl::List::iterator it = devices.begin();
                                it != devices.end();
                                it++)
                        {
                            std::string current_device = it->as<Nodecl::Text>().get_text();
                            _devices.append(current_device);
                        }
                    }

                    const ObjectList<std::string>& get_device_names() const
                    {
                        return _devices;
                    }
            };

            class SetTaskCreationContext : public Nodecl::ExhaustiveVisitor<void>
            {
                private:
                    const Nodecl::OmpSs::TaskCall& _context;
                    std::string                    _contextDeviceName;
                    size_t                         _count;

                public:
                    SetTaskCreationContext(const Nodecl::OmpSs::TaskCall& ctx) :
                            _context(ctx), _contextDeviceName("NULL"), _count(0)
                    {}

                    virtual void visit(const Nodecl::OmpSs::TaskCall& node)
                    {
                        if (_contextDeviceName == "NULL")
                        {
                            Nodecl::OpenMP::Task ctx_task = _context.as<Nodecl::OpenMP::Task>();
                            TargetDevicesVisitor ctx_devices_visitor;
                            ctx_devices_visitor.walk(ctx_task.get_environment());

                            ERROR_CONDITION(ctx_devices_visitor.get_device_names().size() > 1,
                                    "A child task cannot be created in a context with multiple targets.\n", 0);
                            _contextDeviceName = ctx_devices_visitor.get_device_names().at(0);
                        }
                        Nodecl::List task_environment = node.get_environment().as<Nodecl::List>();
                        task_environment.append(Nodecl::OmpSs::CreationCtx::make(_contextDeviceName));
                        node.get_environment().replace(task_environment);
                        ++_count;
                    }

                    virtual void visit(const Nodecl::FunctionCall& node)
                    {
                        Nodecl::NodeclBase called = node.get_called();
                        if (!called.has_symbol() || called.get_symbol().get_function_code().is_null())
                        {
                            return;
                        }
                        Nodecl::FunctionCode function_code =
                            called.get_symbol().get_function_code().as<Nodecl::FunctionCode>();
                        Nodecl::NodeclBase function_statements = function_code.get_statements();
                        walk(function_statements);
                    }

                    size_t get_num_inner_tasks() const
                    {
                        return _count;
                    }
            };

        public:

            virtual void visit(const Nodecl::OmpSs::TaskCall& node)
            {
                Nodecl::NodeclBase called = node.get_call().as<Nodecl::FunctionCall>().get_called();
                if (!called.has_symbol() || called.get_symbol().get_function_code().is_null())
                {
                    //The task is not in the same translation unit?
                    return;
                }

                //Set the creation context of inner task calls based on current task call
                Nodecl::FunctionCode function_code =
                    called.get_symbol().get_function_code().as<Nodecl::FunctionCode>();
                Nodecl::NodeclBase function_statements = function_code.get_statements();//.as<Nodecl::List>();
                // set_task_context_of_task_calls(function_statements);
                SetTaskCreationContext setter_visitor = SetTaskCreationContext(node);
                setter_visitor.walk(function_statements);

                //Set the number of inner tasks for the current task call
                std::stringstream tmp_str_num;
                tmp_str_num << setter_visitor.get_num_inner_tasks();
                Nodecl::List task_environment = node.get_environment().as<Nodecl::List>();
                task_environment.append(Nodecl::OmpSs::NumInnerTasks::make(tmp_str_num.str()));
                node.get_environment().replace(task_environment);
            }
    };

    Lowering::Lowering()
        : _ancillary_file(NULL),
        _static_weak_symbols(false),
        _ompss_mode(false),
        _instrumentation_enabled(false),
        _nanos_debug_enabled(false),
        _final_clause_transformation_disabled(false),
        _firstprivates_always_references(false)
    {
        set_phase_name("Nanos++ lowering");
        set_phase_description("This phase lowers from Mercurium parallel IR into real code involving Nanos++ runtime interface");

        register_parameter("omp_dry_run",
                "Disables OpenMP transformation",
                _openmp_dry_run,
                "0");

        register_parameter("weaks_as_statics",
                "Some compilers do not allow weak symbols be defined in specific sections. Make them static instead",
                _static_weak_symbols_str,
                "0").connect(std::bind(&Lowering::set_weaks_as_statics, this, std::placeholders::_1));

        register_parameter("ompss_mode",
                "Enables OmpSs semantics instead of OpenMP semantics",
                _ompss_mode_str,
                "0").connect(std::bind(&Lowering::set_ompss_mode, this, std::placeholders::_1));

        register_parameter("instrument",
                "Enables Nanos++ instrumentation",
                _instrumentation_str,
                "0").connect(std::bind(&Lowering::set_instrumentation, this, std::placeholders::_1));

        register_parameter("nanos-debug",
                "Enables Nanos++ debugging features",
                _nanos_debug_str,
                "0").connect(std::bind(&Lowering::set_nanos_debug, this, std::placeholders::_1));

        register_parameter("disable_final_clause_transformation",
                "Disables the OpenMP/OmpSs transformation of the 'final' clause",
                _final_clause_transformation_str,
                "0").connect(std::bind(&Lowering::set_disable_final_clause_transformation, this, std::placeholders::_1));

        register_parameter("firstprivates_always_references",
                "For C/C++, passes firstprivates always by reference",
                _firstprivates_always_references_str,
                "0").connect(std::bind(&Lowering::set_firstprivates_always_references, this, std::placeholders::_1));
    }

    void Lowering::run(DTO& dto)
    {
        if (_openmp_dry_run != "0")
        {
            std::cerr << "Not running Nanos++ phase (by request)" << std::endl;
            return;
        }

        std::cerr << "Nanos++ phase" << std::endl;

        Nodecl::NodeclBase n = *std::static_pointer_cast<Nodecl::NodeclBase>(dto["nodecl"]);
        FORTRAN_LANGUAGE()
        {
            Nodecl::NodeclBase api_tree = TL::OpenMP::Lowering::Utils::Fortran::preprocess_api(n);

            Source::source_language = SourceLanguage::C;
            Nanos::Interface interface;
            interface.walk(api_tree);
            Source::source_language = SourceLanguage::Current;
        }


        TaskCreationContextVisitor task_context_visitor;
        task_context_visitor.walk(n);

        TL::OpenMP::Lowering::FinalStmtsGenerator final_generator(_ompss_mode, "omp_in_final");
        // If the final clause transformation is disabled we shouldn't generate the final stmts
        if (!_final_clause_transformation_disabled)
            final_generator.walk(n);

        LoweringVisitor lowering_visitor(
                this,
                std::static_pointer_cast<TL::OmpSs::FunctionTaskSet>(dto["openmp_task_info"]),
                final_generator.get_final_stmts());
        lowering_visitor.walk(n);

        finalize_phase(n);
    }

    void Lowering::finalize_phase(Nodecl::NodeclBase global_node)
    {
        emit_nanos_requirements(global_node);
    }

    void Lowering::set_weaks_as_statics(const std::string& str)
    {
        parse_boolean_option("set_weaks_as_statics", str, _static_weak_symbols, "Assuming false.");
    }

    void Lowering::set_ompss_mode(const std::string& str)
    {
        parse_boolean_option("ompss_mode", str, _ompss_mode, "Assuming false.");
    }

    void Lowering::set_instrumentation(const std::string& str)
    {
        parse_boolean_option("instrument", str, _instrumentation_enabled, "Assuming false.");
    }

    void Lowering::set_nanos_debug(const std::string& str)
    {
        parse_boolean_option("nanos-debug", str, _nanos_debug_enabled, "Assuming false.");
    }

    void Lowering::set_disable_final_clause_transformation(const std::string& str)
    {
        parse_boolean_option("disable_final_clause_transformation", str, _final_clause_transformation_disabled, "Assuming false.");
    }

    void Lowering::set_firstprivates_always_references(const std::string& str)
    {
        parse_boolean_option("firstprivates_always_references", str, _firstprivates_always_references, "Assuming false.");
    }

    bool Lowering::nanos_debug_enabled() const
    {
        return _nanos_debug_enabled;
    }

    bool Lowering::instrumentation_enabled() const
    {
        return _instrumentation_enabled;
    }

    bool Lowering::in_ompss_mode() const
    {
        return _ompss_mode;
    }

    bool Lowering::final_clause_transformation_disabled() const
    {
        return _final_clause_transformation_disabled;
    }

    bool Lowering::firstprivates_always_by_reference() const
    {
        return _firstprivates_always_references;
    }

    void Lowering::emit_nanos_requirements(Nodecl::NodeclBase global_node)
    {
        Source src;
        if (Nanos::Version::interface_is_at_least("master", 5040))
        {
            Source weak_fun_defs;
            bool at_least_one_weak_fun_def = false;

            if (seen_task_with_priorities)
            {
                at_least_one_weak_fun_def = true;
                weak_fun_defs  <<  "__attribute__((weak)) void nanos_needs_priorities_fun(void) {}";
            }

            if (seen_opencl_task)
            {
                at_least_one_weak_fun_def = true;
                weak_fun_defs  <<  "__attribute__((weak)) void nanos_needs_opencl_fun(void) {}";
            }

            if (seen_cuda_task)
            {
                at_least_one_weak_fun_def = true;
                weak_fun_defs  <<  "__attribute__((weak)) void nanos_needs_cuda_fun(void) {}";
            }

            if (seen_gpu_cublas_handle)
            {
                at_least_one_weak_fun_def = true;
                weak_fun_defs  <<  "__attribute__((weak)) void nanos_needs_cublas_fun(void) {}";
            }

            if (seen_fpga_task)
            {
                at_least_one_weak_fun_def = true;
                weak_fun_defs  <<  "__attribute__((weak)) void nanos_needs_fpga_fun(void) {}";
            }

            if (at_least_one_weak_fun_def)
            {
                if (IS_CXX_LANGUAGE)
                {
                    src << "extern \"C\""
                        << "{"
                        ;
                }

                src << weak_fun_defs;

                if (IS_CXX_LANGUAGE)
                {
                    src << "}";
                }
            }
        }
        else
        {
            if (seen_task_with_priorities)
                src << "__attribute__((common)) char nanos_need_priorities_;";

            if (seen_opencl_task)
                src << "__attribute__((common)) char ompss_uses_opencl;";

            if (seen_cuda_task)
                src << "__attribute__((common)) char ompss_uses_cuda;";

            if (seen_gpu_cublas_handle)
                src << "__attribute__((common)) char gpu_cublas_init;";

            // Nanos++ FPGA do not support this mechanism
        }

        if (!Nanos::Version::interface_is_at_least("master", 5028))
            set_openmp_programming_model(src);

        Nodecl::List n;
        if (!src.empty()) // avoid parsing an empty source
        {
            FORTRAN_LANGUAGE()
            {
                // Parse in C
                Source::source_language = SourceLanguage::C;
            }
            n = src.parse_global(global_node).as<Nodecl::List>();
            FORTRAN_LANGUAGE()
            {
                Source::source_language = SourceLanguage::Current;
            }
        }

        Nodecl::List& extra_c_code = this->get_extra_c_code();
        extra_c_code.append(n);

        if (extra_c_code.empty())
            return;

        if (!IS_FORTRAN_LANGUAGE)
        {
            Nodecl::Utils::append_to_top_level_nodecl(n);
        }
        else
        {
            CURRENT_CONFIGURATION->source_language = SOURCE_LANGUAGE_C;

            compilation_configuration_t* configuration = ::get_compilation_configuration("auxcc");
            ERROR_CONDITION (configuration == NULL, "auxcc profile is mandatory when using Fortran", 0);

            // Make sure phases are loaded (this is needed for codegen)
            load_compiler_phases(configuration);

            Codegen::CodegenPhase* phase = reinterpret_cast<Codegen::CodegenPhase*>(configuration->codegen_phase);

            FILE* ancillary = get_ancillary_file();
            std::string ancillary_filename = get_ancillary_filename();
            phase->codegen_top_level(extra_c_code, ancillary, ancillary_filename);

            CURRENT_CONFIGURATION->source_language = SOURCE_LANGUAGE_FORTRAN;
        }
    }

    void Lowering::set_openmp_programming_model(Source &src)
    {
        if (!_static_weak_symbols)
        {
            src
                << "__attribute__((weak, section(\"nanos_init\"))) nanos_init_desc_t __section__nanos_init = { nanos_omp_set_interface, (void*)0 };"
                ;
        }
        else
        {
            // Some compilers (like ICC) may require this
            src
                << "static __attribute__((section(\"nanos_init\"))) nanos_init_desc_t __section__nanos_init = { nanos_omp_set_interface, (void*)0 };"
                ;
        }

    }

    FILE* Lowering::get_ancillary_file()
    {
        if (_ancillary_file == NULL)
        {
            std::string file_name = "aux_nanox_omp_file_" + TL::CompilationProcess::get_current_file().get_filename(/* fullpath */ false) + ".c";
            _ancillary_file = fopen(file_name.c_str(), "w");
            if (_ancillary_file == NULL)
            {
                fatal_error("%s: error: cannot open file '%s'. %s\n",
                        TL::CompilationProcess::get_current_file().get_filename().c_str(),
                        file_name.c_str(),
                        strerror(errno));
            }

            TL::CompilationProcess::add_file(file_name, "auxcc");

            ::mark_file_for_cleanup(file_name.c_str());
            _ancillary_filename = file_name;
        }
        return _ancillary_file;
    }

    Nodecl::List Lowering::_extra_c_code;

    Nodecl::List& Lowering::get_extra_c_code()
    {
        return _extra_c_code;
    }

    void Lowering::phase_cleanup(DTO& data_flow)
    {
        if (_ancillary_file != NULL)
            fclose(_ancillary_file);

        _ancillary_file = NULL;
        _extra_c_code = Nodecl::List();

        // Cleanup flags
        seen_task_with_priorities = false;
        seen_opencl_task = false;
        seen_cuda_task = false;
        seen_gpu_cublas_handle = false;
        seen_fpga_task = false;
    }
} }


EXPORT_PHASE(TL::Nanox::Lowering);
