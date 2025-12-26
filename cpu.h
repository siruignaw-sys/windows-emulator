//
//  cpu.h
//  
//
//  Created by Siri Wang on 12/25/25.
//
#pragma once
#include <cstdint>

struct CPU
{
    // general purpose registers
    // rax - main work, rbx - storage, rcx - counter, rdx - extra data
    uint64_t rax, rbx, rcx, rdx;
    
    // stack registers
    // rsp - top of stack, rbp - stack base
    uint64_t rsp, rbp;
    
    // control registers
    // rip - instruction pointer: holds next instruction
    uint64_t rip;
    
    // flags (zero, carry, sign)
    bool ZF;
    bool CF;
    bool SF;
    
    // running state
    bool halted;
    
    // memory pointer
    uint8_t* memory;
    size_t memory_size;
};

//opcodes
const uint8_t MOV_RAX_IMM64 = 0xb8;
const uint8_t ADD_RAX_IMM32 = 0x05;
const uint8_t SUB_RAX_IMM32 = 0x2D;
const uint8_t CMP_RAX_IMM32 = 0x3D;

const uint8_t MOV_RBX_IMM64 = 0xBB;

const uint8_t MOV_RCX_IMM64 = 0xB9;

const uint8_t MOV_RDX_IMM64 = 0xBA;

const uint8_t JE_REL8 = 0x74;
const uint8_t JNE_REL8 = 0x75;
const uint8_t JMP_REL8 = 0xEB;

const uint8_t PUSH_RAX = 0x50;
const uint8_t POP_RAX = 0x58;
const uint8_t CALL_REL32 = 0xE8;
const uint8_t RET = 0xC3;

const uint8_t HLT = 0xF4;


//helper method
void dumpRegisters(const CPU& cpu);
void runCPU(CPU& cpu);
