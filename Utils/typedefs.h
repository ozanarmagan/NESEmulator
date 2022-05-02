#ifndef TYPEDEFS_H
#define TYPEDEFS_H


namespace nesemulator
{
    class CPU;
    // Type declerations
    typedef uint8_t BYTE;
    typedef uint16_t ADDRESS;
    typedef uint16_t AUDIOINT;
    typedef uint32_t PIXEL;
    typedef uint32_t ADDRESS32;
    typedef uint64_t TIMER;
    typedef uint8_t FLAG;
    typedef uint32_t HERTZ;

    using OPEXEC = void;
    using OPEXEC_PTR = OPEXEC (CPU::*)(ADDRESS source);
    using ADDRESSING_MODE = ADDRESS(CPU::*)();
}

#endif 
