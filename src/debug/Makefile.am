YFLAGS = -d
LFLAGS =

AUTOMAKE_OPTIONS = foreign

noinst_LIBRARIES = libdebug.a

libdebug_a_SOURCES = \
    debugger.cc debugger.h \
    tracers.h \
    parsehelper.c parsehelper.h \
    lex.l lex.h \
    debugparse.y debugparse.h \
    debugtype.h \
    asm.cc asm.h \
    stdfuncs.cc stdfuncs.h

AM_CPPFLAGS = -I ..
