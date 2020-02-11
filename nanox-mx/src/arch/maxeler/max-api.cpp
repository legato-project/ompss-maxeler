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

#include "maxdd.hpp"
#include "maxprocessor.hpp"
#include "maxworker.hpp"
#include "nanos-max.h"

using namespace nanos;

NANOS_API_DEF( void *, nanos_max_factory, ( void *args ) )
{
   nanos_max_args_t *maxArgs = ( nanos_max_args_t * ) args;
   return ( void * ) NEW ext::MaxDD( maxArgs->outline, maxArgs->type );
}

NANOS_API_DEF( void, nanos_max_register_dfe,
        ( void *init_fun, const char* name, unsigned int type ) )
{
   //Create a list of DFE as PEs cannot be created yet
   ext::MaxWorker::addDFE(init_fun, name, type);
}

NANOS_API_DEF( void, nanos_max_queue_input,
        ( const char* name, void *addr, size_t size ) )
{
    ext::MaxProcessor *maxPE = ( ext::MaxProcessor * ) myThread->runningOn();
    maxPE->queueInput( name, addr, size );

}

NANOS_API_DEF( void, nanos_max_queue_output,
        ( const char *name, void *addr, size_t size ) )
{
    ext::MaxProcessor *maxPE = ( ext::MaxProcessor * ) myThread->runningOn();
    maxPE->queueOutput( name, addr, size );
}

