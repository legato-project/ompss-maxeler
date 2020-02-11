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

#include "maxconfig.hpp"

using namespace nanos;
using namespace ext;

bool MaxConfig::_enableMax = true;
int MaxConfig::_ticks = 0;

void MaxConfig::prepare( Config &config ) { 
   //TODO register configuration options
   config.setOptionsSection( "Maxeler DFE Arch", "Maxeler specific options");
   config.registerConfigOption( "maxeler-enable",
         NEW Config::FlagOption( _enableMax ),
         "Enable Maxeler architecture" );
   config.registerEnvOption( "maxeler-enable", "MX_MAXELER_ENABLE" );
   config.registerArgOption( "maxeler-enable", "maxeler-enable" );

   config.registerConfigOption( "maxeler-ticks",
           NEW Config::IntegerVar( _ticks ),
           "Maxeler DFE ticks" );
   config.registerEnvOption( "maxeler-ticks", "NX_MAXELER_TICKS" );
   config.registerArgOption( "maxeler-ticks", "maxeler-ticks" );
}

bool MaxConfig::isDisabled() {
   return !_enableMax;  //FIXME
}
int MaxConfig::getDFECount() {
   return 1;   //FIXME
}
int MaxConfig::getNumMaxThreads() {
   return 1;   //FIXME
}

int MaxConfig::getTimeoutTicks() {
    return _ticks;   //TODO: different timeout per accelerator
}
