unixtools
=========

These are tools that provide a unix-style interface to `FeOS`

cat
---

Concatenate files and print on the standard output

Usage:

    cat [FILE]...

cd
--

Change working directory

Usage:

    cd <PATH>

cp
--

Copy files

Usage:

    cp <SOURCE FILE>... <TARGET DIRECTORY>
    cp <SOURCE FILE> <TARGET FILE>

ls
--

List directory contents

Usage:

    ls [PATH]...

mkdir
-----

Create a directory

Usage:

    mkdir [DIR]...

mv
--

Rename a file or directory

Usage:

    mv <SOURCE> <TARGET>

pwd
---

Print working directory

Usage:

    pwd

rm
--

Delete a file or directory

Usage:

    rm [TARGET]...

*NOTE*: This command may give an invalid error message if TARGET is a non-empty directory, but it will correctly error.

