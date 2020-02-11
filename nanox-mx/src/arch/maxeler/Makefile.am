#####################################################################################
#      Copyright 2009-2018 Barcelona Supercomputing Center                          #
#                                                                                   #
#      This file is part of the NANOS++ library.                                    #
#                                                                                   #
#      NANOS++ is free software: you can redistribute it and/or modify              #
#      it under the terms of the GNU Lesser General Public License as published by  #
#      the Free Software Foundation, either version 3 of the License, or            #
#      (at your option) any later version.                                          #
#                                                                                   #
#      NANOS++ is distributed in the hope that it will be useful,                   #
#      but WITHOUT ANY WARRANTY; without even the implied warranty of               #
#      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                #
#      GNU Lesser General Public License for more details.                          #
#                                                                                   #
#      You should have received a copy of the GNU Lesser General Public License     #
#      along with NANOS++.  If not, see <https://www.gnu.org/licenses/>.            #
#####################################################################################

include $(top_srcdir)/src/common.am

EXTRA_DIST= \
	$(END)

pkginclude_HEADERS=\
	nanos-max.h \
	$(END)

devinclude_HEADERS = \
	maxconfig.hpp \
	maxdd.hpp \
	maxdevice.hpp \
	maxprocessor.hpp \
	maxprocessorinfo.hpp \
	maxthread.hpp \
	maxworker.hpp \
	$(END)

max_api_sources = \
	nanos-max.h \
	max-api.cpp \
	$(END)

max_sources = \
	maxconfig.cpp \
	maxdd.cpp \
	maxdevice.cpp \
	maxplugin.cpp \
	maxprocessor.cpp \
	maxthread.cpp \
	maxworker.cpp \
	$(END)

pe_max_sources = \
	maxplugin.cpp \
	$(END)

noinst_LTLIBRARIES =
debug_LTLIBRARIES =
instrumentation_LTLIBRARIES =
instrumentation_debug_LTLIBRARIES =
performance_LTLIBRARIES =

if is_debug_enabled
noinst_LTLIBRARIES += debug/libmaxeler.la
debug_LTLIBRARIES += debug/libnanox-max-api.la debug/libnanox-pe-max.la

debug_libmaxeler_la_CPPFLAGS=$(common_debug_CPPFLAGS) @maxelerinc@
debug_libmaxeler_la_CXXFLAGS=$(common_debug_CXXFLAGS)
debug_libmaxeler_la_LDFLAGS=$(AM_LDFLAGS) @maxlib@ @maxoslib@
debug_libmaxeler_la_LIBADD=@maxlibs@ @maxoslibs@
debug_libmaxeler_la_SOURCES=$(max_sources)

debug_libnanox_max_api_la_CPPFLAGS=$(common_debug_CPPFLAGS) $(api_includes) @maxelerinc@
debug_libnanox_max_api_la_CXXFLAGS=$(common_debug_CXXFLAGS)
debug_libnanox_max_api_la_LDFLAGS=$(AM_LDFLAGS) $(version_flags)
debug_libnanox_max_api_la_LIBADD=$(debug_max_core_LIBS)
debug_libnanox_max_api_la_SOURCES=$(max_api_sources)

debug_libnanox_pe_max_la_CPPFLAGS=$(common_debug_CPPFLAGS) @maxelerinc@
debug_libnanox_pe_max_la_CXXFLAGS=$(common_debug_CXXFLAGS)
debug_libnanox_pe_max_la_LDFLAGS=$(AM_LDFLAGS) $(ld_plugin_flags)
debug_libnanox_pe_max_la_LIBADD=$(debug_max_core_LIBS)
debug_libnanox_pe_max_la_SOURCES=$(pe_max_sources)
endif

if is_instrumentation_debug_enabled
noinst_LTLIBRARIES += instrumentation-debug/libmaxeler.la
instrumentation_debug_LTLIBRARIES += instrumentation-debug/libnanox-max-api.la instrumentation-debug/libnanox-pe-max.la

instrumentation_debug_libmaxeler_la_CPPFLAGS=$(common_instrumentation_debug_CPPFLAGS) @maxelerinc@
instrumentation_debug_libmaxeler_la_CXXFLAGS=$(common_instrumentation_debug_CXXFLAGS)
instrumentation_debug_libmaxeler_la_LDFLAGS=$(AM_LDFLAGS) @maxlib@ @maxoslib@
instrumentation_debug_libmaxeler_la_LIBADD=@maxlibs@ @maxoslibs@
instrumentation_debug_libmaxeler_la_SOURCES=$(max_sources)

