SUBDIRS = .
ACLOCAL_AMFLAGS = -I m4

commoncppflags = -std=c++11 -O2 -I/usr/local/include -Ithird_party

nodist_wpn_SOURCES =
BUILT_SOURCES = $(gengrpcs)
CLEANFILES = $(gengrpcs)

#
#	Binaries
#
bin_PROGRAMS = wpn-lnotify

nobase_dist_include_HEADERS = \
	wpn-lnotify-config.h  wpn-lnotify.h \
	$(gengrpcs_h)
common_src = 

commonlibs = -L/usr/local/lib/ -largtable2 -lnotify

#
#	wpn
#
wpn_lnotify_SOURCES= \
	wpn-lnotify-config.cpp wpn-lnotify.cpp \
	$(common_src)

wpn_lnotify_LDADD = $(commonlibs)
wpn_lnotify_CPPFLAGS = $(commoncppflags)

#
#	Configs, readme, CMake etc.
#
configdir = $(datadir)
dist_config_DATA = \
	$(proto) \
	COPYING  CODE_OF_CONDUCT.md  CONTRIBUTING.md  HISTORY  README.md autogen.sh .kdev_include_paths
