# Precision Shell Recipes

This directory includes different recipes and examples for using precision shell to solve different problems.

These are constructed as Dockerfile files, as that's considered the target for precision shell.  Though they are located in this directory, they are intended to be run from the parent directory.  The `_build_all.sh` script creates all of them.


## env-config-file.Dockerfile

Shows how precision shell can be used to enhance a ["distroless"](https://github.com/GoogleContainerTools/distroless/) container.

The base server allows for a configuration file to set runtime parameters on how to set up the server.  Precision shell constructs this file from a template + environment variables, launches the program, and adds signal handlers for stopping execution.

Presh adds 30,136 bytes by using the musl library.  This could be shrunk down smaller by eliminating the debug commands.

To run the Presh shell in docker image to explore what's in the image, you can run something like:

```bash
docker run --rm -it --entrypoint "/bin/sh" local/presch-env-config-file \
    "cat-fd 1 /etc/group"
```

## non-root.Dockerfile

Runs the service as a non-root user.  Normally, this would be done just by using the Docker instruction `USER nonroot`.  However, there exist circumstances where the running container requires root-privilege adjustments before the program can start running.  For example, docker containers with attached volumes can have permissions that make the files unusable to all but root.

In this example, the `/opt/logs` path is marked as a mountable volume.  This makes running it with the mounted drive, even as ":rw", to have root ownership and no write access for non-root users:

```bash
docker run --rm -it --entrypoint "/bin/sh" local/presch-non-root \
    "file-stat /opt/logs"
d-rwxr-xr-x 2 0 0 0 0 4096 /opt/logs
```

Presh adds 34,240 bytes to the docker image by using the musl library.  This could be shrunk down smaller by eliminating the debug commands.

## delay-until-ready.Dockerfile

Waits for a dependent service to start running before starting the service.  If the dependent service doesn't start within 5 minutes (300 seconds), then the service fails to run.
