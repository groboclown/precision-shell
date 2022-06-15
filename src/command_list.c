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

#include <stdlib.h>
#include "uses.h"
#include "command_list.h"
#include "command_common.h"
#include "globals.h"
#include "helpers.h"
#include "output.h"


int intern__list_setup();


// Initialize the commands.  Returns an error.
int initialize_commands() {
    int err = intern__list_setup();

    // each command's initializer.
    CALL_INIT__CMD_FIND_CMD
    CALL_INIT__CMD_NOOP
    CALL_INIT__CMD_VERSION
    CALL_INIT__CMD_CHMOD
    CALL_INIT__CMD_CHOWN
    CALL_INIT__CMD_DUP
    CALL_INIT__CMD_ECHO
    CALL_INIT__CMD_EXEC
    CALL_INIT__CMD_FMODE
    CALL_INIT__CMD_LN_H
    CALL_INIT__CMD_LN_S
    CALL_INIT__CMD_MV
    CALL_INIT__CMD_ERR

    return err;
}


// DietLibC and some other compilers / linkers have trouble with the data section
//   containing pointers.  Thus, we unfortunately have this strange two-part
//   duplication of code.

#ifdef BROKEN_DATA_POINTERS

const char **intern__command_list_names = NULL;
const CommandSetup *intern__command_setup = NULL;
const CommandFunc *intern__command_functions = NULL;


int intern__list_setup() {
    if (intern__command_list_names != NULL) {
        return 0;
    }
    const char **names = malloc(sizeof(const char *) * COMMAND_INDEX__FINAL_INDEX);
    if (names == NULL) {
        return 1;
    }
    CommandSetup *setups = malloc(sizeof(const CommandSetup) * COMMAND_INDEX__FINAL_INDEX);
    if (setups == NULL) {
        free(names);
        return 1;
    }
    CommandFunc *runs = malloc(sizeof(const CommandFunc) * COMMAND_INDEX__FINAL_INDEX);
    if (runs == NULL) {
        free(names);
        free(setups);
        return 1;
    }

    // Each command.  Order isn't too important except for cache mises.
    NAME_VS__CMD_FIND_CMD
    SETUP_S__CMD_FIND_CMD
      RUN_S__CMD_FIND_CMD

    NAME_VS__CMD_NOOP
    SETUP_S__CMD_NOOP
      RUN_S__CMD_NOOP

    NAME_VS__CMD_VERSION
    SETUP_S__CMD_VERSION
      RUN_S__CMD_VERSION

    NAME_VS__CMD_CHMOD
    SETUP_S__CMD_CHMOD
      RUN_S__CMD_CHMOD

    NAME_VS__CMD_CHOWN
    SETUP_S__CMD_CHOWN
      RUN_S__CMD_CHOWN

    NAME_VS__CMD_DUP
    SETUP_S__CMD_DUP
      RUN_S__CMD_DUP

    NAME_VS__CMD_ECHO
    SETUP_S__CMD_ECHO
      RUN_S__CMD_ECHO

    NAME_VS__CMD_EXEC
    SETUP_S__CMD_EXEC
      RUN_S__CMD_EXEC

    NAME_VS__CMD_FMODE
    SETUP_S__CMD_FMODE
      RUN_S__CMD_FMODE

    NAME_VS__CMD_LN_H
    SETUP_S__CMD_LN_H
      RUN_S__CMD_LN_H

    NAME_VS__CMD_LN_S
    SETUP_S__CMD_LN_S
      RUN_S__CMD_LN_S

    NAME_VS__CMD_MV
    SETUP_S__CMD_MV
      RUN_S__CMD_MV

    NAME_VS__CMD_ERR
    SETUP_S__CMD_ERR
      RUN_S__CMD_ERR

    // Intentionally not present
    // COMMAND_INDEX__FINAL_INDEX
    
    intern__command_list_names = names;
    intern__command_setup = setups;
    intern__command_functions = runs;
    return 0;
}

const char **get_command_list_names() {
    intern__list_setup();
    return intern__command_list_names;
}


// Each command's callback, for execution on the current argument.
const CommandSetup *get_command_setup() {
    intern__list_setup();
    return intern__command_setup;
}

// Each command's callback, for execution on the current argument.
const CommandFunc *get_command_function() {
    intern__list_setup();
    return intern__command_functions;
}



#else /* BROKEN_DATA_POINTERS */

// Here, we can maintain a list of pointers in the data section.



const char *intern__command_list_names[] = {
    // Each command's name.  In exact order.

    NAME_TC__CMD_FIND_CMD
    NAME_TC__CMD_NOOP
    NAME_TC__CMD_VERSION
    NAME_TC__CMD_CHMOD
    NAME_TC__CMD_CHOWN
    NAME_TC__CMD_DUP
    NAME_TC__CMD_ECHO
    NAME_TC__CMD_EXEC
    NAME_TC__CMD_FMODE
    NAME_TC__CMD_LN_H
    NAME_TC__CMD_LN_S
    NAME_TC__CMD_MV
    NAME_TC__CMD_ERR

    // Intentionally not present
    // COMMAND_INDEX__FINAL_INDEX
};

const CommandSetup intern__command_setup[] = {
    SETUP_C__CMD_FIND_CMD
    SETUP_C__CMD_NOOP
    SETUP_C__CMD_VERSION
    SETUP_C__CMD_CHMOD
    SETUP_C__CMD_CHOWN
    SETUP_C__CMD_DUP
    SETUP_C__CMD_ECHO
    SETUP_C__CMD_EXEC
    SETUP_C__CMD_FMODE
    SETUP_C__CMD_LN_H
    SETUP_C__CMD_LN_S
    SETUP_C__CMD_MV
    SETUP_C__CMD_ERR

    // Intentionally not present
    // COMMAND_INDEX__FINAL_INDEX
};

const CommandFunc intern__command_functions[] = {
    RUN_C__CMD_FIND_CMD
    RUN_C__CMD_NOOP
    RUN_C__CMD_VERSION
    RUN_C__CMD_CHMOD
    RUN_C__CMD_CHOWN
    RUN_C__CMD_DUP
    RUN_C__CMD_ECHO
    RUN_C__CMD_EXEC
    RUN_C__CMD_FMODE
    RUN_C__CMD_LN_H
    RUN_C__CMD_LN_S
    RUN_C__CMD_MV
    RUN_C__CMD_ERR

    // Intentionally not present
    // COMMAND_INDEX__FINAL_INDEX
};


int intern__list_setup() {
    return 0;
}

const char **get_command_list_names() {
    return intern__command_list_names;
}

const CommandSetup *get_command_setup() {
    return intern__command_setup;
}

const CommandFunc *get_command_function() {
    return intern__command_functions;
}


#endif /* BROKEN_DATA_POINTERS */
