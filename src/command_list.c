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

    return err;
}



// DietLibC and other compilers / linkers have trouble with the data section
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
    const CommandSetup *setups = malloc(sizeof(const CommandSetup) * COMMAND_INDEX__FINAL_INDEX);
    const CommandFunc *runs = malloc(sizeof(const CommandFunc) * COMMAND_INDEX__FINAL_INDEX);
    if (names == NULL || setups == NULL || runs == NULL) {
        // Half broken state - at least one of these could be non-null.
        return 1;
    }

    // COMMAND_INDEX__FIND_CMD
    names[COMMAND_INDEX__FIND_CMD] = command_common_empty_name;

    // Each command.  Order isn't too important except for cache mises.
    NAME_VS__CMD_FIND_CMD
    SETUP_S__CMD_FIND_CMD
      RUN_S__CMD_FIND_CMD

    NAME_VS__CMD_NOOP
    SETUP_S__CMD_NOOP
      RUN_S__CMD_NOOP

    // Intentionally not present
    // COMMAND_INDEX__FINAL_INDEX
    
    intern__command_list_names = names;
    intern__command_setup = setups;
    intern__command_functions = runs;
}

const char **get_command_list_names() {
    intern__list_setup();
    return intern__command_list_names;
}

// Each command's callback, for execution on the current argument.
CommandSetup *get_command_setup() {
    intern__list_setup();
    return intern__command_setup;
}

// Each command's callback, for execution on the current argument.
CommandFunc *get_command_function() {
    intern__list_setup();
    return intern__command_functions;
}



#else /* BROKEN_DATA_POINTERS */

// Here, we can maintain a list of pointers in the data section.



const char *intern__command_list_names[] = {
    // Each command's name.  In exact order.

    NAME_TC__CMD_FIND_CMD
    NAME_TC__CMD_NOOP

    // Intentionally not present
    // NAME_TC__CMD_ERR
    // COMMAND_INDEX__FINAL_INDEX
};
const CommandSetup intern__command_setup[] = {
    SETUP_C__CMD_FIND_CMD
    SETUP_C__CMD_NOOP

    // Intentionally not present
    // COMMAND_INDEX__FINAL_INDEX
};
const CommandFunc intern__command_functions[] = {
    RUN_C__CMD_FIND_CMD
    RUN_C__CMD_NOOP
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

// Each command's callback, for execution on the current argument.
CommandSetup *get_command_setup() {
    return intern__command_setup;
}

// Each command's callback, for execution on the current argument.
CommandFunc *get_command_function() {
    return intern__command_functions;
}


#endif /* BROKEN_DATA_POINTERS */
