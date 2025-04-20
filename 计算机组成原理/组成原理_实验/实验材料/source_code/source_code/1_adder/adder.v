`timescale 1ns / 1ps
//*************************************************************************
//   > �ļ���: adder.v
//   > ����  ���ӷ�����ֱ��ʹ��"+"�����Զ����ÿ���ļӷ���
//   > ����  : LOONGSON
//   > ����  : 2016-04-14
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
