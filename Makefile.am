SUBDIRS = .
ACLOCAL_AMFLAGS = -I m4

commoncppflags = -std=c++11 -O2 -I/usr/local/include -Ithird_party -I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include/ -I/usr/include/gdk-pixbuf-2.0

#
#	Libraries
#
lib_LTLIBRARIES = libwpn-lnotify.la

nobase_dist_include_HEADERS = \
	wpn-lnotify.h
common_src = 
commonlibs = -L/usr/local/lib/ -lnotify

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

#
#	Binaries
#
bin_PROGRAMS = wpn-lnotify


#
#	wpn-lnotify
#
wpn_lnotify_SOURCES= \
	test/main.cpp \
	$(common_src)

wpn_lnotify_LDADD = $(commonlibs) -lwpn-lnotify
wpn_lnotify_CPPFLAGS = $(commoncppflags)



