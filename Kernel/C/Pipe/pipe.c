// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com

#include <pipe.h>
#include <lib.h>
#include <mem_manager.h>
#include <processManager.h>
#include <scheduler.h>
#include <PCBQueueADT.h>
#include <mySem.h>
#include <videodriver.h>
#include <interrupts.h>
#include <limits.h>

static Pipe pipes[MAX_PIPES];
static PipeFD pipeFDs[MAX_PIPES * 2];  // cada pipe puede tener hasta 2 descriptores

static int started = 0;

void initPipes() {
    for (int i = 0; i < MAX_PIPES; i++) {
        pipes[i].inUse = 0;
    }
    for (int i = 0; i < MAX_PIPES * 2; i++) {
        pipeFDs[i].inUse = 0;
    }
}

uint8_t pipe_open(const char* name) {
	if (!started) {
		initPipes();
		started = 1;
	}

	Pipe* pipe = NULL;
	int i;

	// Ver si ya existe
	for (i = 0; i < MAX_PIPES; i++) {
		if (pipes[i].inUse && strcmp(pipes[i].name, name) == 0) {
			pipe = &pipes[i];
			break;
		}
	}
    
	// Si no existe, crearlo
	if (!pipe) {
        for (i = 0; i < MAX_PIPES; i++) {
            if (!pipes[i].inUse) {
                pipe = &pipes[i];
				pipe->inUse = 1;
				safe_strncpy(pipe->name, name, MAX_PIPE_NAME);
				pipe->name[MAX_PIPE_NAME - 1] = '\0';  // Asegurar nul-terminación
				pipe->readIdx = 0;
				pipe->writeIdx = 0;
				pipe->size = 0;
                char sem_write_name[MAX_PIPE_NAME + 5];
                safe_strncpy(sem_write_name, name, MAX_PIPE_NAME);
                my_strcat(sem_write_name, "read");
                uint64_t desiredSlots = PIPE_BUFFER_SIZE - 1;
                uint8_t initialSlots = (desiredSlots > UINT8_MAX) ? UINT8_MAX : (uint8_t)desiredSlots;
               	pipe->write_sem=sem_open(sem_write_name, initialSlots);
                pipe->read_sem=sem_open(name, 0);
				break;
			}
		}
	}

	if (!pipe) return -1;  // No hay espacio

	return i + 2; // le sumo mas 2 porque el 0 y 1 son stdin y stdout para consola
}

uint64_t pipe_write(int fd, const char* buf, uint64_t count) {
    fd -= 2; // Ajustar fd para que coincida con el índice de pipeFDs
    if (fd < 0 || fd >= MAX_PIPES)
        return -1;
    
    Pipe* pipe = &pipes[fd];
    int written = 0;

    while (written < count) {
        sem_wait(pipe->write_sem);

        pipe->buffer[pipe->writeIdx] = buf[written];
        pipe->writeIdx = (pipe->writeIdx + 1) % PIPE_BUFFER_SIZE;
        pipe->size++;
        written++;
        sem_post(pipe->read_sem);
    }

    return written;
}

uint64_t pipe_read(int fd, char* buf, uint64_t count) {
    fd -= 2; // Ajustar fd para que coincida con el índice de pipeFDs
    if (fd < 0 || fd >= MAX_PIPES * 2 )
        return -1;
    
    Pipe* pipe = &pipes[fd];
    int read = 0;
    
    while (read < count) {
        sem_wait(pipe->read_sem);
        buf[read] = pipe->buffer[pipe->readIdx];
        pipe->readIdx = (pipe->readIdx + 1) % PIPE_BUFFER_SIZE;
        if (pipe->size >0) pipe->size--;
        read++;
        sem_post(pipe->write_sem);
    }

    return read;
}

void pipe_close(int fd) {
    fd -= 2; // Ajustar fd para que coincida con el índice de pipeFDs
    if (fd < 0 || fd >= MAX_PIPES || !pipeFDs[fd].inUse)
        return;

    pipes[fd].inUse = 0;
}

void resetBuffer(uint16_t fd){
    fd-= 2; // Ajustar fd para que coincida con el índice de pipes
    for (size_t i = 0; i < PIPE_BUFFER_SIZE; i++)
    {
        pipes[fd].buffer[i] = 0;
    }
}

int8_t changeProcessFd(uint16_t pid, uint8_t fd, uint8_t end){
    if(pid >= MAX_PROCESSES || fd >= MAX_PIPES || end >= 2) {
        return -1; // Invalid PID, fd, or end
    }
    PCB* process= getPCBByPID(pid);
    process->fd[end]=fd;
    return 0;
}