
#===============================================================================
# GLOW shared library Makefile
# (Temporary version for use with GLOW 1.0.x until I get autoconf working)
#===============================================================================

.PHONY: everything debug nodebug

everything: debug nodebug

nodebug:
	rm -f .glowmake.DEBUG
	$(MAKE) clean
	$(MAKE) libglow.so
	mv libglow.so libglow.so.1.0.1

debug:
	touch .glowmake.DEBUG
	$(MAKE) clean
	$(MAKE) libglow.so
	mv libglow.so libglowdebug.so.1.0.1

IGNOREDEPTARGETS:=everything debug nodebug

PROG=libglow.so

SRCDIR=.
GLOWDIR=.

MODULES=\
	glow \
	glowAux \
	glowCheckBoxWidget \
	glowDebug \
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
	glowTextData \
	glowTextFieldWidget \
	glowTextFieldWindow \
	glowUtilities \
	glowVectorAlgebra \
	glowViewTransform \
	glowWidget


COMPILE_Linux=gcc
COMPILE_IRIX=CC
SHELL=/bin/sh

#LIBS=-lglut -lpthread -lGL -lGLU -lXi -lXmu -lX11 -lm -lstdc++
#LIBS_IRIX=-lCio -lglut -lX11 -lXmu -lXext -lGL -lGLU -lm

DEPFLAGS_Linux=-MM
DEPFLAGS_IRIX=-M

CFLAGS_Linux=-Wall \
	-DGLOW_PLATFORM_LINUX \
	-DGLOW_PLATFORM_LITTLEENDIAN
NODEBUGCFLAGS_Linux=-O2 -DNODEBUG
LDFLAGS_Linux=-shared

CFLAGS_IRIX=-fullwarn -no_auto_include \
	-LANG:std \
	-woff 1209,3201,1424,1375 \
	-OPT:Olimit=0 \
	-I../Compat-SGI \
	-DGLOW_COMPAT_CLIBNOSTDNAMESPACE \
	-DGLOW_PLATFORM_IRIX \
	-DGLOW_PLATFORM_BIGENDIAN
NODEBUGCFLAGS_IRIX=-O2 -DNODEBUG
LDFLAGS_IRIX=-shared


include $(GLOWDIR)/glow_base.Makefile
