
The GLOW Toolkit
version 1.1.2dev (?? August 2000)
Copyright (C) 1997-2000  Daniel Azuma


==============================================================================


CONTENTS

   (1) Introduction

   (2) Terms and conditions

   (3) Release notes

   (4) Installing GLOW

   (5) For more information


==============================================================================


INTRODUCTION


    The GLOW Toolkit is a cross-platform object-oriented framework for
    building interactive applications using OpenGL(tm) or similar APIs.
    It provides an object-oriented API for creating windows, menus and
    other user interface elements, and an inheritance-based event
    handling model. GLOW also includes a full-featured and extensible
    library of cross-platform widgets for building powerful graphical
    user interfaces. And, of course, full integration with OpenGL as the
    imaging API.

    The GLOW Toolkit is a free software library, distributed under the
    GNU Lesser General Public License. This means you have access to the
    source code, and may modify it and write your own programs derived
    from it, subject to certain restrictions.


==============================================================================


TERMS AND CONDITIONS


    This library is free software; you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as
    published by the Free Software Foundation; either version 2.1 of the
    License, or (at your option) any later version. By modifying or
    distributing this library or any work based on this library, you
    indicate your acceptance of all the terms and conditions described
    in the GNU Lesser General Public License.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library. If not, you can obtain a copy
    by writing to:

        Free Software Foundation, Inc.
        59 Temple Place - Suite 330,
        Boston, MA  02111-1307, USA.


==============================================================================


RELEASE NOTES


    The GLOW Toolkit
    Version 1.1.2dev (?? August 2000)

    These are the differences between versions 1.1.1dev and 1.1.2dev.
    A complete history of publicly released versions can be found on the
    GLOW web site.


    Features added
    --------------

      Added GlowComponent::IsInitialized() and GlowComponent::IsClosed()

      GlowLabelWidget can now align text center and right.

      Added TGlowImage::GrabArray() and TGlowImage::ReleaseArray().

      Added GlowColorImage::Scale().

      Added GlowTextAreaWidget class.


    Features changed
    ----------------

      Widget windows and subwindows used to request alpha, stencil and
      depth, but now default to Glow::rgbBuffer | Glow::doubleBuffer.
      (also in 1.0.2)

      Removed conversion from Vec3f and Quatf to const GLfloat* and 
      replaced with method Array().

      GlowTextData contains rather than inherits from std::string.

      Renamed GlowImage, GlowPNMReader and GlowPNMWriter to
      TGlowImage, TGlowPNMReader and TGlowPNMWriter, respectively.


    Bugs fixed
    ----------

      QuickPalette included widgets that were deferred-deleted using
      Close(), resulting in blank areas when repacking a palette with
      closed widgets. Fixed. (also in 1.0.2)

      GlowLabelWidget didn't draw strings with Mac-style newlines
      correctly. Fixed. (also in 1.0.2)

      Several classes in the tutorial weren't cleaning up all their
      resources if they were deleted. Fixed. (also in 1.0.2)

      GlowSubwindow::OnJoystick() was missing. Fixed.

      Several more reference fixes.


    Internal changes
    ----------------

      Fixed underscores (changed from leading to trailing) for private
      class members in the tutorial.


    To-do list
    ----------

      Support for autoconfig. This will probably involve, among other
      things, ditching the current makefile system, as well as ditching
      the idea of using GLOW as source code.

      GlowTabbedPanelWidget class.

      GlowGeneralButtonWidget hierarchy. Basically a generalized push-
      button with more flexible event reporting and drawing options.
      GlowPushButtonWidget will inherit from it, as well as a new
      GlowImagePushButtonWidget class.

      GlowGeneralListWidget hierarchy. GlowTextListWidget will be a
      major subclass.

      Add support for "either-or" to QuickPalette.

      GlowComboBoxWidget class? May use GlowTextListWidget.

      GlowScrollableWindow class? Not sure how to design this.

      Add stroke fonts to GlowFont? Not sure how to design this.

      Add TGlowImage, TGlowPNMReader and TGlowPNMWriter info to the
      reference.

      Add GlowTextAreaWidget to the reference.

      Update tutorial.


    Known issues
    ------------

      I don't think it's currently possible to compile a Win32 DLL of
      GLOW under MSVC++ 6.0 because of the heavy use of templates and
      STL. If you succeed in building a DLL for GLOW, I'd appreciate
      finding out how you did it.

      Under certain conditions, GLUT can lose track of some of its
      windows. This is most commonly manifested as windows or subwindows
      which unexpectedly stop redrawing. I'm fairly certain this is due
      to a bug in GLUT, and I've contacted Mark Kilgard concerning it.
      There is no known complete workaround short of applying a fix to
      GLUT and recompiling it, but GLOW now provides an optional partial
      workaround that can be activated by defining the symbol
      GLOW_OPTION_GLUTREDISPLAYFIX.

      Because of the continuing incomplete state of GLUT in the Mac OS
      OpenGL SDK, many features of GLOW are still unusable on the Mac.
      I do have it on good authority that it is being worked on, so I'm
      hopeful that this will be resolved soon.

      Because of GLOW's heavy ANSI dependence, there are some issues
      with various compilers, especially older ones. If you've worked
      on getting GLOW to build on a compiler that isn't on my supported
      compilers list, please let me know of any changes you needed to
      make, so I can get them merged into the main code base.


==============================================================================


INSTALLING GLOW


    The files comprising GLOW are located in the directory "glow_src".
    You should install this directory in a reasonable place such as in
    /usr/src. I prefer to keep it with the rest of my source code in a
    data directory. If you are running SGI IRIX and using the SGI
    MISPPro compiler version 7.3, you should also install the directory
    "Compat-SGI". This directory contains header files using the new
    style C++ header names, which GLOW needs in order to compile
    properly. (Alternately, you may install the headers directly into
    your /usr/include/CC directory, but do not overwrite any headers
    already there.)

    To get started using GLOW, I strongly recommend going through at
    least the first lesson (lesson 0) of the tutorial. This should give
    you a reasonable idea of how to use GLOW as a source code library.

    If you want to build a unix shared library for GLOW, I've provided a
    sample makefile in the glow_src directory. It should work under
    Linux or IRIX, and will build two shared libraries:
    libglow.so.1.0.1 and libglowdebug.so.1.0.1. The former is compiled
    with debugging turned off (i.e. GLOW_OPTION_DEBUG is not defined)
    and the latter is compiled with debugging on (i.e. GLOW_OPTION_DEBUG
    is defined.) You should move them into the appropriate directory
    such as /usr/lib, and you should create appropriate soft links (e.g.
    libglow.so -> libglow.so.1.0.1 and libglowdebug.so ->
    libglowdebug.so.1.0.1). You should link with one of those libraries,
    the one that matches the debug option used to compile your program.

    The Makefile used in the tutorials supports using GLOW either as a
    library or as source code. The default is source code; however, to
    change to using GLOW as a precompiled library, add file-option
    "GLOWLIB" (e.g. create the file ".glowmake.GLOWLIB".) See lesson 0
    for details.

    I don't think it's currently possible to make a Windows DLL of GLOW
    because of issues with MSVC++'s template support.


==============================================================================


FOR MORE INFORMATION


    For up-to-date information, please visit the GLOW home page, located
    at http://www.ugcs.caltech.edu/~dazuma/glow/

    Mailing lists, bug reporting, CVS repository and other features are
    also located on the web.

    The author can be reached via email at dazuma@kagi.com


==============================================================================


OpenGL(tm) is a trademark of SGI. Other names referenced may be
trademarks of their respective owners.

