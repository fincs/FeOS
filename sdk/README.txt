 _______________________________
/                               \
| FeOS Software Development Kit | v0.1-alpha Developer Preview
\_______________________________/

Copydinner (cd) fincs 2010-13, All Meals Reserved

Introduction
============

The FeOS SDK allows you to develop software for FeOS. It is intended to be used with a working
devkitARM and libnds installation. This SDK also comes with all the information you need in
order to get started.

Installation
============

Place this folder in any accessible place, then set the FEOSSDK environment variable to point to it.
If using Windows, you need to specify it using a Unix-style path (like /c/devkitPro/FeOS-SDK).

If you are building FeOS from source this folder is part of the FeOS repository and it should stay
where it is right now.

Folder Description
==================

bin        - Contains development tools required by the build system.
examples   - Contains software examples.
include    - Contains header files.
lib        - Contains supporting run-time code.
mk         - Contains the build system.
templates  - Contains project templates.
userlib    - Contains user library folders. The SDK comes preloaded with headers and import stubs for
             FeOS' system libraries.

Templates
=========

arm7module     - ARM7 module project. Needs libnds to be installed.
consoleapp     - Console application project with package building support.
coopguiapp     - Coop GUI application project with package building and app registration support.
directmodeapp  - Direct mode application project with package building and app registration support.
sharedlib      - Shared library (aka DLL) project with package building support.
staticlib      - Static library project.

Documentation
=============

I lied, there is no documentation yet :D

Help resources:

  feos.mtheall.com/forum/
  #FeOS at irc.blitzed.org
