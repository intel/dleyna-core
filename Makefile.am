libdleyna_coreincdir = $(includedir)/dleyna-1.0/libdleyna/core
connectordir = $(libdir)/dleyna-1.0/connectors
sysconfigdir = $(sysconfdir)

DLEYNA_CORE_VERSION = 1:0:0

AM_CFLAGS =	$(GLIB_CFLAGS)				\
		$(GIO_CFLAGS)				\
		$(GMODULE_CFLAGS)			\
		$(GUPNP_CFLAGS)				\
		-include config.h			\
		-DSYS_CONFIG_DIR="\"$(sysconfdir)\""	\
		-DCONNECTOR_DIR="\"$(connectordir)\""

ACLOCAL_AMFLAGS = -I m4 ${ACLOCAL_FLAGS}

lib_LTLIBRARIES = libdleyna-core-1.0.la

libdleyna_coreinc_HEADERS = 	src/connector.h		\
				src/connector-mgr.h	\
				src/control-point.h	\
				src/error.h		\
				src/log.h		\
				src/main-loop.h		\
				src/service-task.h	\
				src/settings.h		\
				src/task-atom.h		\
				src/task-processor.h


libdleyna_core_1_0_la_LDFLAGS =	-version-info $(DLEYNA_CORE_VERSION) \
				-no-undefined

libdleyna_core_1_0_la_SOURCES =	$(libdleyna_coreinc_HEADERS)	\
				src/connector-mgr.c		\
				src/error.c			\
				src/log.c			\
				src/main-loop.c			\
				src/service-task.c		\
				src/settings.c			\
				src/task-processor.c

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

maintainer-clean-local:
	rm -rf build-aux
