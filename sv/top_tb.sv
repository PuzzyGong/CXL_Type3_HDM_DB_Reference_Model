import uvm_pkg::*;
`include "uvm_macros.svh"

module top_tb;

    import "DPI-C" function void cpp_adder_i(int i1, int i2);
    import "DPI-C" function int cpp_adder_o1_pop();
    
    logic [31:0] a, b, dut_sum;
    int ref_sum;

    dut u_dut (
        .a(a),
        .b(b),
        .sum(dut_sum)
    );

    int o1[100];
    int size;
    initial begin
        for (int i = 0; i < 500; i++) begin
            a = $urandom_range(0, 100);
            b = $urandom_range(0, 100);
            #1;
            cpp_adder_i(a, b);
            ref_sum = cpp_adder_o1_pop();

            $display("Test %0d: a=%0d, b=%0d | DUT=%0d, REF=%0d %s",
                    i, a, b, dut_sum, ref_sum,
                    (dut_sum == ref_sum) ? "PASS" : "FAIL");
        end
    end

    initial begin
        run_test("test");
    end
endmodule