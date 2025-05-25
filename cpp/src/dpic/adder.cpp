#include "../../include/dpic/adder.h"
#include "../../include/dpic/subadder.h"

static int tmp = 0;
void cpp_adder_i(int i1, int i2)
{
    subadder sa;
    sa.func(i1, i2, tmp);
}

void cpp_adder_process();

int cpp_adder_o1_size();
int cpp_adder_o1_pop()
{
    return tmp;
}
int cpp_adder_o2_size();
int cpp_adder_o2_pop();