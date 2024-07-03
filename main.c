#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>

#define SET     1
#define RESET   0

uint16_t setBitToRegImp1(uint16_t reg_number, uint8_t bit_number, uint8_t set_reset, uint16_t *base );
uint16_t setBitToRegImp2(uint16_t reg_number, uint8_t bit_number, uint8_t set_reset, uint16_t *base );

void run_tests(){
    uint16_t *memory_page;
    size_t elements_count = 4096 / sizeof(uint16_t); // 4096 byte = 4 kB
    memory_page = (uint16_t *) malloc(elements_count * sizeof(uint16_t));
    
    uint16_t *base = memory_page;
    
    uint16_t *reg1 = base;
    *reg1 = 0xFFFF;
    
    uint16_t last_state = setBitToRegImp1(0, 1, SET, base);
    assert(last_state == 1);
    assert((*reg1 & (0x1 << 1)) >> 1 == SET);
    assert(*reg1 == 0xFFFF);
    
    last_state = setBitToRegImp1(0, 1, RESET, base);
    assert(last_state == SET);
    assert((*reg1 & (0x1 << 1)) >> 1 == RESET);
    assert(*reg1 == 0xFFFD);//  1111 1111 1111 1101
    printf("Tests for setBitToRegImp1: Passed\n");
    
    uint16_t *reg2 = base + 1;
    *reg2 = 0x0000;
    
    last_state = setBitToRegImp2(1, 3, SET, base);
    assert(last_state == 0);
    assert(((*reg2 >> 3) & 0x1) == SET);
    assert(*reg2 == 0x8);//     1000
    
    last_state = setBitToRegImp2(1, 3, RESET, base);
    assert(last_state == SET);
    assert(((*reg2 >> 3) & 0x1) == RESET);
    assert(*reg2 == 0x0);
    printf("Tests for setBitToRegImp2: Passed\n");
    
    free(memory_page);
}

uint16_t setBitToRegImp1(uint16_t reg_number, uint8_t bit_number, uint8_t set_reset, uint16_t *base ){
    uint16_t *reg = base + reg_number;
    
    uint16_t bit_pos            = 0x1 << bit_number;
    uint16_t mask_to_reset      = ~bit_pos;
    // uint16_t last_state         = ((*reg) >> bit_number) & 0x1;
    uint16_t last_state         = (*reg & bit_pos) >> bit_number;
    
    *reg                        &= mask_to_reset;
    *reg                        |= set_reset << bit_number;
    return last_state;
}

uint16_t setBitToRegImp2(uint16_t reg_number, uint8_t bit_number, uint8_t set_reset, uint16_t *base ){
    uint16_t *reg = base + reg_number;
    
    uint16_t last_state         = (*reg >> bit_number) & 0x1;
    uint16_t bit_pos            = 0x1 << bit_number;
    
    set_reset   ? *reg |= bit_pos
                : *reg &= ~bit_pos;
        
    return last_state;
}

int main()
{
    run_tests();
    return 0;
}
