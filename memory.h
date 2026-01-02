//
//  memory.h
//  
//
//  Created by Siri Wang on 12/25/25.
//

#include <cstdint>
#include <cstddef>

uint64_t read64(uint8_t* memory, size_t addr);
void write64(uint8_t* memory, size_t addr, uint64_t value);
uint32_t read32(uint8_t* memory, size_t addr);

