#include "util.h"

#include <stddef.h>
#include <stdbool.h>

char *itoa(int value, int base)
{
    static char buffer[32] = {0};
    int n = 0;
    bool is_negative = false;

    if (value == 0) {
        buffer[n++] = '0';
        buffer[n] = '\0';
        return buffer;
    }

    if (value < 0 && base == 10) {
        is_negative = true;
        value = -value;
    }

    while (value != 0) {
        int remainder = value % base;
        buffer[n++] = (remainder > 9) ? (remainder - 10) + 'a' : remainder + '0';
        value /= base;
    }

    if (is_negative) {
        buffer[n++] = '-';
    }

    buffer[n] = '\0';

    for (int i = 0, j = n - 1; i < j; i++, j--) {
        char temp = buffer[i];
        buffer[i] = buffer[j];
        buffer[j] = temp;
    }

    return buffer;
}
