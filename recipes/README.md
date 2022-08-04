# Precision Shell Recipes

This directory includes different recipes and examples for using precision shell to solve different problems.

These are constructed as Dockerfile files, as that's considered the target for precision shell.  Though they are located in this directory, they are intended to be run from the parent directory.  The `_build_all.sh` script creates all of them.


## env-config-file.Dockerfile

Shows how precision shell can be used to enhance a ["distroless"](https://github.com/GoogleContainerTools/distroless/) container.

The base server allows for a configuration file to set runtime parameters on how to set up the server.  Precision shell constructs this file from a template + environment variables, launches the program, and adds signal handlers for stopping execution.

Presh adds 30,136 bytes by using the musl library.  This could be shrunk down smaller by eliminating the debug commands.
