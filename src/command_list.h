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
#ifndef _FS_SHELL_COMMAND_LIST_
#define _FS_SHELL_COMMAND_LIST_

#include "uses.h"
#include "command_def.h"


// Include each command.  It already has ifdefs around it internally.
#include "cmd_find_cmd.h"
#include "cmd_noop.h"
#include "cmd_version.h"
#include "cmd_err.h"


// WARNING this enum must be identically matched with the lists
//   in command_list.c ; changes to the ordering or anything else
//   here must be identically reflected in the lists.
enum CommandIndex {
    // Each of these values is an index into the command list.

    // Find command is always the first entry, and, because
    // its string match is "" (empty), it means it always prevents the
    // virtual commands from matching.
    ENUM_LIST__CMD_FIND_CMD

    ENUM_LIST__CMD_NOOP
    ENUM_LIST__CMD_VERSION

    // The error command is always penultimate; it's a signal for the number of entries in the index.
    //   It also prevents odd errors for trailing commas.
    ENUM_LIST__CMD_ERR

    // Final index is essentially the count
    COMMAND_INDEX__FINAL_INDEX
};

// Initialize the commands.  Returns an error.
int initialize_commands();

// Each command's name, for input parsing.  If a command entry is not entrant
//   from the CLI (e.g. it only runs after initial processing from earlier commands),
//   then it has a zero-length entry.
const char **get_command_list_names();

// Each command's callback, for execution on the current argument.
const CommandSetup *get_command_setup();

// Each command's callback, for execution on the current argument.
const CommandFunc *get_command_function();


#endif /* _FS_SHELL_COMMAND_LIST_ */
