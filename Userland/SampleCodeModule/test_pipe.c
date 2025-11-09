#include <stdint.h>
#include <stdio.h>
#include <test_print.h>

void testPipeReader(){
	char** name = "testpipe";
	char** msg;
	int pipe = sys_pipe_open(name);
	while(1){
		sys_pipe_read(pipe, msg, 8);
		printf("%s\n", msg);
	}
}

void testPipeWriter(){
	char** name = "testpipe";
	char** msg = "funciona";
	int pipe = sys_pipe_open(name);
	while(1){
		sys_timer_wait(1);
		sys_pipe_write(pipe, msg, 8);
	}
}