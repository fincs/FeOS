FeOS v0.0-prerelease
====================

Introduction
------------

FeOS is a free, open-source operating system for the Nintendo DS. Its aim is to provide a *usable* abstraction layer that builds on top of said platform. Because the Nintendo DS is a *very* resource-constrained platform, a port of a multi-platform OS is utterly pointless, even if said OS is designed for embedded platforms. Therefore, the OS must be specifically written for the DS.

Build prerequisites
-------------------

You need the following in order to build FeOS:

- devkitARM r34 or higher
- libnds 1.5.2 or higher
- libfat 1.0.10 or higher
- A working C compiler for the host (Windows users: use MinGW)

Before building, you must set the `FEOSSDK` environment variable to point to the `/sdk` directory (if on Windows, you **must** use Unix-style paths, like `/c/Users/.../gitrepos/FeOS/sdk`).

How to build the FeOS SDK
-------------------------

`cd` to the `/sdk` directory and run `make` on it.

How to build FeOS
-----------------

`cd` to the following directories and run `make` on them:

- `/kernel` (FeOS kernel)
- `/cmdprompt` (FeOS command prompt)

How to transfer FeOS to the SD card
-----------------------------------

- Create a `/data/FeOS/` folder in the SD card.
- Create the folders `bin/` and `lib/` inside it. This is where FeOS apps and libraries will be stored, respectively.
- Copy `/kernel/FeOS.nds` to the root of the SD card.
- Copy `/cmdprompt/cmd.fx2` to the binary folder.
- Now copy the applications you want (read the "Demo applications" section).

How to transfer a FeOS application to the SD card
-------------------------------------------------

Just copy the fx2 file to the `/data/FeOS/bin/` folder.

The application might have dependencies on some libraries. If so, then copy the libraries to the `/data/FeOS/lib/` folder.

Demo applications
-----------------

This repo contains some demo applications:
- `/testbin`: it tests some FeOS features.
- `/sdk/examples/hello_world`: the classic Hello World program.

Opening the source in Programmers Notepad
-----------------------------------------

- Start Programmers Notepad
- `File` -> `Open Project(s)...` -> Select `FeOS.ppg` -> Click `Open`

To do
-----

- Write the *To do* section