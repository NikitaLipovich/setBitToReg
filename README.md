# setBitToReg
The solution to the first problem from the interview is in the main.с

# Explanation
When I came to the decision during the interview, I experienced two logical difficulties:
1. I forgot the bitwise not operator as a symbol (~) in the syntax and therefore thought about how to use xor (exclusive or).
2. I thought that I could do this with xor, because recently, for preparation purposes, I wrote two implementations of the function and a solution using exclusive and I really liked it because of the speed of execution.

# 2. The task was formulated as follows:
There is some kind of control register. I write a value into it and if the value is not written to all the bits, identify the bits in which the writing did not occur. Let's say because some bits are read only. But I don’t know in advance and must find these bits with code.
## Part 1: Typical solution
```
#include <stdio.h>

void check_bits(unsigned int var) {
    unsigned int compare_value = 19; // value for compare
    unsigned int mask = 1; // mask for check bits
    int bit_position = 0;

    printf("Comparing variable %u with value %u:\n", var, compare_value);

    const unsigned int count_bit = sizeof(compare_value) * 8;
    
    printf("%u\n", count_bit);
    
    
    for(unsigned int mask = 1, bit = 0;  bit < count_bit; mask <<= 1, bit++){
        if( (var & mask) != (compare_value & mask) )
            printf("The bit is different than expected. The write did not occur in the bit: %d\n", bit);
    }
}

int main() {
    unsigned int variable = 25; // Пример переменной для проверки

    check_bits(variable);

    return 0;
}
```
## Part 2: Fast solution which i like
```
#include <stdio.h>

void check_bits(unsigned int var) {
    unsigned int compare_value = 19;

    printf(""Comparing variable %u with value %u:\n", var, compare_value);
    
    for(unsigned int xor_res = var ^ compare_value, bit_pos = 0; xor_res != 0; xor_res >>= 1, bit_pos++ )
        if (xor_res & 1)
            printf("The bit is different than expected. The write did not occur in the bit: %d\n", bit_pos);
        
}

int main() {
    unsigned int variable = 25; // Пример переменной для проверки

    check_bits(variable);

    return 0;
}
```

3. Why did I forget the bitwise not?
3.1. When I work under a MCU (stm32f*, infineon xe167, pic, etc), for example Cortex-m. I usually write macro functions
```
   #define beforeSomphing(a,b) ~(a&b) and use macros.
```
3.2. When I was parsing a lot of packets from the CAN network and applying masks under SOC imx6 or Nvidia Jetson using C++ Qt framework, for routine operations it was enough for me to simply multiply by the mask which I collected with the hex value 0xFA for example. And I simply added the installation bit I needed.

Therefore, when I realized that in the first part I partially wrote rubbish, I had to implement this task as well.

## Solution
I implemented two functions:
```
uint16_t setBitToRegImp1(uint16_t reg_number, uint8_t bit_number, uint8_t set_reset, uint16_t *base );
uint16_t setBitToRegImp2(uint16_t reg_number, uint8_t bit_number, uint8_t set_reset, uint16_t *base );
```
base is passed by pointer so that I can compile and check.

The setBitToRegImp2 function will work faster because if it sets a value, it only needs to do one operation.
```
 *reg |= bit_pos.
```

When the function is setBitToRegImp1, the same function will always do 3 operations.
```
 *reg &= mask_to_reset;
 *reg |= set_reset << bit_number;
```
