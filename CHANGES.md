## v4.6.0

* [c12690c](c12690c5282fdc1aa5ab93bf7b361b742f83fb74)
    * Added an additional build target to create a compressed version of the `presh` executable.  In some circumstances, it creates a smaller executable, at the expense of a slightly longer startup time.
* [3b03d11](3b03d111d2f50fd0bf24c0311e98134d6b0aa88b)
    * As glibc (and potentially others) cannot call `getaddrinfo` from a statically linked executable, added a make flag (`NO_GETADDRINFO=1`) that prevents calls that depend upon it from running.
* [0f21e00](0f21e0063b2a11d895d790a505a7832dfb111d7d)
    * Updated Dockerfile parent images and their dependent versions to keep up to date with the state-of-the-art.
    * Updated the dietlibc version, as it had a new release in 2024.
    * With this update, the clang flags need to change.
* [d8b8ed7](d8b8ed796122604702250e40d72b01360ee5345f)
    * Updated `cmd-sizes.sh` to include the compressed sizes.
    * Updated the command flags file to include the use of the `NO_GETADDRINFO` flag.
    * Updates to the `README.md` file.
        * Include notes about compressed capability.
        * Fixed the wrong use flag for the `exec` command.
        * Included a note about the `export-host-lookup` command not working with statically compiled glibc.
        * After some discovery on GPL and static compilation, added a note about it.
* [0915c1f](0915c1f5c43a31bec4c70416a6a481acf70fc97f)
    * Version bump to 4.6.0.
* [4cf0f92](4cf0f926102c383ed068c54aa4ae04138664517b)
    * Updated recipes to also use recent dependency versions.
    * Updated recipes text to reflect new `presh` sizes.
    * Updated `README.md` to reflect new compiled sizes.
* [9a1465a](9a1465a3e0f57c5bf96a59f41aaab08994bea6dd)
    * Added some notes on future experiments on compressed executables.  It's getting into already explored territory.
