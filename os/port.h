#ifndef __PORT_H__
#define __PORT_H__

#include "types.h"

class Port
{
protected:
    uint16_t portnumber;
    Port(uint16_t portnumber);
    ~Port();
};

class Port8Bit : public Port
{
public:
    uint16_t portnumber;
    Port8Bit (uint16_t portnumber);
    ~Port8Bit ();
    virtual void Write(uint8_t data);
    virtual uint8_t Read();
};

class Port8BitSlow : public Port8Bit
{
public:
    uint16_t portnumber;
    Port8BitSlow (uint16_t portnumber);
    ~Port8BitSlow ();
    virtual void Write(uint8_t data);
};

class Port16Bit : public Port
{
public:
    uint16_t portnumber;
    Port16Bit (uint16_t portnumber);
    ~Port16Bit ();
    virtual void Write(uint16_t data);
    virtual uint16_t Read();
};

class Port32Bit : public Port
{
public:
    uint16_t portnumber;
    Port32Bit (uint16_t portnumber);
    ~Port32Bit ();
    virtual void Write(uint32_t data);
    virtual uint32_t Read();
};


#endif /* end of include guard: __PORT_H__ */
