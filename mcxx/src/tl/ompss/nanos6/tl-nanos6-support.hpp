/*--------------------------------------------------------------------
  (C) Copyright 2016-2018 Barcelona Supercomputing Center
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


#ifndef TL_NANOS6_SUPPORT_HPP
#define TL_NANOS6_SUPPORT_HPP

#include "tl-nanos6.hpp"

#include "tl-objectlist.hpp"
#include "tl-scope.hpp"
#include "tl-nodecl.hpp"
#include "tl-nodecl-utils.hpp"

#include <fortran03-scope.h>
#include <fortran03-intrinsics.h>


namespace TL
{
namespace Nanos6
{
    namespace
    {
        template < unsigned int num_arguments>
        TL::Symbol get_fortran_intrinsic_symbol(const std::string &name, const Nodecl::List& actual_arguments, bool is_call)
        {
            // Note that this function is template to avoid to use VLAs in C++ or dynamic memory allocation
            nodecl_t arguments[num_arguments];

            int index = 0;
            for (Nodecl::List::const_iterator it = actual_arguments.begin();
                    it != actual_arguments.end();
                    it++)
            {
                arguments[index++]=it->get_internal_nodecl();
            }
            TL::Symbol intrinsic(
                    fortran_solve_generic_intrinsic_call(
                        fortran_query_intrinsic_name_str(TL::Scope::get_global_scope().get_decl_context(), name.c_str()),
                        arguments,
                        num_arguments,
                        is_call));

            return intrinsic;
        }
    }

    TL::Symbol get_nanos6_class_symbol(const std::string &name);
    TL::Symbol get_nanos6_function_symbol(const std::string &name);

    void add_extra_mappings_for_vla_types(
            TL::Type t,
            Scope sc,
            /* out */
            Nodecl::Utils::SimpleSymbolMap &symbol_map,
            TL::ObjectList<TL::Symbol> &vla_vars);

    Nodecl::NodeclBase compute_call_to_nanos6_bzero(Nodecl::NodeclBase pointer_expr_to_be_initialized);

    void create_static_variable_depending_on_function_context(
        const std::string &var_name,
        TL::Type var_type,
        Nodecl::NodeclBase context,
        LoweringPhase* phase,
        /* out */
        TL::Symbol &new_var);


    //! Create a detached symbol with the same name as the real one We need to
    //! do that otherwise Fortran codegen attempts to initialize this symbol
    //! (We may want to fix this somehow)
    Symbol fortran_create_detached_symbol_from_static_symbol(
        Symbol &static_symbol);

    Scope compute_scope_for_environment_structure(Symbol related_function);

    Symbol add_field_to_class(Symbol new_class_symbol,
        Scope class_scope,
        const std::string &var_name,
        const locus_t *var_locus,
        bool is_allocatable,
        Type field_type);

}
}

#endif // TL_NANOS6_SUPPORT_HPP
