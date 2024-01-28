/*
MIT License

Copyright (c) 2024 groboclown

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
#ifndef _FS_SHELL__VCOMPAT_
#define _FS_SHELL__VCOMPAT_

// Precision Shell Version compatibility.
// Some things changed between versions.  By including this, it allows for
//   old environment names to work with the current version.

// v4.4.1 <-> v4.4.2 compatibility.
//   Fixes the typo, so that USE_ENVIRONMENT_INPUT is now the documented and recommended name.
#ifdef USE_ENVIROMENT_INPUT
#define USE_ENVIRONMENT_INPUT
#endif

#endif /* _FS_SHELL__VCOMPAT_ */
