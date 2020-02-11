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


/*
<testinfo>
test_generator=config/mercurium
</testinfo>
*/
enum E { A = 0, B };

void g()
{
    char c[10], *p;

    // 0
    c == 0;
    p == 0;

    c == (char)0;
    p == (char)0;

    c == (short)0;
    p == (short)0;

    // '\0'
    c == '\0';
    p == '\0';

    c == (int)'\0';
    p == (int)'\0';

    c == (short)'\0';
    p == (short)'\0';

    // A
    c == A;
    p == A;

    c == (char)A;
    p == (char)A;

    c == (short)A;
    p == (short)A;
}
