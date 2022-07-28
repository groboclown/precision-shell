/*
MIT License

Copyright (c) 2022 groboclown

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef _FS_SHELL_LOAD_INPUT_
#define _FS_SHELL_LOAD_INPUT_

#include "uses.h"


/**
 * @brief The source for the input to the environment variable parser.
 * 
 */
enum DataSrcType {
    // Things with closable actions are first.
    DATA_SRC_MANAGED_STRING,

#ifdef USES_FD_READ
    DATA_SRC_MANAGED_FD,
#endif

    // These have no close actions.

#ifdef USES_FD_READ
    DATA_SRC_STATIC_FD,
#endif
    DATA_SRC_NONE,
    DATA_SRC_ARGV,
    DATA_SRC_STATIC_STRING
};


/**
 * @brief String input defined in argv/argc style values.
 * 
 * The memory in the argv value is managed by the caller.
 * The source type for this is DATA_SRC_ARGV
 */
typedef struct {
    // Not allocated memory.
    // Parsing will start at argv[1].
    char **argv;
    int argc;
} DataSrcArgv;


/**
 * @brief String input contained in a c-style (\0 terminated) string.
 * The memory of this data will be managed (freed) by the input loader if
 * the type is DATA_SRC_MANAGED_STRING; if DATA_SRC_STATIC_STRING, then
 * the caller must manage the memory.
 */
typedef struct {
    // source is allocated
    char *source_chars;
} DataSrcString;


#ifdef USES_FD_READ
/**
 * @brief Data loaded through a file descriptor.
 * 
 * The file descriptor is managed (closed) by the input loader
 * if the type is DATA_SRC_MANAGED_FD; if DATA_SRC_STATIC_FD, then
 * the caller must manage closing it.
 */
typedef struct {
    int fd;
} DataSrcFd;
#endif /* USES_FD_READ */


typedef union {
    // struct none {};  "none" has no data.
    DataSrcArgv argv;
    DataSrcString string;
#ifdef USES_FD_READ
    DataSrcFd fd;
#endif
} DataSrc;


/**
 * @brief The source for the input.
 * 
 */
typedef struct {
#ifdef USES_ENVIRONMENT
    // if non-zero, then the environment variables will be used.
    // if 0, then the environment variables will not be used.
    int uses_environment;
#endif
    enum DataSrcType   input_type;
    // input must match the input_type
    DataSrc            input;

    // Memory managed by the caller where the data read in by
    // the loader will be placed.  Data loaded here will never
    // be \0 terminated unless the input data also had a
    // \0.
    char *read_buffer;

    // Number of characters allocated for the read buffer.
    int   read_buffer_size;
} LoadInputContext;


/**
 * @brief Result of a single data read call.
 * 
 */
enum LoadInputStatus {
    // Everything is fine.  The data length is the number
    // of characters read.
    LOAD_INPUT_STATUS_OK,

    // data_length characters were read, then the EOF was encountered.
    // The load_input_close will still need to be called.
    LOAD_INPUT_STATUS_EOF,

    // An error occurred reading the data, and no more data should be
    // considered valid from this input state.
    // The load_input_close will still need to be called.
    LOAD_INPUT_STATUS_ERR
};


/**
 * @brief 
 * 
 */
typedef struct {
    enum LoadInputStatus status;
    int data_length;

    // This is the same pointer passed in with the context.
    // It is not \0 appended; use data_length instead.
    char *data;
} LoadInputResult;


/**
 * @brief opaque type used to record the state of the input loading.
 * 
 * The structure is always passed as a pointer, and managed by the
 * load_input functions.
 */
struct LoadInputState;


/**
 * @brief Begin the process for loading the input defined in the context.
 * 
 * @param context data context used to load the data.
 * @return LoadInputState* data used by load_input functions, or NULL on error.
 */
struct LoadInputState *load_input_initialize(LoadInputContext *context);

/**
 * @brief Closes the input.  The data passed in must be considered invalid
 * after this call.  The load_input will free its memory.
 * 
 * @param state loader internal state data; must not be NULL
 * @return int 0 if the load completed without issue, and anything else for an error.
 */
int load_input_close(struct LoadInputState *state);


/**
 * @brief Reads data from the input into the context buffer, and reports status to the result.
 * 
 * Can potentially read 0 bytes and still be in a valid state for reading
 * more data.  If the result state is error, then the read data buffer is in
 * an unknown state but the load_input_close will still need to be called.
 * 
 * @param state internal load input state data.  Must not be NULL.
 * @param result result information for caller referencing.  Must not be NULL.  This can
 *      be safely reused.
 */
void load_input_read(struct LoadInputState *state, LoadInputResult *result);


#endif /* _FS_SHELL_LOAD_INPUT_ */
