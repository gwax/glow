
The GLOW Toolkit
version 0.9.8 (23 May 2000)
Copyright (C) 1997-2000  Daniel Azuma
All rights reserved worldwide


==============================================================================


CONTENTS

   (1) Introduction

   (2) Terms and conditions

   (3) Release notes 0.9.8

   (4) Installing GLOW

   (5) For more information


==============================================================================


INTRODUCTION


    The GLOW Toolkit is a cross-platform object-oriented framework for
    building interactive applications using OpenGL(tm) or similar APIs.
    It provides an object-oriented API for creating windows, menus and
    other user interface elements, and an inheritance-based event
    handling model. GLOW also includes a full-featured and extensible
    library of cross-platform widgets for building high-powered user
    interfaces. And, of course, full integration with OpenGL as the
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
    Version 0.9.8 (23 May 2000)

    These notes detail the differences between versions 0.9.7 and 0.9.8.
    A complete history of publicly released versions can be found on the
    GLOW web site.

    This release of GLOW focused on bug fixes.


    Features added
    --------------

      Added Glow::NumToplevelWindows().

      Added Glow::SetAutoQuitting() and Glow::IsAutoQuitting(). You can
      now set GLOW to automatically quit when the last toplevel window
      has been closed.

      Added GlowComponent::Paint().

      GlowSliderWidget can now handle maximum == minimum. In that case,
      the indicator is not drawn and does not respond to mouse clicks.

      Added IsSpinning(), StartSpinning(), StopSpinning and StepSpin()
      to GlowViewTransform and GlowViewManipulator.

      Added Glow::SetMenuStatusFunc(); updated lesson 6 to match.


    Features changed
    ----------------

      GlowCheckBoxWidget and GlowRadioButtonWidget's default heights are
      one pixel less.

      Rolled GlowStickyButtonWidget interface into GlowPushButtonWidget.
      Removed GlowStickyButtonWidget.

      Reordered the parameters of Set() in GlowTransformData and related
      classes to (translation, rotation, scale).


    Bugs fixed
    ----------

      GlowLabelWidget's autopack wasn't honoring expandPreferredSize,
      which caused labeled widgets (esp. sliders) to pack incorrectly
      in certain cases. Fixed.

      GlowWidget's destructor wasn't removing the widget from the widget
      notification list, which sometimes led to a crash. Fixed.

      Hiding or deactivating a widget caused a temporary inconsistency
      in the widget's state that could cause crashes if the widget was
      deleted before the deferred event was handled. Fixed.

      GlowWidget::OnGotKeyboardFocus() and OnLostKeyboardFocus() weren't
      being deferred. Fixed.

      GlowQuickPalette wasn't honoring widget visibility, resulting in
      holes in palettes with invisible widgets. Fixed.

      GlowCheckBoxWidget's mark appeared off-kilter under Mesa3D, and
      didn't scale well with widget size. Fixed.

      glowUtilities.cpp didn't link in Codewarrior for Windows. I think
      this may be a result of a Codewarrior bug, but I found and put in
      a workaround anyway.

      Multiple bugs and broken links in the reference fixed.

      The reference didn't document the classes in glowVectorAlgebra.
      Fixed.

      Tutorial lesson 7 said the wrong thing about the return value for
      event filters. Fixed.


    Internal changes
    ----------------

      (none)


    Known issues
    ------------

      Because of the continuing incomplete state of GLUT in the Mac OS
      OpenGL SDK, many features of GLOW are still unusable on the Mac.
      I do have it on good authority that it is being worked on, so I'm
      hopeful that this will be resolved soon, perhaps with the next
      release of Apple's OpenGL SDK.

      Because of GLOW's heavy ANSI dependence, there are some issues
      with various compilers, especially older ones. If you've worked
      on getting GLOW to build on a compiler that isn't on my supported
      compilers list, please let me know of any changes you needed to
      make, so I can get them merged into the main code base.


==============================================================================


INSTALLING GLOW


    The files comprising GLOW are located in the directory "glow".
    You should install this directory in a reasonable place such as in
    /usr/lib. I prefer to keep it with the rest of my source code in a
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

    You may also want to build a library archive or shared library of
    GLOW. I leave it up to the reader to figure out how to do this,
    since I just use it in its source code form myself.


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