instrumentation_debug_libnanox_max_api_la_CPPFLAGS=$(common_instrumentation_debug_CPPFLAGS) $(api_includes) @maxelerinc@
instrumentation_debug_libnanox_max_api_la_CXXFLAGS=$(common_instrumentation_debug_CXXFLAGS)
instrumentation_debug_libnanox_max_api_la_LDFLAGS=$(AM_LDFLAGS) $(version_flags)
instrumentation_debug_libnanox_max_api_la_LIBADD=
instrumentation_debug_libnanox_max_api_la_SOURCES=$(max_api_sources)

instrumentation_debug_libnanox_pe_max_la_CPPFLAGS=$(common_instrumentation_debug_CPPFLAGS) @maxelerinc@
instrumentation_debug_libnanox_pe_max_la_CXXFLAGS=$(common_instrumentation_debug_CXXFLAGS)
instrumentation_debug_libnanox_pe_max_la_LDFLAGS=$(AM_LDFLAGS) $(ld_plugin_flags)
instrumentation_debug_libnanox_pe_max_la_LIBADD=
instrumentation_debug_libnanox_pe_max_la_SOURCES=$(pe_max_sources)
endif

if is_instrumentation_enabled
noinst_LTLIBRARIES += instrumentation/libmaxeler.la
instrumentation_LTLIBRARIES += instrumentation/libnanox-max-api.la instrumentation/libnanox-pe-max.la

instrumentation_libmaxeler_la_CPPFLAGS=$(common_instrumentation_CPPFLAGS) @maxelerinc@
instrumentation_libmaxeler_la_CXXFLAGS=$(common_instrumentation_CXXFLAGS)
instrumentation_libmaxeler_la_LDFLAGS=$(AM_LDFLAGS) @maxlib@ @maxoslib@
instrumentation_libmaxeler_la_LIBADD=@maxlibs@ @maxoslibs@
instrumentation_libmaxeler_la_SOURCES=$(max_sources)

instrumentation_libnanox_max_api_la_CPPFLAGS=$(common_instrumentation_CPPFLAGS) $(api_includes) @maxelerinc@
instrumentation_libnanox_max_api_la_CXXFLAGS=$(common_instrumentation_CXXFLAGS)
instrumentation_libnanox_max_api_la_LDFLAGS=$(AM_LDFLAGS) $(version_flags)
instrumentation_libnanox_max_api_la_LIBADD=$(instrumentation_max_core_LIBS)
instrumentation_libnanox_max_api_la_SOURCES=$(max_api_sources)

instrumentation_libnanox_pe_max_la_CPPFLAGS=$(common_instrumentation_CPPFLAGS) @maxelerinc@
instrumentation_libnanox_pe_max_la_CXXFLAGS=$(common_instrumentation_CXXFLAGS)
instrumentation_libnanox_pe_max_la_LDFLAGS=$(AM_LDFLAGS) $(ld_plugin_flags)
instrumentation_libnanox_pe_max_la_LIBADD=$(instrumentation_max_core_LIBS)
instrumentation_libnanox_pe_max_la_SOURCES=$(pe_max_sources)
endif

if is_performance_enabled
noinst_LTLIBRARIES += performance/libmaxeler.la
performance_LTLIBRARIES += performance/libnanox-max-api.la performance/libnanox-pe-max.la

performance_libmaxeler_la_CPPFLAGS=$(common_performance_CPPFLAGS) @maxelerinc@
performance_libmaxeler_la_CXXFLAGS=$(common_performance_CXXFLAGS)
performance_libmaxeler_la_LDFLAGS=$(AM_LDFLAGS) @maxlib@ @maxoslib@
performance_libmaxeler_la_LIBADD=@maxlibs@ @maxoslibs@
performance_libmaxeler_la_SOURCES=$(max_sources)

performance_libnanox_max_api_la_CPPFLAGS=$(common_performance_CPPFLAGS) $(api_includes) @maxelerinc@
performance_libnanox_max_api_la_CXXFLAGS=$(common_performance_CXXFLAGS)
performance_libnanox_max_api_la_LDFLAGS=$(AM_LDFLAGS) $(version_flags)
performance_libnanox_max_api_la_LIBADD=$(performance_max_core_LIBS)
performance_libnanox_max_api_la_SOURCES=$(max_api_sources)

performance_libnanox_pe_max_la_CPPFLAGS=$(common_performance_CPPFLAGS) @maxelerinc@
performance_libnanox_pe_max_la_CXXFLAGS=$(common_performance_CXXFLAGS)
performance_libnanox_pe_max_la_LDFLAGS=$(AM_LDFLAGS) $(ld_plugin_flags)
performance_libnanox_pe_max_la_LIBADD=$(performance_max_core_LIBS)
performance_libnanox_pe_max_la_SOURCES=$(pe_max_sources)
endif

