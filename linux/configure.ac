#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.68])
AC_INIT([TuxDroidServer], [0.0.1], [joel.matteotti@free.fr])

AC_CONFIG_SRCDIR([../src/TuxServer.c])
AC_CONFIG_SRCDIR([../src/TuxDriver.c])
AC_CONFIG_SRCDIR([../src/TuxAudio.c])
AC_CONFIG_SRCDIR([../src/TuxDownloader.c])
AC_CONFIG_SRCDIR([../src/TuxLogger.c])
AC_CONFIG_SRCDIR([../src/TuxStrings.c])
AC_CONFIG_SRCDIR([../src/TuxTime.c])
AC_CONFIG_SRCDIR([../src/TuxUtils.c])
AC_CONFIG_SRCDIR([../src/TuxCommands.c])
AC_CONFIG_SRCDIR([../src/TuxDroid.c])
AC_CONFIG_SRCDIR([../src/TuxASR.c])


AC_CONFIG_SRCDIR([../include/TuxVersion.h])
AC_CONFIG_SRCDIR([../include/TuxServer.h])
AC_CONFIG_SRCDIR([../include/TuxDriver.h])
AC_CONFIG_SRCDIR([../include/TuxAudio.h])
AC_CONFIG_SRCDIR([../include/TuxAttitune.h])
AC_CONFIG_SRCDIR([../include/TuxLogger.h])
AC_CONFIG_SRCDIR([../include/TuxStrings.h])
AC_CONFIG_SRCDIR([../include/TuxTime.h])
AC_CONFIG_SRCDIR([../include/TuxUtils.h])
AC_CONFIG_SRCDIR([../include/TuxDownloader.h])
AC_CONFIG_SRCDIR([../include/TuxTypes.h])
AC_CONFIG_SRCDIR([../include/TuxCommands.h])
AC_CONFIG_SRCDIR([../include/TuxDroid.h])
AC_CONFIG_SRCDIR([../include/TuxASR.h])



# Checks for programs.
AC_PROG_CC

# Checks for header files.
AC_FUNC_ALLOCA
AC_CHECK_HEADERS([inttypes.h limits.h malloc.h memory.h stddef.h stdint.h stdlib.h string.h strings.h unistd.h sys/types.h netinet/in.h sys/un.h arpa/inet.h sys/socket.h])


# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_INT8_T
AC_TYPE_INT16_T
AC_TYPE_INT32_T
AC_TYPE_INT64_T

AC_TYPE_UINT8_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT64_T

AC_TYPE_MODE_T
AC_TYPE_PID_T
AC_TYPE_SIZE_T

AC_CHECK_TYPES([ptrdiff_t])

# Checks for library functions.
AC_FUNC_MALLOC


# Checks for libraries.

ARCH=`getconf LONG_BIT`
if test "${ARCH}" = "32" ; then
    LIBFMOD_EMBEDDED=./libfmodex.so
else
    LIBFMOD_EMBEDDED=./libfmodex64.so
fi


dnl if success, define LIBCURL="-lcurl" and LIBCURL_CPPFLAGS="-I/some/where"
LIBCURL_CHECK_CONFIG(yes,,,[AC_MSG_FAILURE("The CURL library or its headers are missing")])

dnl if success, define XML_LIBS and XML_CPPFLAGS
AM_PATH_XML2(,,[AC_MSG_FAILURE("The XML2 library or its headers are missing.")])

dnl if lib found, adds "-lLIB" to $LIBS
AC_CHECK_LIB(fmodex, FMOD_System_Create,,[LIBS+="${LIBFMOD_EMBEDDED} "])

dnl if lib found, adds "-lLIB" to $LIBS
AC_CHECK_LIB(FLAC, FLAC__StreamEncoder,,[LIBS+="-lFLAC "])



LIBS+="${XML_LIBS} ${LIBCURL} -lm -pthread -ldl"

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
