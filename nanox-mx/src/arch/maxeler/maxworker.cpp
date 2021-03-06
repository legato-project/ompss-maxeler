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

namespace nanos {
namespace ext {

//std::queue< MaxWorker::MaxDFE > _dfeList;
//std::queue< WD * > _runningTasks;

}
}

using namespace nanos;
using namespace ext;



//std::list< MaxWorker::MaxDFE > MaxWorker::_dfeList;
//MaxWorker::MaxDFE MaxWorker::_dfeList;
//int MaxWorker::sz;
//std::queue< MaxWorker::MaxDFE > MaxWorker::_dfeList =
//         std::queue< MaxWorker::MaxDFE >();
//std::queue<MaxWorker::MaxDFE> q = std::queue< MaxWorker::MaxDFE >();

//MaxWorker::MaxDFE  globalList;

//std::queue< WD * > MaxWorker::_runningTasks;

//std::queue <MaxWorker::MaxDFE> & MaxWorker::getDFEList() { /* printf ("_dfeList %p\n", &_dfeList); */ return _dfeList; }

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
#if 0
         if ( ! _runningTasks.empty() ) {
            wd = _runningTasks.front();
            if ( dfe->tryPostOutlineWork( wd ) ) {
               _runningTasks.pop();
               Scheduler::postOutlineWork( wd, true, currentThread );
            }
#else
         if ( ! getRunningTasks().empty() ) {
            wd = getRunningTasks().front();
            if ( dfe->tryPostOutlineWork( wd ) ) {
               getRunningTasks().pop();
               Scheduler::postOutlineWork( wd, true, currentThread );
            }
#endif
         }
      } else {
         //Run a ready task
         Scheduler::prePreOutlineWork( wd );
         if ( Scheduler::tryPreOutlineWork( wd ) ) {
            dfe->preOutlineWorkDependent( *wd );
            Scheduler::outlineWork( currentThread, wd );
#if 0
            _runningTasks.push( wd );
#else
            getRunningTasks().push( wd );
#endif
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
#include <stdio.h>

void MaxWorker::addDFE(void *initFun, const char *name, unsigned int type) {
   //MaxWorker::MaxWorker();
   //_dfeList = std::queue< MaxWorker::MaxDFE >();

   static std::list<MaxWorker::MaxDFE> q = std::list< MaxWorker::MaxDFE >();
   //q = std::queue< MaxWorker::MaxDFE >();
   //
   //_dfeList = q;

#ifdef MAXDEBUG
   //printf ("qsize %ld %ld\n", q.size(), _dfeList.size());
   printf ("qsize %ld %ld\n", q.size(), MaxWorker::getDFEList().size());

   MaxWorker::MaxDFE a = MaxDFE(initFun, name, type ) ;
   //printf ("a..name %s list size %ld run size %ld\n", a.name, _dfeList.size(), getRunningTasks().size());
   printf ("a..name %s list size %ld run size %ld\n", a.name, MaxWorker::getDFEList().size(), getRunningTasks().size());
#endif
   //_dfeList.push_back( MaxDFE( initFun, name, type ) );
   MaxWorker::getDFEList().push_back( MaxDFE( initFun, name, type ) );
   //globalList = MaxDFE( initFun, name, type );
   //printf("name %s\n", _dfeList.back().name);
   //////printf ("MaxDFE %p\n", &(MaxDFE( initFun, name, type )));
   //printf ("MaxDFE %p\n", &a);
   //if (MaxWorker::getDFEList().empty()) printf ("List empty\n");
   //if (MaxWorker::sz == 0) printf ("List empty\n");
   //else { printf ("Initializing List\n"); MaxWorker::_dfeList.clear(); }
//printf ("trying push_back %p %p %s %d\n", &MaxWorker::_dfeList, initFun, name, type);
//printf ("trying push_back %p %p %s %d\n", &MaxWorker::getDFEList(), initFun, name, type);
   //MaxWorker::getDFEList().push_back(a);
   //MaxWorker::_dfeList[MaxWorker::_sz++] = a;
   
  // printf("queue size %ld DFE %p\n", _dfeList.size(), &a);
  // _dfeList.push( a );
#ifdef MAXDEBUG
   printf ("addDFE ok\n");
#endif
}
