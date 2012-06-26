FeOS v0.0-prerelease (WIP)
==========================

Introduction
------------

FeOS is a free, open-source operating system for the Nintendo DS. Its aim is to provide a *usable* abstraction layer that builds on top of said platform. Because the Nintendo DS is a *very* resource-constrained platform, a port of a multi-platform OS is utterly pointless, even if said OS is designed for embedded platforms. Therefore, the OS must be specifically written for the DS.

Build prerequisites
-------------------

You need the following in order to build FeOS:

- devkitARM r41 or higher
- libnds 1.5.8 or higher
- libfat 1.0.11 or higher
- dswifi 0.3.15 or higher
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

How to transfer a FeOS module to the SD card
--------------------------------------------

Use the following command to compile the project:

    make install

The module might have dependencies on some libraries. If so, then repeat this procedure for them.

Demo applications
-----------------

This repo contains some demo applications:

- `/sdk/examples/hello_world`: the classic Hello World program.
- `/sdk/examples/time`: it prints the current time and date.
- `/sdk/examples/inputtest`: basic keypad/touchscreen test.
- `/sdk/examples/irqdemo`: basic IRQ hooking test.
- `/sdk/examples/directmode`: shows how to build a simple Direct mode application.
- `/sdk/examples/arm7let`: shows how to build a simple ARM7 extension module.
- `/sdk/examples/load_arm7`: shows how to load the before-mentioned ARM7 module.

The command prompt
------------------

FeOS comes with a command prompt utility that is run on startup. It also ships with several Unix-like commands, courtesy of mtheall. Refer to the source code of each of them at `/apps/unixtools` for more information.

The command prompt provides these pseudo-commands:

- `exit <code>`: it exits from the current cmd session with the specified error code. If omitted, it defaults to 0.
- `echo <string>`: it outputs the specified string plus a newline. If omitted, it defaults to an empty string. **NOTE**: if your string contains spaces, you have to wrap it in quotes, `"like this"`.

You can create .cmd files, which contain a list of commands to run. In order to execute them, type this:

    cmd <filename>

FeOS runs `/data/FeOS/autoexec.cmd` on startup, in a similar fashion to MS-DOS' `autoexec.bat`.

When an application hangs...
----------------------------

When an application hangs, you can terminate said application by holding down Down on the D-pad then pressing the "Menu" key on the on-screen keyboard.

**WARNING**: This feature forcefully exits the application. It could lead to system unstability, memory leaks, disk corruption and/or data loss. **Use with caution**. It also does not work with multi-threaded code.

To do
-----

- Write the *To do* section
