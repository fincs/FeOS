FeOS SDK user libraries
=======================

This folder contains library folders (that have include and lib subfolders) that programs can link to.
You're completely free to place here your own libraries.

In order to use them them, edit the makefile as follows:

- Add the name of the folder that contains the library to the CONF_USERLIBS variable.
- Add -llibname to the CONF_LIBS variable.

For example:

CONF_USERLIBS := examplelib
CONF_LIBS := -lexamplelib
