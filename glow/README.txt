
The GLOW Toolkit
version 0.9.6 (10 April 2000)
Copyright (C) 1997-2000  Daniel Azuma
All rights reserved worldwide


==============================================================================


CONTENTS

   (1) Introduction

   (2) Terms and conditions

   (3) Release notes 0.9.6

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
    Version 0.9.6 (10 April 2000)

    These notes detail the differences between versions 0.9.5 and 0.9.6.
    A complete history of publicly released versions can be found on the
    GLOW web site.

    This release of GLOW focused on getting Microsoft Visual Dev Studio
    (Visual C++ 6.0) to work with GLOW, finishing up a few lingering
    unfinished features, and writing lesson 5.

    In this release, most of the integer constants have been changed to
    enumerations. This was done so that more typechecking (correctness
    checking of parameters) can be done at compile time, and also
	because MSVC 6.0 didn't seem to like static const ints. For the most
    part, this shouldn't affect your usage, unless you used ints to
    store some of these values. However, there are two places where this
    will be visible to you and require code changes:

    The OnMouseDown(), OnMouseUp() and OnKeyboard() methods, plus the
    corresponding widget methods, will need their parameter lists
    updated. For example, if you declared:

       MyWindow::OnMouseDown(int button, int x, int y, int modifiers)

    That needs to be changed to:

       MyWindow::OnMouseDown(Glow::MouseButton button, int x, int y,
          Glow::Modifiers modifiers);

    Note that, in addition to types, the order of the parameters for
    OnKeyboard() has been modified. So if you declared:

       MyWindow::OnKeyboard(int key, int modifiers, int x, int y)

    That needs to be changed to:

       MyWindow::OnKeyboard(Glow::KeyCode key, int x, int y,
          Glow::Modifiers modifiers);

    Second, the constant "parentSetting", used in the QuickPalette
    API, needs to be changed for arrangements and alignments. If you
    were using it for an arrangement setting, you need to change it to
    "parentArrangement". If you were using it for an alignment setting,
    it needs to be changed to "parentAlignment".


    Features added
    --------------

      Added GlowWidgetRoot::FindWidget().

      Added GlowPushButtonWidget::Hit(), GlowStickyButtonWidget::Hit(),
	  GlowCheckBoxWidget::ToggleState() and GlowCheckBoxWidget::Hit()
	  and GlowStickyButtonWidget::ToggleState().

      Added mouse and keyboard event filtering to Glow, and keyboard
	  filtering to GlowWidgetRoot.

      Added two widget filters: GlowWidgetTabFilter for tabbing between
	  keyboard-handling widgets, and GlowWidgetMapToPushButtonFilter for
	  mapping keypresses to buttons.

      GlowMessageWindow and GlowTextFieldWindow can now automatically
	  map return and escape keypresses to buttons.

      Added SetIdentity() method to GlowTransformData, GlowViewTransform
      and GlowViewManipulator.

      Added ReceiverTracker class for tracking and automatic deletion
	  of receivers. Added GlowReceiverTrackerComponent.

      Implemented a few additional hacks to get MSVC compatibility.


    Features changed
    ----------------

      Changed the order of parameters in GlowSubwindow::OnKeyboard() and
      GlowWidget::OnWidgetKeyboard() to (key, x, y, modifiers) for
      consistency with the mouse methods.

      Renamed GlowPushButtonWidget::OnPressed() to OnHit(),
	  GlowCheckBoxWidget::OnToggled() to OnHit() and
	  GlowStickyButtonWidget::OnPressed() to OnHit().

      Renamed ApplyGLMatrix() to ApplyToGLMatrix(), and
      ApplyGLMatrixInverse() to ApplyInverseToGLMatrix() in
      GlowTransformData, GlowViewTransform and GlowViewManipulator.

      Renamed GlowViewManipulator::State() to GetState().

      Implemented a better system for handling compilers that put the
      C++ standard library in namespace std but the C library in the
      global namespace (e.g. MIPSPro, MSVC). Now, #defining
      GLOW_COMPAT_CLIBNOSTDNAMESPACE causes glow to assume that only
      the C standard library functions are in the global namespace.
      The old method of using GLOW_COMPAT_INTERNALUSINGSTD is now
      deprecated and will probably go away soon.


    Bugs fixed
    ----------

      GlowPopupMenuWidget constructor hung if the items field of the
      params was nonzero (nonempty). Introduced in 0.9.5 (oops!) Fixed.

      Packing a QuickPanel arranged horizontally with alignExpand,
      containing labeled QuickPanels, caused the enclosed panels to end
      up too tall. Fixed.

      Determining spin rate in GlowViewManipulator didn't work too
      well for very fast frame rates. Fixed.

      GlowViewManipulator's constructor and initialization ignored the
      spinnable field in its params. Fixed.

      Glow::Version() didn't return the correct value in 0.9.5. Fixed.

      A call to std::find() in glowSenderReceiver.inl.h wasn't put into
      namespace std. Fixed.

      Several references to std::vector in glowTextFieldWindow.cpp and
      glowMessageWindow.cpp weren't put into namespace std. Fixed.

      Quaternion i/o operators were incorrectly defined inline in
      glowVectorAlgebra.cpp. Fixed.

      A whole bunch of errors and broken links in the reference pages
      were fixed. (However, a bunch of changes were made as a result
      of the API adjustments for this version, and so new errors may
      have been introduced.)


    Internal changes
    ----------------

      Changed all the static constants from static const int to enum,
      for MSVC compatibility. Shouldn't really affect the interface.

      GlowWidget no longer keeps an extra children list. (dynamic_cast
      is now used to iterate over child widgets.)

      Switched around some of the internal classes in QuickPalette.
      Shouldn't really affect the user-level API.


    Known issues
    ------------

      Because of the continuing incomplete state of GLUT in the Mac OS
      OpenGL SDK, many features of GLOW are still unusable on the Mac.
      Hopefully this will be resolved soon.

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

