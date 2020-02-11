/*************************************************************************************/
/*      Copyright 2009-2018 Barcelona Supercomputing Center                          */
/*                                                                                   */
/*      This file is part of the NANOS++ library.                                    */
/*                                                                                   */
/*      NANOS++ is free software: you can redistribute it and/or modify              */
/*      it under the terms of the GNU Lesser General Public License as published by  */
/*      the Free Software Foundation, either version 3 of the License, or            */
/*      (at your option) any later version.                                          */
/*                                                                                   */
/*      NANOS++ is distributed in the hope that it will be useful,                   */
/*      but WITHOUT ANY WARRANTY; without even the implied warranty of               */
/*      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                */
/*      GNU Lesser General Public License for more details.                          */
/*                                                                                   */
/*      You should have received a copy of the GNU Lesser General Public License     */
/*      along with NANOS++.  If not, see <https://www.gnu.org/licenses/>.            */
/*************************************************************************************/

#ifndef _NANOS_MAX_H
#define _NANOS_MAX_H

#include "nanos-int.h"
#include "nanos_error.h"

#ifdef __cplusplus
extern "C" {
#endif //__cplusplus

typedef struct {
   void ( *outline ) ( void * );
   unsigned int     type;
} nanos_max_args_t;


NANOS_API_DECL( void *, nanos_max_factory, ( void *args ) );

NANOS_API_DECL( void, nanos_max_register_dfe,
      ( void *initFun, const char *name, unsigned int type) );

NANOS_API_DECL( void, nanos_max_queue_input,
        ( const char* name, void *addr, size_t size ) );

NANOS_API_DECL( void, nanos_max_queue_output,
        ( const char* name, void *addr, size_t size ) );

#ifdef __cplusplus
}
#endif //__cplusplus


#endif //_NANOS_FPGA_H
