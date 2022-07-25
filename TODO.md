# Planning Board

Here's what's planned for the future of the tool.  Because of the backwards compatibility issues, this will be a major version bump.  With that, the tool will be renamed to "Precision Shell", because the Hadoop system has a "file system shell" that makes searching for `fs-shell` hard.


## Environment Variable Parsing

Issue #11

The environment variables will be interpreted within the parsing of the commands, assuming the env handling flag is turned on.  Environment variables must be in the form `${VALUE}`; the standard shell supported syntax of `$VALUE` will not be supported.


## Change Argument Quoting

Issue #12

The argument quoting will change to allow for better embedded arguments.  The current candidate for quoting are the characters `[` (start) and `]` (end).  This becomes more necessary when the sub-command parsing is implemented.


## Sub-Command Support

Issue #13

The special command `(` will cause the shell to enter a sub-command, allowing for propigation of error codes to the outer command.  The special argument `)` will mark the end of a command, only if it is not quoted.  Due to this, argument parsing will need to include marking whether an argument includes quoting or not.


## Command Line Arguments

Historically, arguments were used as-is, assuming the calling program already parsed them.  Starting with v3, the parsing algorithm significantly changed to support the much more common use case of "-c" argument parsing.  This raises the question if the old command line parsing should be maintained?  No other major shell supports this execution mode.
