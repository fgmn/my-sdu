# NOTE: this is a GNU Makefile.  You must use "gmake" rather than "make".
#
# Makefile for the multiprogramming assignment
#    Defines set up assuming multiprogramming is done before the file system.
#    If not, use the "filesys first"  defines below. 
#
#
# Copyright (c) 1992 The Regents of the University of California.
# All rights reserved.  See copyright.h for copyright notice and limitation 
# of liability and disclaimer of warranty provisions.

ifndef MAKEFILE_USERPROG
define MAKEFILE_USERPROG
yes
endef

# If the filesystem assignment is done before userprog, then
# uncomment the include below

include ../threads/Makefile.local
# include ../filesys/Makefile.local
include ../userprog/Makefile.local
include ../Makefile.dep
include ../Makefile.common

endif # MAKEFILE_USERPROG
