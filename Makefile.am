libdleyna_coreincdir = $(includedir)/dleyna-1.0/libdleyna/core
connectordir = $(libdir)/dleyna-1.0/connectors
sysconfigdir = $(sysconfdir)

DLEYNA_CORE_VERSION = 5:0:0

AM_CFLAGS =	$(GLIB_CFLAGS)				\
		$(GIO_CFLAGS)				\
		$(GMODULE_CFLAGS)			\
		$(GUPNP_CFLAGS)				\
		-include config.h			\
		-DSYS_CONFIG_DIR="\"$(sysconfdir)\""	\
		-DCONNECTOR_DIR="\"$(connectordir)\""

ACLOCAL_AMFLAGS = -I m4 ${ACLOCAL_FLAGS}

lib_LTLIBRARIES = libdleyna-core-1.0.la

libdleyna_coreinc_HEADERS = 	libdleyna/core/connector.h	\
				libdleyna/core/connector-mgr.h	\
				libdleyna/core/control-point.h	\
				libdleyna/core/core.h		\
				libdleyna/core/error.h		\
				libdleyna/core/log.h		\
				libdleyna/core/main-loop.h	\
				libdleyna/core/service-task.h	\
				libdleyna/core/settings.h	\
				libdleyna/core/task-atom.h	\
				libdleyna/core/task-processor.h	\
				libdleyna/core/white-list.h


libdleyna_core_1_0_la_LDFLAGS =	-version-info $(DLEYNA_CORE_VERSION) \
				-no-undefined

libdleyna_core_1_0_la_SOURCES =	$(libdleyna_coreinc_HEADERS)	\
				libdleyna/core/connector-mgr.c	\
				libdleyna/core/core.c		\
				libdleyna/core/error.c		\
				libdleyna/core/log.c		\
				libdleyna/core/main-loop.c	\
				libdleyna/core/service-task.c	\
				libdleyna/core/settings.c	\
				libdleyna/core/task-processor.c	\
				libdleyna/core/white-list.c

libdleyna_core_1_0_la_LIBADD =	$(GLIB_LIBS)	\
				$(GIO_LIBS)	\
				$(GMODULE_LIBS)	\
				$(GUPNP_LIBS)

MAINTAINERCLEANFILES =	Makefile.in		\
			aclocal.m4		\
			configure		\
			config.h.in		\
			config.h.in~		\
			build-aux/depcomp	\
			build-aux/compile	\
			build-aux/missing	\
			build-aux/install-sh

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = dleyna-core-1.0.pc

CLEANFILES = $(pkgconfig_DATA)
DISTCLEANFILES = $(pkgconfig_DATA)

EXTRA_DIST = meson.build meson_options.txt libdleyna/meson.build libdleyna/core/meson.build

maintainer-clean-local:
	rm -rf build-aux
