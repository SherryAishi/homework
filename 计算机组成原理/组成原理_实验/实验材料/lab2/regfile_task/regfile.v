module regfile(
  input         clk,
  input  [ 4:0] raddr1,
  output [31:0] rdata1,
  input  [ 4:0] raddr2,
  output [31:0] rdata2,
  input         we,
  input  [ 4:0] waddr,
  input  [31:0] wdata
);
reg [31:0] rf[31:0];
// WRITE,利用waddr可以确定是32个寄存器中的哪一个，确定后将数据wdata传入到该寄存器中
always @(posedge clk) begin
    if (we) rf[waddr]<= wdata;
end
// READ OUT 1
assign rdata1 = (raddr1==5'b0) ? 32'b0 : rf[raddr1];
// READ OUT 2
assign rdata2 = (raddr2==5'b0) ? 32'b0 : rf[raddr2];
endmodule
