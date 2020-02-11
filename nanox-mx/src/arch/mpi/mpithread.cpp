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

#include "mpi.h"
#include "mpiprocessor_decl.hpp"
#include "mpithread.hpp"
#include "mpispawn.hpp"
#include "schedule.hpp"
#include "debug.hpp"
#include "system.hpp"
#include <iostream>
#include <sched.h>
#include <unistd.h>
#include "instrumentation.hpp"
#include <os.hpp>

#include "finish.hpp"

using namespace nanos;
using namespace nanos::ext;

void MPIThread::initializeDependent() {
}

void MPIThread::runDependent() {
    WD &work = getThreadWD();
    setCurrentWD(work);

    MPIDD &dd = (MPIDD &) work.activateDevice(MPI);

    dd.getWorkFct()(work.getData());
}

//Switch to next PE (Round robin way of change PEs to query scheduler)
bool MPIThread::switchToNextFreePE(int uuid){
   int startPE   = _currentPE;
   int currentPE = _currentPE;

   bool success = false;
   std::vector<MPIProcessor*>& remotes = getSpawnGroup().getRemoteProcessors();
   do {
       currentPE = (currentPE+1) % remotes.size();
       success = switchToPE( currentPE, uuid );
   } while ( !success && currentPE != startPE );

   return success;
}

//Switch to PE, under request, if we see a task for this group of PEs
//and the PE is not busy, we can run on that PE and execute the task
bool MPIThread::switchToPE(int rank, int uuid) {
   std::vector<MPIProcessor*>& remotes = getSpawnGroup().getRemoteProcessors();

   bool success = false;
   if( remotes.at(rank)->acquire(uuid) ) {
       _currentPE = rank;
       setRunningOn(remotes[rank]);
       success = true;
   }
   return success;
}

void MPIThread::idle( bool debug ) {
    getSpawnGroup().waitFinishedTasks();
}

void MPIThread::finish() {
    SMPThread::finish();
}

