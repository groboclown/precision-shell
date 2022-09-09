# About

Tests for the recipes, to ensure that they continue to work as the tool changes.

This expects the parent script [`_build_all.sh`](../_build_all.sh) to have run successfully.  This makes the image named `local/presch-(name):build` available.

Environment variables accessible to the scripts:

* `TEST_TMP_DIR` - scratch directory usable by the test.
* `CONTAINER_RUNNER` - program that runs the container handler (docker or podman)

The script tool automatically cleans up container bits left behind.
