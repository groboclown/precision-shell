# How You Can Help


## Report Bugs

You can report bugs in the GitHub project.


## Contributing Code

To contribute code to the project, you must be okay with your submission to follow the project license.


## Developing

In order to compile in a very, very small executable, the code uses C, but with a layer that pre-compiles into `#define` macros.  This allows for easier-to-manage coding, but at the expense of a custom file format.  A guide to the special format for command code is in the [source tree](src/README.md).

To compile the shell, you will need a C compiler that includes a version of the standard C libraries.

To test, run:

```bash
chmod +x tests/*.sh tests/*/*.sh && docker build -f build-(libname).Dockerfile .
```

You can build directly, but some of the tests require root privileges, which are safer to run from within a container.

To build through Docker and capture the built executables:

```bash
mkdir -p out
for libname in glibc glibc-arch musl ; do
    docker build -t local/presh-${libname} -f build-${libname}.Dockerfile .
    container=$( docker create local/presh-${libname} )
    docker cp "${container}:/opt/code/out/presh" out/presh.${libname}
    docker cp "${container}:/opt/code/out/presh-zipped" out/presh-zipped.${libname}
    docker cp "${container}:/opt/code/out/presh-debug" out/presh-debug.${libname}
    docker rm "${container}"
done
```

The build generates combinations of the flags for different executables with various enabled actions.  The `version` command lists which flags the executable has enabled.

To enable different commands in the compiled executable from the Docker build, run the build command like so:

```bash
docker build \
  -t local/presh-${libname} -f build-${libname}.Dockerfile \
  --build-arg COMMANDS="rm rmdir chmod chown" \
  .
```

Additionally, for your own purposes, you can build it against the "dietlibc" library.  However, this has special considerations in that it's GPL v2, and [you can't distribute the binary under this program's MIT license](http://www.fefe.de/dietlibc/FAQ.txt).

```bash
docker build -t local/presh-dietlibc -f build-dietlibc.Dockerfile . \
    && ./extract-executables.sh local/presh-dietlibc -o out -s .dietlibc -d
```

To bump the version number, change the [version.txt](version.txt) file.  Version numbers must be in a dewey decimal format MAJOR.MINOR.PATCH (e.g. `1.2.3`).


## Releasing

The normal development process requires:

1. Create a new branch off of `main`, named `feature.(name of feature)` or `bug.(bug id)`.
2. Develop the code and test it.
3. Update the [`CHANGES.md`](CHANGES.md) file.  Do this by first adding in the changes, commit, then update the file with the commit ID.
4. Create a PR into the `dev` branch.
5. Ensure all builds pass.

When ready to release:

1. Bump the version number in [version.txt](version.txt).
2. Bump the version number in [Dockerfile](Dockerfile) and [sample.Dockerfile](sample.Dockerfile).
3. Update the root [README.md](README.md) file with the latest binary file sizes.  The automated builds include the file sizes.
4. Update [`recipes/README.md`](recipes/README.md) file with the binary file sizes from the recipes build.
5. Create a PR into the `main` branch.
6. Ensure all builds pass.
7. Author a new release in GitHub, with the title & tag set to the new version number.  No binary files are included here, but it auto-generates the source tarball.

Version numbers generally follow the guideline of bug fixes increment the patch (third number), added functionality but not backwards incompatible increases the minor (second number), and backwards incompatible changes increases the major (first number).  Some things such as documentation improvements or added platform support, which does not change the functionality of the tool itself, trigger a release, but these will increase the patch number.
