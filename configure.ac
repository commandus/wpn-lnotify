AC_PREREQ([2.63])
AC_INIT([libwpn-lnotify], [0.1], [andrei.i.ivanov@commandus.com])
AM_INIT_AUTOMAKE([1.11 foreign no-define subdir-objects])

LT_PREREQ([2.2])
LT_INIT([dlopen shared])

AC_CONFIG_FILES([Makefile])
AC_CONFIG_SRCDIR([wpn-lnotify.cpp])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIR([m4])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_CPP
AC_PROG_INSTALL

LDFLAGS="$LDFLAGS -L/usr/local/lib"

# Checks for libraries. Add /usr/local/lib for OS X
AC_CHECK_LIB([notify], [notify_init], [], AC_MSG_ERROR([*** linbotify is required ***]))

# Checks for header files. Add /usr/local/include for IS X.
CFLAGS="$CFLAGS -I/usr/local/include -I/usr/include -I/usr/include/glib-2.0"
AC_CHECK_HEADERS([libnotify/notify.h fcntl.h limits.h stdint.h string.h sys/time.h termios.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
# AC_CHECK_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_INT32_T
AC_TYPE_INT64_T
AC_TYPE_SIZE_T
AC_TYPE_UINT64_T

# Checks for library functions.
AC_FUNC_FORK
AC_CHECK_FUNCS([memmove memset])

AC_OUTPUT
