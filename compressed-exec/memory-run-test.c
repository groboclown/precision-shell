/*
 * Tests loading a binary blob into memory and running it.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>

static long getFileSize(const char *fileName)
{
    struct stat sb;
    if (stat(fileName, &sb) != 0) {
        printf("stat() failed\n");
        return -1;
    }
    return sb.st_size;
}

int main(const int argc, const char *argv[], const char *envp[]) {
    long fileSize = 0;
    const int (*data) (const int, const char **, const char **) = NULL;
    int fd;
    size_t bytesRead;
    int status;

    if (argc < 4) {
        printf("Usage: memory-run-test (name of file) (args ...)\n");
        return 1;
    }

    if (strlen(argv[index]) > 2 && argv[index][0] == '0' && argv[index][1] == 'x') {
        baseAddr = strtoul(&(argv[index][2]), NULL, 16);
    } else {
        baseAddr = strtoul(argv[index], NULL, 16);
    }

    printf("Reading file size...\n");
    fileSize = getFileSize(argv[1]);
    if (fileSize < 0) {
        return 2;
    }
    printf("Allocating memory...\n");
    data = mmap(NULL, fileSize + 1, PROT_READ|PROT_WRITE|PROT_EXEC, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0); 
    if (data == MAP_FAILED) {
        printf("mmap() failed\n");
        return 3;
    }
    printf("Opening file...\n");
    fd = open(argv[1], O_RDONLY);
    if (fd == -1) {
        printf("open() failed\n");
        return 1;
    }
    printf("Reading data...\n");
    bytesRead = read(fd, data, fileSize);
    if (bytesRead != fileSize) {
        printf("Did not read enough data\n");
        return 1;
    }
    printf("Closing file...\n");
    status = close(fd);
    if (status != 0) {
        printf("close() failed\n");
        return 1;
    }

    // data should now be executable.
    printf("Running code...\n");
    return data(argc - 1, &(argv[1]), envp);
}
