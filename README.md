sequitur
========

This is `sequitur`. `sequitur` is something like a multiplexing software.

It reads from `stdin` if option `-q` is missing. `module(s)` are loaded dynamically and each is started in a separate thread. Each thread gets
the same data from `sequitur`.

Usage
-----

    $ ./sequitur
    missing module(s) to load
    $ ./sequitur -h
    ./sequitur [OPTIONS] module [module [module]]
       starts dynamically the given module(s)
       -V	print version and exit
       -h	prints this help and exit
       -q	source to use
         	if no file is given, the input is read from stdin
    $ echo "123" | ./sequitur ./exemplum.so ./exemplum2.so
    Hello World from 1
    Hello World from 2
    123
    [2] \x31
    [2] \x32
    [2] \x33
    [2] \x0a
    [1] 1
    [1] 2
    [1] 3
    [1]

License
-------

Copyright 2014 Florian Lehner <dev@der-flo.net>

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are
met:

    (1) Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.

    (2) Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in
    the documentation and/or other materials provided with the
    distribution.

    (3)The name of the author may not be used to
    endorse or promote products derived from this software without
    specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE AUTHOR ''AS IS'' AND ANY EXPRESS OR
IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT,
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.
