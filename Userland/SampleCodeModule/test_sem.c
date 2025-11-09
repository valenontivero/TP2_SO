#include <stdint.h>
#include <userio.h>
#include <test_sem.h>
#include <usyscalls.h>

void testSemPoster(){
	while(1){
		char* name = "my_sem";
		int sem = sys_sem_open(name, 0);
		long int i = 500000000;
		while(i > 0){
			i--;
		}
		printf("sem poster\n");
		sys_sem_post(sem);
	}
}

void testSemWaiter(){
	while(1){
		char* name = "my_sem";
		int sem = sys_sem_open(name, 0);
		long int i = 500000;
		while(i > 0){
			i--;
		}
		printf("sem waiter\n");
		sys_sem_wait(sem);
	}
}