#include <stdint.h>
#include <stdio.h>
#include <test_print.h>

void testSemPoster(){
	while(1){
		printf("Post\n");
	}
}

void testSemWaiter(){
	while(1){
		printf("Wait\n");
	}
}