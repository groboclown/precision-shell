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


## Globals and Command Execution Frame

To optimize the size of the executable, the commands are a simple #include in-line and reuse variables declared in the command_runner.c function.  Because of this, there are severe limitations to what a command can do.

* commands have an index which is carefully constructed in [command_list.h](command_list.h) in the enum.
* command files are named ".inc.h" even though they really aren't header files.
* each command name declared in the command list must have declared 2 `#define` statements:
    * `STARTUP__CMD_(CMDNAME)` (which can be empty).  Runs when the command name is encountered.
    * `CASE__CMD_(CMDNAME)`.  Runs when the command index encounters an argument.  This must be the `case (CMD_CMDNAME):` code and the trailing `break`, if necessary.  Commands which don't have a case part to run can omit this.
    * If the command can be excluded due to `USE_CMD_*` semantics, then a version must be made for with USE_CMD and without.

## Commands

Each command's code is defined in its own file.  These are carefully constructed such that:

* They are defined in an enum in `command_list.h`, using the `USE_CMD_*` defines, in order to tightly pack their numbering.  This enum encodes not only the command, but also special argument processing required for it.
* They take no argument (as they should use the [globals.h](globals.h) to fetch values) and return an error value (non-zero means error).
