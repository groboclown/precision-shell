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
* `USES_SHARED_INT` - one or more command stores an argument as a parsed integer for later processing.
* `USES_SHARED_STR` - one or more command stores an argument for later processing.


## Globals and Command Execution Frame

To optimize the size of the executable, the commands are a simple #include in-line and reuse variables declared in the command_runner.c function.  Because of this, there are severe limitations to what a command can do and how the command source code is written.

## Generating Commands

In order to reduce the chances of typos or misuse of the command structure, the command files are constructed through the [generate-command-template.py](../generate-command-template.py) command file.  This processes the `cmd_*.h.in` file to create the real `cmd_*.h` file in the `gen-cmd` directory.

The files in the `gen-cmd` directory are checked into the source code repository, so that outside tooling doesn't need to know about the special generator format, and so that other people examining the source code can see the full picture straight from source control without needing to download and compile.

## Creating Custom Commands

In order to add a new command, you need to perform three things:

* Add a new `USE_CMD_*` flag into the [`Makefile.command-flags`](../Makefile.command-flags) file.  It should be put into the full command list.
* Create the command code in the `src` directory, named `cmd_*.h.in`.  You can copy the `cmd_example.h.in` to get started.  Format for this file is described later.
* Add the new file into the `Makefile`, under the `COMMANDS` section.  Make sure that this is named `cmd_*.h` (the final name), and not the original source name.  The make file will handle the conversion.

Then your new command is ready to use!

### Command File Format

The command file is in a custom syntax that allows for wrapping your command code in the correct format.

The general format is:

* Code between syntax markup is added into the header file in the appropriate places.  In some cases, depending on the context, the lines will be `\` terminated to work within a multi-line `#define` statement.
* Wrapper sections are in the form `Name(key="value", list=[value1, value2],`.  They must appear on a line by themselves, and each argument is a key-value pair (either a single string enclosed in double-quotes, or a list of comma-separated strings).
* The wrapper section terminates by a close parenthesis (`)`) on a line by itself (ignoring whitespace).

Rather than detailing all the nuances of each wrapper, let's go through examples.

#### Simple Optional Command.

Let's look at [`cmd_rm.h.in`](cmd_rm.h.in), as it's a simple command.

```c
AsOptional(command="rm",

#include <stdio.h>

    WithNamedStep(enum="rm", name="rm",
        OnArg(
            LOG(":: rm ");
            LOGLN(global_arg);
            global_err = unlink(global_arg);
        )
    )

)
```

The outer wrapper section is `AsOptional` with the argument `command`.  This defines the command file as an optional command, triggered by the compile flag `USE_CMD_RM` (rm comes from the command value).

The `#include`, because it's inside the `AsOptional`, is only included if the `USE_CMD_RM` flag is enabled.  You can have code outside the `AsOptional` section, but that should be limited to comments, because it will be outside the standard C `#ifndef` / `#endif` barrier.

Commands are split into steps, which can be run when the command name is parsed, or when an argument for the command is parsed.  In this case, the file defines a "named" step, meaning that the command name parser can cause this command step to start running.  When the command name parser matches the `name` argument, it sets the `global_cmd` variable to the enum value (here, `COMMAND_INDEX__RM`).

The named step runs on every argument, defined by the `OnArg` wrapper.  The ok/failure *for this one argument execution* is set in the `global_err` variable.  Note that encountering an error here does not stop the command from processing more arguments.

#### Required Command Without Arguments

Now look at [`cmd_version.h.in`](cmd_version.h.in).  It only runs behavior when the command name is parsed, but does not allow any arguments.

```c
AsRequired(command="version",

#include "output.h"
#include "globals.h"
#include "helpers.h"

    WithNamedStep(enum="VERSION", name="version",
        OnCmd(
            stdoutP("presh\n");
            // ... ommitted for size
            // should be no arguments, so immediately switch to error mode.
            global_cmd = COMMAND_INDEX__ERR;
        )
    )
)
```

This is a required command (`AsRequired`), which means that there's no flag for triggering it on or off.  The `command` argument here is mainly for internal bookkeeping.

The `#include` statements here are inside the `AsRequired` section, and so will be placed inside the C header `#ifndef` / `#endif` barrier.

The named step is setup similarly as with the [`rm`](#simple-optional-command).  In this case, rather than running on each argument, it runs when the command name is parsed, referenced by the section `OnCmd`.  At the end of running, it changes the command index with `global_cmd = COMMAND_INDEX__ERR`; which means that the next argument encountered will now run the error handling block, not the named step block.  If this wasn't done, then the `OnArg` wrapper code will run; because that section isn't present, that would end up being a no-op.

#### Multiple Commands with Shared Behavior

[`cmd_mknod_mkdev.h.in`](cmd_mknod_mkdev.h.in) exhibits complex behavior through the use of shared code blocks.

It wraps multiple commands through the outer section `AsOptionalUnion`.  This allows the file to contain multiple `AsRequired` and `AsOptional` sections, but must also include a final section `WithAuxFor` that unifies the other commands into a single name space.

The `mknode` command's behavior works like this:

1. When the command name is parsed, the `OnCmd` runs.  That sets the next command to be the `COMMAND_INDEX__MKNOD_DEV__TYPE` command, defined in the `WithAuxFor` section.
2. The `WithAuxFor` defines a virtual command `MKNOD_DEV__TYPE`, meaning it has no name and can't be called through the command parser but still has an index for running its code.  When the argument is parsed and the command index is `COMMAND_INDEX__MKNOD_DEV__TYPE`, then this virtual step's `OnArg` code is run.  If there is no error with its execution, it sets the next command to `COMMAND_INDEX__MKNOD_DEV__RUN`.
3. `COMMAND_INDEX__MKNOD_DEV__RUN` is another virtual command that runs when the next argument is processed.  This performs the final behavior.  Because the `global_cmd` value isn't changed, it means that if another argument is processed, this code will run again.

#### Other Interesting Abilities

* [`cmd_fmode.h.in`](cmd_fmode.h.in) is a real command, but it only exists if other flags are set.  Because of this, its wrapper includes the explicit `ifdef` name:
    ```c
    AsOptional(command="fmode", ifdef="USES_FMODE",
    ```
* [`cmd_shared_virtual.h.in`](cmd_shared_virtual.h.in) provides extra "case" statements in the argument processing for common behavior shared with other commands.  It could be split into one file per shared behavior, but in order to make the files saner, they're lumped into this one file.  Of important note is the final `WithAuxFor`, which must exist in order to unify the previous optional commands together.  You can see this being used in commands like [`cmd_mv.h.in`](cmd_mv.h.in).
* [`cmd_find_cmd.h.in`](cmd_find_cmd.h.in) is a very special command that shouldn't be messed with.  It requires to be at the start of the list and uses special syntax that's reserved for just this file.  But it's interesting because it shows using a file-wide `OnInit` to define the global variables shared almost everywhere.


### Troubleshooting

Common mistakes that cause either failures to generate code or incorrect C code:

* Is the section name in the right format?  It must be on a single line by itself, in the form:
    ```
    WithName(key="value", list=[value1, value 2, value3],
    ```
    and terminate with a `)` by itself on its own line.  Failing to do either of these will make the parser skip over the section.
* Is the code contained in the right wrapper?  Did you put an `OnInit` outside a step, or use `OnCmd` instead of `OnArg`?
