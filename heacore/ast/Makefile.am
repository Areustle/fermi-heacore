## Process this file with automake to produce Makefile.in

# First declare various groups of files.  These were initially extracted
# from the grp.make file, as constructed by the SDT newdev command
GRP_C_ROUTINES = \
    axis.c \
    box.c \
    c2f77.c \
    channel.c \
    circle.c \
    cmpframe.c \
    cmpmap.c \
    cmpregion.c \
    dsbspecframe.c \
    dssmap.c \
    ellipse.c \
    error.c \
    fbox.c \
    fchannel.c \
    fcircle.c \
    fcmpframe.c \
    fcmpmap.c \
    fcmpregion.c \
    fdsbspecframe.c \
    fdssmap.c \
    fellipse.c \
    ferror.c \
    ffitschan.c \
    ffluxframe.c \
    fframe.c \
    fframeset.c \
    fgrismmap.c \
    finterval.c \
    fintramap.c \
    fitschan.c \
    fkeymap.c \
    flutmap.c \
    fluxframe.c \
    fmapping.c \
    fmathmap.c \
    fmatrixmap.c \
    fnullregion.c \
    fobject.c \
    fpcdmap.c \
    fpermmap.c \
    fplot.c \
    fplot3d.c \
    fpointlist.c \
    fpolygon.c \
    fpolymap.c \
    fprism.c \
    frame.c \
    frameset.c \
    fratemap.c \
    fnormmap.c \
    fregion.c \
    fshiftmap.c \
    fskyframe.c \
    fslamap.c \
    fspecfluxframe.c \
    fspecframe.c \
    fspecmap.c \
    ftimeframe.c \
    ftimemap.c \
    fsphmap.c \
    ftranmap.c \
    fselectormap.c \
    fswitchmap.c \
    funitmap.c \
    fwcsmap.c \
    fwinmap.c \
    fxmlchan.c \
    fzoommap.c \
    globals.c \
    grismmap.c \
    interval.c \
    intramap.c \
    keymap.c \
    loader.c \
    lutmap.c \
    mapping.c \
    mathmap.c \
    matrixmap.c \
    memory.c \
    nullregion.c \
    object.c \
    pcdmap.c \
    permmap.c \
    plot.c \
    plot3d.c \
    pointlist.c \
    pointset.c \
    polygon.c \
    polymap.c \
    prism.c \
    normmap.c \
    ratemap.c \
    region.c \
    shiftmap.c \
    skyaxis.c \
    skyframe.c \
    slamap.c \
    specfluxframe.c \
    specframe.c \
    specmap.c \
    sphmap.c \
    stc.c \
    stcresourceprofile.c \
    stcsearchlocation.c \
    stccatalogentrylocation.c \
    stcobsdatalocation.c \
    fstc.c \
    fstcresourceprofile.c \
    fstcsearchlocation.c \
    fstccatalogentrylocation.c \
    fstcobsdatalocation.c \
    timeframe.c \
    timemap.c \
    tranmap.c \
    selectormap.c \
    switchmap.c \
    unit.c \
    unitmap.c \
    wcsmap.c \
    winmap.c \
    xml.c \
    xmlchan.c \
    zoommap.c

# Header files which contribute to the "ast.h" file, organised to correspond
# with the class hierarchy.
AST_H_FILES = \
    xml.h \
    wcstrig.h \
    proj.h \
    memory.h \
    error.h \
    globals.h \
    unit.h \
    ast_err.h \
    version.h \
    object.h \
       keymap.h \
       pointset.h \
       axis.h \
          skyaxis.h \
       mapping.h \
          cmpmap.h \
          dssmap.h \
          grismmap.h \
          intramap.h \
          lutmap.h \
          mathmap.h \
          matrixmap.h \
          pcdmap.h \
          permmap.h \
          polymap.h \
          ratemap.h \
          normmap.h \
          shiftmap.h \
          slamap.h \
          specmap.h \
          sphmap.h \
          timemap.h \
          selectormap.h \
          switchmap.h \
          tranmap.h \
          unitmap.h \
          wcsmap.h \
          winmap.h \
          zoommap.h \
          frame.h \
             cmpframe.h \
                specfluxframe.h \
             fluxframe.h \
             frameset.h \
                plot.h \
                   plot3d.h \
             skyframe.h \
             specframe.h \
                dsbspecframe.h \
             region.h \
                box.h \
                circle.h \
                cmpregion.h \
                ellipse.h \
                interval.h \
                nullregion.h \
                pointlist.h \
                polygon.h \
                prism.h \
                stc.h \
                    stcresourceprofile.h \
                    stcsearchlocation.h \
                    stccatalogentrylocation.h \
                    stcobsdatalocation.h \
             timeframe.h \
       channel.h \
          fitschan.h \
          xmlchan.h 

