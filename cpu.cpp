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
    uint32_t value = read32(cpu.memory, cpu.rip);
    uint64_t temp = cpu.rax;
    cpu.rax += value;
    cpu.ZF = (cpu.rax == 0);
    cpu.CF = (cpu.rax < temp);
    cpu.SF = ((int64_t)cpu.rax < 0);
    cpu.rip += 4;
}

void subRaxImm32(CPU& cpu) {
    uint32_t value = read32(cpu.memory, cpu.rip);
    uint64_t temp = cpu.rax;
    cpu.rax -= value;
    cpu.ZF = (cpu.rax == 0);
    cpu.CF = (cpu.rax > temp);
    cpu.SF = ((int64_t)cpu.rax < 0);
    cpu.rip += 4;
}

void cmpRaxImm32(CPU& cpu) {
    uint32_t value = read32(cpu.memory, cpu.rip);
    uint64_t result = cpu.rax - value;
    cpu.ZF = (result == 0);
    cpu.SF = ((int64_t)result < 0);
    cpu.CF = (cpu.rax < value);
    cpu.rip += 4;
}

void incRax(CPU& cpu) {
    cpu.rax+=1;
    cpu.ZF = (cpu.rax == 0);
    cpu.SF = ((int64_t)cpu.rax < 0);
}

void pushRax(CPU& cpu) {
    cpu.rsp -= 8;
    write64(cpu.memory, cpu.rsp, cpu.rax);
    if ((cpu.rsp < 0) || (cpu.rsp + 8 > cpu.memory_size)) {
        printf("ERROR: stack out of bounds\n");
        cpu.halted = true;
        return;
    }
}

void popRax(CPU& cpu) {
    cpu.rax = read64(cpu.memory, cpu.rsp);
    cpu.rsp += 8;
    
    if (cpu.rsp > cpu.memory_size) {
        printf("ERROR: stack out of bounds\n");
        cpu.halted = true;
        return;
    }
}

void jeRel8(CPU& cpu) {
    int8_t offset = (int8_t)cpu.memory[cpu.rip];
    cpu.rip += 1;
    
    if (cpu.ZF) {
        uint64_t target = cpu.rip + offset;
        if (target >= cpu.memory_size) {
            printf("ERROR: JE jump out of bounds (target = %llu)\n", target);
            cpu.halted = true;
            return;
        }
        cpu.rip = target;
    }
}

void jneRel8(CPU& cpu) {
    int8_t offset = (int8_t)cpu.memory[cpu.rip];
    cpu.rip += 1;
    if (!cpu.ZF) {
        uint64_t target = cpu.rip + offset;
        if (target >= cpu.memory_size) {
            printf("ERROR: JNE jump out of bounds (target = %llu)\n", target);
            cpu.halted = true;
            return;
        }
        cpu.rip = target;
    }
}

void callRel32(CPU& cpu) {
    int32_t offset = (int32_t)read32(cpu.memory, cpu.rip);
    cpu.rip += 4;
    push64(cpu, cpu.rip);
    int64_t target = (int64_t)cpu.rip + offset;
    if (target < 0 || (uint64_t)target >= cpu.memory_size) {
        printf("ERROR: CALL target out of bounds (target = %llu)\n", target);
        cpu.halted = true;
        return;
    }
    cpu.rip = (uint64_t)target;
}

void ret(CPU& cpu) {
    uint64_t returnAddr = pop64(cpu);
    if (cpu.halted) {
        return;
    }
    if (returnAddr >= cpu.memory_size) {
        printf("RET address out of bounds (%llu)\n", returnAddr);
        cpu.halted = true;
        return;
    }
    cpu.rip = returnAddr;
}

// helper functions
void push64(CPU& cpu, uint64_t value) {
    cpu.rsp -= 8;
    if (cpu.rsp > cpu.memory_size) {
        printf("ERROR: Stack out of bounds\n");
        cpu.halted = true;
        return;
    }
    write64(cpu.memory, cpu.rsp, value);
}

uint64_t pop64(CPU& cpu){
    if (cpu.rsp + 8 > cpu.memory_size) {
        printf("ERROR: Stack out of bounds\n");
        cpu.halted = true;
        return -1;
    }
    uint64_t value = read64(cpu.memory, cpu.rsp);
    cpu.rsp += 8;
    return value;
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
        if ((size_t)cpu.rip+1 > cpu.memory_size) {
            printf("Error: RIP out of bounds. (RIP + %llu, MEM = %zu)\n", cpu.rip, cpu.memory_size);
            cpu.halted = true;
            return;
        }
        uint8_t opcode = cpu.memory[cpu.rip++];
        
        switch(opcode) {
            case MOV_RAX_IMM64:
                movRaxImm64(cpu);
                break;
            case ADD_RAX_IMM32:
                addRaxImm32(cpu);
                break;
            case CMP_RAX_IMM32:
                cmpRaxImm32(cpu);
                break;
            case INC_RAX:
                incRax(cpu);
                break;
            case (JE_REL8):
                jeRel8(cpu);
                break;
            case CALL_REL32:
                callRel32(cpu);
            case RET:
                ret(cpu);
            case PUSH_RAX:
                pushRax(cpu);
                break;
            case POP_RAX:
                popRax(cpu);
                break;
            case HLT:
                cpu.halted = true;
                break;
            default:
                printf("unknown opcode 0x%X at RIP=%llu\n", opcode, cpu.rip-1);
                cpu.halted = true;
                break;
                
            }
        dumpRegisters(cpu);
    }
}

