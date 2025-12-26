//
//  cpu.cpp
//  
//
//  Created by Siri Wang on 12/25/25.
//

#include "cpu.h"
#include "memory.h"
#include <cstdint>
#include <cstdio>

void movRaxImm64(CPU& cpu) {
    uint64_t value = read64(cpu.memory, cpu.rip);
    cpu.rax = value;
    cpu.rip += 8;
}

void addRaxImm32(CPU& cpu) {
    uint32_t value = *(uint32_t*)(cpu.memory + cpu.rip);
    cpu.rax += value;
    cpu.ZF = (cpu.rax == 0);
    cpu.CF = (cpu.rax < value);
    cpu.SF = ((int64_t)cpu.rax < 0);
    cpu.rip += 4;
}

void cmpRaxImm32(CPU& cpu) {
    uint32_t value = *(uint32_t*)(cpu.memory + cpu.rip);
    uint64_t = cpu.rax - value;
    cpu.ZF = (result == 0);
    cpu.SF = ((int64_t)result < 0);
    cpu.CF = (cpu.rax < value);
    cpu.rip += 4;
}

void jeRel8(CPU& cpu) {
    int8_t offset = *(int8_t*)(cpu.memory + cpu.rip);
    cpu.rip+=1;
    if (cpu.ZF) {
        cpu.rip += offset;
    }
}

void dumpRegisters(const CPU& cpu) {
    printf("RAX=%llu RBX=%llu RCX=%llu RDX=%llu\n",
           cpu.rax, cpu.rbx, cpu.rcx, cpu.rdx);
    printf("RSP=%llu RBP=%llu RIP=%llu\n",
           cpu.rsp, cpu.rbp, cpu.rip);
    printf("ZF=%d SF=%d CF=%d\n", cpu.ZF, cpu.SF, cpu.CF);
    printf("Halted=%d\n\n", cpu.halted);
}

void runCPU(CPU& cpu) {
    while (!cpu.halted) {
        uint8_t opcode = cpu.memory[cpu.rip++];
        
        switch(opcode) {
            case MOV_RAX_IMM64:
                movRaxImm64(cpu);
                break;
            case ADD_RAX_IMM32:
                addRaxImm32(cpu);
                break;
            case CMP_RAX_IMM32:
                cmpRaxImm32(cpu)
                break;
            case (JE_REL8):
                jeRel8(cpu)
                break;
            case PUSH_RAX:
                break;
            case POP_RAX:
                break;
            case HLT:
                cpu.halted = true;
                break;
            default:
                printf("unknown opcode 0x%X at RIP=%llu\n", opcode, cpu.rip-1);
                cpu.halted = true;
                break;
                
            }
        dumpRegisters(cpu)
        }
    }
}

