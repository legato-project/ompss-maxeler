/*************************************************************************************/
/*      Copyright 2009-2019 Barcelona Supercomputing Center                          */
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


#include "maxdevice.hpp"
#include "deviceops.hpp"

using namespace nanos;
using namespace nanos::ext;

//All data is streamed in/out Maxeler's dataflow engines
//therefore, no explicit memory operation are performed

void *MaxDevice::memAllocate( std::size_t size, SeparateMemoryAddressSpace &mem,
      WorkDescriptor const *wd, unsigned int copyIdx ) {
   ensure(copyIdx < wd->getNumCopies(), "Unexpected copyIdx");
   //Do not return NULL as it means that the allocation failed
   return ( void * ) wd->getCopies()[copyIdx].getAddress();
}

void MaxDevice::memFree( uint64_t addr, SeparateMemoryAddressSpace &mem ) {}

void MaxDevice::_canAllocate( SeparateMemoryAddressSpace &mem,
      std::size_t *sizes, unsigned int numChunks,
      std::size_t *remainingSizes ) {
   //Assume always can allocate as there is no actual allocation
   remainingSizes[ 0 ] = 0;
}


std::size_t MaxDevice::getMemCapacity( SeparateMemoryAddressSpace &mem ) {
   return -1;
}

void MaxDevice::_copyIn( uint64_t devAddr, uint64_t hostAddr, std::size_t len,
      SeparateMemoryAddressSpace &mem, DeviceOps *ops,
      WorkDescriptor const *wd, void *hostObject, reg_t hostRegionId ) {

   //do nothing
}

void MaxDevice::_copyOut( uint64_t hostAddr, uint64_t devAddr, std::size_t len,
      SeparateMemoryAddressSpace &mem, DeviceOps *ops,
      WorkDescriptor const *wd, void *hostObject, reg_t hostRegionId ) {
   //do nothing
}
bool MaxDevice::_copyDevToDev( uint64_t devDestAddr, uint64_t devOrigAddr,
      std::size_t len, SeparateMemoryAddressSpace &memDest,
      SeparateMemoryAddressSpace &memorig, DeviceOps *ops,
      WorkDescriptor const *wd, void *hostObject, reg_t hostRegionId ) {
   fatal("Operation not supported");
   return true;
}
void MaxDevice::_copyInStrided1D( uint64_t devAddr, uint64_t hostAddr,
      std::size_t len, std::size_t numChunks, std::size_t ld,
      SeparateMemoryAddressSpace &mem, DeviceOps *ops,
      WorkDescriptor const *wd, void *hostObject, reg_t hostRegionId ) {
   fatal("Operation not supported");
}
void MaxDevice::_copyOutStrided1D( uint64_t hostAddr, uint64_t devAddr,
      std::size_t len, std::size_t numChunks, std::size_t ld,
      SeparateMemoryAddressSpace &mem, DeviceOps *ops,
      WorkDescriptor const *wd, void *hostObject, reg_t hostRegionId ) {
   fatal("Operation not supported");
}
bool MaxDevice::_copyDevToDevStrided1D( uint64_t devDestAddr,
      uint64_t devOrigAddr, std::size_t len, std::size_t numChunks,
      std::size_t ld, SeparateMemoryAddressSpace &memDest,
      SeparateMemoryAddressSpace &memOrig, DeviceOps *ops,
      WorkDescriptor const *wd, void *hostObject, reg_t hostRegionId ) {
   fatal("Operation not supported");
   return true;
}
void MaxDevice::_getFreeMemoryChunksList( SeparateMemoryAddressSpace &mem,
      SimpleAllocator::ChunkList &list ) {
   //This should never be called as allocations will not fail for maxeler device.
   fatal("Trying to free memory from maxeler device!");
}
