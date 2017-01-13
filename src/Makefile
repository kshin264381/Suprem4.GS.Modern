#----------------------------------------------------------------------
# Makefile for SUPREM-IV.GS
#
#   Makefile.proto                Version 1.27
#   Last Modification : 7/31/94 21:29:00
#
#  Original: Stephen Hansen (hansen@gloworm.stanford.edu)
#		Mon Feb  1 14:57:45 PST 1993
#
#----------------------------------------------------------------------

# change these lines when porting to other machines
#
# SYS should be one of {BSD, CONVEX, HPUX, CRAY}
# BSD - "vanilla" BSD4.2 system
# SYSV - vanilla system V unix
# CONVEX - like BSD but uses vectorizing Fortran sparse matrix routines
# HPUX - example of a non-BSD system
# CRAY - like HPUX, but uses vectorizing Fortran sparse matrix routines
SYS     = BSD

#.. Where are we?
HERE	 := $(realpath .)

# Detect cygwin
SYS := $(shell gcc -dumpmachine)
X11FLAGS := 
ifneq (, $(findstring linux, $(SYS)))
	X11FLAGS := -static
else ifneq(, $(findstring mingw, $(SYS)))
	X11FLAGS := 
else ifneq(, $(findstring cygwin, $(SYS)))
	X11FLAGS := 
else
	X11FLAGS := -static
endif

#.. What are we going to call it and where does it go?
BINDIR  = $(HERE)
SUPREM	= suprem

#.. Edit these to change the location of the help and data files
HELPDIR	= $(HERE)/help
DATADIR	= $(HERE)/data

#.. Which C and Fortran Compiler?
CC	= /usr/bin/gcc
FC	= /usr/bin/gfortran

#.. What optimization level for the compilers?
#COPT	= -O3
#FOPT	= -O2
#COPT	= -g3 -gdwarf-2 -Wno-write-strings -std=c90
#FOPT	= -g3 -gdwarf-2 -ffixed-form

#.. if you want the X11 plotting uncomment the following
GRLIBS	= xsupr4/xsup4.o -L/usr/lib -lX11 -lXaw -lXmu -lXt -lXext
#GRINC	=
#GRARC	=
GRSRCS	= xsupr4/Graph.c xsupr4/app1.c xsupr4/draw.c xsupr4/help.c \
 	xsupr4/hpgl.c xsupr4/idraw.c xsupr4/interface.c xsupr4/ps.c \
 	xsupr4/xgX.c
GROBJS	= xsupr4/Graph.o xsupr4/app1.o xsupr4/draw.o xsupr4/help.o \
 	xsupr4/hpgl.o xsupr4/idraw.o xsupr4/interface.o xsupr4/ps.o \
	xsupr4/xgX.o

#.. otherwise if you want the gplot plotting package then use this.
#.. although you need to change the /icl/include and /icl/lib to
#.. wherever the package include and archive files are.
#GRLIBS	= -L/usr/lib -lgplot -lhigh2 -L/usr/lib/X11 -lXaw -lXmu -lXext -lXt -lX11
#GRINC	= -I/usr/include
#GRARC	= $(LIBDEST)/gpsup.o
#GRSRCS	= gpsup/gpcover.c
#GROBJS	= gpsup/gpcover.o

TCLLIB  = -L/usr/lib -ltcl
TCLINCLUDES = -I/usr/include

# MALLOC DEBUG
#add this to the above for dbmalloc and put malloc.h into include dir.
#dbmalloc/libdbmalloc.a
#also, if using usr/lang/cc the index function in malloc.h may
#have to be commented out.

# sysV uses termlib
#LIBS	= $(GRLIBS) -ly -ll -lm -ltermlib $(TCLLIB)
# bsd uses termcap
#LIBS	= $(GRLIBS) -ly -ll -lm -ltermcap $(TCLLIB)
LIBS     = $(GRLIBS) -lm -ly -lfl $(TCLLIB)

#.. SysV users take note!  Down where the archive files are ranlib'd
#.. is some real crufty stuff.  This is to get around a bug in ranlib
#.. that shows up mainly when running gmake with the multi-job flag.
AR	= ar
ARFLAGS	= rv
#.. BSD folks need this.
#RANLIB	= ranlib
#.. SysV folks need to do this.
#RANLIB	= \#

