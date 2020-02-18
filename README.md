# uintx_t

The *extensible unsigned integer* `uintx_t` is an arbitrary-precision
unsigned integer type designed to be used in place of their fixed-precision
counterparts with minimal code changes (usually just an `x` here or there).

## Compiling the Code
### Windows and Visual Studio

Two Visual Studio solution files, `uintx_t.sln` and `test.sln`
have been provided. You must compile `uintx_t.sln` first. It generates
a library file `uintx_t.lib`, the appropriate version of which for your platform and
configuration can be found in the corresponding sub-folder of `lib`.
You may then compile `test.sln` if you wish, although this is optional.
Open `test.sln` and examine the `properties` of project `test` to see how
to link `uintx_t.lib` into your own Visual Studio solutions.
The solution files have been tested with Visual Studio 2019 Community under Windows 10.

### UNIX and g++

A `makefile` for `g++` has been placed in the root directory. Simply type `make all`.
The library files`uintx_t.a` in `lib` can then be linked into your own projects
(see `test/Src/makefile` for an example).
A test executable file `test.exe` will be found in the root directory. 
The makefile has been tested with g++ 7.4 on the Ubuntu 18.04.1 subsystem under Windows 10.

## License

This project is released under the [MIT License](https://github.com/Ian-Parberry/Tourney/blob/master/LICENSE).