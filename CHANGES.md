
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