#--- You shouldn't need to mess with anything below here ---
#----------------------------------------------------------------------
MANDIR = -DMANLOC='"$(HELPDIR)"'
KEYFILE = -DKEYLOC='"$(DATADIR)/suprem.uk"'
MODELFILE = -DMODELLOC='"$(DATADIR)/modelrc"'
IMP_FILE = -DIMPLOC='"$(DATADIR)/sup4gs.imp"'
NEWIMP_FILE = -DNEWIMPLOC='"$(DATADIR)/implant/implant.tcl"'
DATAFILES = $(MANDIR) $(KEYFILE) $(MODELFILE) $(IMP_FILE) $(NEWIMP_FILE)
LOCDEFS = -DDEVICE -D$(SYS) -DNO_F77
#LOCDEFS = -DDEVICE -D$(SYS) -DNO_F77 -DNEWIMPLANT
FORESTINCLUDES = -I$(HERE)/foreststub
TCLINCLUDES = -I/usr/include/tcl8.5
INCLUDE = $(GRINC) -Iinclude $(FORESTINCLUDES) $(TCLINCLUDES)
CACCFLAGS = -c $(COPT) $(LOCDEFS) $(INCLUDE) -DSUPREM9130 -DSUPREM_IV_GS
CFLAGS	= $(CACCFLAGS) -DCPLUSPLUS $(ROBERTFLAGS) $(COMPFLAG)
#-DNEWC -DCPLUSPLUS
LFLAGS	= -v -n -q $(COPT)  $(INCLUDE)
FFLAGS	= -c $(FOPT)
# if using gcc then take out the B before static below.
#LDFLAGS	= $(COPT) $(LOCDEFS) -static -v
LDFLAGS	= $(COPT) $(LOCDEFS) $(X11FLAGS)
LDFLAGS2= -r -o -G

MAINSRCS = ./main.c
MAINOBJS = ./main.o

CHECKSRCS = check/check.c check/eval.c check/lexical.c check/parse.c \
        check/reduce.c check/sint.c check/split.c check/vector.c \
        check/verify.c
CHECKOBJS = check/check.o check/eval.o check/lexical.o check/parse.o \
        check/reduce.o check/sint.o check/split.o check/vector.o \
        check/verify.o

DBASESRCS = dbase/alloc.c dbase/check.c dbase/clktri.c dbase/couple.c \
        dbase/dimen.c dbase/dispose.c dbase/edge.c dbase/element.c \
        dbase/flip.c dbase/geom.c dbase/geometry.c dbase/grid_loop.c \
        dbase/grid_upd.c dbase/kill_node.c dbase/list.c dbase/locate.c \
        dbase/make_db.c dbase/neigh.c dbase/new_layer.c dbase/node.c \
        dbase/point.c dbase/region.c dbase/remove.c
DBASEOBJS = dbase/alloc.o dbase/check.o dbase/clktri.o dbase/couple.o \
        dbase/dimen.o dbase/dispose.o dbase/edge.o dbase/element.o \
        dbase/flip.o dbase/geom.o dbase/geometry.o dbase/grid_loop.o \
        dbase/grid_upd.o dbase/kill_node.o dbase/list.o dbase/locate.o \
        dbase/make_db.o dbase/neigh.o dbase/new_layer.o dbase/node.o \
        dbase/point.o dbase/region.o dbase/remove.o

DEVICESRCS = device/Electron.c device/Hole.c device/Psi.c \
        device/circuit.c device/contact.c device/dev_blkset.c \
        device/dev_prep.c device/dev_solve.c device/device.c
DEVICEOBJS = device/Electron.o device/Hole.o device/Psi.o \
        device/circuit.o device/contact.o device/dev_blkset.o \
        device/dev_prep.o device/dev_solve.o device/device.o

DIFFUSESRCS = diffuse/Antimony.c diffuse/Arsenic.c diffuse/Beryllium.c \
        diffuse/Boron.c diffuse/Carbon.c diffuse/Cesium.c \
        diffuse/Generic.c diffuse/Germanium.c diffuse/Gold.c \
        diffuse/Interst.c diffuse/Magnesium.c diffuse/Phosphorus.c \
        diffuse/Selenium.c diffuse/Silicon.c diffuse/Tin.c \
        diffuse/Trap.c diffuse/Vacancy.c diffuse/Zinc.c \
        diffuse/active.c diffuse/block_set.c diffuse/defect.c \
        diffuse/diff_init.c diffuse/diffuse.c diffuse/dislocation.c \
        diffuse/loop.c diffuse/movie.c \
        diffuse/moving.c diffuse/norm.c diffuse/poly.c \
        diffuse/predep.c diffuse/prepare.c diffuse/setup.c \
        diffuse/solve.c diffuse/solve_time.c diffuse/species.c \
        diffuse/time.c
