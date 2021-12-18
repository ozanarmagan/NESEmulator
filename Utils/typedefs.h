#ifndef TYPEDEFS_H
#define TYPEDEFS_H
#define CARRY_BIT 0
#define ZERO_BIT 1
#define INTERRUPT_DISABLE_BIT 2
#define DECIMAL_MODE_BIT 3
#define BREAK_BIT 5
#define OVERFLOW_BIT 6
#define NEGATIVE_BIT 7


#include <iostream>

using std::string;

typedef uint8_t BYTE;
typedef uint16_t ADDRESS;
typedef uint8_t FLAG;
#endif