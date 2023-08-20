#include "print.h"

#include <stdarg.h>

#include "drivers/terminal.h"
#include "util.h"

void kprintf(const char *fmt, ...)
{
    va_list ap;
    char *p, *sval;
    int ival;
    unsigned int uval;
    double dval;
    char cval;

    va_start(ap, fmt);
    for (p = fmt; *p; p++) {
        if (*p != '%') {
            TerminalPutChar(*p);
            continue;
        }
        switch (*++p) {
        case 'd':
            ival = va_arg(ap, int);
            kprint(itoa(ival, 10));
            break;
        case 'f':
            dval = va_arg(ap, double);
            kprint(itoa((int)dval, 10));
            kputc('.');
            kprint(itoa((int)(dval * 1000000) % 1000000, 10));
            break;
        case 's':
            for (sval = va_arg(ap, char *); *sval; sval++)
                kputc(*sval);
            break;
        case 'c':
            cval = va_arg(ap, int);
            kputc(cval);
            break;
        case 'x':
            ival = va_arg(ap, int);
            kprintf("0x%s", itoa(ival, 16));
            break;
        default:
            kputc(*p);
            break;
        }
    }
}

void kprint(const char *str)
{
    TerminalWrite(str);
}

void kputc(char c)
{
    TerminalPutChar(c);
}
