
# About this

This project will try to implement the language described in the paper
["Communicating Sequential Processes"](http://spinroot.com/courses/summer/Papers/hoare_1978.pdf).

I've created this project out of curiosity, just for fun. Don't expect any
serious thing out of it ;-)

This is "Work on Progress" and I'm developing it in my spare time (which is
minimal), so don't expect this project to be finished anytime soon.

## Language

The language is being implemented using the exact same syntax as described in
the paper. I've also included though C's `printf` as a builtin function, so
it's easier to debug what's going on.

## Building & playing around

Since it's a personal project, I don't care that much about compiler
requirements, so I'm just sticking with the latest versions of GCC & Clang. This
allows me to use features from C++11 and C++14. To build this guy you need
`bison` and `LLVM` installed. With this in mind, you can just:

    $ mkdir -p build && cd build
    $ cmake ..
    $ make

And it should produce the `hoare` executable file. This file can be executed
like this:

    $ hoare file.hoare
    $ hoare -S file.hoare

The first command will effectively execute the `file.hoare` file. The
second command will produce a `.ll` file (`file.ll` in our previous
example). This `.ll` file contains the same input file translated into LLVM's
IR. Note that you can compile this file with `clang` in order to produce a
binary file. For example:

    $ hoare -S file.hoare
    $ clang file.ll -o file
    $ ./file

Moreover, you can pass as many input files as you want in a simple command. The
usage of this compiler is as follows:

    hoare [options] <input files>

## License

Copyright (C) 2014 Miquel Sabaté Solà <mikisabate@gmail.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

