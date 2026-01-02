//
//  main.cpp
//  
//
//  Created by Siri Wang on 12/25/25.
//
#include "cpu.h"
#include "memory.h"
#include <cstdint>
#include <cstdlib>
#include <cstddef>
#include <cstdio>


int main() {
    uint8_t program[] = {
        MOV_RAX_IMM64, 0x01,0,0,0,0,0,0,0, // RAX = 1
        CALL_REL32, 0x05,0,0,0,             // call to +5 bytes
        INC_RAX,                             // will run after return
        HLT,                                 // stop
        INC_RAX,                             // function body: RAX++
        RET                                  // return
    };
    
    CPU cpu = {};
    cpu.rip = 0;
    cpu.halted = false;

    cpu.memory_size = sizeof(program) + 1024;
    cpu.memory = (uint8_t*)malloc(cpu.memory_size + 1024);
    cpu.rsp = cpu.memory_size - 8;
    
    for (size_t i = 0; i < cpu.memory_size; i++) {
        cpu.memory[i] = program[i];
    }
    
    runCPU(cpu);
    
    free(cpu.memory);
    return 0;
}
