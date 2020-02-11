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

#ifndef _MAXPROCESSORINFO_H
#define _MAXPROCESSORINFO_H

#pragma GCC diagnostic ignored "-Wshadow"
#include <MaxSLiCInterface.h>
#pragma GCC diagnostic warning "-Wshadow"

namespace nanos {
namespace ext {

   class MaxProcessorInfo
   {
      private:
         max_file_t* maxFile;
         max_engine_t* engine;
         max_actions_t* actions;
         const char *name;

      public:
         MaxProcessorInfo( void *fun, const char *n ): name(n)
         {
            max_file_t *( *initFun )( void ) = ( max_file_t *( * )( void ) )fun;
            maxFile = initFun();
            if ( !max_ok( maxFile->errors ) ) {
               warning0( "Error creating MaxFile for DFE " << name );
            }
            engine = max_load( maxFile, "*" ); //load all dfes in the maxfile
            if ( !max_ok( engine->errors ) ) {
               warning0( "Error creating engine from maxfile" );
            }
            actions = max_actions_init( maxFile, NULL );
            if ( !max_ok( actions->errors ) ) {
               warning0( "Error creating ActionList" );
            }
         }
         int getId() const {
            return 0;   //FIXME
         }

         max_actions_t *getActions() { return actions; }
         max_engine_t *getEngine() { return engine; }
         const char *getName() { return name; }
   };
}
}

#endif
