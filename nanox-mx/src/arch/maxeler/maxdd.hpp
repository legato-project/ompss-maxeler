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

#ifndef _NANOS_MAXDD_H
#define _NANOS_MAXDD_H

#include "workdescriptor.hpp"
#include "maxdevice.hpp"

namespace nanos {
namespace ext {
   class MaxDD : public DD
   {
      private:
         //we may need to have an identifier as in fpga
         static MaxDeviceMap *_dfeMap;
         void *_maxRunHandle;

      public:
         static void init ( MaxDeviceMap *devs ) {
            _dfeMap = devs;
         }
         MaxDD( work_fct w, unsigned int dfeType ) :
            DD( (*_dfeMap)[dfeType], w ) {}
         MaxDD( const MaxDD &dd ) : DD( dd ) {}

         const MaxDD & operator= ( const MaxDD &dd );
         virtual ~MaxDD() {}

         virtual void lazyInit ( WD &wd, bool isUserLevelThread, WD *previous ) {}
         virtual size_t size ( void ) { return sizeof( MaxDD ); }
         virtual MaxDD *copyTo ( void *toAddr );
         virtual MaxDD *clone() const { return NEW MaxDD ( *this ); }

         void setRunHandle( void *handle ) { _maxRunHandle = handle; }
         void *getRunHandle( ) { return _maxRunHandle; }


   };

   inline const MaxDD & MaxDD::operator= ( const MaxDD &dd ) {
      if ( &dd == this ) return *this;
      DD::operator= ( dd );
      return *this;
   }
}  //namespace ext
}  //namespace nanos


#endif //_NANOS_MAXDD_H
