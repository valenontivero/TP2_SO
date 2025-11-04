
// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com
#ifndef BUDDY
#include <mem_manager.h>
#include <videodriver.h>
#include <stdint.h>
#include <lib.h>    

typedef struct mem_block {
    void *addr;                  // dirección de inicio del bloque
    uint64_t size;               // tamaño del bloque
    int free;                    // 1 = libre, 0 = ocupado
    struct mem_block *prev;      // bloque anterior
    struct mem_block *next;      // bloque siguiente
} mem_block;

static mem_block head_block;     // primer bloque
static void *node_region;        // dónde guardamos los metadatos
static memoryData mem_info;      // estadísticas globales

// Alineaa tamaño a multiplos de 8bytes
static inline uint64_t align8(uint64_t n) {
    return (n + 7) & ~((uint64_t)7);
}

// Inicialización del heap
void startMemoryManager(const void *start, uint64_t total_size) {
    printString("Using Linked Memory Manager\n\n");

    // Primer parte del espacio como región para nodos
    node_region = (void *)start;
    void *heap_start = (void *)((uint64_t)start + LIST_SPACE);
    uint64_t heap_size = total_size - LIST_SPACE;

    // Bloque inicial (todo libre)
    head_block.addr = heap_start;
    head_block.size = heap_size;
    head_block.free = 1;
    head_block.prev = NULL;
    head_block.next = NULL;

    mem_info.total = heap_size;
    mem_info.free  = heap_size;
    mem_info.used  = 0;
}

// malloc(): busca el primer bloque libre suficiente
void *malloc(uint64_t req_size) {
    if (req_size == 0)
        return NULL;

    req_size = align8(req_size);

    mem_block *current = &head_block;

    // buscar primer bloque libre con tamaño suficiente
    while (current && !(current->free && current->size >= req_size)) {
        current = current->next;
    }

    if (!current){
        return NULL; // sin espacio
    }
    // Si sobra espacio, dividir bloque
    if (current->size > req_size + sizeof(mem_block)) {
        mem_block *newblk = (mem_block *)node_region;
        node_region = (void *)((uint64_t)node_region + sizeof(mem_block));

        newblk->addr = (void *)((uint64_t)current->addr + req_size);
        newblk->size = current->size - req_size;
        newblk->free = 1;
        newblk->prev = current;
        newblk->next = current->next;

        if (current->next)
            current->next->prev = newblk;
        current->next = newblk;
        current->size = req_size;
    }

    current->free = 0;
    mem_info.used += current->size;
    mem_info.free -= current->size;

    return current->addr;
}

// free(): libera bloque y fusiona si hay vecinos libres
int free(void *ptr) {
    if (!ptr)
        return 1;

    mem_block *curr = &head_block;
    while (curr && curr->addr != ptr)
        curr = curr->next;

    if (!curr)
        return 2; // dirección no encontrada
    if (curr->free)
        return 3; // ya estaba libre

    curr->free = 1;
    mem_info.free += curr->size;
    mem_info.used -= curr->size;

    // fusionar hacia atrás
    if (curr->prev && curr->prev->free) {
        curr->prev->size += curr->size;
        curr->prev->next = curr->next;
        if (curr->next)
            curr->next->prev = curr->prev;
        curr = curr->prev;
    }

    // fusionar hacia adelante
    if (curr->next && curr->next->free) {
        curr->size += curr->next->size;
        curr->next = curr->next->next;
        if (curr->next)
            curr->next->prev = curr;
    }

    return 0;
}

//stats del heap

MemoryDataPtr getMemoryData() {
    return &mem_info;
}

#endif
