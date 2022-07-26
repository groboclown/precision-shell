# About

Set of Bash shell scripts used to test the executable.

## Run Tests

To run the scripts, you will need these environment variables set:

* `TEST_TMP_DIR` - directory to place files that the test manipulates.
* `PRESH` - location of the compiled `presh` program.
* `UID1`, `UID2` - two user ids, other than the current user, to use with testing changing file ownership.
* `GID1`, `GID2` - two group ids, other than the current user's primary group, to use with testing changing file ownership.


## Write Tests

Tests are scripts that have an exit code to indicate the failure (!= 0 means failure).

The test script should include a line with the text:

```bash
# desc: This test purpose
```

Tests should use these environment variables:

* `TEST_DIR` - the directory to use for file manipulation.  The test will be run from this directory.
* `TEST_NAME` - the name of the test.
* `FS` - absolute path to the `presh` tool.
* `UID0`, `GID0` - user and group ID of the currently running user.
* `UID1`, `UID2`, `GID1`, `GID2` - additional user and group IDS for testing.
