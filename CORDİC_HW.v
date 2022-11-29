`timescale 1ns / 1ps
module deneme(input clk,CORDIC_Valid_out,input [31:0] sayi1,sayi2,input [23:0]CORDIC_in,output reg [31:0] CORDIC_out,SONUC=32'd0,
output reg CORDIC_Valid_in);

    reg [2:0]state=0,stateNext;
    
    reg [31:0]op1=0;
    reg [31:0]op2=0;
    reg [23:0] araDeger=0;
    
    always@(posedge clk) begin
		state <= stateNext;
	end
    
    always@(*) begin
        stateNext=state;
        
        case(state)
        
            0:begin
                CORDIC_out=0;
                op1=((sayi1*sayi1)+(sayi2*sayi2));
                CORDIC_Valid_in=1;
                stateNext=1;
                
            end
            1:begin
                CORDIC_out=op1;
                stateNext=2;
                                 
            end
            2:begin
                CORDIC_Valid_in=0;
                op1=0;
                CORDIC_out=0;
                if(CORDIC_Valid_out)begin//CORDIC_in!=0
                   araDeger=CORDIC_in;
                   stateNext=3;
                end    
            end
            3:begin
                 op2=(araDeger+(sayi1*sayi2));
                 CORDIC_Valid_in=1; 
                 stateNext=4;                  
            end
            4:begin
                 CORDIC_out=op2;
                 stateNext=5;                         
            end
            5:begin
                CORDIC_Valid_in=0;
                op2=0;
                CORDIC_out=0;
                if(CORDIC_Valid_out)begin
                   SONUC=CORDIC_in;
                   stateNext=0;
                end    
            end
            
        endcase
        
    end


endmodule
