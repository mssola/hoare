
# About this [![Build Status](https://travis-ci.org/mssola/hoare.svg?branch=master)](https://travis-ci.org/mssola/hoare)

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

    $ ./hoare file.hoare
    $ ./hoare -f file.hoare

The first command will effectively execute the `file.hoare` file, and the
second command will just print LLVM's IR code for the given file. Note that
with the second command you can produce a binary file:

    $ ./hoare -f file.hoare > file.bc
    $ clang file.bc -o file
    $ ./file

## License

Copyright &copy; 2014 Miquel Sabaté Solà

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

