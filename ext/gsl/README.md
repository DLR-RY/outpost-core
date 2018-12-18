GSL: Guideline Support Library 
=================================

# viboes/GSL - Microsoft/GSL fork supporting C++11/C++14/C++1z compilers

This is the C++11 fork of the GSL LIbrary, to make it compile for C++11 compilers.

Note that the CMakeList.txt needs to be adapted yet so that you will need to hack them  :(

The fork adds a gsl_config.hpp file to manage with C++14 constexpr and the definition in stdex of some C++14 library missing in C++11.

The GSL sources have been modified taking in account the constexpr macro and the stdex features.

## Features

See also section [GSL: Guideline support library](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#S-gsl) of the C++ Core Guidelines [2]. 

V11-GSL means master branch of this fork viboes-GSL.
 
Feature / library           | GSL     | M-GSL   | GSL-Lite| V11-GSL   | Notes |
----------------------------|:-------:|:-------:|:-------:|:--------:|:-------|
**1.Lifetime&nbsp;safety**  | &nbsp;  | &nbsp;  | &nbsp;  | &nbsp;  | &nbsp; |
**1.1 Indirection**         | &nbsp;  | &nbsp;  | &nbsp;  | &nbsp;  | &nbsp; |
not_null<>                  | &#10003;| &#10003;| &#10003;| &#10003;| Wrap any indirection and enforce non-null |
maybe_null<>                | -       | &#10003;| -       | -       | &nbsp; |
**1.2 Ownership**           | &nbsp;  | &nbsp;  | &nbsp;  | &nbsp;  | &nbsp; |
owner<>                     | &#10003;| &#10003;| >=C++11 | &#10003;| Owned raw pointers |
Owner()                     | -       | -       | &#10003;| -       | Macro for pre-C++11; |
unique_ptr<>                | &#10003;| &#10003;| >=C++11 |&#10003;| std::unique_ptr<> |
unique_ptr<>                | -       | -       | < C++11 | -       |VC10, VC11 |
shared_ptr<>                | &#10003;| &#10003;| >=C++11 | &#10003;| std::shared_ptr<> |
shared_ptr<>                | -       | -       | < C++11 | -       | VC10, VC11 |
stack_array<>               | &#10003;| -       | -       | -       | A stack-allocated array, fixed size |
dyn_array<>                 | ?       | -       | -       | -       | A heap-allocated array, fixed size |
**2.Bounds&nbsp;safety**    | &nbsp;  | &nbsp;  | &nbsp;  | &nbsp;  | &nbsp; |
**2.1 Tag Types**           | &nbsp;  | &nbsp;  | &nbsp;  | &nbsp;  | &nbsp; |
zstring                     | &#10003;| &#10003;| &#10003;| &#10003;| a char* (C-style string) |
wzstring                    | -       | &#10003;| &#10003;| &#10003;| a wchar_t* (C-style string) |
czstring                    | &#10003;| &#10003;| &#10003;| &#10003;| a const char* (C-style string) |
cwzstring                   | -       | &#10003;| &#10003;| &#10003;| a const wchar_t* (C-style string) |
**2.2 Views**               | &nbsp;  | &nbsp;  | &nbsp;  | &nbsp; | &nbsp; |
span<>                      | &#10003;| &#10003;| 1D views| &#10003;| A view of contiguous T's, replace (*,len) |
span_p<>                    | &#10003;| -       | -       | -       | A view of contiguous T's that ends at the first element for which predicate(*p) is true |
as_span()                   | -       | &#10003;| &#10003;| &#10003;| Create a span |
string_span                 | &#10003;| &#10003;| &#10003;| &#10003;| span&lt;char> |
wstring_span                | -       | &#10003;| &#10003;| &#10003;| span&lt;wchar_t > |
cstring_span                | &#10003;| &#10003;| &#10003;| &#10003;| span&lt;const char> |
cwstring_span               | -       | &#10003;| &#10003;| &#10003;| span&lt;const wchar_t > |
ensure_z()                  | -       | &#10003;| &#10003;| &#10003;| Create a cstring_span or cwstring_span |
to_string()                 | -       | &#10003;| &#10003;| &#10003;| Convert a string_span to std::string or std::wstring |
**2.3 Indexing**            | &nbsp;  | &nbsp;  | &nbsp;  | &nbsp;  | &nbsp; |
at()                        | &#10003;| &#10003;| >=C++11 | &#10003;| Bounds-checked way of accessing<br>static arrays, std::array, std::vector |
at()                        | -       | -       | < C++11 | -       | static arrays, std::vector<br>std::array : VC11 |
**3. Assertions**           | &nbsp;  | &nbsp;  | &nbsp;  | &nbsp; | &nbsp; |
Expects()                   | &#10003;| &#10003;| &#10003;| &#10003;| Precondition assertion |
Ensures()                   | &#10003;| &#10003;| &#10003;| &#10003;| Postcondition assertion |
**4. Utilities**            | &nbsp;  | &nbsp;  | &nbsp;  | &nbsp; | &nbsp; |
final_act<>                 | &#10003;| &#10003;| >=C++11 | &#10003;| Action at the end of a scope |
final_act                   | -       | -       | < C++11 | &#10003;| Currently only void(*)() |
finally()                   | &#10003;| &#10003;| >=C++11 | &#10003;| Make a final_act<> |
finally()                   | -       | -       | < C++11 | &#10003;| Make a final_act |
narrow_cast<>               | &#10003;| &#10003;| &#10003;| &#10003;| Searchable narrowing casts of values |
narrow()                    | &#10003;| &#10003;| &#10003;| &#10003;| Checked version of narrow_cast() |
implicit                    | &#10003;| -       | &#10003;| -       | Symmetric with explicit |
move_owner                  | ?       | -       | -       | -       | ... |
**5. Concepts**             | &nbsp;  | &nbsp;  | &nbsp;  | &nbsp; | &nbsp; |
...                         | &nbsp;  | &nbsp;  | &nbsp;  | &nbsp; | &nbsp; |


## Reported to work with

The table below mentions the compiler versions viboes-GSL master is reported to work with.

OS        | Compiler   | Versions | Status | Language version | 
---------:|:-----------|:---------|--------|------------------|
Windows   | Clang/LLVM | ?        |&#10003;| ?                |
&nbsp;    | GCC        | ?        |&#10003;| ?                |
&nbsp;    | Visual C++<br>(Visual Studio)| 8 (2005), 10 (2010), 11 (2012),<br>12 (2013), 14 (2015) |?| ? |
GNU/Linux | Clang/LLVM | 3.4      | ? | ? | 
&nbsp;    | GCC        | 5.1      | ? | ? |
OS X      | Clang/LLVM | 3.7.0    |&#10003;| -std=c++11 -std=c++14 -std=c++1z |
&nbsp;    | Clang/LLVM | 3.9.0    |&#10003;| -std=c++11 -std=c++14 -std=c++1z |
&nbsp;    | GCC        | 5.2.0    |&#10003;| -std=c++11 -std=c++14 -std=c++1z | 
&nbsp;    | GCC        | 6.1.0    |&#10003;| -std=c++11 -std=c++14 -std=c++1z |


## Other GSL implementations

- Microsoft. [Guidelines Support Library (GSL)](https://github.com/microsoft/gsl).
- Microsoft. [GSL Lite](https://github.com/martinmoene/gsl-lite).
- Mattia Basaglia. CxxMiscLib [gsl.hpp](https://github.com/mbasaglia/Cxx-MiscLib/blob/master/include/misclib/gsl.hpp), [tests](https://github.com/mbasaglia/Cxx-MiscLib/blob/master/test/gsl.cpp).

#@ Notes and references

### References

[1] [Standard C++ Foundation](https://isocpp.org/).  
[2] Standard C++ Foundation. [C++ Core Guidelines](https://github.com/isocpp/CppCoreGuidelines).  
[3] Microsoft. [Guidelines Support Library (GSL)](https://github.com/microsoft/gsl).  
[4] Bjarne Stroustrup. [Writing good C++14 (PDF)](https://github.com/isocpp/CppCoreGuidelines/raw/master/talks/Stroustrup%20-%20CppCon%202015%20keynote.pdf) &mdash; [Video](https://www.youtube.com/watch?t=9&v=1OEu9C51K2A). CppCon 2015.  
[5] Herb Sutter. [Writing good C++14&hellip; By default (PDF)](https://github.com/isocpp/CppCoreGuidelines/raw/master/talks/Sutter%20-%20CppCon%202015%20day%202%20plenary%20.pdf) &mdash; [Video](https://www.youtube.com/watch?v=hEx5DNLWGgA). CppCon 2015.  
[6] Gabriel Dos Reis. [Contracts for Dependable C++ (PDF)](https://github.com/isocpp/CppCoreGuidelines/raw/master/talks/Contracts-for-Dependable-C%2B%2B.pdf)  &mdash; Video. CppCon 2015.  
[7] Bjarne Stroustrup et al. [A brief introduction to C++’s model for type- and resource-safety](https://github.com/isocpp/CppCoreGuidelines/raw/master/docs/Introduction%20to%20type%20and%20resource%20safety.pdf).  
[8] Herb Sutter and Neil MacIntosh. [Lifetime Safety: Preventing Leaks and Dangling](https://github.com/isocpp/CppCoreGuidelines/raw/master/docs/Lifetimes%20I%20and%20II%20-%20v0.9.1.pdf). 21 Sep 2015.
Some useful links
[9] Brief documentation of [GSL: Guideline support library](http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#S-gsl).
[10] [Talks and slides](https://github.com/isocpp/CppCoreGuidelines/tree/master/talks) related to GSL and the C++ Core Guielines in particular:
[11] [Evolving array_view and string_view for safe C++ code](https://github.com/isocpp/CppCoreGuidelines/blob/master/talks/MacIntosh%20-%20A%20Few%20Good%20Types.pdf)     [Video](https://www.youtube.com/watch?v=C4Z3c4Sv52U)



# Microsoft/GSL

The Guideline Support Library (GSL) contains functions and types that are suggested for use by the
[C++ Core Guidelines](https://github.com/isocpp/CppCoreGuidelines) maintained by the [Standard C++ Foundation](https://isocpp.org).
This repo contains Microsoft's implementation of GSL.

The library includes types like `span<T>`, `string_span`, `owner<>` and others.

The entire implementation is provided inline in the headers under the [gsl](./gsl) directory. The implementation generally assumes a platform that implements C++14 support. There are specific workarounds to support MSVC 2013 and 2015.

While some types have been broken out into their own headers (e.g. [gsl/span](./gsl/span)),
it is simplest to just include [gsl/gsl](./gsl/gsl) and gain access to the entire library.

> NOTE: We encourage contributions that improve or refine any of the types in this library as well as ports to
other platforms. Please see [CONTRIBUTING.md](./CONTRIBUTING.md) for more information about contributing.

# Project Code of Conduct
This project has adopted the [Microsoft Open Source Code of Conduct](https://opensource.microsoft.com/codeofconduct/). For more information see the [Code of Conduct FAQ](https://opensource.microsoft.com/codeofconduct/faq/) or contact [opencode@microsoft.com](mailto:opencode@microsoft.com) with any additional questions or comments.

# Quick Start
## Supported Platforms
The test suite that exercises GSL has been built and passes successfully on the following platforms:<sup>1)</sup>

* Windows using Visual Studio 2013
* Windows using Visual Studio 2015
* Windows using Visual Studio 2017
* Windows using Clang/LLVM 3.6
* Windows using GCC 5.1
* GNU/Linux using Clang/LLVM 3.6
* GNU/Linux using GCC 5.1
* OS X Yosemite using Xcode with AppleClang 7.0.0.7000072
* OS X Yosemite using GCC-5.2.0
* FreeBSD 10.x with Clang/LLVM 3.6

> If you successfully port GSL to another platform, we would love to hear from you. Please submit an issue to let us know. Also please consider
contributing any changes that were necessary back to this project to benefit the wider community.

<sup>1)</sup> For `gsl::byte` to work correctly with Clang and GCC you might have to use the ` -fno-strict-aliasing` compiler option.

## Building the tests
To build the tests, you will require the following:

* [CMake](http://cmake.org), version 2.8.7 or later to be installed and in your PATH.
* [UnitTest-cpp](https://github.com/Microsoft/unittest-cpp), to be cloned under the [tests/unittest-cpp](./tests/unittest-cpp) directory
of your GSL source.

These steps assume the source code of this repository has been cloned into a directory named `c:\GSL`.

1. Create a directory to contain the build outputs for a particular architecture (we name it c:\GSL\build-x86 in this example).

        cd GSL
        md build-x86
        cd build-x86

2. Configure CMake to use the compiler of your choice (you can see a list by running `cmake --help`).

        cmake -G "Visual Studio 14 2015" c:\GSL

3. Build the test suite (in this case, in the Debug configuration, Release is another good choice).    

        cmake --build . --config Debug

4. Run the test suite.    

        ctest -C Debug

All tests should pass - indicating your platform is fully supported and you are ready to use the GSL types!

## Using the libraries
As the types are entirely implemented inline in headers, there are no linking requirements.

You can copy the [gsl](./gsl) directory into your source tree so it is available
to your compiler, then include the appropriate headers in your program.

Alternatively set your compiler's *include path* flag to point to the GSL development folder (`c:\GSL` in the example above) or installation folder (after running the install). Eg.

MSVC++

    /I c:\GSL

GCC/clang

    -I$HOME/dev/GSL

Include the library using:

    #include <gsl/gsl>

<<<<<<< HEAD

    
=======
## Debugging visualization support
For Visual Studio users, the file [GSL.natvis](./GSL.natvis) in the root directory of the repository can be added to your project if you would like more helpful visualization of GSL types in the Visual Studio debugger than would be offered by default.
>>>>>>> 96eaf274f8c57829080100f7cd1d2e7744bae1ae
