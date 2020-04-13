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

#include "maxworker.hpp"
#include "maxprocessor.hpp"
#include "maxthread.hpp"

#include "schedule.hpp"

using namespace nanos;
using namespace ext;

std::list< MaxWorker::MaxDFE > MaxWorker::_dfeList;
std::queue< WD * > MaxWorker::_runningTasks;

void MaxWorker::MaxWorkerLoop() {
   BaseThread *parent = getMyThreadSafe();
   //TODO: Instrumentation stuff

   myThread = parent->getNextThread();
   BaseThread *currentThread = myThread;


   for (;;) {
      if ( !parent->isRunning() ) break;
      MaxProcessor *dfe = ( MaxProcessor * ) currentThread->runningOn();
      WD *wd;

      wd = MaxWorker::getMaxWD( currentThread );
      if ( wd == NULL ) { //no ready tasks -> finish running tasks
         if ( ! _runningTasks.empty() ) {
            wd = _runningTasks.front();
            if ( dfe->tryPostOutlineWork( wd ) ) {
               _runningTasks.pop();
               Scheduler::postOutlineWork( wd, true, currentThread );
            }
         }
      } else {
         //Run a ready task
         Scheduler::prePreOutlineWork( wd );
         if ( Scheduler::tryPreOutlineWork( wd ) ) {
            dfe->preOutlineWorkDependent( *wd );
            Scheduler::outlineWork( currentThread, wd );
            _runningTasks.push( wd );
         } else {
            fatal( "Maxeler task could not allocate memory (!!!)" );
         }
      }


      //Do not yield
      currentThread->idle( false );

      currentThread = parent->getNextThread();
      myThread = currentThread;
   }

   SMPMultiThread *parentM = ( SMPMultiThread *) parent;
   for ( unsigned int i = 0; i < parentM->getNumThreads(); i++ ) {
      myThread = parentM->getThreadVector()[ i ];
      myThread->joined();
   }
   myThread = parent;
}

WD * MaxWorker::getMaxWD( BaseThread * thread ) {
   WD *wd = NULL;
   if ( thread->getTeam() != NULL ) {
      wd = thread->getNextWD();
      if ( !wd ) {
         wd = thread->getTeam()->getSchedulePolicy().atIdle( thread, 0 );
      }
   }
   return wd;
}

void MaxWorker::addDFE(void *initFun, const char *name, unsigned int type) {
   _dfeList.push_back( MaxDFE( initFun, name, type ) );

}
