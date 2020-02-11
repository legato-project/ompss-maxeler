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

#ifndef _NANOS_MAXDEVICE_H
#define _NANOS_MAXDEVICE_H

#include "workdescriptor_decl.hpp"
#include "compatibility.hpp"

namespace nanos {
namespace ext {

class MaxDevice;

typedef TR1::unordered_map<uint64_t, MaxDevice const *> MaxDeviceMap;

class MaxDevice: public Device {

   public:
      MaxDevice( std::string name ): Device( name.c_str() ) {}

      virtual void *memAllocate( std::size_t size, SeparateMemoryAddressSpace &mem,
            WorkDescriptor const *wd, unsigned int copyIdx);
      virtual void memFree( uint64_t addr, SeparateMemoryAddressSpace &mem );
      virtual void _canAllocate( SeparateMemoryAddressSpace &mem,
            std::size_t *sizes, unsigned int numChunks,
            std::size_t *remainingSizes );
      virtual std::size_t getMemCapacity( SeparateMemoryAddressSpace &mem );
      virtual void _copyIn( uint64_t devAddr, uint64_t hostAddr, std::size_t len,
            SeparateMemoryAddressSpace &mem, DeviceOps *ops,
            WorkDescriptor const *wd, void *hostObject, reg_t hostRegionId );
      virtual void _copyOut( uint64_t hostAddr, uint64_t devAddr, std::size_t len,
            SeparateMemoryAddressSpace &mem, DeviceOps *ops,
            WorkDescriptor const *wd, void *hostObject, reg_t hostRegionId );
      virtual bool _copyDevToDev( uint64_t devDestAddr, uint64_t devOrigAddr,
            std::size_t len, SeparateMemoryAddressSpace &memDest,
            SeparateMemoryAddressSpace &memorig, DeviceOps *ops,
            WorkDescriptor const *wd, void *hostObject, reg_t hostRegionId );
      virtual void _copyInStrided1D( uint64_t devAddr, uint64_t hostAddr,
            std::size_t len, std::size_t numChunks, std::size_t ld,
            SeparateMemoryAddressSpace &mem, DeviceOps *ops,
            WorkDescriptor const *wd, void *hostObject, reg_t hostRegionId );
      virtual void _copyOutStrided1D( uint64_t hostAddr, uint64_t devAddr,
            std::size_t len, std::size_t numChunks, std::size_t ld,
            SeparateMemoryAddressSpace &mem, DeviceOps *ops,
            WorkDescriptor const *wd, void *hostObject, reg_t hostRegionId );
      virtual bool _copyDevToDevStrided1D( uint64_t devDestAddr,
            uint64_t devOrigAddr, std::size_t len, std::size_t numChunks,
            std::size_t ld, SeparateMemoryAddressSpace &memDest,
            SeparateMemoryAddressSpace &memOrig, DeviceOps *ops,
            WorkDescriptor const *wd, void *hostObject, reg_t hostRegionId );
      virtual void _getFreeMemoryChunksList( SeparateMemoryAddressSpace &mem,
            SimpleAllocator::ChunkList &list );
};

}  //namespace ext
}  //namespace nanos
#endif //_NANOS_MAXDEVICE_H