DIFFUSEOBJS = diffuse/Antimony.o diffuse/Arsenic.o diffuse/Beryllium.o \
        diffuse/Boron.o diffuse/Carbon.o diffuse/Cesium.o \
        diffuse/Generic.o diffuse/Germanium.o diffuse/Gold.o \
        diffuse/Interst.o diffuse/Magnesium.o diffuse/Phosphorus.o \
        diffuse/Selenium.o diffuse/Silicon.o diffuse/Tin.o \
        diffuse/Trap.o diffuse/Vacancy.o diffuse/Zinc.o \
        diffuse/active.o diffuse/block_set.o diffuse/defect.o \
        diffuse/diff_init.o diffuse/diffuse.o diffuse/dislocation.o \
        diffuse/loop.o diffuse/movie.o \
        diffuse/moving.o diffuse/norm.o diffuse/poly.o \
        diffuse/predep.o diffuse/prepare.o diffuse/setup.o \
        diffuse/solve.o diffuse/solve_time.o diffuse/species.o \
        diffuse/time.o

FOREST_SRCS = foreststub/foreststub.c

FOREST_OBJS = foreststub/foreststub.o

FINELSRCS = finel/FEassmb.c finel/FEsolve.c finel/tomcal.c \
        finel/tri6.c finel/tri7.c
FINDELFSRCS = finel/interface.r finel/toms508.f
FINELOBJS = finel/FEassmb.o finel/FEsolve.o finel/tomcal.o \
        finel/tri6.o finel/tri7.o finel/interface.o finel/toms508.o

GEOMSRCS = geom/limits.c geom/misc.c geom/tnabor.c
GEOMOBJS = geom/limits.o geom/misc.o geom/tnabor.o

IMAGETOOLSRCS = imagetool/fill_grid.c imagetool/image_write.c \
        imagetool/make_grid.c imagetool/makeframe.c imagetool/window.c
IMAGETOOLOBJS = imagetool/fill_grid.o imagetool/image_write.o \
        imagetool/make_grid.o imagetool/makeframe.o imagetool/window.o

IMPLANTSRCS = implant/damage.c implant/fgtmom.c implant/imp_gtprs.c \
        implant/imp_lat.c implant/imp_prson.c implant/imp_qeqv.c \
        implant/imp_qpgauss.c implant/imp_qpprs.c implant/imp_setprs.c \
        implant/imp_vert.c implant/imp_zeqv.c implant/implant.c \
        implant/pearson.c implant/surf.c
IMPLANTOBJS = implant/damage.o implant/fgtmom.o implant/imp_gtprs.o \
        implant/imp_lat.o implant/imp_prson.o implant/imp_qeqv.o \
        implant/imp_qpgauss.o implant/imp_qpprs.o implant/imp_setprs.o \
        implant/imp_vert.o implant/imp_zeqv.o implant/implant.o \
        implant/pearson.o implant/surf.o

MATHSRCS = math/blklu.c math/coldata.c math/generate.c math/genrow.c \
        math/min_fill.c math/mxv.c math/solblk.c math/symb.c math/fp_utils.c
MATHFSRCS = math/vector.f
MATHOBJS = math/blklu.o math/coldata.o math/generate.o math/genrow.o \
        math/min_fill.o math/mxv.o math/solblk.o math/symb.o math/vector.o \
        math/fp_utils.o

MESHSRCS = mesh/check_active.c mesh/ig2_meshio.c mesh/initialize.c \
        mesh/newbackside.c mesh/pi_meshio.c mesh/profile.c \
        mesh/rect.c mesh/save_simpl.c mesh/structure.c
MESHOBJS = mesh/check_active.o mesh/ig2_meshio.o mesh/initialize.o \
        mesh/newbackside.o mesh/pi_meshio.o mesh/profile.o \
        mesh/rect.o mesh/save_simpl.o mesh/structure.o

MISCSRCS = misc/bsd.c misc/cpu.c misc/echo.c misc/get.c misc/man.c \
        misc/pa.c misc/panic.c misc/print_time.c misc/read.c \
        misc/reader.c
