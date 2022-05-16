#ifndef FS_SHELL_VERSION
#define FS_SHELL_VERSION

#ifdef DEBUG
#define _VERSION_DEBUG " +debug"
#else
#define _VERSION_DEBUG
#endif

#ifdef USE_SIGNALS
#define _VERSION_SIGNALS " +signals"
#else
#define _VERSION_SIGNALS
#endif

#ifdef USES_INPUT
#define _VERSION_INPUT " +input"
#else
#define _VERSION_INPUT
#endif

#ifdef USE_EXEC
#define _VERSION_EXEC " +exec"
#else
#define _VERSION_EXEC
#endif

static const char *VERSION_STR =
    // The "fs-shell" should come from the arg0, but we lose that
    // in the argument processing.
    "fs-shell " VERSION_NUMBER
    _VERSION_SIGNALS _VERSION_INPUT _VERSION_EXEC _VERSION_DEBUG "\n";

#endif
