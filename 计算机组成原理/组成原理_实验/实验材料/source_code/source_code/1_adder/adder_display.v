//*************************************************************************
//   > �ļ���: adder_display.v
//   > ����  ���ӷ�����ʾģ�飬����FPGA���ϵ�IO�ӿںʹ�����
//   > ����  : LOONGSON
//   > ����  : 2016-04-14
//*************************************************************************
module adder_display(
    //ʱ���븴λ�ź�
    input clk,
    input resetn,    //��׺"n"����͵�ƽ��Ч

    //���뿪�أ�����ѡ���������Ͳ���cin
    input  input_sel, //0:����Ϊ������a;1:�������b
    output  led_cout,
    
    //��������ؽӿڣ�����Ҫ����
    output lcd_rst,
    output lcd_cs,
    output lcd_rs,
    output lcd_wr,
    output lcd_rd,
    inout[15:0] lcd_data_io,
    output lcd_bl_ctr,
    inout ct_int,
    inout ct_sda,
    output ct_scl,
    output ct_rstn
    );

//-----{���üӷ�ģ��}begin
    reg  [31:0] adder_a;
    reg  [31:0] adder_b;
    wire [31:0] adder_result  ;
    wire [1:0]  adder_carry_out;
    adder adder_module(
        .a(adder_a),
        .b(adder_b),
        .result(adder_result  ),
        .carry_out(adder_carry_out    )
    );
     assign led_out  = adder_carry_out;
//-----{���üӷ�ģ��}end

//---------------------{���ô�����ģ��}begin--------------------//
//-----{ʵ����������}begin
//��С�ڲ���Ҫ����
    reg         display_valid;
    reg  [39:0] display_name;
    reg  [31:0] display_value;
    wire [5 :0] display_number;
    wire        input_valid;
    wire [31:0] input_value;

    lcd_module lcd_module(
        .clk            (clk           ),   //10Mhz
        .resetn         (resetn        ),

        //���ô������Ľӿ�
        .display_valid  (display_valid ),
        .display_name   (display_name  ),
        .display_value  (display_value ),
        .display_number (display_number),
        .input_valid    (input_valid   ),
        .input_value    (input_value   ),

        //lcd��������ؽӿڣ�����Ҫ����
        .lcd_rst        (lcd_rst       ),
        .lcd_cs         (lcd_cs        ),
        .lcd_rs         (lcd_rs        ),
        .lcd_wr         (lcd_wr        ),
        .lcd_rd         (lcd_rd        ),
        .lcd_data_io    (lcd_data_io   ),
        .lcd_bl_ctr     (lcd_bl_ctr    ),
        .ct_int         (ct_int        ),
        .ct_sda         (ct_sda        ),
        .ct_scl         (ct_scl        ),
        .ct_rstn        (ct_rstn       )
    ); 
//-----{ʵ����������}end

//-----{�Ӵ�������ȡ����}begin
//����ʵ����Ҫ��������޸Ĵ�С�ڣ�
//�����ÿһ���������룬��д����һ��always��
    //��input_selΪ0ʱ����ʾ������Ϊ������a
    always @(posedge clk)
    begin
        if (!resetn)
        begin
            adder_a <= 32'd0;
        end
        else if (input_valid && input_sel==0)
        begin
            adder_a <= input_value;
        end
    end
    
    //��input_selΪ1ʱ����ʾ������Ϊ����b
    always @(posedge clk)
    begin
        if (!resetn)
        begin
            adder_b <= 32'd0;
        end
        else if (input_valid && input_sel==1)
        begin
            adder_b <= input_value;
        end
    end
    
 
//-----{�Ӵ�������ȡ����}end

//-----{�������������ʾ}begin
//������Ҫ��ʾ�����޸Ĵ�С�ڣ�
//�������Ϲ���44����ʾ���򣬿���ʾ44��32λ����
//44����ʾ�����1��ʼ��ţ����Ϊ1~44��
    always @(posedge clk)
    begin
        case(display_number)
            6'd1 :
            begin
                display_valid <= 1'b1;
                display_name  <= "A";
                display_value <= adder_a;
            end
            6'd2 :
            begin
                display_valid <= 1'b1;
                display_name  <= "B";
                display_value <= adder_b;
            end          
            6'd3 :
            begin
                display_valid <= 1'b1;
                display_name  <= "RESUL";
                display_value <= adder_result;
            end
            default :
            begin
                display_valid <= 1'b0;
                display_name  <= 40'd0;
                display_value <= 32'd0;
            end
        endcase
    end
//-----{�������������ʾ}end
//----------------------{���ô�����ģ��}end---------------------//
endmodule
