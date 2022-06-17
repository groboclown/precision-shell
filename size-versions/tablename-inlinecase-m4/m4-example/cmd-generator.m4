dnl Tools to build a command-let.
dnl This removes the boiler plate from creating commands,
dnl   and makes the process less error prone.
changecom(`@@')dnl
define(`InternalUndefine', `defn(`undefine')')dnl
define(`InternalRename', `define(`$2',defn(`$1'))InternalUndefine(`$1')')dnl
define(`InternalUndefine', `defn(`undefine')')dnl
define(`InternalDefine', `defn(`define')')InternalUndefine(`define')dnl
InternalRename(`define', `InternalDefine')dnl
InternalRename(`divert', `InternalDivert')dnl
InternalRename(`undivert', `InternalUndivert')dnl
dnl Helpers
dnl Note that this requires GNU m4.
InternalDefine(`Macroize', `patsubst(patsubst(`$1', `//.*$', `'), `$', ` \\')')dnl
dnl
InternalDefine(`FileStart', `ifdef(`FileStartSkip',,`
#ifndef _FS_SHELL__CMD_$1_
#define _FS_SHELL__CMD_$1_

#include "uses.h"
')')dnl
InternalDefine(`FileEnd', `ifdef(`FileEndSkip',,`

#endif /* _FS_SHELL__CMD_$1_ */
')')dnl
InternalDefine(`WithLicense', `/*
$1
*/')dnl
InternalDefine(`AppendEnumList', `InternalDivert(1) \
            ifelse($1,COMMAND_INDEX__FIND_CMD,$1 = 0,$1)`,'InternalDivert(0)')dnl
InternalDefine(`AppendVirtualEnumList', `InternalDivert(2) \
            $1`,'InternalDivert(0)')dnl
InternalDefine(`AppendGlobalVarDef', `InternalDivert(3) \
            const char $1[] = $2;InternalDivert(0)')dnl
InternalDefine(`AppendSetName', `InternalDivert(4) \
            command_list_names[EnumName()] = $1;InternalDivert(0)')dnl
InternalDefine(`OnInit', `InternalDivert(4) \
            Macroize($1)`
            /* end init */'InternalDivert(0)')dnl
InternalDefine(`OnCmd', `InternalDivert(5) \
        case EnumName(): \
            Macroize($1)
            break;InternalDivert(0)')dnl
InternalDefine(`OnArg', `InternalDivert(6) \
        case EnumName: \
            Macroize($1)
            break;InternalDivert(0)')dnl
dnl
dnl WithStep defines a new step that will be added to the exported definitions.
InternalDefine(`WithNamedStep', `// Named Step $1 `('command $2)
InternalDefine(`Arg1', `$1')dnl
InternalDefine(`ARG1', translit(Arg1, `a-z-', `A-Z_'))dnl
InternalDefine(`arg1', translit(Arg1, `A-Z-', `a-z_'))dnl
InternalDefine(`EnumName', `COMMAND_INDEX__'ARG1)dnl
AppendEnumList(EnumName)dnl
AppendSetName(`cmd_name_'arg1)dnl
AppendGlobalVarDef(`cmd_name_'arg1, $2)dnl
$3dnl
')dnl
InternalDefine(`WithVirtualStep', `// Virtual Step $1
InternalDefine(`Arg1', `$1')dnl
InternalDefine(`ARG1', translit(Arg1, `a-z-', `A-Z_'))dnl
InternalDefine(`arg1', translit(Arg1, `A-Z-', `a-z_'))dnl
InternalDefine(`EnumName', `COMMAND_INDEX__'ARG1)dnl
AppendVirtualEnumList(EnumName)dnl
$2dnl
')dnl
InternalDefine(`InCommandStdDef',`
dnl RCommand - the raw argument for the command name.
dnl UCommand - all upper case, dashes become underscores.
dnl LCommand - all lower case, dashes become underscores.
dnl SCommand - the script command name, all lower case with dashes maintained.
    InternalDefine(`UCommand', `translit(RCommand, `a-z-', `A-Z_')')dnl
    InternalDefine(`LCommand', `translit(RCommand, `A-Z-', `a-z_')')dnl
    InternalDefine(`SCommand', `translit(RCommand, `A-Z', `a-z')')dnl
')dnl
InternalDefine(`AsOptional', `InternalDefine(`RCommand', $1)dnl
InCommandStdDef()dnl
FileStart(UCommand)dnl

// RCommand is an optional command
#ifdef USE_CMD_`'UCommand()

#include "helpers.h"
$2
#define ENUM_LIST__`'UCommand() InternalUndivert(1)
#define VIRTUAL_ENUM_LIST__`'UCommand() InternalUndivert(2)
#define GLOBAL_VARDEF__`'UCommand() InternalUndivert(3)
#define INITIALIZE__`'UCommand() InternalUndivert(4)
#define STARTUP_CASE__`'UCommand() InternalUndivert(5)
#define RUN_CASE__`'UCommand() InternalUndivert(6)

#else /* USE_CMD_`'UCommand() */
#define ENUM_LIST__`'UCommand()
#define VIRTUAL_ENUM_LIST__`'UCommand()
#define GLOBAL_VARDEF__`'UCommand()
#define INITIALIZE__`'UCommand()
#define STARTUP_CASE__`'UCommand()
#define RUN_CASE__`'UCommand()
#endif /* USE_CMD_`'UCommand() */

FileEnd(UCommand)
')dnl
InternalDefine(`AsRequired', `InternalDefine(`RCommand', $1)dnl
InCommandStdDef()
FileStart(UCommand)dnl

// RCommand is a required command

#include "helpers.h"
$2
#define ENUM_LIST__`'UCommand() InternalUndivert(1)
#define VIRTUAL_ENUM_LIST__`'UCommand() InternalUndivert(2)
#define GLOBAL_VARDEF__`'UCommand() InternalUndivert(3)
#define INITIALIZE__`'UCommand() InternalUndivert(4)
#define STARTUP_CASE__`'UCommand() InternalUndivert(5)
#define RUN_CASE__`'UCommand() InternalUndivert(6)

FileEnd(UCommand)
')dnl
InternalDefine(`ForEachEnd',`ifelse(eval($#>2),1,
`pushdef(`Last$1',eval($#==3))dnl
`'pushdef(`$1',`$3')$2`'popdef(`$1')dnl
`'popdef(`Last$1')dnl
`'ifelse(eval($#>3),1,`$0(`$1',`$2',shift(shift(shift($@))))')')')dnl
InternalDefine(`WithAuxFor', `#if ForEachEnd(`CName',`defined(CName)ifelse(LastCName,0,` || ')',$1)
dnl Set up the streams for creating the final list.
InternalDivert(1)ForEachEnd(`CName',`ENUM_LIST__`'CName`'ifelse(LastCName,0,` \
            ')',$1)InternalDivert(0)
InternalDivert(2)ForEachEnd(`CName',`VIRTUAL_ENUM_LIST__`'CName`'ifelse(LastCName,0,` \
            ')',$1)InternalDivert(0)
InternalDivert(3)ForEachEnd(`CName',`GLOBAL_VARDEF__`'CName`'ifelse(LastCName,0,` \
            ')',$1)InternalDivert(0)
InternalDivert(4)ForEachEnd(`CName',`INITIALIZE__`'CName`'ifelse(LastCName,0,` \
            ')',$1)InternalDivert(0)
InternalDivert(5)ForEachEnd(`CName',`STARTUP_CASE__`'CName`'ifelse(LastCName,0,` \
            ')',$1)InternalDivert(0)
InternalDivert(6)ForEachEnd(`CName',`RUN_CASE__`'CName`'ifelse(LastCName,0,` \
            ')',$1)InternalDivert(0)
$2
#define ENUM_LIST__`'UnionUCommand() \
            InternalUndivert(1)
#define VIRTUAL_ENUM_LIST__`'UnionUCommand() \
            InternalUndivert(2)
#define GLOBAL_VARDEF__`'UnionUCommand() \
            InternalUndivert(3)
#define INITIALIZE__`'UnionUCommand() \
            InternalUndivert(4)
#define STARTUP_CASE__`'UnionUCommand() \
            InternalUndivert(5)
#define RUN_CASE__`'UnionUCommand() \
            InternalUndivert(6)

#else /* ForEachEnd(`CName',`defined(CName)ifelse(LastCName,0,` || ')',$1) */
#define ENUM_LIST__`'UnionUCommand()
#define VIRTUAL_ENUM_LIST__`'UnionUCommand()
#define GLOBAL_VARDEF__`'UnionUCommand()
#define INITIALIZE__`'UnionUCommand()
#define STARTUP_CASE__`'UnionUCommand()
#define RUN_CASE__`'UnionUCommand()
#endif /* ForEachEnd(`CName',`defined(CName)ifelse(LastCName,0,` || ')',$1) */
')dnl
InternalDefine(`AsOptionalUnion', `
  InternalDefine(`UnionUCommand', `translit($1, `a-z-', `A-Z_')')dnl
  FileStart(UnionUCommand)dnl
  InternalDefine(`FileStartSkip', Yes)dnl
  InternalDefine(`FileEndSkip', Yes)dnl
 $2
  InternalUndefine(`FileEndSkip')dnl
#endif /* _FS_SHELL__CMD_`'UnionUCommand`'_ */
')