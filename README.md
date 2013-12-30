EmbedPy Compiler
================

EmbedPy is a small subset of the Python language and standard library, 
compiled to run in a virtual machine on embedded controller hardware.

This repository contains the compiler, which is designed to present an API
that is easily consumable by applications, with a very simple IDE planned.

Optimisation and code generation will likely be implemented using LLVM.

Current Progress
----------------

Currently the lexical analysis is working fairly well. Parsing is in its
very early stages, and code generation is not yet started.

There is a simple CLI frontend, which has functions to test individual
parts of the compiler.

The virtual machine has also not been started.

Building and running
--------------------

Building from git requires libtool, make, automake and autoconf. Run
`autoreconf --force --install --verbose` to generate the configure script
and then `./configure` to generate makefiles. Then you can just run `make`
to build everything.

You can test the tokeniser by running

    # ./embedpy -t examples/function.py

and test the parser in interactive mode by just executing the frontend

    # ./embedpy
