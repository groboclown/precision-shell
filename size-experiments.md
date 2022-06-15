# Experimenting with Base Functional Form and Sizes

For this tool, three different implementations were created:

* **if-else**: comparing argument tokens against the command name is done through if-else statements.  When a command is found, it's assigned to a unique number which is then used in a big case statement for executing the command over each argument.
* **table-case**: command names are stored in a table, which are looped over to perform comparison for the assigned command integer.  This integer is then used for a case statement when first discovered (only a few commands are in this case), and also for each argument to the command.
* **table**: all command names and execution functions are stored in tables.

| Commands | if-else / glibc | . / musl | . / dietlibc | table-case / glibc | . / musl | . / dietlibc | table-func / glibc | . / musl | . / dietlibc |
|----------|----------------:|---------:|-------------:|-------------------:|---------:|-------------:|--------------:|---------:|-------------:|
| Zero     | 819656 | 21944 | 13256 | 819656 | 21944 | 13256 | 819688 | 22056 | 13256 |
| Minimal  | 819656 | 21944 | 17352 | 819656 | 21944 | 13256 | 819784 | 22152 | 17352 |
| Standard | 831944 | 26048 | 17424 | 831944 | 26048 | 17424 | 832168 | 26344 | 17424 |
| All      | 831944 | 30144 | 21520 | 831944 | 26048 | 17424 | 832264 | 26424 | 21520 |

To generate these values, run this command for each branch:

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


<!--

Table ready for updates

| Zero     | zieg | ziem | zied | ztcg | ztcm | ztcd | ztfg | ztfm | ztfd |
| Minimal  | mieg | miem | mied | mtcg | mtcm | mtcd | mtfg | mtfm | mtfd |
| Standard | sieg | siem | sied | stcg | stcm | stcd | stfg | stfm | stfd |
| All      | aieg | aiem | aied | atcg | atcm | atcd | atfg | atfm | atfd |

-->
