#include <stdint.h>
#include <test_print.h>
#include <usyscalls.h>
#include <uStrings.h>
#include <stdio.h>

void testPipeReader() {
    const char *name = "testpipe";
    char buffer[16] = {0};
    int pipe = (int)sys_pipe_open(name);

    if (pipe < 0) {
        printf("testPipeReader: pipe open failed\n");
        return;
    }

    while (1) {
        uint64_t readBytes = sys_pipe_read((unsigned int)pipe, buffer, sizeof(buffer) - 1);
        if (readBytes == (uint64_t)-1) {
            printf("testPipeReader: pipe read failed\n");
            return;
        }
        if (readBytes == 0) {
            continue;
        }

        if (readBytes >= sizeof(buffer)) {
            readBytes = sizeof(buffer) - 1;
        }
        buffer[readBytes] = 0;
        printf("%s\n", buffer);
    }
}

void testPipeWriter() {
    const char *name = "testpipe";
    const char *msg = "funciona";
    uint64_t length = strlen((char *)msg);
    int pipe = (int)sys_pipe_open(name);

    if (pipe < 0) {
        printf("testPipeWriter: pipe open failed\n");
        return;
    }

    while (1) {
        sys_timer_wait(1);
        if (sys_pipe_write((unsigned int)pipe, msg, length) == (uint64_t)-1) {
            printf("testPipeWriter: pipe write failed\n");
            return;
        }
    }
}
