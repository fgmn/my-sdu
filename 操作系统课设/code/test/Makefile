#
# Makefile for building user programs to run on top of Nachos
#
# Several things to be aware of:
#
#    Nachos assumes that the location of the program startup routine (the
# 	location the kernel jumps to when the program initially starts up)
#       is at location 0.  This means: start.o must be the first .o passed 
# 	to ld, in order for the routine "Start" to be loaded at location 0
#

# Changes at Waterloo:  
#     o Cross-compilation is handled automatically by the Makefile now.
#         - when necessary, it assumes that /software/gcc_nachos exists, 
#           which is true on cayley, napier, descartes.
#
# This file has been changed for USQ use to add links into the arch/bin
# directory and to allow the creation of .s files from .c files

ifndef MAKEFILE_TEST
define MAKEFILE_TEST
yes
endef

# Machine dependent stuff
include ../Makefile.dep

real_bin_dir := $(bin_dir)

# If user programs generated on different platforms were
# interchangable, you could uncomment the following line, and
# then only a single set of user program files would be shared
# among architectures.  This should be the case (?).  But, it
# seems that at least user programs generated on the SPARC do
# not run correctly on MIPS versions of nachos...

#arch = $(mips_arch)

# GCCDIR will point to the location of the cross-compiler when
# appropriate.

CC = $(GCCDIR)gcc 
AS = $(GCCDIR)as 
LD = $(GCCDIR)ld

# User programs.  Add your own stuff here. 
# 
# Note:  The convention is that there is exactly one .c file per target.
#        The target is built by compiling the .c file and linking the 
#        corresponding .o with start.o.  If you want to have more than
#        one .c file per target, you will have to change stuff below.

targets = halt shell matmult sort bar exec join yield0 yield1 create fileTest

# Targest are put in the architecture specific 'bin' dir.

all = $(targets:%=$(bin_dir)/%)

all_coff = $(targets:%=$(obj_dir)/%.coff)
all_noff = $(all:%=%.noff)
all_flat = $(all:%=%.flat)

#all: $(all_noff) $(all_flat)
all: $(all_noff)

$(targets): % : $(bin_dir)/%
	ln -sf $(bin_dir)/$@ $@

CFILES = $(targets:%=%.c)

SFILES = start.s

include ../Makefile.common

INCDIR =-I../userprog -I../threads
CFLAGS = -G 0 -c $(INCDIR)

coff2noff = ../bin/$(real_bin_dir)/coff2noff
coff2flat = ../bin/$(real_bin_dir)/coff2flat

$(all_coff): $(obj_dir)/%.coff: $(obj_dir)/start.o $(obj_dir)/%.o
	@echo ">>> Linking" $(obj_dir)/$(notdir $@) "<<<"
	$(LD) $(LDFLAGS) $^ -o $(obj_dir)/$(notdir $@)

$(all_noff): $(bin_dir)/%.noff: $(obj_dir)/%.coff
	@echo ">>> Converting to noff file:" $@ "<<<"
	$(coff2noff) $^ $@
	ln -sf $@ $(notdir $@)


#$(all_flat): $(bin_dir)/%.flat: $(obj_dir)/%.coff
#	@echo ">>> Converting to flat file:" $@ "<<<"
#	$(coff2flat) $^ $@
#	ln -sf $@ $(notdir $@)	
	
#$(warning hhhhhhhhhhhhhhhhhhhhhhhhhhh $@)

%.s: %.c
	@echo ">>> Compiling .s file for" $< "<<<"
	$(CC) $(CFLAGS) -S -c -o $@ $<
#rename:
#	for f in `find . -type f -name makefile`; do mv $f `dirname $f`/Makefile; done
cleantest:
	rm *.noff *.flat arch/unknown-i386-linux/depends/*.*  arch/unknown-i386-linux/objects/*.* arch/unknown-i386-linux/bin/*.* 
endif # MAKEFILE_TEST
