/*
MIT License

Copyright (c) 2022 groboclown

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
#ifndef FS_SHELL_ARGS
#define FS_SHELL_ARGS

/**
 * @brief Parse the shell execution request into tokens.
 * 
 * After calling this and all advanceToken calls, "closeTokenizer()" must be called.
 * 
 * @return 0 if okay, != 0 if problem.
 */
int setupTokenizer(const int srcArgc, char *srcArgv[]);

/**
 * @brief advance to the next token.
 * 
 * "tokenize" must be called first.
 * 
 * @return const char* 
 */
const char *advanceToken();

/**
 * @brief Clean out the tokenizer after using it.
 * 
 * @return int 0 if okay, non-zero if error.
 */
int closeTokenizer();

#endif
