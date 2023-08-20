#include "interrupts/gdt.h"
#include <stdint.h>

typedef struct {
    uint16_t LimitLow;          // Limit (bits 0-15)
    uint16_t BaseLow;           // Base (bits 0-15)
    uint8_t BaseMiddle;         // Base (bits 16-23)
    uint8_t Access;             // Access
    uint8_t FlagsLimitHi;       // Limit (bits 16-19) | flags
    uint8_t BaseHigh;           // Base (bits 24-31)
} __attribute__((packed)) GDTEntry;

typedef struct {
    uint16_t Limit;             // sizeof(gdt) - 1
    GDTEntry* Ptr;              // Address of GDT
} __attribute__((packed)) GDTDescriptor;

typedef enum {
    GDT_ACCESS_CODE_READABLE                = 0x02,
    GDT_ACCESS_DATA_WRITEABLE               = 0x02,

    GDT_ACCESS_CODE_CONFIRMING              = 0x04,
    GDT_ACCESS_DATA_DIRECTION_NORMAL        = 0x00,
    GDT_ACCESS_DATA_DIRECTION_DOWN          = 0x04,

    GDT_ACCESS_DATA_SEGMENT                 = 0x10,
    GDT_ACCESS_CODE_SEGMENT                 = 0x18,

    GDT_ACCESS_DESCRIPTOR_TSS               = 0x00,

    GDT_ACCESS_RING0                        = 0x00,
    GDT_ACCESS_RING1                        = 0x20,
    GDT_ACCESS_RING2                        = 0x40,
    GDT_ACCESS_RING3                        = 0x60,

    GDT_ACCESS_PRESENT                      = 0x80,
} GDTAccess;   

typedef enum {  
    GDT_FLAG_64BIT                          = 0x20,
    GDT_FLAG_32BIT                          = 0x40,
    GDT_FLAG_16BIT                          = 0x00,

    GDT_FLAG_GRANULARITY_1B                 = 0x00,
    GDT_FLAG_GRANULARITY_4K                 = 0x80,
} GDTFlags;    

#define GDT_LIMIT_LOW(limit)                (limit & 0xFFFF)
#define GDT_BASE_LOW(base)                  (base & 0xFFFF)
#define GDT_BASE_MIDDLE(base)               ((base >> 16) & 0xFF)
#define GDT_FLAGS_LIMIT_HI(limit, flags)    (((limit >> 16) & 0xF) | (flags & 0xF0))
#define GDT_BASE_HIGH(base)                 ((base >> 24) & 0xFF)

#define GDT_ENTRY(base, limit, access, flags) { \
    GDT_LIMIT_LOW(limit),                       \
    GDT_BASE_LOW(base),                         \
    GDT_BASE_MIDDLE(base),                      \
    access,                                     \
    GDT_FLAGS_LIMIT_HI(limit, flags),           \
    GDT_BASE_HIGH(base)                         \
}

GDTEntry GDT[] = {
    // NULL Descriptor
    GDT_ENTRY(0, 0, 0, 0),

    // Kernel 32-bit code segment
    GDT_ENTRY(0, 
        0xFFFFF, 
        GDT_ACCESS_PRESENT | GDT_ACCESS_RING0 | GDT_ACCESS_CODE_SEGMENT | GDT_ACCESS_CODE_READABLE, 
        GDT_FLAG_32BIT | GDT_FLAG_GRANULARITY_4K),

    // Kenrel 32-bit data segment
    GDT_ENTRY(0, 
        0xFFFFF, 
        GDT_ACCESS_PRESENT | GDT_ACCESS_RING0 | GDT_ACCESS_DATA_SEGMENT | GDT_ACCESS_DATA_WRITEABLE, 
        GDT_FLAG_32BIT | GDT_FLAG_GRANULARITY_4K)
};

GDTDescriptor Descriptor = { sizeof(GDT) - 1, GDT};

void __attribute__((cdecl)) GDTLoad(GDTDescriptor* descriptor, uint16_t codeSegment, uint16_t dataSegment);

void GDTInitialize() {
    GDTLoad(&Descriptor, GDT_CODE_SEGMENT, GDT_DATA_SEGMENT);
}