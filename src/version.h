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

#ifdef USE_ENVIROMENT_INPUT
#define _VERSION_ENVIRON " +enviro"
#else
#define _VERSION_ENVIRON ""
#endif

#ifdef REQUIRE_FULL_CMD
#define _VERSION_REQARGS " +reqargs"
#else
#define _VERSION_REQARGS ""
#endif

static const char VERSION_STR[] =
    " " VERSION_NUMBER _VERSION_INPUT _VERSION_ENVIRON _VERSION_REQARGS _VERSION_DEBUG;

#endif /* _FS_SHELL_VERSION_ */
