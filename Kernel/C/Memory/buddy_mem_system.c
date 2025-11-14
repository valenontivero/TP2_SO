// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com

#ifdef BUDDY

#include <lib.h>
#include <mem_manager.h>
#include <videodriver.h>
#include <stdint.h>
#include <stdbool.h>

#define MIN_BLOCK_ORDER 5     
#define MAX_BLOCK_ORDERS 32    

typedef struct BuddyBlock {
    uint8_t order;             // Orden del bloque (tamaño = 2^orden)
    bool is_allocated;         // true = en uso, false = libre
    struct BuddyBlock *prev;   // Bloque anterior en la lista libre
    struct BuddyBlock *next;   // Bloque siguiente en la lista libre
} BuddyBlock;

static void *memoryBase;

static uint8_t maxOrder;

static BuddyBlock *freeBlocks[MAX_BLOCK_ORDERS];

static memoryData mem_stats;

static uint8_t calculateLog2(uint64_t value) {
    if (value == 0)
        return 0;
    
    uint8_t result = 0;
    while (value > 1) {
        value >>= 1;
        result++;
    }
    return result;
}

static BuddyBlock *initializeBlock(void *address, uint8_t blockOrder, BuddyBlock *nextBlock) {
    BuddyBlock *block = (BuddyBlock *)address;
    block->order = blockOrder;
    block->is_allocated = false;
    block->prev = NULL;
    block->next = nextBlock;
    
    if (nextBlock != NULL) {
        nextBlock->prev = block;
    }
    
    return block;
}

static BuddyBlock *extractFromFreeList(BuddyBlock *block) {
    if (block->prev != NULL) {
        block->prev->next = block->next;
    } else {
        // primer bloque de la lista
        freeBlocks[block->order - 1] = block->next;
    }
    
    if (block->next != NULL) {
        block->next->prev = block->prev;
    }
    
    return block->next;
}

static void splitBlock(uint8_t orderIndex) {
    BuddyBlock *largeBlock = freeBlocks[orderIndex];
    if (largeBlock == NULL)
        return;
    
    extractFromFreeList(largeBlock);
    
    uint64_t blockSize = 1ULL << largeBlock->order;
    BuddyBlock *buddyBlock = (BuddyBlock *)((uint8_t *)largeBlock + (blockSize >> 1));
    
    uint8_t newOrder = largeBlock->order - 1;
    
    initializeBlock(buddyBlock, newOrder, freeBlocks[newOrder - 1]);
    freeBlocks[newOrder - 1] = buddyBlock;
    
    initializeBlock(largeBlock, newOrder, buddyBlock);
    freeBlocks[newOrder - 1] = largeBlock;
}

static BuddyBlock *mergeBuddies(BuddyBlock *block1, BuddyBlock *block2) {
    extractFromFreeList(block2);
    
    BuddyBlock *leftBlock = (block1 < block2) ? block1 : block2;
    
    leftBlock->order++;
    
    return leftBlock;
}

static BuddyBlock *findBuddy(BuddyBlock *block) {
    uint64_t relativeOffset = (uint64_t)((uint8_t *)block - (uint8_t *)memoryBase);
    
    uint64_t blockSize = 1ULL << block->order;
    
    uint64_t buddyOffset = relativeOffset ^ blockSize;
    
    return (BuddyBlock *)((uint8_t *)memoryBase + buddyOffset);
}

static bool isValidBlock(BuddyBlock *block) {
    if (block == NULL)
        return false;
    
    uint64_t totalSize = 1ULL << maxOrder;
    uint64_t blockOffset = (uint64_t)((uint8_t *)block - (uint8_t *)memoryBase);
    
    if (blockOffset >= totalSize) {
        return false;
    }
    
    if (block->order > maxOrder || block->order < MIN_BLOCK_ORDER) {
        return false;
    }
    
    uint64_t blockSize = 1ULL << block->order;
    if (blockOffset + blockSize > totalSize) {
        return false;
    }
    
    return true;
}

// Inicializa el sistema de memoria Buddy
void startMemoryManager(const void *start_address, uint64_t size) {
    printString("Using Buddy Memory Manager\n\n");
    
    memoryBase = (void *)start_address;
    maxOrder = calculateLog2(size);
    
    if (maxOrder < MIN_BLOCK_ORDER) {
        return;
    }
    
    memset(freeBlocks, 0, sizeof(BuddyBlock *) * MAX_BLOCK_ORDERS);
    
    mem_stats.total = size;
    mem_stats.free = size;
    mem_stats.used = 0;
    
    freeBlocks[maxOrder - 1] = initializeBlock(memoryBase, maxOrder, NULL);
}

void *malloc(uint64_t requested_size) {
    if (requested_size == 0)
        return NULL;
    
    uint64_t totalSize = requested_size + sizeof(BuddyBlock);
    
    if (totalSize > (1ULL << maxOrder)) {
        return NULL;
    }
    
    uint8_t requiredOrder = calculateLog2(totalSize);
    
    if (totalSize > (1ULL << requiredOrder)) {
        requiredOrder++;
    }
    
    if (requiredOrder < MIN_BLOCK_ORDER) {
        requiredOrder = MIN_BLOCK_ORDER;
    }
    
    if (requiredOrder > maxOrder) {
        return NULL;
    }
    
    uint8_t availableOrder = requiredOrder;
    while (availableOrder <= maxOrder && freeBlocks[availableOrder - 1] == NULL) {
        availableOrder++;
    }
    
    if (availableOrder > maxOrder) {
        return NULL;
    }
    
    while (availableOrder > requiredOrder) {
        splitBlock(availableOrder - 1);
        availableOrder--;
    }
    
    BuddyBlock *allocatedBlock = freeBlocks[requiredOrder - 1];
    extractFromFreeList(allocatedBlock);
    
    allocatedBlock->prev = NULL;
    allocatedBlock->next = NULL;
    allocatedBlock->is_allocated = true;
    
    uint64_t blockSize = 1ULL << allocatedBlock->order;
    mem_stats.used += blockSize;
    mem_stats.free -= blockSize;
    
    return (void *)(allocatedBlock + 1);
}

int free(void *address) {
    if (address == NULL) {
        return -1;
    }
    
    BuddyBlock *block = ((BuddyBlock *)address) - 1;
    
    if (!isValidBlock(block)) {
        return -1;
    }
    
    if (!block->is_allocated) {
        return -1;
    }
    
    if (block->order > maxOrder) {
        return -1;
    }
    
    block->is_allocated = false;
    
    uint64_t blockSize = 1ULL << block->order;
    mem_stats.free += blockSize;
    mem_stats.used -= blockSize;
    
    while (block->order < maxOrder) {
        BuddyBlock *buddy = findBuddy(block);
        
        if (!isValidBlock(buddy)) {
            break;
        }
        
        if (buddy->order != block->order || buddy->is_allocated) {
            break;
        }
        
        block = mergeBuddies(block, buddy);
    }
    
    uint8_t orderIndex = block->order - 1;
    if (orderIndex >= MAX_BLOCK_ORDERS) {
        return -1;
    }
    
    block->next = freeBlocks[orderIndex];
    block->prev = NULL;
    
    if (freeBlocks[orderIndex] != NULL) {
        freeBlocks[orderIndex]->prev = block;
    }
    
    freeBlocks[orderIndex] = block;
    
    return 0;
}

//mem stats
 MemoryDataPtr getMemoryData(void) {
    return &mem_stats;
}

#endif // BUDDY
