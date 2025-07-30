#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>  // Для exit()

uint32_t asm_mod(uint32_t a, uint32_t b);

uint32_t asm_mod(uint32_t a, uint32_t b) 
{
    if (b == 0) {
        fprintf(stderr, "Ошибка: деление на ноль!\n");
        exit(EXIT_FAILURE);
    }

    uint32_t result;
    __asm__ volatile (
        "xor %%edx, %%edx\n\t"     // edx = 0
        "divl %[divisor]\n\t"      // eax / divisor, остаток в edx
        "movl %%edx, %[res]\n\t"   // сохраняем остаток в result
        : [res] "=r" (result)      // выход
        : "a" (a), [divisor] "r" (b) // вход
        : "edx", "cc"
    );
    return result;
}

int main()
{
    uint32_t a = 0;
    uint32_t b = 0;
    printf("Введите первый операнд:");
    scanf("%u", &a);
    printf("Введите второй операнд:");
    scanf("%u", &b);
    uint32_t result = asm_mod(a, b);
    printf("остаток = %u\n", result);
    return 0;
}