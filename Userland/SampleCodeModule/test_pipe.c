#include <stdint.h>
#include <stdio.h>
#include <test_print.h>

void testPipeReader(){
	while(1){
		printf("HIGH\n");
	}
}

void testPipeWriter(){
	while(1){
		printf("low\n");
	}
}