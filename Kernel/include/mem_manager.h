#ifndef MM_MANAGER_H
#define MM_MANAGER_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


#define LIST_SPACE (1 << 22)   // 128 mb  - > esto es 2^27 bytes osea mi espacio para asignar la mem dinámica desde 0x60000

// Nodo que representa un bloque de memoria
typedef struct node {
    void *address;              // Dirección de inicio del bloque
    uint64_t size;              // Tamaño del bloque en bytes
    bool is_free;               // Estado: libre (true) o en uso (false)
    struct node *prev;          // Bloque anterior en la lista
    struct node *next;          // Bloque siguiente en la lista
} node;

typedef node *NodePtr;

// struct con stats del heapp
typedef struct memoryData {
    uint32_t free;              // Bytes libres
    uint32_t used;              // Bytes usados
    uint32_t total;             // Total administrado
} memoryData;

typedef memoryData *MemoryDataPtr;



// Inicializa el heap con dirección base y tamaño
void startMemoryManager(const void *start_address, uint64_t size);

// Asigna memoria dinámica
void *malloc(uint64_t size);

// Libera un bloque previamente asignado
int free(void *address);

//Me da stats del heap
MemoryDataPtr getMemoryData(void);

#endif
