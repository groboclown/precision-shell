dnl Tools to build a command-let.
dnl This removes the boiler plate from creating commands,
dnl   and makes the process less error prone.
changecom(`@@')dnl
dnl Helpers
dnl Note that this requires GNU m4.
define(`Macroize', `patsubst(patsubst(`$1', `//.*$', `'), `$', ` \\')')dnl
dnl
define(`FileStart', `ifdef(`FileStartSkip',,`
#ifndef _FS_SHELL__CMD_$1_
#define _FS_SHELL__CMD_$1_

#include "uses.h"
')')dnl
define(`FileEnd', `ifdef(`FileEndSkip',,`

#endif /* _FS_SHELL__CMD_$1_ */
')')dnl
define(`WithLicense', `/*
$1
*/')dnl
define(`AppendEnumList', `divert(1) \
            $1`,'divert(0)')dnl
define(`AppendSetName', `divert(2) \
            names[EnumName()] = $1;divert(0)')dnl
define(`AppendGlobalVarDef', `divert(3) \
            const char *$1[] = $2;divert(0)')dnl
define(`OnInit', `divert(4) \
            Macroize($1)`
            /* end init */'divert(0)')dnl
define(`OnCmd', `divert(5) \
        case EnumName(): \
            Macroize($1)
            break;divert(0)')dnl
define(`OnArg', `divert(6) \
        case EnumName: \
            Macroize($1)
            break;divert(0)')dnl
dnl
dnl WithStep defines a new step that will be added to the exported definitions.
define(`WithNamedStep', `// Named Step $1 `('command $2)
define(`Arg1', `$1')dnl
define(`ARG1', translit(Arg1, `a-z-', `A-Z_'))dnl
define(`arg1', translit(Arg1, `A-Z-', `a-z_'))dnl
define(`EnumName', `COMMAND_INDEX__'ARG1)dnl
AppendEnumList(EnumName)dnl
AppendSetName(`cmd_name_'arg1)dnl
AppendGlobalVarDef(`cmd_name'arg1, $2)dnl
extern const char *`cmd_name_'arg1`[];'
$3dnl
')dnl
define(`WithVirtualStep', `// Virtual Step $1
define(`Arg1', `$1')dnl
define(`ARG1', translit(Arg1, `a-z-', `A-Z_'))dnl
define(`arg1', translit(Arg1, `A-Z-', `a-z_'))dnl
define(`EnumName', `COMMAND_INDEX__'ARG1)dnl
AppendEnumList(EnumName)dnl
AppendSetName(`empty_string')dnl
$2dnl
')dnl
define(`InCommandStdDef',`
dnl RCommand - the raw argument for the command name.
dnl UCommand - all upper case, dashes become underscores.
dnl LCommand - all lower case, dashes become underscores.
dnl SCommand - the script command name, all lower case with dashes maintained.
    define(`UCommand', `translit(RCommand, `a-z-', `A-Z_')')dnl
    define(`LCommand', `translit(RCommand, `A-Z-', `a-z_')')dnl
    define(`SCommand', `translit(RCommand, `A-Z', `a-z')')dnl
')dnl
define(`AsOptional', `define(`RCommand', $1)dnl
InCommandStdDef()dnl
FileStart(UCommand)dnl

// RCommand is an optional command
#ifdef USE_CMD_`'UCommand()

#include "helpers.h"
$2
#define ENUM_LIST__`'UCommand() undivert(1)
#define SET_NAME__`'UCommand() undivert(2)
#define GLOBAL_VARDEF__`'UCommand() undivert(3)
#define INITIALIZE__`'UCommand() undivert(4)
#define STARTUP_CASE__`'UCommand() undivert(5)
#define RUN_CASE__`'UCommand() undivert(6)

#else /* USE_CMD_`'UCommand() */
#define ENUM_LIST__`'UCommand()
#define SET_NAME__`'UCommand()
#define GLOBAL_VARDEF__`'UCommand()
#define INITIALIZE__`'UCommand()
#define STARTUP_CASE__`'UCommand()
#define RUN_CASE__`'UCommand()
#endif /* USE_CMD_`'UCommand() */

FileEnd(UCommand)
')dnl
define(`AsRequired', `define(`RCommand', $1)dnl
InCommandStdDef()
FileStart(UCommand)dnl

// RCommand is a required command

#include "helpers.h"
$2
#define ENUM_LIST__`'UCommand() undivert(1)
#define SET_NAME__`'UCommand() undivert(2)
#define GLOBAL_VARDEF__`'UCommand() undivert(3)
#define INITIALIZE__`'UCommand() undivert(4)
#define STARTUP_CASE__`'UCommand() undivert(5)
#define RUN_CASE__`'UCommand() undivert(6)

FileEnd(UCommand)
')dnl
define(`ForEachEnd',`ifelse(eval($#>2),1,
`pushdef(`Last$1',eval($#==3))dnl
`'pushdef(`$1',`$3')$2`'popdef(`$1')dnl
`'popdef(`Last$1')dnl
`'ifelse(eval($#>3),1,`$0(`$1',`$2',shift(shift(shift($@))))')')')dnl
define(`WithAuxFor', `#if ForEachEnd(`CName',`defined(CName)ifelse(LastCName,0,` || ')',$1)
dnl Set up the streams for creating the final list.
divert(1)ForEachEnd(`CName',`ENUM_LIST__`'CName`'ifelse(LastCName,0,` \
            ')',$1)divert(0)
divert(2)ForEachEnd(`CName',`SET_NAME__`'CName`'ifelse(LastCName,0,` \
            ')',$1)divert(0)
divert(3)ForEachEnd(`CName',`GLOBAL_VARDEF__`'CName`'ifelse(LastCName,0,` \
            ')',$1)divert(0)
divert(4)ForEachEnd(`CName',`INITIALIZE__`'CName`'ifelse(LastCName,0,` \
            ')',$1)divert(0)
divert(5)ForEachEnd(`CName',`STARTUP_CASE__`'CName`'ifelse(LastCName,0,` \
            ')',$1)divert(0)
divert(6)ForEachEnd(`CName',`RUN_CASE__`'CName`'ifelse(LastCName,0,` \
            ')',$1)divert(0)
$2
#define ENUM_LIST__`'UnionUCommand() \
            undivert(1)
#define SET_NAME__`'UnionUCommand() \
            undivert(2)
#define GLOBAL_VARDEF__`'UnionUCommand() \
            undivert(3)
#define INITIALIZE__`'UnionUCommand() \
            undivert(4)
#define STARTUP_CASE__`'UnionUCommand() \
            undivert(5)
#define RUN_CASE__`'UnionUCommand() \
            undivert(6)

#else /* ForEachEnd(`CName',`defined(CName)ifelse(LastCName,0,` || ')',$1) */
#define ENUM_LIST__`'UnionUCommand()
#define SET_NAME__`'UnionUCommand()
#define GLOBAL_VARDEF__`'UnionUCommand()
#define INITIALIZE__`'UnionUCommand()
#define STARTUP_CASE__`'UnionUCommand()
#define RUN_CASE__`'UnionUCommand()
#endif /* ForEachEnd(`CName',`defined(CName)ifelse(LastCName,0,` || ')',$1) */
')dnl
define(`AsOptionalUnion', `
  define(`UnionUCommand', `translit($1, `a-z-', `A-Z_')')dnl
  FileStart(UnionUCommand)dnl
  define(`FileStartSkip', Yes)dnl
  define(`FileEndSkip', Yes)dnl
 $2
  undefine(`FileEndSkip')dnl
FileEnd(UnionUCommand)dnl
')