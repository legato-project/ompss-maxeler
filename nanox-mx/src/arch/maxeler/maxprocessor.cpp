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

#include "maxprocessor.hpp"
#include "maxthread.hpp"
#include "maxdd.hpp"
#include "maxworker.hpp"

using namespace nanos;
using namespace nanos::ext;


std::vector< MaxProcessor * > *nanos::ext::maxPEs;

BaseThread & MaxProcessor::createThread (
      WorkDescriptor &helper, SMPMultiThread* parent ) {
   ensure( helper.canRunIn( getSMPDevice() ), "Incompatible worker thread" );
   MaxThread &th = *NEW MaxThread( helper, this, parent );
   return th;
}

bool MaxProcessor::inlineWorkDependent (WD &work) {
   work.start( WD::IsNotAUserLevelThread );
   outlineWorkDependent( work );    //this should be done synchronously
   return true;
}

void MaxProcessor::outlineWorkDependent (WD &work) {
   //createTask( wd, work.getParent() );

   //Create action list
   MaxDD &dd = ( MaxDD & )work.getActiveDevice();
   ( dd.getWorkFct() )( work.getData() );

   submitTask(dd); //Run action list
}

void MaxProcessor::preOutlineWorkDependent (WD &work) {
   work.preStart(WorkDescriptor::IsNotAUserLevelThread);
   //Flush previous queued actions
   max_actions_t *actions = _maxProcessorInfo.getActions();
   max_clear_queues( actions );
   if ( !max_ok( actions->errors ) ) {
      warning( "Error clearing queues" );
   }
}

bool MaxProcessor::tryPostOutlineWork( WD *wd ) {
    MaxDD &dd = ( MaxDD & )wd->getActiveDevice();
    max_run_t *handle = ( max_run_t * ) dd.getRunHandle();
    max_wait( handle ); //blocking
    return true;
    
}

void MaxProcessor::submitTask( MaxDD &dd ) {
   //TODO run actions
   max_actions_t *actions = _maxProcessorInfo.getActions();
   max_engine_t *engine = _maxProcessorInfo.getEngine();
   max_run_t *run;
   run = max_run_nonblock( engine, actions );
   dd.setRunHandle( ( void * )run );


   if ( !max_ok( engine->errors ) || !max_ok( actions->errors ) ) {
      warning0("error submitting max task");
   }
}

void MaxProcessor::setTaskArg( WD &wd, char *argName, bool isInput, bool isOutput,
      uintptr_t argval, size_t argLen) {
   //TODO set parameters into action list
}

WorkDescriptor & MaxProcessor::getWorkerWD () const
{
   //SMPDD *dd = NEW SMPDD( ( SMPDD::work_fct )Scheduler::workerLoop );
   SMPDD *dd = NEW SMPDD( ( SMPDD::work_fct )MaxWorker::MaxWorkerLoop );
   WD *wd = NEW WD( dd );
   return *wd;
}

void MaxProcessor::queueInput( const char* name, void * addr, size_t size ) {
   max_actions_t *actions = _maxProcessorInfo.getActions();
   max_queue_input(actions, name, addr, size);
   if ( !max_ok( actions->errors ) ) {
      warning( "Error setting input " << name );
   }
}

void MaxProcessor::queueOutput( const char* name, void * addr, size_t size ) {
   max_actions_t *actions = _maxProcessorInfo.getActions();
   max_queue_output(actions, name, addr, size);
   if ( !max_ok( actions->errors ) ) {
      warning( "Error setting output " << name );
   }
}

void MaxProcessor::setTimeoutTicks( unsigned int ticks ) {
   max_actions_t *actions = _maxProcessorInfo.getActions();
   max_set_ticks( actions,
         _maxProcessorInfo.getName(),
         ticks);
   if ( !max_ok( actions->errors ) ) {
      warning( "Error setting timeout ticks" );
   }
}
