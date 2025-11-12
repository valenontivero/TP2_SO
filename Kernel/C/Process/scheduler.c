// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com
#include <scheduler.h>
#include <stdint.h>
#include <stddef.h>
#include <PCBQueueADT.h>
#include <processManager.h>
#include <mem_manager.h>
#include <ourTime.h>
#include <videodriver.h>


static const uint8_t quantumPerPriority[PRIORITY_LEVELS] = {1* BASE_QUANTUM, 2* BASE_QUANTUM, 3* BASE_QUANTUM, 4 * BASE_QUANTUM, 5*BASE_QUANTUM};
static PCB* idlePCB;
static int idlePID;

schedulerStruct *scheduler;

void initScheduler(void *stackBase) {
  scheduler = malloc(sizeof(schedulerStruct));
  if(scheduler == NULL) {
    printString("Error al inicializar el scheduler: malloc fallido");
    timer_wait(5000);
    return;
  }
  for (uint8_t i = 0; i < MAX_PRIO; i++) {
    scheduler->schedule[i] = CreatePCBQueueADT();
  }

  scheduler->currentRunningPCB = malloc(sizeof(PCB));
  scheduler->currentRunningPCB->state= TERMINATED;
  scheduler->currentRunningPCB->stackBase = stackBase;
  for (uint8_t i = 0; i < MAX_PRIO; i++) {
      scheduler->count[i] = 0;
  }
  char *argv[] = { "idle", NULL };
  idlePID= createProcess((void*)idleProcess,0,0,argv,argv[0]);
  idlePCB= getPCBByPID(idlePID);
}

void scheduleProcess(PCB *pcb) {
  queueProcess(scheduler->schedule[pcb->priority], pcb);
}

void descheduleProcess(PCB *pcb) {
  removeProcess(scheduler->schedule[pcb->priority], pcb);
}

void *schedule(void *rsp) {
    if (!scheduler || !scheduler->currentRunningPCB)
        return rsp;

    PCB *old = scheduler->currentRunningPCB;
    old->stackPointer = rsp;

    // Guardar estado del proceso actual
    if (old->state == RUNNING) {
      if (--old->remainingQuantum == 0)
      {
        old->state = READY;
        old->remainingQuantum= quantumPerPriority[old->priority];
        queueProcess(scheduler->schedule[old->priority], old);
      }else{
        return rsp; // Sigue ejecutando el mismo proceso
      }
    }

    PCB *next = NULL;

    // Elegir el próximo proceso según presupuesto
    for (int8_t i = MAX_PRIO - 1; i >= 0; i--) {
        if (!isEmpty(scheduler->schedule[i]) && scheduler->count[i] <= (i * 5 + 1)) {
            next = dequeueProcess(scheduler->schedule[i]);
			/* if(next->state != READY){
				queueProcess(scheduler->schedule[next->priority], next);
				continue;
			} */
            scheduler->currentRunningPCB = next;
            scheduler->currentRunningPCB->state = RUNNING;
            scheduler->count[i]++;
            return next->stackPointer;
        }
    }

    // Si nadie fue elegido, reseteo y vuelvo a intentar
    for (int i = 0; i < MAX_PRIO; i++)
        scheduler->count[i] = 0;

    // Segunda pasada después del reset
    for (int8_t i = MAX_PRIO - 1; i >= 0; i--) {
        if (!isEmpty(scheduler->schedule[i])) {
            next = dequeueProcess(scheduler->schedule[i]);
            scheduler->currentRunningPCB = next;
            scheduler->currentRunningPCB->state = RUNNING;
            scheduler->count[i]++;
            return next->stackPointer;
        }
    }

    // No hay procesos listos
    scheduler->currentRunningPCB = idlePCB;
    idlePCB->state = RUNNING;
    return idlePCB->stackPointer;
}

uint16_t getCurrentPID(){return scheduler->currentRunningPCB->pid;}
PCB* getCurrentProcess(){return scheduler->currentRunningPCB;}