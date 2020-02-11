/*--------------------------------------------------------------------
  (C) Copyright 2006-2013 Barcelona Supercomputing Center
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

#ifndef TL_OMP_GOMP_HPP
#define TL_OMP_GOMP_HPP

#include "tl-compilerphase.hpp"
#include "tl-nodecl.hpp"

namespace TL { namespace GOMP {

    class Lowering : public TL::CompilerPhase
    {
        public:
            Lowering();

            virtual void phase_cleanup(DTO& data_flow);

            virtual void run(DTO& dto);
            virtual void pre_run(DTO& dto);

            bool instrumentation_enabled() const;

            bool simd_reductions_knc() const;

        private:
            std::string _openmp_dry_run;

            std::string _instrumentation_str;
            bool _instrumentation_enabled;
            void set_instrumentation(const std::string& str);

            std::string _simd_reductions_knc_str;
            bool _simd_reductions_knc;
            void set_simd_reduction_knc(const std::string &str);
    };

} }

#endif // TL_OMP_GOMP_HPP
