module dut (
    input logic [31:0] a,
    input logic [31:0] b,
    output logic [31:0] sum
);
    always_comb begin
        sum = a + b;
    end
endmodule
