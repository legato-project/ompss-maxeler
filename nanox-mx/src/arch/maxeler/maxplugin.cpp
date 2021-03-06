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

#include "plugin.hpp"
#include "archplugin.hpp"
#include "smpprocessor.hpp"

#include "maxthread.hpp"
#include "maxdevice.hpp"
#include "maxconfig.hpp"
#include "maxprocessor.hpp"
#include "maxworker.hpp"
#include "maxdd.hpp"

#include <stdio.h>

namespace nanos {
namespace ext {

//extern MaxWorker::MaxDFE  globalList;


class MaxPlugin : public ArchPlugin
{
   private:
      std::vector< SMPMultiThread *> _helperThreads;
      std::vector< ext::SMPProcessor *> _helperCores;
      MaxDeviceMap _maxDevices;

   public:
      MaxPlugin() : ArchPlugin( "Maxeler DFE plugin", 1 ) {}
      void config( Config &cfg ) {
#ifdef MAXDEBUG
printf ("MaxConfig::prepare\n");
#endif
         MaxConfig::prepare( cfg );
      }

      void init() {
         maxPEs = NEW std::vector< MaxProcessor * >();
#ifdef MAXDEBUG
printf ("maxPEs.size %p %ld\n", maxPEs, maxPEs->size());
#endif
         if ( MaxConfig::isDisabled() ) {
            debug("MaxDFE support is disabled. Skipping initialization");
            return;
         }
         //TODO: free
         MaxDD::init( &_maxDevices );
#ifdef MAXDEBUG
printf ("MaxDD::init\n");
#endif
         //std::list < MaxWorker::MaxDFE > & dfeList = MaxWorker::getDFEList();
         //MaxWorker::MaxDFE * dfeList = MaxWorker::getDFEList();
         std::list < MaxWorker::MaxDFE > & dfeList = MaxWorker::getDFEList();
#ifdef MAXDEBUG
printf ("getDFEList ok %p\n", &dfeList);
printf ("dfeList.size() %ld\n", dfeList.size());
#endif
#if 1
         maxPEs->reserve( dfeList.size() );
#else
         maxPEs->reserve( 1 );
#endif
#ifdef MAXEBUG
printf ("reserved maxthreads %d\n", MaxConfig::getNumMaxThreads());
#endif

         _helperThreads.reserve( MaxConfig::getNumMaxThreads() );
         _helperCores.reserve( MaxConfig::getNumMaxThreads() );



         //TODO: Initialize maxeler runtime

#if 1
         for ( std::list< MaxWorker::MaxDFE >::const_iterator it = dfeList.begin();
               it != dfeList.end();
               it++ )
#endif
         {
#if 1
#else
	    MaxWorker::MaxDFE & it = dfeList.back();
#endif

#ifdef MAXDEBUG
            printf ("&it %p\n", &it);
            //MaxDevice *maxDev = NEW MaxDevice( it->name );
            printf ("name %s\n", it->name);
#endif
            MaxDevice *maxDev = NEW MaxDevice( it->name );
            memory_space_id_t memId = sys.addSeparateMemoryAddressSpace(
                  *maxDev, false, 0);
            SeparateMemoryAddressSpace &maxMem = sys.getSeparateMemory( memId );
            maxMem.setAcceleratorNumber( sys.getNewAcceleratorId() );
            maxMem.setNodeNumber( 0 ); //TODO set actual numa node

            SMPProcessor * core = sys.getSMPPlugin()->getLastFreeSMPProcessorAndReserve();
            if ( core != NULL )  {
               core->setNumFutureThreads( 1 );
            } else {
               core = sys.getSMPPlugin()->getLastSMPProcessor();
               core->setNumFutureThreads( core->getNumFutureThreads() + 1 );
            }

            //Creating a device for each pe may not be needed
            //as we don't have types yet
            MaxProcessorInfo info( it->dfeInit, it->name );
            //MaxProcessorInfo info( it.dfeInit, it.name );
            MaxProcessor *dfe = NEW MaxProcessor( info, memId, maxDev );
            //Set timeout for accelerator
            dfe->setTimeoutTicks( MaxConfig::getTimeoutTicks() );
            maxPEs->push_back( dfe );
            _maxDevices.insert( std::make_pair( it->type, maxDev ) );
            //_maxDevices.insert( std::make_pair( it.type, maxDev ) );
         }
#ifdef MAXDEBUG
printf ("trying %d\n", MaxConfig::getNumMaxThreads());
#endif
         for ( int i = 0; i < MaxConfig::getNumMaxThreads(); i++ ) {
            SMPProcessor * core = sys.getSMPPlugin()->getLastFreeSMPProcessorAndReserve();
            if ( core != NULL ) {
               core->setNumFutureThreads( 1 );
            } else {
               core = sys.getSMPPlugin()->getLastSMPProcessor();
               core->setNumFutureThreads( core->getNumFutureThreads() + 1 );
            }
            _helperCores.push_back( core );
         }

      }

      virtual void finalize() {
         //clean up stuff
         printf ("Finalizing arch DFE\n");
      }



      virtual unsigned int getNumHelperPEs() const {
         return _helperCores.size();
      }

      virtual unsigned int getNumPEs() const {
         return maxPEs->size();
      }

      virtual unsigned int getNumThreads() const {
         return getNumWorkers();
      }

      virtual unsigned int getNumWorkers() const {
         return _helperThreads.size();
      }

      virtual void addPEs( PEMap &pes ) const {
         for ( std::vector< MaxProcessor *>::const_iterator it = maxPEs->begin();
               it != maxPEs->end();
               it++ ) {
            pes.insert( std::make_pair( (*it)->getId(), *it ) );
         }
      }

      virtual void addDevices( DeviceList &devices ) const {
#ifdef MAXDEBUG
printf ("addDevices\n");
#endif
         for ( MaxDeviceMap::const_iterator it = _maxDevices.begin();
               it != _maxDevices.end();
               it++) {
            devices.insert( it->second );
         }
      }

      virtual void startSupportThreads() {
      }

      virtual void startWorkerThreads(
            std::map<unsigned int, BaseThread*> &workers ) {
         for ( std::vector<SMPProcessor*>::const_iterator it = _helperCores.begin();
               it != _helperCores.end();
               it++) {
            SMPMultiThread *maxHelper = dynamic_cast< ext::SMPMultiThread * >(
                  &(*it)->startMultiWorker( maxPEs->size(),
                     (ProcessingElement **) &maxPEs->at(0),
                     //XXX: may want to use the default worker thread
                     ( DD::work_fct ) MaxWorker::MaxWorkerLoop ) );
            _helperThreads.push_back( maxHelper );
            workers.insert( std::make_pair( maxHelper->getId(), maxHelper ) );
         }
      }

      virtual ProcessingElement * createPE( unsigned id , unsigned uid ) {
         return NULL;
      }
};
}  //namespace ext
}  //namespace nanos

DECLARE_PLUGIN("arch-max", nanos::ext::MaxPlugin);
