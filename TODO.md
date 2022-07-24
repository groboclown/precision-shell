# Planning Board

Here's what's planned for the future of the tool.  Because of the backwards compatibility issues, this will be a major version bump.  With that, the tool will be renamed to "Precision Shell", because the Hadoop system has a "file system shell" that makes searching for `fs-shell` hard.


## Join Code for Input Parsing

The code bases for streaming input support is different than the argv parsing, and leads to subtle differences in how arguments are handled.  This change will unify those code bases and remove those differences.


## Error Parsing Error Codes

Currently, parsing arguments does not indicate errors, such as file reading errors.  To correct this, argument parsing will return a pointer to a structure that contains the argument and a state value.


## Environment Variable Parsing

The environment variables will be interpreted within the parsing of the commands, assuming the env handling flag is turned on.  Environment variables must be in the form `${VALUE}`; the standard shell supported syntax of `$VALUE` will not be supported.


## Change Quoting

The argument quoting will change to allow for better embedded arguments.  The current candidate for quoting are the characters `[` (start) and `]` (end).  This becomes more necessary when the sub-command parsing is implemented.


## Sub-Commands

The special command `(` will cause the shell to enter a sub-command, allowing for propigation of error codes to the outer command.  The special argument `)` will mark the end of a command, only if it is not quoted.  Due to this, argument parsing will need to include marking whether an argument includes quoting or not.
