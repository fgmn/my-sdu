# Use normal make for this Makefile
#
# Makefile for:
#	coff2noff -- converts a normal MIPS executable into a Nachos executable
#	disassemble -- disassembles a normal MIPS executable 
#
# Copyright (c) 1992 The Regents of the University of California.
# All rights reserved.  See copyright.h for copyright notice and limitation 
# of liability and disclaimer of warranty provisions.

ifndef MAKEFILE_BIN
define MAKEFILE_BIN
yes
endef

include ../Makefile.dep

CFILES = coff2noff.c coff2flat.c

# Define targets.  This must precede Makefile.common because
# it will define the target nachos, and we don't want that to
# be the default target here

# Define targets based on what architecture we're on.  The disassemble
# program doesn't deal with BIG_ENDIAN, as in the SPARC, yet.

ifeq (,$(findstring HOST_MIPS,$(HOST)))
targets = $(bin_dir)/coff2noff $(bin_dir)/coff2flat
else
targets = $(bin_dir)/coff2noff $(bin_dir)/coff2flat $(bin_dir)/disassemble 
CFILES += out.c opstrings.c
endif

all : $(targets)

# Makefile.common contains all the rules.

include ../Makefile.common

INCPATH += -I. -I../threads
CFLAGS = $(INCPATH) $(DEFINES) $(HOST)
CC=gcc
LD=gcc

# converts a COFF file to Nachos object format
$(bin_dir)/coff2noff: $(obj_dir)/coff2noff.o

# converts a COFF file to flat object format
$(bin_dir)/coff2flat: $(obj_dir)/coff2flat.o

# dis-assembles a COFF file
$(bin_dir)/disassemble: $(obj_dir)/out.o $(obj_dir)/opstrings.o

endif # MAKEFILE_BIN





