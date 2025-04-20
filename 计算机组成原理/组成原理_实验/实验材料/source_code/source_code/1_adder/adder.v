`timescale 1ns / 1ps
//*************************************************************************
//   > 文件名: adder.v
//   > 描述  ：加法器，直接使用"+"，会自动调用库里的加法器
//   > 作者  : LOONGSON
//   > 日期  : 2016-04-14
//*************************************************************************
module adder(
    input  [31:0] a,  
    input  [31:0] b,  
    output [31:0] result,  
    output carry_out  
    );
      wire [31:0] b_comp = ~b + 1;  
      assign {carry_out, result} = a + b_comp; 

endmodule
