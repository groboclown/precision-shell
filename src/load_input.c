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

#include <string.h>
#include <stdlib.h>

#ifdef USES_FD_READ
#include <stdio.h>
#include <fcntl.h>
#endif

#include "output.h"
#include "helpers.h"
#include "load_input.h"


// This was originally included in the arg parser code.
// However, in order to support new functions that may
// take advantage of environment variable parsing, this
// is split out for reuse.  Additionally, this new
// split allows for more standard environment variable
// usage - multi-word values are parsed according to
// quote rules.


typedef struct {
    char **argv;
    int argc;
    int argv_index;
} _InnerDataSrcArgv;

#ifdef USES_FD_READ
typedef struct {
    int fd;
} _InnerDataSrcFd;
#endif /* USES_FD_READ */

typedef union {
    // struct none {};  "none" has no data.
    _InnerDataSrcArgv argv;
#ifdef USES_FD_READ
    _InnerDataSrcFd fd;
#endif
} _InnerDataSrc;


#ifndef MAX_ENVIRONMENT_NAME_LENGTH
#define MAX_ENVIRONMENT_NAME_LENGTH  100
#endif
#ifndef MAX_ENVIRONMENT_VALUE_LENGTH
#define MAX_ENVIRONMENT_VALUE_LENGTH  500
#endif
#ifndef MAX_ENVIRONMENT_VARIABLE_COUNT
#define MAX_ENVIRONMENT_VARIABLE_COUNT  100
#endif
#ifndef FD_READ_BUFFER_LENGTH
#define FD_READ_BUFFER_LENGTH  4096
#endif

#define _LOAD_INPUT_ENV_ROW_LEN \
    (MAX_ENVIRONMENT_NAME_LENGTH + MAX_ENVIRONMENT_VALUE_LENGTH + 2)

#define _load_input_env_pos(idx) \
    (i * _LOAD_INPUT_ENV_ROW_LEN)


// State-based reading only matters when parsing environment
// variables.
#ifdef USES_ENVIRONMENT
enum _LoadInputEnvState {
    LOAD_INPUT_ENV_STATE_NO_PARSE,
    LOAD_INPUT_ENV_STATE_NORMAL,
    LOAD_INPUT_ENV_STATE_MAYBE,
    LOAD_INPUT_ENV_STATE_NAME,
    LOAD_INPUT_ENV_STATE_VALUE,
};
#endif


/**
 * @brief The real state data used internally.
 * 
 */
struct LoadInputState {
#ifdef USES_ENVIRONMENT
    // allocated data
    char *env_buffer;
    int env_buffer_pos;

    // env_value is not allocated, but instead references data managed
    // elsewhere.
    char *env_value;
    int env_value_pos;
    enum _LoadInputEnvState env_read_state;
#endif

    enum DataSrcType   input_type;
    _InnerDataSrc      input;

    // Memory managed by the caller where the data read in by
    // the loader will be placed.
    char *read_buffer;

    // Number of characters allocated for the read buffer.
    int   read_buffer_size;

    // buffer is allocated or not, depending on the data source type.
    // This is where data is written to, depending on the data
    // source type.  For argv, this points to the current argv
    // index.  For static or allocated strings, it's a constant
    // value.  For input stream, it's allocated by this code.
    // buffer_str MUST ALWAYS be \0 terminated.
    char *buffer_str;
    int buffer_pos;
};

#ifdef USES_ENVIRONMENT
char *_load_input_find_env_value(struct LoadInputState *data, char *name);
#endif



