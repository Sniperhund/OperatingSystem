#pragma once

#include <stdbool.h>

struct MemoryBlock {
    int size;
    struct MemoryBlock* next;
};

struct MemoryManager {
    void* memoryStart;
    int totalSize;
    struct MemoryBlock* freeBlocks;
};

void MemoryManagerInitialize();