MISCOBJS = misc/bsd.o misc/cpu.o misc/echo.o misc/get.o misc/man.o \
        misc/pa.o misc/panic.o misc/print_time.o misc/read.o \
        misc/reader.o

OXIDESRCS = oxide/FEbc.c oxide/FEconvert.c oxide/Oxidant.c oxide/coeffox.c \
        oxide/elast.c oxide/erfit.c oxide/mater.c oxide/oxgrow.c \
        oxide/oxide_vel.c oxide/oxrate.c oxide/triox.c oxide/vert.c \
        oxide/viscous.c
OXIDEOBJS = oxide/FEbc.o oxide/FEconvert.o oxide/Oxidant.o oxide/coeffox.o \
        oxide/elast.o oxide/erfit.o oxide/mater.o oxide/oxgrow.o \
        oxide/oxide_vel.o oxide/oxrate.o oxide/triox.o oxide/vert.o \
        oxide/viscous.o

PLOTSRCS = plot/contour.c plot/do_1d.c plot/intersect.c plot/label.c \
        plot/matedge.c plot/material.c plot/option.c plot/plot.c \
        plot/plot_1d.c plot/plot_2d.c plot/print_1d.c plot/select.c
PLOTOBJS = plot/contour.o plot/do_1d.o plot/intersect.o plot/label.o \
        plot/matedge.o plot/material.o plot/option.o plot/plot.o \
        plot/plot_1d.o plot/plot_2d.o plot/print_1d.o plot/select.o

REFINESRCS = refine/bfill.c refine/dbase.c refine/deloop.c \
        refine/deposit.c refine/etch.c refine/grid.c refine/hinit.c \
        refine/interp.c refine/offset.c refine/rate.c refine/rect_tri.c \
        refine/skel.c refine/sp_edge.c refine/string_etch.c \
        refine/string_off.c refine/surface.c refine/triang.c \
        refine/tridata.c refine/triheur.c
REFINEOBJS = refine/bfill.o refine/dbase.o refine/deloop.o \
        refine/deposit.o refine/etch.o refine/grid.o refine/hinit.o \
        refine/interp.o refine/offset.o refine/rate.o refine/rect_tri.o \
        refine/skel.o refine/sp_edge.o refine/string_etch.o \
        refine/string_off.o refine/surface.o refine/triang.o \
        refine/tridata.o refine/triheur.o

SHELLSRCS = shell/do_action.c shell/help.c shell/input.c shell/lex.c \
        shell/list.c shell/macro.c shell/parser.c shell/parser_boot.c \
        shell/proc.c shell/set.c shell/smisc.c
SHELLYSRCS = shell/parser.y
SHELLOBJS = shell/parser.o shell/do_action.o shell/help.o shell/input.o \
        shell/lex.o shell/list.o shell/macro.o shell/parser_boot.o \
        shell/proc.o shell/set.o shell/smisc.o

CSRCS   = $(MAINSRCS) $(CHECKSRCS) $(DBASESRCS) $(DEVICESRCS) \
        $(DIFFUSESRCS) $(FOREST_SRCS) $(FINELSRCS) \
        $(GEOMSRCS) $(IMAGETOOLSRCS) \
        $(IMPLANTSRCS) $(MATHSRCS) $(MESHSRCS) $(MISCSRCS) $(OXIDESRCS) \
        $(PLOTSRCS) $(REFINESRCS) $(SHELLSRCS) $(SPEEDIESRCS) $(GRSRCS)

YSRCS	= $(SHELLYSRCS)
FSRCS	= $(FINDELFSRCS) $(MATHFSRCS )
SRCS	= $(CSRCS) $(FSRCS)

OBJS    = $(MAINOBJS) $(CHECKOBJS) $(DBASEOBJS) $(DEVICEOBJS) \
        $(DIFFUSEOBJS) $(FOREST_OBJS) $(FINELOBJS) \
        $(GEOMOBJS) $(IMAGETOOLOBJS) \
        $(IMPLANTOBJS) $(MATHOBJS) $(MESHOBJS) $(MISCOBJS) $(OXIDEOBJS) \
        $(PLOTOBJS) $(REFINEOBJS) $(SHELLOBJS) $(SPEEDIEOBJS) $(GROBJS)

# change the default actions
.c~.o	:
	$(GET) $(GFLAGS) $<
	$(CC) $(CFLAGS) $*.c -o $*.o

