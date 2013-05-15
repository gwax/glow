
#===============================================================================
# GLOW shared library Makefile
# (Temporary version for use with GLOW 1.0.x until I get autoconf working)
#===============================================================================

.PHONY: everything debug nodebug

everything: debug nodebug

nodebug:
	rm -f .glowmake.DEBUG
	$(MAKE) clean
	touch .glowmake.SHARED
	$(MAKE) libglow.so
	mv libglow.so libglow.so.1.1.1
	rm -r .glowmake.SHARED
	$(MAKE) libglow.a
	mv libglow.a libglow.a.1.1.1

debug:
	touch .glowmake.DEBUG
	$(MAKE) clean
	touch .glowmake.SHARED
	$(MAKE) libglow.so
	mv libglow.so libglowdebug.so.1.1.1
	rm -r .glowmake.SHARED
	$(MAKE) libglow.a
	mv libglow.a libglowdebug.a.1.1.1

CLEANFILES:=libglow.so libglow.a
IGNOREDEPTARGETS:=everything debug nodebug .

PROG_SHARED=libglow.so
PROG=libglow.a

SRCDIR=.
GLOWDIR=.

MODULES=\
	glow \
	glowAux \
	glowButtonWidget \
	glowCheckBoxWidget \
	glowDebug \
	glowImageFile \
	glowLabelWidget \
	glowMenuButtonWidget \
	glowMessageWindow \
	glowPanelWidget \
	glowPushButtonWidget \
	glowQuickPalette \
	glowRadioButtonWidget \
	glowRenderSwitch \
	glowScrollBarWidget \
	glowSenderReceiver \
	glowSeparatorWidget \
	glowSliderWidget \
	glowTextAreaWidget \
	glowTextData \
	glowTextFieldWidget \
	glowTextFieldWindow \
	glowUtilities \
	glowVectorAlgebra \
	glowViewTransform \
	glowWidget


COMPILE_Linux=gcc
COMPILE_IRIX=CC
LINK_Linux=ar crs libglow.a
LINK_IRIX=CC -ar -o libglow.a
LINK_Linux_SHARED=gcc -shared -o libglow.so
LINK_IRIX_SHARED=CC -shared -o libglow.a
SHELL=/bin/sh

#LIBS=-lglut -lpthread -lGL -lGLU -lXi -lXmu -lX11 -lm -lstdc++
#LIBS_IRIX=-lCio -lglut -lX11 -lXmu -lXext -lGL -lGLU -lm

DEPFLAGS_Linux=-MM
DEPFLAGS_IRIX=-M

CFLAGS_Linux=-Wall \
	-DGLOW_COMPAT_USEOLDSTREAMPOS \
	-DGLOW_PLATFORM_LINUX \
	-DGLOW_PLATFORM_LITTLEENDIAN
NODEBUGCFLAGS_Linux=-O2 -DNODEBUG

CFLAGS_IRIX=-fullwarn -no_auto_include \
	-LANG:std \
	-woff 1209,3201,1424,1375 \
	-OPT:Olimit=0 \
	-I../Compat-SGI \
	-DGLOW_COMPAT_CLIBNOSTDNAMESPACE \
	-DGLOW_PLATFORM_IRIX \
	-DGLOW_PLATFORM_BIGENDIAN
NODEBUGCFLAGS_IRIX=-O2 -DNODEBUG


include $(GLOWDIR)/glow_base.Makefile
