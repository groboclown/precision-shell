# Experimenting with Base Functional Form and Sizes

For this tool, three different implementations were created:

* **if-else**: comparing argument tokens against the command name is done through if-else statements.  When a command is found, it's assigned to a unique number which is then used in a big case statement for executing the command over each argument.
* **table-case**: command names are stored in a table, which are looped over to perform comparison for the assigned command integer.  This integer is then used for a case statement when first discovered (only a few commands are in this case), and also for each argument to the command.
* **table**: all command names and execution functions are stored in tables.  Commands are implemented in separate functions.

Each of these commands can be found in the `v2` branch of the code.

The implementations were compiled against the glibc, musl, and dietlibc libraries using four different command sets, as defined in the `Makefile.command-flags` file ("zero" being a version built with only the `version` command, which cannot be turned off).


| Commands | if-else / glibc | . / musl | . / dietlibc | table-case / glibc | . / musl | . / dietlibc | table-func / glibc | . / musl | . / dietlibc |
|----------|----------------:|---------:|-------------:|-------------------:|---------:|-------------:|--------------:|---------:|-------------:|
| Zero     | 819656 | 21944 | 13256 | 819656 | 21944 | 13256 | 819688 | 22056 | 13256 |
| Minimal  | 819656 | 21944 | 17352 | 819656 | 21944 | 13256 | 819784 | 22152 | 17352 |
| Standard | 831944 | 26048 | 17424 | 831944 | 26048 | 17424 | 832168 | 26344 | 17424 |
| All      | 831944 | 30144 | 21520 | 831944 | 26048 | 17424 | 832264 | 26424 | 21520 |

To generate these values, run this command for each implementation in the `v2` branch:

```bash

test -d .tmp/sizes && rm -r .tmp/sizes
mkdir -p .tmp/sizes
for version in size-versions/* ; do
    if [ -d ${version} ]; then
        branch=$( basename ${version} )
        for libn in glibc musl dietlibc ; do
            ( cd ${version} && docker build -t local/fs-shell-${branch}-${libn} -f build-${libn}.Dockerfile --build-arg BUILD_MODE=combos . ) 2>&1 | tee .tmp/sizes/${branch}-${libn}.txt
        done
    fi
done

```


## Remarks on Development

The table-func version, being developed last, has a consistent and maintainable use of defines.

Unfortunately, due to how the dietlibc tooling works, data tables with pointers to other sections doesn't work right, and must instead be constructed in code.  However, this doesn't seem to affect file size too much.  The code size is identical whether the table is explicitly constructed through a function, or if the values are inserted at value definition, so the compatible version should be used instead of splitting this into two parts.  The split in setup has been removed and instead has just a single function to construct the tables.

The implementations that put behavior into `#define` statements make debugging very tricky.  Compile errors are buried deep in the defines, and can hide the real problem.  Breakpoints in `gdb` are near impossible to make in a good way.


## Observations

* **if-else** - This has very stringent requirements around the coding style, where minor mistakes (like leaving out the "else") can lead to very odd behavior that is hard to debug.  Interesting behavior, such as with [`cmd_ln_mv.inc.h`](size-versions/ifname-inlinecase/src/cmd_ln_mv.inc.h), requires special handling in the command runner.  This has file size on par with the table-case version, but scales much worse as more commands are added.  The functionality is implemented inside `#define` statements, which makes for difficult coding.
* **table-case** - This has coding style requirements that force for strict ordering in the command runner and command list, which can make maintenance tricky.  File sizes grow in chunks, and small number of commands keep the executable very small.  Like the if-else construction, the functionality is implemented inside `#define` statements, which makes for difficult coding.
* **table-func** - This has the easiest and most standard C coding style of the three, and is easiest to debug because of the procedural separation of execution.  A particular style was developed for this that allows a per-file set of defines to build up the included functionality, which makes maintenance of the common files easier.  File sizes are strangely in the middle of the two other setups, and is highly dependent upon the underlying c library.


## Other Attempts

Because the table-case setup is the most efficient from a size perspective, but is difficult to write code for, different attempts were made to simplify it.

* Replace `#define` functionality macros with inline functions.  This failed because of the recursion of include files, where a function needed to reference another command index, but it wasn't in-scope in the command-list file.  This created a huge headache and was deemed unsolvable without even more hacks.  If it worked, it would have made debugging easier.
* Use `m4` to generate the source files.  After much hacking, m4, though incredibly powerful, has limitations, such as single tick marks (`'`) and commas (`,`) that when read in comments caused the macro to freak out.  Though this could be solved, the m4 script was extremely complex, broke all kinds of code assist tools, and was difficult to maintain even during development.  Generated code couldn't be traced back to the source easily.
* Use a custom parser and custom mini-language to assemble the code.  Though really custom, and requires even more tooling (in this case, a Python 3 environment), it's a middle-of-the-road solution.  Debugging is still tricky, compiler errors require a bunch of tracing, and the parser + language are really boutique.  However, it gets us our size and ease of coding new commands.

## Results

`fs-shell` philosophy primarily focuses on *just enough functionality*.  File size is secondary.

Note that the table-func version increases linearly as the number of functions is added, whereas the case statements one increase in blocks, most likely because the case lookup table is aligned in blocks.


<!--

Table ready for updates

| Zero     | zieg | ziem | zied | ztcg | ztcm | ztcd | ztfg | ztfm | ztfd |
| Minimal  | mieg | miem | mied | mtcg | mtcm | mtcd | mtfg | mtfm | mtfd |
| Standard | sieg | siem | sied | stcg | stcm | stcd | stfg | stfm | stfd |
| All      | aieg | aiem | aied | atcg | atcm | atcd | atfg | atfm | atfd |

-->
