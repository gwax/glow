
#===============================================================================
# mandelglow lesson 4
#===============================================================================

PROG=mandelglow_lesson4

SRCDIR=.
GLOWDIR=../../glow_src

MODULES=\
	mandeldata \
	mandelwind \
	mandelglow

GLOWMODULES=\
	glow \
	glowAux \
	glowDebug \
	glowSenderReceiver \
	glowUtilities \
	glowWidget \
	glowCheckBoxWidget \
	glowLabelWidget \
	glowMenuButtonWidget \
	glowPanelWidget \
	glowPushButtonWidget \
	glowRadioButtonWidget \
	glowScrollBarWidget \
	glowSeparatorWidget \
	glowSliderWidget \
	glowTextData \
	glowTextFieldWidget \
	glowTextFieldWindow \
	glowQuickPalette

COMPILE=gcc
COMPILE_IRIX=CC
SHELL=/bin/sh

LIBS=-lglut -lpthread -lGL -lGLU -lXi -lXmu -lX11 -lm -lstdc++
LIBS_IRIX=-lCio -lglut -lX11 -lXmu -lXext -lGL -lGLU -lm

DEPFLAGS=-MM
DEPFLAGS_IRIX=-M

CFLAGS=-O2 -Wall \
	-I/projects/grail21/dazuma/Linux/include \
	-DGLOW_PLATFORM_LINUX \
	-DGLOW_PLATFORM_LITTLEENDIAN
LDFLAGS=-L/usr/X11R6/lib \
	-L/projects/grail21/dazuma/Linux/lib

CFLAGS_IRIX=-O2 -fullwarn -no_auto_include \
	-LANG:std \
	-woff 1209,3201,1424,1375 \
	-OPT:Olimit=0 \
	-I../../Compat-SGI \
	-DGLOW_COMPAT_CLIBNOSTDNAMESPACE \
	-DGLOW_PLATFORM_IRIX \
	-DGLOW_PLATFORM_BIGENDIAN
LDFLAGS_IRIX=


include $(GLOWDIR)/glow_base.Makefile
