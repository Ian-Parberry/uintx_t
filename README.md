# uintx_t

The *extensible unsigned integer* `uintx_t` is an arbitrary-precision
unsigned integer type designed to be used in place of its fixed-precision
counterparts `uint32_t` and `uint64_t` (also known as `unsigned int` and 
`unsigned long long`) with minimal code changes (usually just an `x` here or there).

## Compiling the Code

The code can be compiled under UNIX variants with g++ and under Windows
with Visual Studio. Both 32-bit and 64-bit targets are supported. 

### Windows and Visual Studio

Two Visual Studio solution files `uintx_t.sln` and `test.sln`
can be found at the root of this repository.
 You must compile `uintx_t.sln` first. It generates
a library file `uintx_t.lib`, the appropriate version of which for your platform and
configuration can be found in the corresponding sub-folder of `lib`.
You may then compile `test.sln` if you wish, although this is optional.
Open `test.sln` and examine the `properties` of project `test` to see how
to link `uintx_t.lib` into your own Visual Studio solutions.
This has been tested with Visual Studio 2019 Community under Windows 10.

### UNIX and g++

A `makefile` for `g++` can be found at the root of this repository. Simply type `make all`.
The library file `uintx_t.a` in `lib` can then be linked into your own projects
(see `test/Src/makefile` for an example).
An executable file `test.exe` will also be found in the root directory. 
This has been tested with g++ 7.4 on the Ubuntu 18.04.1 subsystem under Windows 10.

## Code Documentation

Doxygen-generated documentation is available for
[uintx_t](https://ian-parberry.github.io/uintx_t)
and the
[test program](https://ian-parberry.github.io/uintx_t/html2).

## License

This project is released under the [MIT License](https://github.com/Ian-Parberry/Tourney/blob/master/LICENSE).

## To-Do List

1. Perhaps a new function `const uintx_t rootx(const uintx_t x, uint32_t n)` for computing the `n`th root of `x`. The code would be similar to that of `const uintx_t sqrtx(const uintx_t x)` but there's some serious bit-twiddling that needs to be done.

2. Memory management is a bit naive and will certainly slow things down if you really hammer on `uintx_t`. I should probably grow memory in powers-of-2 chunks and shrink it when it's twice as big as it needs to be. On the other hand, `m_pData` could easily be replaced by an instance of `std::vector<uint32_t>`. That would be less work, but then I would have to trust `std` to do the memory management for me, which I don't entirely. Also a shallow copy operation might help reduce the amount of memory being casually slung around in many of the operators.

3. There are faster algorithms that I could use, I'm thinking primarily of multiplication (the standard divide-and-conquer algorithm) and maybe the factorial function.

4. An extensible signed integer `intx_t` might be in the future. I probably wouldn't bother with two's complement, just a sign bit.