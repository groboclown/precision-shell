dnl Uses the same input as cmd-generator.m4, but adds to the command list.
dnl The execution requires the argument "FILE_NAMES" to contain a comma
dnl separated list of macro command file names.
changecom(`@@')dnl
dnl Helpers
define(`ForEach',`ifelse(eval($#>2),1,
`pushdef(`$1',`$3')$2`'popdef(`$1')dnl
`'ifelse(eval($#>3),1,`$0(`$1',`$2',shift(shift(shift($@))))')')')dnl
define(`HeaderNameList', `translit(FILE_NAMES, ` ', `,')')dnl
define(`SourceNameList', `patsubst(FILE_NAMES, ` ', `.m4,').m4')dnl
define(`LoadDefines', `divert(1)
            ENUM_LIST__$1`'divert(0)dnl
divert(2)
            VIRTUAL_ENUM_LIST__$1`'divert(0)dnl
divert(3) \
            GLOBAL_VARDEF__$1`'divert(0)dnl
divert(4) \
            INITIALIZE__$1`'divert(0)dnl
divert(5) \
            STARTUP_CASE__$1`'divert(0)dnl
divert(6) \
            RUN_CASE__$1`'divert(0)dnl
')dnl
dnl
dnl Overload the cmd-generator macros.
define(`WithLicense', `')dnl
define(`OnInit', `')dnl
define(`OnCmd', `')dnl
define(`OnArg', `')dnl
define(`WithNamedStep', `')dnl
define(`WithVirtualStep', `')dnl
define(`InCommandStdDef', `')dnl
dnl
dnl Commands that we need to capture
define(`AsOptional', `define(`RCommand', $1)dnl
define(`UCommand', `translit(RCommand, `a-z-', `A-Z_')')dnl
LoadDefines(UCommand)dnl
')dnl
define(`AsRequired', `define(`RCommand', $1)dnl
define(`UCommand', `translit(RCommand, `a-z-', `A-Z_')')dnl
LoadDefines(UCommand)dnl
')dnl
define(`AsOptionalUnion', `define(`RCommand', $1)dnl
define(`UCommand', `translit(RCommand, `a-z-', `A-Z_')')dnl
LoadDefines(UCommand)dnl
')dnl
dnl
dnl Process each file.
divert(-1)ForEach(`FileName', `include(FileName)', SourceNameList)divert(0)dnl
dnl
dnl Generate file.
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

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, ERMPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef _FS_SHELL__COMMAND_LIST_
#define _FS_SHELL__COMMAND_LIST_

ForEach(`FileName', `#include "FileName"
', HeaderNameList)dnl

#define CMD_GLOBAL_VARDEF undivert(3)
#define CMD_INITIALIZE undivert(4)
#define CMD_STARTUP_CASE undivert(5)
#define CMD_RUN_CASE undivert(6)

// All the commands, by index.
enum CommandIndex {
    undivert(1)

    COMMAND_INDEX__LAST_NAMED_CMD,

    undivert(2)

    COMMAND_INDEX__LAST_VIRTUAL_CMD
};

#endif /* _FS_SHELL__COMMAND_LIST_ */
