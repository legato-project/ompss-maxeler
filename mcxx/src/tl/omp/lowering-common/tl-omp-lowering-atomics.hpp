/*--------------------------------------------------------------------
  (C) Copyright 2006-2015 Barcelona Supercomputing Center
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



#ifndef TL_OMP_LOWERING_ATOMICS_HPP
#define TL_OMP_LOWERING_ATOMICS_HPP

#include"tl-nodecl.hpp"

namespace TL { namespace OpenMP { namespace Lowering {

    bool allowed_expression_atomic(Nodecl::NodeclBase expr, bool &using_builtin, bool &using_nanos_api);

    Nodecl::NodeclBase compare_and_exchange(Nodecl::NodeclBase expr);

    Nodecl::NodeclBase builtin_atomic_int_op(Nodecl::NodeclBase expr);
}}}

#endif // TL_OMP_LOWERING_ATOMICS_HPP
