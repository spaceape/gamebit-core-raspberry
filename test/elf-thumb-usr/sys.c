#include "sys.h"

int __aeabi_idiv(int lhs, int rhs)
{
    (void)lhs;
    (void)rhs;
    return 0;
}

unsigned int __aeabi_uidiv(unsigned int lhs, unsigned int rhs)
{
    (void)lhs;
    (void)rhs;
    return 0u;
}

int __aeabi_idivmod(int lhs, int rhs)
{
    (void)lhs;
    (void)rhs;
    return 0;
}

unsigned int __aeabi_uidivmod(unsigned int lhs, unsigned int rhs)
{
    (void)lhs;
    (void)rhs;
    return 0u;
}
