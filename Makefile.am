ACLOCAL_AMFLAGS=-I m4

lib_LTLIBRARIES=libembedpy-1.0.la
libembedpy_1_0_la_SOURCES=asm/asm.cc parser/parser.cc parser/ast.cc
libembedpy_1_0_la_includedir=$(includedir)/embedpy
libembedpy_1_0_la_include_HEADERS=include/embedpy/ast.h

bin_PROGRAMS=embedpy
embedpy_SOURCES=cli/main.cc
embedpy_LDADD=libembedpy-1.0.la