# All the (C) include files required to build the library.
GRP_C_INCLUDE_FILES = \
    $(AST_H_FILES) \
    c2f77.h \
    ems.h \
    err.h \
    Ers.h \
    f77.h \
    grf.h \
    grf3d.h \
    pg3d.h \
    loader.h \
    skyaxis.h \
    pal.h \
    slamac.h \
    stc.h \
    stcresourceprofile.h \
    stcsearchlocation.h \
    stccatalogentrylocation.h \
    stcobsdatalocation.h \
    wcsmath.h \
    wcstrig.h \
    xmlchan.h

# The following list should include AST_PAR, but that must not be
# distributed, and so it is listed separately in
# nodist_libast_la_SOURCES below.
GRP_F_INCLUDE_FILES = \
    GRF_PAR \
    AST_ERR

## Following declaration isn't used
## LATEX_DOCUMENTATION_FILES = \
##     sun210.tex \
##     sun211.tex

DOCUMENTATION_PRODUCTS = $(PAPER_DOCUMENTATION) $(HYPER_DOCUMENTATION)
PAPER_DOCUMENTATION = sun210.tex sun211.tex sun210.ps sun211.ps
HYPER_DOCUMENTATION = sun210.htx_tar sun211.htx_tar

POSTSCRIPT_FIGURES = \
    cmpframe.eps \
    complex.eps \
    frames.eps \
    frameset.eps \
    fronta.eps \
    fronta_bw.eps \
    frontb.eps \
    frontb_bw.eps \
    frontc.eps \
    frontc_bw.eps \
    fsalign.eps \
    fsconvert.eps \
    fsexample.eps \
    fsmerge.eps \
    fsremap.eps \
    gridplot.eps \
    gridplot_bw.eps \
    mapping.eps \
    overgrid.eps \
    overgrid_bw.eps \
    parallel.eps \
    series.eps \
    simpexamp.eps

WCSLIB_FILES = \
    COPYING.LIB \
    proj.c \
    tpn.c \
    proj.h \
    wcstrig.c \
    wcsmath.h \
    wcstrig.h

PAL_FILES = \
    COPYING.LIB \
    pal.c \
    pal.h \
    slamac.h


bin_SCRIPTS = ast_link
dist_bin_SCRIPTS = ast_link_adam
noinst_SCRIPTS = ast_cpp
dist_noinst_SCRIPTS = makeh
# Scripts are not distributed by default (since they might be derived objects)
# Add these to the distribution below.  In fact, it would be useful
# and straightforward  to make ast_link{,_adam} derived, since they
# could then have installation directories painlessly edited in to
# them.  This might be a requirement for scripts which supported
# linking against shared libraries.

# Headers required by library users.  Both of the following lines
# indicate headers which are installed.
include_HEADERS = GRF_PAR grf.h
# Following are generated, so should not be distributed.
nodist_include_HEADERS = ast.h AST_PAR
include_MESSAGES = AST_ERR ast_err.h

lib_LTLIBRARIES = \
    libast.la \
    libast_err.la \
    libast_ems.la \
    libast_drama.la \
    libast_grf3d.la \
    libast_grf_2.0.la \
    libast_grf_3.2.la \
    libast_pgplot.la \
    libast_pgplot3d.la \
    libast_pal.la \
    libast_wcslib.la


stardocs_DATA = @STAR_LATEX_DOCUMENTATION@
dist_starnews_DATA = ast.news
dist_pkgdata_DATA = LICENCE

