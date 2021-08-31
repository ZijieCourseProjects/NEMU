#include "stdio.h"
#include "FLOAT.h"
#include "lib-common/FLOAT/FLOAT.c"
#include "trap.h"

int main(int argc, char const *argv[])
{
    F_div_F(int2F(-3),int2F(2));
    return 0;
}