struct LoadInputState *load_input_initialize(LoadInputContext *context) {
    int i;
    struct LoadInputState *ret;
    ret = malloc(sizeof(struct LoadInputState));
    if (ret == NULL) {
        stderrP(helper_str__malloc_failed);
        return NULL;
    }
#ifdef USES_ENVIRONMENT
    if (context->uses_environment == 0) {
        ret->env_read_state = LOAD_INPUT_ENV_STATE_NO_PARSE;
        ret->env_buffer = NULL;
    } else {
        ret->env_read_state = LOAD_INPUT_ENV_STATE_NORMAL;

        // + 4 for \0, $, {, and } (added for easy copy back in if )
        ret->env_buffer = malloc(sizeof(char) * (MAX_ENVIRONMENT_NAME_LENGTH + 4));
        if (ret->env_buffer == NULL) {
            stderrP(helper_str__malloc_failed);
            // We're in a bad state, so should we even try to clean up?
            return NULL;
        }

    }
#endif /* USES_ENVIRONMENT */

    ret->input_type = context->input_type;
    ret->read_buffer = context->read_buffer;
    ret->read_buffer_size = context->read_buffer_size;
    ret->buffer_pos = 0;

    switch (context->input_type) {
#ifdef USES_FD_READ
        case DATA_SRC_MANAGED_FD:
        case DATA_SRC_STATIC_FD:
            // Need to allocate the buffer.
            ret->buffer_str = malloc(sizeof(char) * (FD_READ_BUFFER_LENGTH + 1));
            if (ret->buffer_str == NULL) {
                stderrP(helper_str__malloc_failed);
                // We're in a bad state, so should we even try to clean up?
                return NULL;
            }
            ret->input.fd.fd = context->input.fd.fd;
            ret->buffer_str[0] = '\0';
            break;
#endif
        case DATA_SRC_MANAGED_STRING:
        case DATA_SRC_STATIC_STRING:
            ret->buffer_str = context->input.string.source_chars;
            break;

        case DATA_SRC_ARGV:
            ret->input.argv.argc = context->input.argv.argc;
            ret->input.argv.argv = context->input.argv.argv;
            // start parsing at index 1; index 0 is the program name.
            ret->input.argv.argv_index = 1;

            // fall through to set the buffer to the empty string.
            // ret->buffer_pos = empty_string;
            
        case DATA_SRC_NONE:
            ret->buffer_str = (char *) empty_string;
            break;
    }

    return ret;
}


int load_input_close(struct LoadInputState *data) {
    int ret = 0;

#ifdef USES_ENVIRONMENT
    if (data->env_buffer != NULL) {
        free(data->env_buffer);
        data->env_buffer = NULL;
    }
#endif /* USES_ENVIRONMENT */

    switch (data->input_type) {
#ifdef USES_FD_READ
        case DATA_SRC_MANAGED_FD:
            // close returns -1 for errors, so adding -1 would mask errors.
            if (close(data->input.fd.fd) != 0) {
                ret++;
            }

            // fall through to free the buffer.
            // free(data->buffer_str);
        case DATA_SRC_STATIC_FD:
            // need to free the buffer.
            // fall through.
#endif
        case DATA_SRC_MANAGED_STRING:
            free(data->buffer_str);
            break;
        case DATA_SRC_STATIC_STRING:
        case DATA_SRC_ARGV:
        case DATA_SRC_NONE:
            // do nothing
            break;
    }

    free(data);
    return ret;
}