# Make all library code position independent by default. This is handy for
# creating shareable libraries from the static ones (Java JNI libraries). 
# Note we do not simply set the AM_CFLAGS variable, as this would also
# apply to programs compiled without using libtool, possibly causing the
# compilation to fail.

if !NOTHREADS

if !NOPIC
libast_la_CFLAGS = $(AM_CFLAGS) -prefer-pic -DTHREAD_SAFE
else
libast_la_CFLAGS = $(AM_CFLAGS) -DTHREAD_SAFE
endif

else
libast_la_CFLAGS = $(AM_CFLAGS) -prefer-pic

endif

if !NOPIC
libast_err_la_CFLAGS = $(AM_CFLAGS) -prefer-pic
libast_ems_la_CFLAGS = $(AM_CFLAGS) -prefer-pic
libast_drama_la_CFLAGS = $(AM_CFLAGS) -prefer-pic
libast_grf3d_la_CFLAGS = $(AM_CFLAGS) -prefer-pic
libast_grf_2.0_la_CFLAGS = $(AM_CFLAGS) -prefer-pic
libast_grf_3.2_la_CFLAGS = $(AM_CFLAGS) -prefer-pic
libast_pgplot_la_CFLAGS = $(AM_CFLAGS) -prefer-pic
libast_pgplot3d_la_CFLAGS = $(AM_CFLAGS) -prefer-pic
libast_pal_la_CFLAGS = $(AM_CFLAGS) -prefer-pic
libast_wcslib_la_CFLAGS = $(AM_CFLAGS) -prefer-pic
endif

#  The module containing the main AST classes
libast_la_SOURCES = \
    $(GRP_C_ROUTINES) \
    $(GRP_C_INCLUDE_FILES) \
    $(GRP_F_INCLUDE_FILES) \
    ast_err.h

# AST_PAR is really part of GRP_F_INCLUDE_FILES, but it must not be
# distributed, so list it separately.
nodist_libast_la_SOURCES = \
    ast.h \
    AST_PAR

#  The default error reporting module.
libast_err_la_SOURCES = err_null.c

#  The error reporting module that uses EMS to deliver errors.
libast_ems_la_SOURCES = err_ems.c

#  The error reporting module that uses DRAMA Ers to deliver errors.
libast_drama_la_SOURCES = err_drama.c

#  The module containing null implementations of the 3D graphics routines 
#  required by AST 
libast_grf3d_la_SOURCES = grf3d.c 

#  The module containing null implementations of the graphics routines 
#  required by AST V2.0
libast_grf_2_0_la_SOURCES = grf_2.0.c 

#  The module containing null implementations of the graphics routines 
#  added by AST V3.2 and not present in V2.0
libast_grf_3_2_la_SOURCES = grf_3.2.c 

#  The graphics module that uses PGPLOT for 2D graphical output.
libast_pgplot_la_SOURCES = grf_pgplot.c 

#  The graphics module that uses PGPLOT for 3D graphical output.
libast_pgplot3d_la_SOURCES = grf3d_pgplot.c

#  Modules adapted from the wcslib library.
libast_wcslib_la_SOURCES = proj.c wcstrig.c tpn.c

#  Modules supplied by tpoint.
libast_pal_la_SOURCES = pal.c

# The following files are built by the targets in this makefile.
MAINTAINERCLEANFILES = version.h builddocs addversion \
	ast.h $(DOCUMENTATION_PRODUCTS)
CLEANFILES = AST_PAR ast.h

# Special cases start here

# The AST_PAR include file is produced by compiling the astbad.c
# program and editing its output into the ast_par.source file (while
# also changing the "E" exponent character to a "D").  The astbad.c
# and ast_par.source must be distributed (the generation of the
# AST__BAD value must be done on the build host) but not installed.
noinst_PROGRAMS = astbad
astbad_SOURCES = astbad.c pointset.h
AST_PAR: ast_par.source astbad
	sed -e 's/<AST__BAD>/'`./astbad | tr 'E' 'D'`'/' ast_par.source >$@

# ast_link is generated from ast_link.in; ast_link_adam does not
# need configuration, and so is not mentioned in AC_CONFIG_FILES within
# configure.ac, and so is not distributed automatically.
#
# makeh is required in order to build ast.h after distribution (see below).
EXTRA_DIST = ast_par.source sun210_figures sun211_figures

