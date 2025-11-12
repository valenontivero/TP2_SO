#include <stdint.h>
#include <stdio.h>
#include <test_print.h>
#include <usyscalls.h>

void testPriorityHigh(){
	while(1){
		printf("XXXXX\n");
	}
}

void testPriorityMedium(){
	while(1){
		printf("XXX\n");
	}
}

void testPriorityLow(){
	while(1){
		printf("X\n");
	}
}

