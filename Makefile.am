AUTOMAKE_OPTIONS = subdir-objects
ACLOCAL_AMFLAGS = ${ACLOCAL_FLAGS}

bin_PROGRAMS = egrl

egrl_CPPFLAGS = \
	-Isrc/Util \
	-Isrc/Thirdparty

egrl_SOURCES = \
	src/Thirdparty/gzstream.C src/Thirdparty/gzstream.h \
	src/Util/DNAString.cpp src/Util/DNAString.h \
	src/Util/Quality.cpp src/Util/Quality.h \
	src/Util/SeqReader.cpp src/Util/SeqReader.h \
	src/Util/Util.cpp src/Util/Util.h \
	src/count.cpp src/count.h \
	src/cs_probe.cpp src/cs_probe.h \
	src/probe.cpp src/probe.h \
	src/cs_utils.cpp src/cs_utils.h \
  src/egrl.cpp

dist_noinst_SCRIPTS = autogen.sh
