#ifndef ADDER_H
#define ADDER_H

extern "C" {
    void cpp_adder_i(int i1, int i2);

    void cpp_adder_process();
    
    int cpp_adder_o1_size();
    int cpp_adder_o1_pop();
    int cpp_adder_o2_size();
    int cpp_adder_o2_pop();
}

#endif
