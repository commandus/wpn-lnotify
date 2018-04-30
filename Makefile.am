SUBDIRS = .
ACLOCAL_AMFLAGS = -I m4

commoncppflags = -std=c++11 -O2 -I/usr/local/include -Ithird_party

#
#	Libraries
#
lib_LTLIBRARIES = libwpn-lnotify.la

nobase_dist_include_HEADERS = \
	wpn-lnotify.h
common_src = 
commonlibs = -L/usr/local/lib/ -largtable2 -lnotify

#
#	libwpn-lnotify.la
#
libwpn_lnotify_la_SOURCES = \
	wpn-lnotify.cpp \
	$(common_src)
libwpn_lnotify_la_CPPFLAGS = $(commoncppflags)
libwpn_lnotify_la_LIBADD = $(commonlibs)

#
#	Configs, readme, CMake etc.
#
configdir = $(datadir)
dist_config_DATA = \
	$(proto) \
	COPYING  CODE_OF_CONDUCT.md  CONTRIBUTING.md  HISTORY  README.md autogen.sh .kdev_include_paths
