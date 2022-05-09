#ifndef __INTERRUPTS_H__
#define __INTERRUPTS_H__

#include <types.h>

class InterruptManager
{
public:
    static uint32_t handleInterrupt(uint8_t interruptNumber, uint32_t esp);

private:
};


#endif /* end of include guard: __INTERRUPTS_H__ */