# ast.h depends on the error-facility files.  ast.h _could_ be made
# before distribution, but since it's generated from other distributed
# files, it's more robust to distribute makeh and make ast.h on the
# build host.
ast.h: $(AST_H_FILES) ast_err.h makeh config.h
	@PERL@ ./makeh -s $(srcdir) $(AST_H_FILES) >$@

# AST_ERR and ast_err.h are `generated source files', and so must be
# generated before any other compilations are done.  Note that these
# files are generated on the distribution host, and so this
# declaration has no effect post-distribution.
#
# AST_PAR is also a generated source file, but it should _not_ be
# included in the list of BUILT_SOURCES, otherwise `make' tries to make
# it before it makes the `astbad' program it depends on.  Instead,
# just rely on the dependencies expressed in the main body above to
# have AST_PAR built before it is needed.
#
# version.h is included by object.h, and thus indirectly by most modules.
# It's most straightforward to build it at the beginning.
BUILT_SOURCES = AST_ERR ast_err.h version.h

#  Form second links to the main object library (static and shared). This is 
#  used when a second pass through the library is needed during linking.
#  Do not forget to change the contents of the libast_pass2.la file to refer
#  to the pass2 libraries rather than the original libraries. Use target 
#  install-exec-hook rather than install-exec-local so that the soft links 
#  and files are created *after* the main library has been installed. 
install-exec-hook: libast.la
	$(mkdir_p) $(DESTDIR)$(libdir) 
	cd $(DESTDIR)$(libdir); \
	for f in `ls libast.*`; do \
	   ff=`echo $$f | sed -e 's/libast/libast_pass2/'`; \
	   if test -f "$$ff"; then rm "$$ff"; fi; \
	   $(LN_S) $$f $$ff; \
	   $(MANIFEST) && echo "MANIFEST:$(DESTDIR)$(libdir)/$$ff" || :; \
	done; \
	if test -f "libast.la"; then \
	   if test -f "libast_pass2.la"; then rm "libast_pass2.la"; fi; \
	   sed -e 's/libast/libast_pass2/g' libast.la > libast_pass2.la; \
	fi

# Make pre-distribution files.  These are files which are required for
# building the distribution, but are not themselves distributed.
# The source files here should be mentioned in STAR_PREDIST_SOURCES in
# configure.ac
@PREDIST@predist_subs = sed \
@PREDIST@    -e 's,@PACKAGE_VERSION\@,$(PACKAGE_VERSION),' \
@PREDIST@    -e 's,@PACKAGE_VERSION_MAJOR\@,$(PACKAGE_VERSION_MAJOR),' \
@PREDIST@    -e 's,@PACKAGE_VERSION_MINOR\@,$(PACKAGE_VERSION_MINOR),' \
@PREDIST@    -e 's,@PACKAGE_VERSION_RELEASE\@,$(PACKAGE_VERSION_RELEASE),' \
@PREDIST@    -e 's,@PERL\@,$(PERL),'
@PREDIST@version.h: version.h.in configure.ac
@PREDIST@	rm -f $@; $(predist_subs) version.h.in >$@
@PREDIST@builddocs: builddocs.in configure.ac
@PREDIST@	rm -f $@; $(predist_subs) builddocs.in >$@; chmod +x $@
@PREDIST@addversion: addversion.in configure.ac
@PREDIST@	rm -f $@; $(predist_subs) addversion.in >$@; chmod +x $@

# Documentation
@PREDIST@$(PAPER_DOCUMENTATION): sun211_figures builddocs addversion
@PREDIST@	./builddocs

# The contents of the sun211_figures directory is identical to that
# sun210_figures
sun211_figures: sun210_figures
	$(LN_S) sun210_figures sun211_figures

# Installation check

TESTS = ast_test
check_PROGRAMS = ast_test
ast_test_SOURCES = ast_test.c

#ast_test_LDADD = `ast_link`
# Expand ast_link to avoid libast_pass2, which causes problems for Solaris
ast_test_LDADD = libast.la libast_pal.la libast_grf_3.2.la libast_wcslib.la libast_grf_2.0.la libast_grf3d.la libast_err.la -lm
