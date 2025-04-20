`timescale 1ns / 1ps   //仿真单位时间为1ns，精度为1ps
module testbench;

    // Inputs
    reg [31:0] a;
    reg [31:0] b;

    // Outputs
    wire [31:0] result;
    wire carry_out;
    // Instantiate the Unit Under Test (UUT)
    adder uut (
        .a(a),
        .b(b),
        .result(result),
        .carry_out(carry_out) 
    );
    initial begin
        // Initialize Inputs
        a=32'b0;
        b=32'b0;
        #100;
     end    
     always #10 a = $random;  
     always #10 b = $random;  
endmodule

