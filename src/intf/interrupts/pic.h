#pragma once

#include <stdint.h>

void PIC_Configure(uint8_t offsetPic1, uint8_t offsetPic2);
void PIC_Mask(int irq);
void PIC_Unmask(int irq);
void PIC_Disable();
void PIC_SendEndOfInterrupt(int irq);
uint16_t PIC_GetIRQRequestRegister();
uint16_t PIC_GetInServiceRegister();