.c.o   :
	$(CC) $(CFLAGS) $*.c -o $*.o

.c~.c	:
	$(GET) $(GFLAGS) $<

#.y.o    :
#	$(YACC) $(YFLAGS) -d $<
#	mv y.tab.c $*.c
#	mv y.tab.h $*.h
#	$(CC) $(CFLAGS) $*.c -o $*.o
#
#.y.c	:
#	$(YACC) $(YFLAGS) -d $<
#	mv y.tab.c $*.c
#	mv y.tab.h $*.h
#
.y~.y   :
	$(GET) $(GFLAGS) $<

.l~.l   :
	$(GET) $(GFLAGS) $<

.h~.h	:
	$(GET) $(GFLAGS) $<

.f~.o	:
	$(GET) $(GFLAGS) $<
	$(FC) $(FFLAGS) $*.f -o $*.o

.f~.f	:
	$(GET) $(GFLAGS) $<

.f.o	:
	$(FC) $(FFLAGS) $*.f -o $*.o

.r.f    :
	ratfor -C -6c < $*.r > $*.f

#----------------------------------------------------------------------
#local: $(SUPREM)
#	install -m 755 $(SUPREM) $(BINDIR)

$(SUPREM): $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) $(LIBS) -o $@

install: $(SUPREM) $(BINDIR) $(DATADIR) $(HELPDIR) suprem.uk
	install -m 755 $(SUPREM) $(BINDIR)
	install -m 644 data/modelrc data/sup4gs.imp $(DATADIR)

keyread: always
	cd keyread; $(MAKE) SYS='$(SYS)' OPT='$(OPT)' CC='gcc'

suprem.uk : data/suprem.key keyread
	keyread/keyread data/suprem.uk < data/suprem.key
	install -m 644 data/suprem.uk $(DATADIR)

#.. If necessary, create the directory for the suprem executable file.
$(BINDIR):
	mkdir -p $(BINDIR)
	chmod -f 755 $(BINDIR)

#.. If necessary, create the directory for the suprem data files.
$(DATADIR):
	mkdir -p $(DATADIR)
	chmod -f 755 $(DATADIR)

#.. If necessary, create the directory for the suprem help files.
#.. Don't get your hopes up though.  There aren't any yet.
$(HELPDIR):
	mkdir -p $(HELPDIR)
	chmod -f 755 $(HELPDIR)

main.o:
	$(CC) $(CFLAGS) $(DATAFILES) main.c -o main.o

#.. The parser stuff has to use ANSI C compiler and not the C++
#   compiler.  Yacc does not produce code that can go through the C++
#   complier.
shell/parser.o: shell/parser.c
	gcc $(CACCFLAGS) shell/parser.c -o shell/parser.o

shell/parser.c: shell/parser.y
	$(YACC) $(YFLAGS) -y -d -o $@ shell/parser.y
#	mv -fv y.tab.c shell/parser.c
#	mv -fv y.tab.h shell/parser.h

