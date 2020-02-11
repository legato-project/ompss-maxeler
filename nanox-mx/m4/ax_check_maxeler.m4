#
# SYNOPSIS
#
#   AX_CHECK_MAXELER
#
# DESCRIPTION
#
#   Check whether a valid maxeler libraries are available,r
#    and the path to headers and libraries are correctly specified.
#
# LICENSE
#
#   Copyright 2017-2018 Barcelona Supercomputing Center
#
#   This file is part of the NANOS++ library.
#
#   NANOS++ is free software: you can redistribute it and/or modify
#   it under the terms of the GNU Lesser General Public License as published by
#   the Free Software Foundation, either version 3 of the License, or
#   (at your option) any later version.
#
#   NANOS++ is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#   GNU Lesser General Public License for more details.
#
#   You should have received a copy of the GNU Lesser General Public License
#   along with NANOS++.  If not, see <http://www.gnu.org/licenses/>.
#

AC_DEFUN([AX_CHECK_MAXCOMPILER],
[
#Check if maxeler runtime libraries are in place
AC_ARG_WITH(maxcompiler,
    [AS_HELP_STRING([--with-maxcompiler,--with-maxcompiler=PATH],
        [search in system directories or specify a prefix directory for installed maxeler runtime libraries.])],
    [
        AS_IF([test -d "$withval"],[
            maxcompiler=yes
            maxcompiler_path_provided=yes
        ], [
            maxcompiler=$withval
            maxcompiler_path_provided=no
        ])dnl
    ], [
        #Default: Check if maxeler RT is available
        maxcompiler=no
        maxcompiler_path_provided=no
])

AS_IF([test "$maxcompiler_path_provided" = yes], [
    maxelerinc="-I$with_maxcompiler/include -I$with_maxcompiler/include/slic"
    maxlib="-L$with_maxcompiler/lib"
])

AS_IF([test "$maxcompiler" = yes], [
    AX_VAR_PUSHVALUE([CPPFLAGS],[$CPPFLAGS $maxelerinc])
    AX_VAR_PUSHVALUE([CXXFLAGS])
    AX_VAR_PUSHVALUE([LDFLAGS],[$LDFLAGS $maxlib])
    AX_VAR_PUSHVALUE([LIBS])

    #Check for header
    AC_CHECK_HEADERS([MaxSLiCInterface.h],
        [maxcompiler=yes],
        [maxcompiler=no])

    AS_IF([test "$maxcompiler"=yes], [
        AC_SEARCH_LIBS([max_run],
            [slic],
            [maxcompiler=yes],
            [maxcompiler=no],
            [$maxoslib $maxoslibs -lm -lpthread])
    ])dnl
    maxlibs="$LIBS"

    AX_VAR_POPVALUE([CPPFLAGS])
    AX_VAR_POPVALUE([CXXFLAGS])
    AX_VAR_POPVALUE([LDFLAGS])
    AX_VAR_POPVALUE([LIBS])
])dnl

AC_SUBST([maxelerinc])
AC_SUBST([maxlib])
AC_SUBST([maxlibs])


AS_IF([test "$maxcompiler" = yes], [
    AC_SUBST([HAVE_MAXCOMPILER], [MAXCOMPILER])
], [
    AC_SUBST([HAVE_MAXCOMPILER], [NO_MAXCOMPILER])
])

])dnl AX_CHECK_MAXCOMPILER

AC_DEFUN([AX_CHECK_MAXELEROS],
[
#check for maxeleros runtime librarues
AC_ARG_WITH(maxeleros,
    [AS_HELP_STRING([--with-maxeleros,--with-maxeleros=PATH],
        [search in system directories or specify a prefix for installed maxeleros runtime libraries.])],
    [
        AS_IF([test -d "$withval"], [
            maxeleros=yes
            maxeleros_path_provided=yes
        ], [
            maxeleros=$withval
            maxeleros_path_provided=no
        ])
    ], [
        #default
        maxeleros=no
        maxeleros_path_provided=no
    ])

AS_IF([test "$maxeleros_path_provided" = yes], [
#no includes in maxeleros
    maxoslib="-L$with_maxeleros/lib"
])

AS_IF([test "$maxeleros" = yes], [

    AX_VAR_PUSHVALUE([CPPFLAGS])
    AX_VAR_PUSHVALUE([CXXFLAGS])
    AX_VAR_PUSHVALUE([LDFLAGS],[$LDFLAGS $maxoslib])
    AX_VAR_PUSHVALUE([LIBS])

    AS_IF([test "$maxeleros" = yes], [
        AC_SEARCH_LIBS([maxos_session_init],
            [maxeleros],
            [maxeleros=yes],
            [maxeleros=no],
            )
    ])dnl
    maxoslibs="$LIBS"

    AX_VAR_POPVALUE([CPPFLAGS])
    AX_VAR_POPVALUE([CXXFLAGS])
    AX_VAR_POPVALUE([LDFLAGS])
    AX_VAR_POPVALUE([LIBS])
])

AC_SUBST([maxoslib])
AC_SUBST([maxoslibs])

AS_IF([test "maxeleros" = yes],[
    AC_SUBST([HAVE_MAXELEROS], [MAXELEROS])
], [
    AC_SUBST([HAVE_MAXELEROS], [NO_MAXELEROS])
])dnl AX_CHECK_MAXELEROS

])


