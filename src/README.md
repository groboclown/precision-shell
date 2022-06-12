# Source Layout

## Compiler Defines

The source has the [Makefile](Makefile) defines these values, either defined as 1 or not defined:

* `DEBUG` - print debug statements.
* `USE_STREAMING_INPUT` - input parsing can come from streaming input, not just argv.
* `USE_CMD_*` - for each command, this enables it.

The [uses.h](uses.h) file, which should always be included as the first non-system include, performs further extrapolation based on the compiler defines to create the following defines.  Note that these have the word `USES_` rather than `USE_`, as "USE" indicates a direction, and "USES" indicates an implication.

* `USES_SIGNALS` - signal processing commands are used.
* `USES_FMODE` - the file mode common command is used.
* `USES_DUP` - one of the file descriptor dup commands is used.


## Commands

Each command's code is defined in its own file.  These are carefully constructed such that:

* They are defined in an enum in `command_list.h`, using the `USE_CMD_*` defines, in order to tightly pack their numbering.  This enum encodes not only the command, but also special argument processing required for it.
* They take no argument (as they should use the [globals.h](globals.h) to fetch values) and return an error value (non-zero means error).


## Globals

In order to make the code smaller, general use variables are stored in the [globals.c](globals.c) file (and referenced in [globals.h](globals.h)).

General helper functions and shortcut defines are in the [helper_funcs.h](helper_funcs.h) file.
