`default_nettype none

module top(
  input wire clk,
  input wire GBACART_CS,
  input wire GBACART_RD,
  //input wire [7:0]  GBACART_AH,
  inout wire [15:0] GBACART_AD,

  input wire [1:0] din,
  input wire wclk,
  input wire write_en,

  output reg [5:0] buttons
);

reg  [15:0] gba_data_out;
wire [15:0] gba_addr_lo_in;
reg  [15:0] gba_addr_lo;
//wire [23:0] gba_addr;
//assign gba_addr = {GBACART_AH, gba_addr_lo};

reg [15:0] rom [0:609];
initial $readmemh("main.hex", rom);

reg  [1:0] vram [0:19200];
reg [14:0] waddr;
wire [1:0] dout;
assign dout = vram[gba_addr_lo[14:0]];

reg [15:0] sound;
reg [15:0] sound_temp;

always @(posedge wclk)
begin
  if (write_en)
    begin
      vram[waddr] <= din;
      waddr <= waddr + 1'b1; // increment address

      sound <= sound_temp; // latch sound data
    end
  else
    begin
      waddr <= 15'b000000000000000;

      sound_temp <= {sound_temp[13:0], din}; // shift-left register
    end
end

reg risingRD, fallingRD, fallingCS;
reg [1:3] resyncRD;
reg [1:3] resyncCS;

always @(posedge clk)
begin
  if (fallingRD)
    begin
      if (gba_addr_lo < 16'd610) gba_data_out = rom[gba_addr_lo[9:0]];
      else if (gba_addr_lo > 16'b0111_1111_1111_1111)
        begin
          gba_data_out[15:8] = (dout[0]) ? 8'hFF : 8'h00;
          gba_data_out[7:0]  = (dout[1]) ? 8'hFF : 8'h00;
        end
      else if (gba_addr_lo > 16'h7FF && gba_addr_lo < 16'hC00)
        begin
          gba_data_out = sound; // upper 4 bits volume / mute, lower 11 bits frequency
          buttons <= {gba_addr_lo[7:4], gba_addr_lo[1:0]}; // button states encoded into address
        end
    end
  if (risingRD) gba_addr_lo <= gba_addr_lo + 1'b1;
  else if (fallingCS) gba_addr_lo <= gba_addr_lo_in;

  // detect rising and falling edge(s)
  // (https://www.doulos.com/knowhow/fpga/synchronisation/)
  risingRD  <= resyncRD[2] & !resyncRD[3];
  fallingRD <= resyncRD[3] & !resyncRD[2];
  fallingCS <= resyncCS[3] & !resyncCS[2];

  // update history shifter(s)
  resyncRD <= {GBACART_RD, resyncRD[1:2]};
  resyncCS <= {GBACART_CS, resyncCS[1:2]};
end

// instantiate tristate IO
SB_IO #(
    .PIN_TYPE(6'b101001),
    .PULLUP(1'b0)
) gba_io[15:0] (
    .PACKAGE_PIN(GBACART_AD[15:0]),
    .OUTPUT_ENABLE((!GBACART_RD && !GBACART_CS)),
    .D_OUT_0(gba_data_out[15:0]),
    .D_IN_0(gba_addr_lo_in[15:0])
);

endmodule
