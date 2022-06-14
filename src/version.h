#ifndef _FS_SHELL_VERSION_
#define _FS_SHELL_VERSION_

#ifdef DEBUG
#define _VERSION_DEBUG " +debug"
#else
#define _VERSION_DEBUG ""
#endif

#ifdef USE_STREAMING_INPUT
#define _VERSION_INPUT " +input"
#else
#define _VERSION_INPUT ""
#endif

static const char *VERSION_STR =
    // The "fs-shell" should come from the arg0, but we lose that
    // in the argument processing.
    " " VERSION_NUMBER _VERSION_INPUT _VERSION_DEBUG;

#endif /* _FS_SHELL_VERSION_ */
