# NOTE: this is a GNU Makefile.  You must use "gmake" rather than "make".
#
# Makefile for the virtual memory assignment
#    Defines set up assuming the virtual memory assignment is done before 
# 	the file system assignment. If not, use the "filesys first"  
#	defines below. 
#
# Also, if you want to simplify the translation so it assumes
# only linear page tables, don't define USE_TLB.
#
# Copyright (c) 1992 The Regents of the University of California.
# All rights reserved.  See copyright.h for copyright notice and limitation 
# of liability and disclaimer of warranty provisions.

ifndef MAKEFILE_VM
define MAKEFILE_VM
yes
endef

# If the filesystem assignment is done before userprog, then
# uncomment the include below

include ../threads/Makefile.local
# include ../filesys/Makefile.local
include ../userprog/Makefile.local
include ../vm/Makefile.local

include ../Makefile.dep
include ../Makefile.common

endif # MAKEFILE_VM