* [a55becb](a55becbe4dfd97d810dd8ee1554af5d5dacb8f99)
    * Added in an initial cut at the new compressed executable approach (which doesn't run right), and has minimal improvement to the file size.  However, there exist some situations where it looks like it could have a smaller size, so may indicate a place for future investigation to fix.
    * Taught the docker build command (default operation of `internal-docker-make.sh`) to create the smaller version of the `presh` executable as `out/presh-smallest`.  This allows later stages in a `Dockerfile` to use the smallest version, if it cares about that.
    * Updated the `recipes` to use the smallest file size, as they care about that.
    * Updated the file sizes in the `recipes` to reflect the new file sizes.
    * Added a few improvements to the root `Dockerfile` to indicate its differences from the `sample.Dockerfile`, and to fix its use of `CMD`.
    * Updated the license copyright, and added an SPDX to it, and added a note about static compilation affects on the binary distribution.


## v4.5.1

* [1f0755f](1f0755f209db2a326c8876d6f35310450955fdcf)
    * General readability improvements with the top-level README file.
    * Added a typo-fix for the `USE_ENVIRONMENT_INPUT`.  Both the old one (`USE_ENVIROMENT_INPUT`) and the fixed name work.
    * Added a 64-bit / 32-bit CPU fix for outputting information in the test argument runner.
    * Moved LLVM debug mode compiler flags to just use `-gfull` rather than trying to be tricky.
* [f767933](f767933547f654e5b0990e38c6f5a8ebd3e1072c)
    * Move development guidelines into the `CONTRIBUTING.md` file.
    * Add tests for the new `is-eq` function.
    * Bump the version numbers to v4.5.1.


## v4.5.0

* [5d81edd](5d81eddc766e848bd29ebd2540abfe229fe244a0)
    * Added `is-eq` command.
    * Fixed bad links in the README.
    * Reorganized tests into categories (i.e. directories).
* [400365f](400365f32aa1493f14e4087e0a983a1288bd860a)
    * Added storing trapped signal numbers into an environment with `signal`, if environment usage is enabled.
    * Added storing the previous command's exit code, if environment usage is enabled and a command that uses numbers is on.
    * Fixed `noop` debug statements to correctly print the comment.
    * Fixed `cat-fd` to exit with an error if the write fails.
    * Fixed `dup-r` debug to correctly report the command name.
* [1ee5968](1ee5968a8e0da8842b19e6b544f564a9da5aa683)
    * Fixed the `cmd_mkdir` to build correctly with LLVM.
    * Fixed clang+llvm issues with debug compile flags.  It used to contain conflicting argument flags which would cause a crash on some versions of llvm.
    * Updated the `tests/_run.sh` to recognize IPv6 capabilities.  This works around environments where IPv6 is not available.
    * Updated the tests that run a network server.  These now run a helper script that find a pseudorandom port number, and retry several times if the test server fails.  This also includes more logging to help debug server errors.
    * Extracted the HTTP status line buffer size into a definition, instead of a magic number.
    * Updated example Alpine Linux docker version to 3.19.
    * Updated Ubuntu Linux docker to install `iproute2` to allow for network tests.
* [9e21cdd](9e21cddd65a85a19f43ace0767ab6d01cab3d3dd)
    * Updated recipies and other build tools to use more modern tool versions.


## v4.4.1

* [3ef7cf6](3ef7cf6ad76fa289490d71c8c7bd6e64e4cd5c5b)
    * Added `ls-l` documentation.
    * Removed unused `cmd_ls.h.in` file.  It was no longer used after migration to include the `ls-l` command.
* [cf373ca](cf373cabe1785d9a2c728ac8fb0113f1a433eeeb)
    * Added support for clang+llvm compile chain.
    * Clarified documentation around version numbering, for how documentation and platform support changes trigger patch version increases, not minor version increases.


## v4.4.0

* [cd15bd8](cd15bd8ab8fef0651616aacfe7e93fddbf6a096f)
    * Added network commands.
        * Added new `test-connect` command that checks for whether a network port is accepting socket connections.
        * Added new `expect-http-get-response` command for checking HTTP GET response status codes against a known value.
        * Added new command `export-host-lookup` to look up an IP address and export it to an environment variable.  This is highly experimental, as it will use whatever address is discovered first, including IPv6, which may not be desired.
        * Added new command `not` to act similarly to the `subcmd` command, but it makes non-zero exit codes act like zero exit codes, and vice versa.
        * Added new file size split; all non-network commands and network commands.
        * Added a recipe (delay-until-ready.Dockerfile) to show off how to combine loops, sleep, timers, and http checks to delay starting a command until dependencies are ready.
    * Added new `elapsed-time-under` command to check if too much time has passed.
    * Added new build mode, `interactive`, to allow for creating an image suitable for manual exploration without running a build command.
    * Fixed tests for `ls-l` to support different platform sort implementations.
    * Made newline replacement behavior configurable.  Nothing takes advantage of this yet, but this is in preparation of future updates.
    * Cleaned up recipe docker files.
    * Switched Git Actions for the build from podman to docker.  This is due to issues with Git Actions' podman networking with IPv6.


## v4.3.0

* [46389d4](46389d4603708726e3392c65cc719abae06faddf)
    * Added `start-timer`, `elapsed-time`, and `export-elapsed-time` commands.
* [8b10ee5](8b10ee56efae796c1d94a8b71c741974574c5dbc)
    * Cleaned up `gen-cmd` directory so that it matches the current command list.
    * Fixed the make reference so that the right "ls" header is included.  This allows the `ls-l` command to now be buildable.
    * Added tests for `ls-l`.


## v4.2.0

* [3718300](37183007f552ad42b07fc21bbdd155cfac2726d2)
    * Bug fix for spawn.  It now exits the forked process if the execution fails.
* [b915b65](b915b65e077cd3a265a9cc54cee879429f13f6cf)
    * Enhanced file-stat to report output more like "ls -l".
* [93f6ecb](93f6ecb72ec93d34f16fcc300d71562cec040e9e)
    * Added `su-spawn` and `su-exec` commands.


## v4.1.0

* [519e18f](519e18fab1c9413c83283f4eb4456a89d2acb591)
    * Fixed the internal docker build tool to have correct naming for the environment flag.
* [a962acf](a962acf8af05a1ce14b4fc49f7cc86678300b093)
    * Added `ls` command.
    * Added `ls-t` command.
    * Added `file-stat` command.  This is considered an *experimental* command at the moment.  It may change how it works in the future.
    * Added recipes to show different scenarios for using Precision Shell in real-world situations.


## v4.0.0

* [d72dc56](d72dc569173a90cedab6bcb4fae9b534c2798e6c)
    * Switched argument and input parsing to a unified front.  This will allow for commands such as `envsubst` to be more easily implemented.
    * Switched quoting to now be re-entrant.  The quote characters are now starting with '[' and ending with ']'.  This is a non-standard formatting, but allows interesting things like better support of the `exec` command.
* [1acc7cd](1acc7cd17990abc6fdf4e721dd3098b5bcf95ec7)
    * Added `spawn` command.  Including this command in the executable forces the environment variable usage to be enabled, but doesn't force environment variable parsing.
    * Added `wait-pid` command.  Including this command in the executable forces the environment variable usage to be enabled, but doesn't force environment variable parsing.
    * Added `kill-pid` command.
    * Added `export` command.  Including this command in the executable forces the environment variable usage to be enabled, but doesn't force environment variable parsing.
    * Altered `exec` to take a single argument, and if that command doesn't exist or isn't executable, tries to run the next argument, and so on.
    * Added `subcmd` now that the quote parse rewriting has made it trivial.
    * Added `if-else` command now that `subcmd` shows how easy it is to write this.
    * Added `cd` command.
    * Added `pwd` command.
    * Added `cat` and `env-cat` commands.
    * Added `write-fd` command.
    * Added `while-error`, `while-no-error`, and `for-each` looping commands.
    * Modified the noop command to also be called `#` to support comment like structures, and `#!` to allow for running presh scripts natively.
    * Modified the `dup-w` command to back up the standard file descriptors so they can be restored.
    * Fixed a bug in the `dup-*` commands where they did not correctly close file descriptors for opened files if the assigned-to file descriptor gets re-dup'd.
* [1560595](1560595aa133553e5abb3b0553d7452dc8a9f28b)
    * Fixed a bug in the `;` processing that made it incorrectly reported as an argument to the previous command when the previous command generated an error.


## v3.0.0

* [ba250c4](ba250c452722f32677621eba5157b8c1173d0f34)
    * Merged argument parsing implementations together into one file.  This was a backwards incompatible change around how argument parsing without streaming was done, and so required the major version bump to 3.
* [ab726d9](ab726d9c58ecc09dbe234cc28484422c69a69bc6)
    * Added argument parser error checking (#10).  With this, the argument parsing routine was rewritten which made the compiled version smaller.  This significantly changes how non `-c` argument parsing is handled now, and it may not be supported for v3.0.
    * Fixed a bug with the `exec` command when the exec call out fails.  Before, the error was ignored.  Added a test to cover this (`exec-not-a-command.sh`).
* [162b1f1](162b1f1ec35ce8d8fcf19e8dbe274ad01b7d8bc8)
    * Adjusted tests to allow "-" on each flag to run only if the flag is not enabled.
    * Marked `REQUIRE_FULL_CMD` as a top-level Makefile flag.  Added tests for this flag, and added it to the version flags report and combination builds.
    * Added `ENVIRO_INPUT` as top-level Makefile flag.  Added it to the version flags report and combination builds.
    * Added environment variable parsing when the `ENVIRO_INPUT` flag is set (#11).
* [ea4f744](ea4f74499c235c07ead248230c161d94e252078a)
    * Rebranded `fs-shell` as `precision-shell` (presh).


## v2.0.2

* [0875b25](0875b259cf26427f003a570c96ff3620c3a30104)
    * Fixed an issue with allocating memory.  The type sizes were not 100% accurate, but this only led to allocating more memory than what was actually needed.
    * Fixed an issue with tests not running the version command tests.
    * Updated the `sample.Dockerfile` to show how to fully build the command + copy it into a scratch org from a multi-part Dockerfile.


## v2.0.1

* Fixed an issue with many commands reporting ok status when required arguments are not given.  This increases the file size slightly for the minimal commands, though.  This introduces a new element to the command coding.  This is disabled, but can be enabled by adding the define `REQUIRE_FULL_CMD`; turning this on will cause some tests to fail, as it alters the behavior.
* Altered the make to not require Python to build (issue #4).  If Python is not installed and the headers are not generated, then the build will still fail.
* Added a new test build that checks that each individual command flag can build and passes tests.  This also ensures that, with the flag enabled, the `version` command reports the command enabled (issue #3).
* Changed the automated builds to instead run the build once for each command.  The old combinatorics test still exists, but it's been renamed and will only build on pushes to the `combinatorics-test` branch.  This will likely fail because the test takes over 6 hours.
* Added a build to run glibc on Arch Linux, which produces smaller static binaries than on Ubuntu.
* Fixed a bug in the tests that would cause "rm" only command builds to also run rmdir commands.
* General code infrastructure quality improvements.
* Added a `.dockerignore` file to tighten up the docker file sizes.
* Updated documentation to better match the current version.
* Late update to the changes file to include v2.0.0.


## v2.0.0

* Added `fmode` to set a common value to set the file mode used by mkdir, touch, trunc, the dup commands, and some others.  This introduced a backwards-incompatible difference with mkdir, thus making the version jump.
* Changed how the commands are written, based upon a thorough examination on how different coding styles impact the final file size (see [size-experiments.md](size-experiments.md)).  This was a major change that included a lot of experimentation, so the version bump here is too many change files to mention.


## v1.2.0

* Added new commands:
    * touch
    * trunc
    * dup-r
    * dup-w
    * dup-a
* Added a 'build-inventory.txt' file to the output directory to include all the files created.  This helps with scripts to read the list of files, rather than needing to maintain duplicate lists throught the system.


## v1.1.2

* [0b7fd3b](0b7fd3ba4195ab7430773156fd616c133bcf5a09) - changed values to be integers instead of longs.


## v1.1.1

* [1d24bd5](1d24bd5546140a2439c3b88b6f402928b8a1e473) - bug fixes based on test results from running in glibc and dietlibc.
    * Signal handling works better with dietlibc, but not 100%.  dietlibc uses the older POSIX standard for sigwait() which caused incorrect error checking.  This is improved, but there's still an issue with the interesting scenario for ignoring some signals and not waiting on them.
    * Number format error checking is vastly improved to better detect when something is not a numeric value, or has non-numeric text after a number.  This was causing issues with glibc.
* [1d24bd5](1d24bd5546140a2439c3b88b6f402928b8a1e473) - version command now includes the library that built it.  This is pulled from the `version.txt` file.


## v1.1

* Includes new commands:
    * version
    * mv
    * mkdir
    * sleep
    * exec


## v1.0

* Initial release.
* Includes commands:
    * noop
    * echo
    * rm
    * rmdir
    * chmod
    * chown
    * ln-s
    * ln-h
    * signal .. wait (optional build)
* Includes build examples for libraries glibc, musl, dietlibc.
* Standard testing done on musl.
