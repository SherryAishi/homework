
module show_sw (
    input             clk,          
    input             resetn,    
    input      [3 :0] switch,    //input
    output     [7 :0] num_csn,   //new value   
    output     [6 :0] num_a_g,      
    output     [3 :0] led        //previous value
);

reg [3:0] show_data;
reg [3:0] show_data_r;
reg [3:0] prev_data;

//show_data中储存着与拨码开关相反的值
always @(posedge clk)
begin
     show_data <= ~switch;
end

always @(posedge clk)
begin
    show_data_r <= show_data;
end
//previous value
always @(posedge clk)
begin
    if(!resetn)
    begin
        prev_data <= 4'd0;
    end
    else if(show_data_r != show_data)
    begin
        prev_data <= show_data_r;
    end
end

//show led: previous value
assign led = ~prev_data;

//show number: new value
show_num u_show_num(
        .clk        (clk      ),
        .resetn     (resetn   ),
        .show_data  (show_data),
        .num_csn    (num_csn  ),
        .num_a_g    (num_a_g  )
);
endmodule


//---------------------------{digital number}begin-----------------------//
//这个模块的作用为：获取show_data的值，在某个数码管上显示出数据(0-9)
module show_num (
    input             clk,          
    input             resetn,     
    input      [3 :0] show_data,
    output     [7 :0] num_csn,      
    output reg [6 :0] num_a_g      
);
//选定某个特定的数码管
assign num_csn = 8'b0111_1111;

//nxt_a_g的作用是进行数码管数据的更新
wire [6:0] nxt_a_g;

always @(posedge clk)
begin
    if (!resetn)
    begin
        num_a_g <= 7'b0000000;
    end
    else
    begin
        num_a_g <= nxt_a_g;
    end
end

//keep unchange if show_data>=10
wire [6:0] keep_a_g;
assign     keep_a_g = num_a_g;

assign nxt_a_g = show_data==4'd0 ? 7'b1111110 :   //0
                 show_data==4'd1 ? 7'b0110000 :   //1
                 show_data==4'd2 ? 7'b1101101 :   //2
                 show_data==4'd3 ? 7'b1111001 :   //3
                 show_data==4'd4 ? 7'b0110011 :   //4
                 show_data==4'd5 ? 7'b1011011 :   //5
                 show_data==4'd6 ? 7'b1011111 :   //6
                 show_data==4'd7 ? 7'b1110000 :   //7
                 show_data==4'd8 ? 7'b1111111 :   //8
                 show_data==4'd9 ? 7'b1111011 :   //9
                                   keep_a_g   ;
endmodule
//----------------------------{digital number}end------------------------//