clean   :
	rm -rvf $(SUPREM) $(SLIBS) $(MAINOBJS) $(OBJS) $(GROBJS)
	rm -rvf keyread/*.o keyread/keyread
	rm -rvf xsupr4/*.o

tags    : $(SRCS)
	ctags $(SRCS)

lint    : $(CSRCS)
	lint $(CFLAGS) $(CSRCS)

version :
	prs -d":Dt: :F:\n:C:\n\n" $(SCCS)


tape:
	-rm SupremOO7.tar
	-rm SupremOO7.tar.Z
	-mkdir ./SupremOO7distrib
	-mkdir ./SupremOO7distrib/check
	-mkdir ./SupremOO7distrib/data
	-mkdir ./SupremOO7distrib/dbase
	-mkdir ./SupremOO7distrib/device
	-mkdir ./SupremOO7distrib/diffuse
	-mkdir ./SupremOO7distrib/finel
	-mkdir ./SupremOO7distrib/foreststub
	-mkdir ./SupremOO7distrib/geom
	-mkdir ./SupremOO7distrib/gpsup
	-mkdir ./SupremOO7distrib/imagetool
	-mkdir ./SupremOO7distrib/implant
	-mkdir ./SupremOO7distrib/include
	-mkdir ./SupremOO7distrib/keyread
	-mkdir ./SupremOO7distrib/math
	-mkdir ./SupremOO7distrib/mesh
	-mkdir ./SupremOO7distrib/misc
	-mkdir ./SupremOO7distrib/oxide
	-mkdir ./SupremOO7distrib/plot
	-mkdir ./SupremOO7distrib/refine
	-mkdir ./SupremOO7distrib/shell
	-mkdir ./SupremOO7distrib/xsupr4
	-cp -p Makefile.distrib ./SupremOO7distrib/Makefile
	-cp -p main.c ./SupremOO7distrib/main.c
	-cp -p ./check/*.c ./SupremOO7distrib/check/
	-cp -p ./data/* ./SupremOO7distrib/data/
	-cp -p ./dbase/*.c ./SupremOO7distrib/dbase/
	-cp -p ./device/*.c ./SupremOO7distrib/device/
	-cp -p ./diffuse/*.c ./SupremOO7distrib/diffuse/
	-cp -p ./diffuse/*.h ./SupremOO7distrib/diffuse/
	-cp -p ./finel/*.c ./SupremOO7distrib/finel/
	-cp -p ./finel/*.f ./SupremOO7distrib/finel/
	-cp -p ./finel/*.r ./SupremOO7distrib/finel/
	-cp -p ./foreststub/*.c ./SupremOO7distrib/foreststub/
	-cp -p ./foreststub/*.h ./SupremOO7distrib/foreststub/
	-cp -p ./geom/*.c ./SupremOO7distrib/geom/
	-cp -p ./gpsup/*.c ./SupremOO7distrib/gpsup/
	-cp -p ./imagetool/*.c ./SupremOO7distrib/imagetool/
	-cp -p ./implant/*.c ./SupremOO7distrib/implant/
	-cp -p ./include/*.h ./SupremOO7distrib/include/
	-cp -p ./keyread/*.c ./SupremOO7distrib/keyread/
	-cp -p ./keyread/Makefile ./SupremOO7distrib/keyread/
	-cp -p ./keyread/*.l ./SupremOO7distrib/keyread/
	-cp -p ./keyread/*.h ./SupremOO7distrib/keyread/
	-cp -p ./keyread/*.y ./SupremOO7distrib/keyread/
	-cp -p ./math/*.c ./SupremOO7distrib/math/
	-cp -p ./math/*.f ./SupremOO7distrib/math/
	-cp -p ./mesh/*.c ./SupremOO7distrib/mesh/
	-cp -p ./mesh/*.h ./SupremOO7distrib/mesh/
	-cp -p ./misc/*.c ./SupremOO7distrib/misc/
	-cp -p ./oxide/*.c ./SupremOO7distrib/oxide/
	-cp -p ./plot/*.c ./SupremOO7distrib/plot/
	-cp -p ./refine/*.c ./SupremOO7distrib/refine/
	-cp -p ./shell/*.c ./SupremOO7distrib/shell/
	-cp -p ./shell/*.y ./SupremOO7distrib/shell/
	-cp -p ./shell/*.h ./SupremOO7distrib/shell/
	-cp -p ./xsupr4/*.c ./SupremOO7distrib/xsupr4/
	-cp -p ./xsupr4/*.h ./SupremOO7distrib/xsupr4/
	-tar cvf SupremOO7.tar SupremOO7distrib
	-compress SupremOO7.tar
	-rm -rf SupremOO7distrib

# If you're using the ultrix cc then change the -M to -Em
depend  : $(YSRCS)
	echo $(HERE)
ifneq (, $(wildcard ./Makefile.x))
	mv -fv ./Makefile.x ./Makefile.x.bak
else
	touch Makefile.x
endif
	cat Makefile > Makefile.x
	$(CC) -M $(CFLAGS) $(CSRCS)|\
	(while read TARGET D1 D2 D3 ; do \
		TGTNAME=`basename $$TARGET .o:`; \
		TGTNAME=`basename $$TGTNAME .o`; \
		if test $$D1 = ":"; then \
			DEPEND=$$D2; \
		else \
			DEPEND=$$D1; \
		fi; \
		DEPNAME=`basename $$DEPEND .c`; \
		DEPNAME=`basename $$DEPNAME .r`; \
		DEPNAME=`basename $$DEPNAME .f`; \
		if test $$DEPNAME = $$TGTNAME; then \
			DEPDIR=`dirname $$DEPEND`; \
		fi; \
		echo $$DEPDIR/$$TARGET $$D1 $$D2 $$D3; \
	done) >> Makefile.x

always:

