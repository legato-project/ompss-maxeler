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

#ifndef _NANOS_MAXPROCESSOR_H
#define _NANOS_MAXPROCESSOR_H

#include "processingelement.hpp"
#include "maxdd.hpp"
#include "maxprocessorinfo.hpp"
//TODO: Max runtime includes


namespace nanos {
namespace ext {
   class MaxProcessor: public ProcessingElement
   {
      private:
         MaxProcessorInfo     _maxProcessorInfo;

         void submitTask( MaxDD &dd );

      public:
         MaxProcessor( MaxProcessorInfo info, memory_space_id_t memSpaceId,
               Device const *arch) :
            ProcessingElement( arch, memSpaceId, 0, 0, false, 0, false ), 
            _maxProcessorInfo( info ) { }
         ~MaxProcessor() {}

         MaxProcessorInfo getMaxProcessorInfo() const {
            return _maxProcessorInfo;
         }
         virtual WD & getMultiWorkerWD( DD::work_fct ) const {
            fatal( "getMasterWD(): FPGA processor is not allowed to create MultiThreads" );
         }

         BaseThread & createThread ( WorkDescriptor &wd, SMPMultiThread* parent );
         BaseThread & createMultiThread ( WorkDescriptor &wd,
               unsigned int numPEs, ProcessingElement **repPEs ) {
            fatal( "ClusterNode is not allowed to create FPGA MultiThreads" );
         }

         virtual bool hasSeparatedMemorySpace() const { return true; }
         bool supportsUserLevelThreads () const { return false; }
         int getAccelId() const { return _maxProcessorInfo.getId(); }

         virtual void switchHelperDependent( WD* oldWD, WD* newWD, void *arg ) {
            fatal("switchHelperDependent is not implemented in the FPGAProcessor");
         }
         virtual void exitHelperDependent( WD* oldWD, WD* newWD, void *arg ) {}
         virtual bool inlineWorkDependent (WD &work);
         virtual void switchTo( WD *work, SchedulerHelper *helper ) {}
         virtual void exitTo( WD *work, SchedulerHelper *helper ) {}
         virtual void outlineWorkDependent (WD &work);
         virtual void preOutlineWorkDependent (WD &work);
         bool tryPostOutlineWork( WD * wd );

         void setTaskArg( WD &wd, char *argName, bool isInput, bool isOutput,
               uintptr_t argval, size_t argLen);
         virtual nanos::WorkDescriptor& getMasterWD() const {
             fatal( "Trying to create a maxeler master thread!" );
         }
         virtual nanos::WorkDescriptor& getWorkerWD() const;

         void queueInput( const char* name, void * addr, size_t size );
         void queueOutput( const char* name, void * addr, size_t size );
         void setTimeoutTicks( unsigned int ticks );


   };
   extern std::vector< MaxProcessor * > *maxPEs;
}
}


#endif //_NANOS_MAXPROCESSOR_H
