#-----------------------------------------------------------------------------
# GNU makefile for building PGSBOX 4.4.
#
# Summary of the main targets
# ---------------------------
#   build:     Build the library.
#
#   clean:     Delete intermediate object files.
#
#   cleaner:   clean, and also delete the test executables.
#
#   realclean (or cleanest):  cleaner, and also delete the pgsbox and
#              WCSLIB object libraries.
#
#   check (or test): Compile and run the test programs.  By default they are
#              executed in batch mode; use
#
#                gmake MODE=interactive check
#
#              to run them interactively.
#
#   tests:     Compile the test programs (but don't run them).
#
# Notes:
#   1) If you need to make changes then preferably modify ../makedefs.in
#      instead and re-run configure.
#
#   2) In compiling the test programs, this makefile assumes that the
#      WCSLIB 4.4 sources reside in ../C and ../Fortran (as in the
#      distribution kit).
#
# Author: Mark Calabretta, Australia Telescope National Facility
# http://www.atnf.csiro.au/~mcalabre/index.html
# $Id: GNUmakefile,v 1.4 2009/09/14 20:25:25 irby Exp $
#-----------------------------------------------------------------------------
# Get configure settings.
include ../makedefs

PGSBOXLIB := libpgsbox-$V.a
MODULES   := pgsbox.o \
             cpgsbox.o \
             pgwcsl.o \
             pgcrfn.o

LIBWCS   := ../C/$(LIBWCS)

CPPFLAGS += -I. -I.. -I../C

vpath %.h   ..:../C
vpath %.in  ..
vpath %.inc ../Fortran

# Pattern rules
#--------------

%.o : %.c
	-@ echo ''
	   $(CC) $(CPPFLAGS) $(CFLAGS) -c $<

%.i : %.c
	-@ echo ''
	-@ $(RM) $@
	   $(CPP) $(CPPFLAGS) $(CFLAGS) $< > $@

%.d : %.c
	-@ echo ''
	-@ $(CPP) $(CPPFLAGS) $(CFLAGS) $< | \
	   sed -n -e 's|^# 1 "\([^/].*\.h\)".*|\1|p' | \
	   sort -u

%.o : %.F
	-@ echo ''
	   $(FC) $(FFLAGS) -I../Fortran -c -o $@ $<

%.o : %.f
	-@ echo ''
	   $(FC) $(FFLAGS) -I../Fortran -c -o $@ $<

# Static and static pattern rules
#--------------------------------

.PHONY : build check clean cleaner cleanest install realclean test tests \
         wcslib

build : lib

lib : $(PGSBOXLIB)

$(PGSBOXLIB) : $(MODULES:%=$(PGSBOXLIB)(%))
	-@ echo ''
	   $(RANLIB) $@

clean :
	- $(RM) *.o *.i a.out core $(EXTRA_CLEAN)

cleaner : clean
	- $(RM) pgtest cpgtest

realclean cleanest : cleaner
	- $(RM) $(PGSBOXLIB) $(LIBWCS)

check test : tests
	-@ echo ''
	-@ $(TIMER)
	-@ echo 'Running the PGSBOX test program, PGTEST:'
	-@ if [ '$(MODE)' = interactive ] ; then \
	     ./pgtest ; \
	   else \
	     ./pgtest < /dev/null ; \
	   fi
	-@ echo ''
	-@ echo ''
	-@ $(TIMER)
	-@ echo 'Running the cpgsbox() test program, cpgtest:'
	-@ if [ '$(MODE)' = interactive ] ; then \
	     ./cpgtest ; \
	   else \
	     ./cpgtest < /dev/null ; \
	   fi

tests : wcslib pgtest cpgtest

wcslib : $(LIBWCS)

$(LIBWCS) :
	-@ echo ''
	   $(MAKE) -C ../Fortran lib

install : build
	   $(MAKE) -C ../Fortran install
	   $(INSTALL) -m 644 $(PGSBOXLIB) $(LIBDIR)
	   $(RANLIB) $(LIBDIR)/$(PGSBOXLIB)
	   $(RM) $(LIBDIR)/libpgsbox.a
	   $(LN_S) $(LIBDIR)/$(PGSBOXLIB) $(LIBDIR)/libpgsbox.a
	   $(INSTALL) -m 444 *.h $(INCDIR)

pgtest : pgtest.o lngvel.o fscan.o $(PGSBOXLIB) $(LIBWCS)
	-@ rm -f $@
	   $(FC) $(FFLAGS) $(LDFLAGS) -o $@ $^ $(PGPLOTLIB) $(LIBS)

cpgtest : cpgtest.c lngvel.o fscan.o $(PGSBOXLIB) $(LIBWCS)
	-@ rm -f $@
	   $(CC) $(CPPFLAGS) $(PGPLOTINC) $(CFLAGS) $(LDFLAGS) \
	     -o $@ $^ $(PGPLOTLIB) $(FLIBS) $(LIBS)

GNUmakefile : ../makedefs ;

../makedefs ../wcsconfig_f77.h : makedefs.in wcsconfig_f77.h.in \
    ../config.status
	-@ $(RM) ../wcsconfig_f77.h
	   cd .. && config.status

# Dependencies
#-------------

$(PGSBOXLIB)(cpgsbox.o) : cpgsbox.h wcsconfig_f77.h
$(PGSBOXLIB)(pgwcsl.o)  : cel.h lin.h prj.h spc.h sph.h spx.h tab.h wcs.h \
                          wcsconfig_f77.h

pgtest.o  : wcs.inc
cpgtest.o : cel.h cpgsbox.h lin.h prj.h spc.h spx.h tab.h wcs.h wcsfix.h \
            wcsconfig_f77.h
