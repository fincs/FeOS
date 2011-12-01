FeOS v0.0-prerelease
====================

Introduction
------------

FeOS is a free, open-source operating system for the Nintendo DS. Its aim is to provide a *usable* abstraction layer that builds on top of said platform. Because the Nintendo DS is a *very* resource-constrained platform, a port of a multi-platform OS is utterly pointless, even if said OS is designed for embedded platforms. Therefore, the OS must be specifically written for the DS.

Build prerequisites
-------------------

You need the following in order to build FeOS:

- devkitARM r34 or higher
- libnds 1.5.3 or higher
- libfat 1.0.10 or higher
- default ARM7 0.5.22 or higher
- A working C compiler for the host (Windows users: use MinGW)

Before building, you must set the `FEOSSDK` environment variable to point to the `/sdk` directory (if on Windows, you **must** use Unix-style paths, like `/c/Users/.../gitrepos/FeOS/sdk`).

Environmental Variables
-----------------------

The following Environmental Variables are available to configure FeOS:

| Name          | Function                                             | Default Value        |
|:--------------|:-----------------------------------------------------|:---------------------|
| FEOSSDK       | Path to `/sdk` directory                             | MUST BE USER-DEFINED |
| FEOSDEST      | Base path to place files with `make install` command | ${FEOSSDK}/../FeOS   |
| FEOSBUILD_ALL | Components to build with the `make all` command      | sdk lib apps         |

How to build FeOS (including the SDK)
-------------------------------------

`cd` to the directory this repo is in and type the following:

    make install

When it's done compiling, transfer the contents of the generated `FeOS` folder to the root of your SD card.

How to transfer a FeOS application to the SD card
-------------------------------------------------

Just copy the fx2 file to the `/data/FeOS/bin/` folder in the SD card.

The application might have dependencies on some libraries. If so, then copy the libraries to the `/data/FeOS/lib/` folder.

Demo applications
-----------------

This repo contains some demo applications:

- `/testbin`: it tests some FeOS features.
- `/sdk/examples/hello_world`: the classic Hello World program.
- `/sdk/example/time`: it prints the current time and date.

Default commands
----------------

FeOS comes with several Unix-like commands: `ls`, `cd`, `pwd` and `cat`; courtesy of mtheall. Refer to the source code of each of them at `/apps/unixtools` for more information.

When an application hangs...
----------------------------

When an application hangs, you can terminate said application by holding down Down on the D-pad then pressing the "Menu" key on the on-screen keyboard.

**WARNING**: This feature forcefully exits the application. It could lead to system unstability, memory leaks, disk corruption and/or data loss. **Use with caution**.

Opening the source in Programmers Notepad
-----------------------------------------

- Start Programmers Notepad
- `File` -> `Open Project(s)...` -> Select `FeOS.ppg` -> Click `Open`

To do
-----

- Write the *To do* section
