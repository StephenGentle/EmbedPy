ACLOCAL_AMFLAGS=-I m4

GLOBAL_CXXFLAGS=-std=c++11 -W -Wall -pedantic

lib_LTLIBRARIES=libembedpy-1.0.la
libembedpy_1_0_la_SOURCES=asm/asm.cc                            \
                          parser/parser.cc                      \
                          parser/lexer.cc                       \
                          parser/token.cc                       \
                          compiler/fileCompilerContext.cc       \
                          compiler/interactiveCompilerContext.cc

libembedpy_1_0_la_includedir=$(includedir)/embedpy
libembedpy_1_0_la_include_HEADERS=  include/embedpy/ast.h       \
                                    include/embedpy/context.h   \
                                    include/embedpy/error.h     \
                                    include/embedpy/token.h

libembedpy_1_0_la_CXXFLAGS=$(GLOBAL_CXXFLAGS) -Iinclude/embedpy

bin_PROGRAMS=embedpy
embedpy_SOURCES=cli/main.cc     \
                cli/cmdline.cc  \
                cli/cmdline.h
embedpy_LDADD=libembedpy-1.0.la
embedpy_CXXFLAGS=$(GLOBAL_CXXFLAGS) -Iinclude
