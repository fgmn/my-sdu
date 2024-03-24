# NOTE: this is a GNU Makefile.  You must use "gmake" rather than "make".
#
# Makefile for the file system assignment
#    Defines set up assuming multiprogramming and virtual memory done first.
#    If not, use the "bare bones"  defines below. 
#
# Copyright (c) 1992 The Regents of the University of California.
# All rights reserved.  See copyright.h for copyright notice and limitation 
# of liability and disclaimer of warranty provisions.

ifndef MAKEFILE_FILESYS
define MAKEFILE_FILESYS
yes
endef

# You can re-order the assignments.  If filesys comes before userprog, 
# just re-order and comment the includes below as appropriate.

include ../threads/Makefile.local
include ../filesys/Makefile.local
#include ../userprog/Makefile.local
#include ../vm/Makefile.local
#include ../filesys/Makefile.local

include ../Makefile.dep
include ../Makefile.common

endif # MAKEFILE_FILESYS