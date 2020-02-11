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


#ifndef CXX_MSSUPPORT_H
#define CXX_MSSUPPORT_H

#include "libmcxx-common.h"
#include "cxx-ast-decls.h"
#include "cxx-buildscope-decls.h"

LIBMCXX_EXTERN void gather_ms_declspec(AST attribute, 
        gather_decl_spec_t* gather_info, 
        const decl_context_t* decl_context);

LIBMCXX_EXTERN void gather_ms_declspec_list(AST attribute,
        gather_decl_spec_t* gather_info,
        const decl_context_t* decl_context);

LIBMCXX_EXTERN void keep_ms_declspecs_in_symbol(
        scope_entry_t* entry,
        gather_decl_spec_t* gather_info);

LIBMCXX_EXTERN void apply_ms_attribute_to_type(AST a, type_t** type,
        const decl_context_t* decl_context);

#endif // CXX_MSSUPPORT_H
