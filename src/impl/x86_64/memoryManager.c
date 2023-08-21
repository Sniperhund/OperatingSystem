#include "memoryManager.h"

#include <stdint.h>
#include <stddef.h>

struct MemoryManager* memoryManager;

extern uint32_t kernel_end;

// Doesn't work yet

void MemoryManagerInitialize() {
    memoryManager->memoryStart = (void*)&kernel_end;
    memoryManager->totalSize = 0;
    memoryManager->freeBlocks = (struct MemoryBlock*)memoryManager->memoryStart;
    memoryManager->freeBlocks->size = memoryManager->totalSize - sizeof(struct MemoryBlock);
    memoryManager->freeBlocks->next = NULL;
}

void* kmalloc(int size) {
    struct MemoryBlock* currentBlock = memoryManager->freeBlocks;
    struct MemoryBlock* previousBlock = NULL;

    while (currentBlock != NULL) {
        if (currentBlock->size >= size) {
            if (currentBlock->size > size + sizeof(struct MemoryBlock)) {
                struct MemoryBlock* newBlock = (struct MemoryBlock*)((uint64_t)currentBlock + sizeof(struct MemoryBlock) + size);
                newBlock->size = currentBlock->size - size - sizeof(struct MemoryBlock);
                newBlock->next = currentBlock->next;
                currentBlock->size = size;
                currentBlock->next = newBlock;
            }

            if (previousBlock == NULL) {
                memoryManager->freeBlocks = currentBlock->next;
            } else {
                previousBlock->next = currentBlock->next;
            }

            return (void*)((uint64_t)currentBlock + sizeof(struct MemoryBlock));
        }

        previousBlock = currentBlock;
        currentBlock = currentBlock->next;
    }

    return NULL;
}

void kfree(void *ptr) {
    struct MemoryBlock* currentBlock = (struct MemoryBlock*)((uint64_t)ptr - sizeof(struct MemoryBlock));
    struct MemoryBlock* previousBlock = NULL;
    struct MemoryBlock* nextBlock = memoryManager->freeBlocks;

    while (nextBlock != NULL) {
        if (nextBlock > currentBlock) {
            break;
        }

        previousBlock = nextBlock;
        nextBlock = nextBlock->next;
    }

    if (previousBlock == NULL) {
        memoryManager->freeBlocks = currentBlock;
    } else {
        previousBlock->next = currentBlock;
    }

    currentBlock->next = nextBlock;

    if (previousBlock != NULL && (uint64_t)previousBlock + previousBlock->size + sizeof(struct MemoryBlock) == (uint64_t)currentBlock) {
        previousBlock->size += currentBlock->size + sizeof(struct MemoryBlock);
        previousBlock->next = currentBlock->next;
        currentBlock = previousBlock;
    }

    if (nextBlock != NULL && (uint64_t)currentBlock + currentBlock->size + sizeof(struct MemoryBlock) == (uint64_t)nextBlock) {
        currentBlock->size += nextBlock->size + sizeof(struct MemoryBlock);
        currentBlock->next = nextBlock->next;
    }
}