void load_input_read(struct LoadInputState *data, LoadInputResult *result) {
    // Because this is called over and over when the parser finishes reading
    // in the data length, this can reduce the size by returning early rather
    // than continuously rechecking the length if it's okay to insert another
    // character.

    // Previous calls (this function + initialize) MUST setup buffer_str to
    // have at least 1 character in it.  It can't be NULL or an invalid
    // pointer.  Note that this doesn't advance the buffer position here,
    // so that it can work as a kind of a 'put back' operation.
    char ch = data->buffer_str[data->buffer_pos];
    int i;

    result->status = LOAD_INPUT_STATUS_OK;
    result->data_length = 0;
    result->data = data->read_buffer;

    // EOF handling
    // If EOF had not been read yet, then it will not return EOF
    //   as long as there's a chance that there's data in the buffer.
    //   This is our marker that EOF was encountered with possible
    //   data in the buffer.
    if (data->read_buffer_size < 0) {
        result->status = LOAD_INPUT_STATUS_EOF;
        return;
    }

    while (result->data_length < data->read_buffer_size) {

        // Start with things that don't write to result.
        // These must 'continue' or 'return' from their if block;
        // Fall-through advances the buffer_str read.

#ifdef USES_ENVIRONMENT
        if (data->env_read_state == LOAD_INPUT_ENV_STATE_VALUE) {
            // This is reading from the env value, not
            // the read_buffer, so it won't fall out of this if block.
            ch = data->env_value[data->env_value_pos++];
//printf("[trace input] copying env value char %d (%c)\n", ch, ch);
            if (ch == '\0') {
                // Finished copying the variable.
                // Just stop here.
                data->env_read_state = LOAD_INPUT_ENV_STATE_NORMAL;
                return;
            }
//printf("[trace input] appended char %d at %d\n", ch, result->data_length);
            data->read_buffer[result->data_length++] = ch;
            continue;
        }
#endif /* USES_ENVIRONMENT */

        // Handle the current buffer character.
//printf("[trace input] Handling character %d (%c)\n", ch, ch);

        if (ch == '\0') {
            // EOS encountered.  Time to reload the buffer.
            // Read in the data from whatever the input mechanism is.
            if (data->input_type == DATA_SRC_ARGV) {
                if (data->input.argv.argv_index >= data->input.argv.argc) {
                    // EOF
                    LOG(":: Finished parsing argv\n");
                    // mark this as the end, so that the next call will be an EOF.
                    data->read_buffer_size = -1;
                    // No need to adjust data length or append a 0.
                    return;
                }
                // Replace the arg separator with a space.
                //   This means that first argument parsing will introduce a ' '.
//printf("[trace input] appended char %d at %d\n", ' ', result->data_length);
                data->read_buffer[result->data_length++] = ' ';
                data->buffer_str = data->input.argv.argv[data->input.argv.argv_index++];
                data->buffer_pos = 0;

                // rather than re-checking the state + loading ch, return immediately
                // and let the next call handle it all.
                return;
            }
#ifdef USES_FD_READ
            if (
                data->input_type == DATA_SRC_MANAGED_FD
                || data->input_type == DATA_SRC_STATIC_FD
            ) {
                // Write over the whole buffer
                data->buffer_pos = 0;
//printf("[trace input] reading %d bytes from fd %d\n", FD_READ_BUFFER_LENGTH, data->input.fd.fd);
                i = read(data->input.fd.fd, data->buffer_str, FD_READ_BUFFER_LENGTH);
                if (i < 0) {
                    // error state.
                    // in the case of a non-blocking socket, the error would be
                    // EAGAIN or EWOULDBLOCK, which are OK to keep retrying.
                    result->status = LOAD_INPUT_STATUS_ERR;
//printf("[trace input] read failed with %d\n", i);
                    return;
                }
//printf("[trace input] read total of %d bytes\n", i);
                if (i == 0) {
                    // EOF
                    // The full contents have already been read in and
                    // handled in previous calls.
                    // mark this as the end, so that the next call will be an EOF.
                    data->read_buffer_size = -1;
                    // keep the zero assignment + return by falling through.
                    i = 0;
                }
                // buffer is FD_READ_BUFFER_LENGTH + 1 sized...
                data->buffer_str[i] = '\0';

                // Rather than adding code to assign ch + loop,
                // just return immediately.  This is a bit slower, but
                // keeps the caller meaning the same while also reducing
                // code size.

                return;
            }
#endif
            // in all other cases, this is an EOF.
            // mark this as the end, so that the next call will be an EOF.
            data->read_buffer_size = -1;
            // No need to adjust data length or append a 0.
            return;


#ifdef USES_ENVIRONMENT
        } else if (data->env_read_state == LOAD_INPUT_ENV_STATE_MAYBE) {
            // This state means that env parsing is being done, and the
            // last buffer character was a '$'.

            if (ch != '{') {
                // Nope, not an env.  Put back the '$' that was stepped over.
//printf("[trace input] appended char %d at %d\n", '$', result->data_length);
                data->read_buffer[result->data_length++] = '$';
                data->env_read_state = LOAD_INPUT_ENV_STATE_NORMAL;
                // Rather than going through the code to check if
                // there's buffer space for the '{' to insert back,
                // this will just return the current position, and
                // let the call back into here finish it up.
                // Note that, at this point, the buffer_pos hasn't
                // moved past the ch position.

                // If this ch is a '$', then this is the escape mechanism
                // for inserting a '$'.  In this one case, we advance the
                // buffer position.
                if (ch == '$') {
                    data->buffer_pos++;
                }
            } else {
                // It's an env name.  Switch to that state and update the
                // env buffer.

                data->env_read_state = LOAD_INPUT_ENV_STATE_NAME;
                // Load the '{' at the start of the next loop.

                // Because this is an environment variable start,
                // return right away to allow the caller to possibly adjust
                // environment variables based on the buffer read so far.
            }

            // Both of the cases in this if block require returning early,
            // each for their own reasons.
            return;

        } else if (data->env_read_state == LOAD_INPUT_ENV_STATE_NAME) {
            // Reading the environment variable name
            // Note that this doesn't do any kind of out-guessing the user.
            // a '${blah${blah}' text means an error on the user side.

            if (ch == '}') {
                // end of the variable.  Make str-compare easy.
                data->env_buffer[data->env_buffer_pos] = '\0';
//printf("[trace input] finding value for env %s}\n", data->env_buffer);
                data->env_value = getenv(
                    // skip the ${ prefix.
                    &(data->env_buffer[2]));
                if (data->env_value == NULL) {
                    // Not found.  Put the env buffer back.
                    // The other option is to replace the ${blah} with
                    // empty space.  However, keeping it in the buffer is
                    // just a touch of extra space (this setup code),
                    // and helps the end user identify issues.

                    // Set up the env_value to be the same as though the value
                    // was found, but it'll put back the env_buffer that was read.
                    data->env_buffer[data->env_buffer_pos++] = '}';
                    data->env_buffer[data->env_buffer_pos] = '\0';
                    data->env_value = data->env_buffer;
                }
                // else it was found, so we'll insert this value instead.
                data->env_value_pos = 0;
                data->env_read_state = LOAD_INPUT_ENV_STATE_VALUE;

                // skip this character and return.
                data->buffer_pos++;
                return;
            }

            // Still reading the name
            if (data->env_buffer_pos >= MAX_ENVIRONMENT_NAME_LENGTH + 2) {
                data->env_buffer[data->env_buffer_pos] = '\0';
                stderrP("ERROR environment variable name too long: ");
                stderrPLn(data->env_buffer);
                result->status = LOAD_INPUT_STATUS_ERR;
                return;
            }
            data->env_buffer[data->env_buffer_pos++] = ch;
            
            // fall through to read advance
        } else if (
                ch == '$'
                && data->env_read_state == LOAD_INPUT_ENV_STATE_NORMAL
        ) {
            // Maybe an environment variable?
            data->env_buffer_pos = 1;
            data->env_buffer[0] = ch;
            data->env_read_state = LOAD_INPUT_ENV_STATE_MAYBE;

            // fall through to the read advance.  If this
            // is a variable, stop and let the caller parse and
            // maybe adjust the environment variables.
#endif /* USES_ENVIRONMENT */

        } else {
            // Note that there's no special handling for escape sequences.
            // This allows the load_input to be used for normal file copy
            // as well as env replacement.

//printf("[trace input] appended char %d at %d\n", ch, result->data_length);
            data->read_buffer[result->data_length++] = ch;
        }
        data->buffer_pos++;
        ch = data->buffer_str[data->buffer_pos];
    }
}

