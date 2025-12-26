//
//  memory.cpp
//  
//
//  Created by Siri Wang on 12/25/25.
//
#include "memory.h"
#include <cstdint>

uint64_t read64(uint8_t* memory, size_t addr) {
    for (int i = 0; i < 8; i++) {
        value |= ((uint64_t)memory[addr+i]) << (8*i);
    }
    return value;
}

void write64(uint8_t* memory, size_t addr, uint64_t value) {
    for (int i = 0; i < 8; i++) {
        memory[addr + i] = (value >> (8*i)) & 0xFF;
    }
}
