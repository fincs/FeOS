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

`cd` to the following directories and run `make` on them:
- `/sdk/bin/fxe2tool_src/` (ELF to FXE2 converter)
- `/sdk/` (libfeos, for use in user programs)

How to build FeOS
-----------------

`cd` to the following directories and run `make` on them:
- `/kernel` (FeOS kernel)
- `/testbin` (Test executable)

How to transfer FeOS to the SD card
-----------------------------------

- Create a `/data/FeOS/sys/` folder in the SD card.
- Copy `/kernel/FeOS.nds` to the root of the SD card.
- Copy `/testbin/testbin.imp.a` to `/data/FeOS/sys/` in the SD card.

To do
-----

- Write the *To do* section