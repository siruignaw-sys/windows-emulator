//
//  main.cpp
//  
//
//  Created by Siri Wang on 12/25/25.
//
#include "cpu.h"
#include "memory.h"
#include <stdint>
#include <stdlib>



int main() {
    uint8_t program[] = {
        MOV_RAX_IMM64, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        ADD_RAX_IMM32, 0x03, 0x00, 0x00, 0x00,
        CMP_RAX_IMM32, 0x08, 0x00, 0x00, 0x00,
        JE_REL8, 0x02,
        HLT
    }
    
    CPU cpu = {};
    cpu.rip = 0;
    cpu.rsp = 1024;
    cpu.halted = false;

    cpu.memory_size = sizeof(program);
    cpu.memory = (uint8_t*)malloc(cpu.memory_size);
    cpu.rsp = cpu.memory_size;
    
    for (size_t i = 0; i < cpu.memory_size; i++) {
        cpu.memory[i] = program[i];
    }
    
    runCPU(cpu);
    return 0;
}
