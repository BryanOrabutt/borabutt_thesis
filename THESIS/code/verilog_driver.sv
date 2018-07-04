`include "localparams.vh"

`define  NEGATIVE	

module verilog_driver(
	output reg [DATABITS-1:0] DATA,
	output reg [CHANNELS-1:0] TRIG,
	output reg NEG_POL,
	output reg AGND_INT_DISABLE,
	output reg RST_L,
	output reg STB,
	output reg GEN,
	output real AVDD,
	output real AVSS,
	output real DVDD,
	output real DGND,
	output real PEAK[CHANNELS-1:0],
	output real RISE
	);

	`include "verilog_driver_tasks.sv"
	
	integer i;
	reg gmode;
	real level[CHANNELS-1:0];
	real delay[CHANNELS-1:0];

	initial begin
		fork
			gmode = 1'b1;
			/* Initialize the chip */
			init;
		
			/* Apply power (AVDD, DVDD) */
			#(POWER) begin fork
				power_on;
			join end
			
			/* Set pin mapped control lines */
			#(CONTROL_SIGS) begin fork
				set_rst_l(1);
				set_neg_pol(0);
				set_agnd_int_disable(0);
			join end

			/* Cofigure all channels identically */
			#(CONFIG_ALL) begin fork
				set_addr_mode(4'h0, {gmode, PCAPMODE}); //system verilog for some reason does not set DATA correctly without this
				#10ns
				set_addr_mode(4'h0, {gmode, PCAPMODE}); //change addr to 0 and mode to select programmable capacitor register
				#150ns	
				set_stb(1); //register addr/mode
				#300ns	
				set_prog_cap(4'h1); //set programmable capacitor for 2ns rise
				#310ns
				set_tp_mux(TPLOCK); //set testpoint to see lockout pulse
				#320ns
				set_nowlin_mode(1); //set to short mode
				#450ns 	
				set_stb(0); //register data
				#600ns
				set_addr_mode(4'h0, {gmode, ONESHOTMODE}); //set mode to select oneshot register
				#750ns
				set_stb(1); 
				#900ns
				set_trim(3'h4); //set AGND trim to nominal
				#910ns
				set_lockout_mode(1); //short mode
				#920ns
				set_os_width(2'h2); //set oneshot width to 500 ns
				#1.05us
				set_stb(0); 
				#1.2us
				set_addr_mode(4'h0, {gmode, LOCKOUTMODE}); //set mode to select lockout dac register
				#1.35us
				set_stb(1);
				#1.5us
				set_lockout_cv(5'h01); //set lockout to 3.4 uS
				#1.51us
				set_lockout_en(0); //enable lockout
				#1.65us
				set_stb(0); 
				#1.8us
				set_addr_mode(4'h0, {gmode, DACMODE}); //set mode to select threshold DAC
				#1.95us
				set_stb(1);
				#2.1us
				set_le_dac(6'h2f); //set threshold value
				#2.11us
				set_channel_enable(1); //enable channel
				#2.25us
				set_stb(0);
				#2.5us
				set_addr_mode(4'h0, 4'h0); //set address for test point
			join end	

			/* Set global enable */
			#(GEN_ON) begin
				gen(1);
			end

			#(HIT_ALL) 
			begin
				/* Trigger the exp pules */
					for(i = 0; i < CHANNELS; i = i + 1) begin
						delay[i] = 100ns; 
						level[i] = (i == 0) ? 0.015:level[i-1]*1.36;
					end
				
					trigger_pulse(level, delay, 2e-9);
					
			end

			/* Fire shortly after HIT_ALL to test lockout */
			#(TEST_LOCKOUT)
			begin
				/* Trigger the exp pules */
					for(i = 0; i < CHANNELS; i = i + 1) begin
						delay[i] = 100ns; 
						level[i] = 0.15;
					end
				
					trigger_pulse(level, delay, 2e-9);
			end

			/* Hit all of the channels with the same pulses, and hit them again
			   100us later to test how quick DC offset recovery is */
			#(HIT_ALL_100us) begin fork
				/* Trigger the exp pules */
					for(i = 0; i < CHANNELS; i = i + 1) begin
						delay[i] = 100ns; 
						level[i] = (i == 0) ? 0.015:level[i-1]*1.36;
					end
				
					trigger_pulse(level, delay, 2e-9);
					#100us
					trigger_pulse(level, delay, 2e-9);

			join end

			#(GEN_OFF) begin
				gen(0);
			end

			/* Configure for long rise time constants */
			#(CONFIG_LONG) begin fork
				set_addr_mode(4'h0, {gmode, PCAPMODE}); 
				#10ns
				set_addr_mode(4'h0, {gmode, PCAPMODE});
				#150ns	
				set_stb(1);
				#300ns	
				set_prog_cap(4'hf); //set PCAP at maximum value (192 ns)
				#310ns
				set_tp_mux(TPLOCK);
				#320ns
				set_nowlin_mode(0); //set to long mode
				#450ns 	
				set_stb(0);
				#600ns
				set_addr_mode(4'h0, {gmode, ONESHOTMODE});
				#750ns
				set_stb(1);
				#900ns
				set_trim(3'h4);
				#910ns
				set_lockout_mode(0); //long mode
				#920ns
				set_os_width(2'h0);
				#1.05us
				set_stb(0);set_addr_mode(4'h0, {gmode, LOCKOUTMODE}); //set mode to select lockout dac register
				#1.2us
				set_stb(1);
				#1.35us
				set_lockout_cv(5'h01); //set lockout to 16.8 uS
				#1.36us
				set_lockout_en(0); //enable lockout
				#1.5us
				set_stb(0); 
				#1.65us
				set_addr_mode(4'h0, 4'h0); //set address for test point
			join end

			#(GEN_ON2) begin
				gen(1);
			end

			/* Hit channels with long rise time pulses */
			#(HIT_LONG) begin
				/* Trigger the exp pules */
					for(i = 0; i < CHANNELS; i = i + 1) begin
						delay[i] = 100ns; 
						level[i] = (i == 0) ? 0.015:level[i-1]*1.36;
					end
				
					trigger_pulse(level, delay, 192e-9);
				end

			`ifdef NEGATIVE

			#(GEN_OFF2) begin
				gen(0);
			end
			/* Configurate for negative pulses */
			#(NEG_TEST + CONFIG_ALL) begin fork
				set_addr_mode(4'h0, {gmode, PCAPMODE}); //system verilog for some reason does not set DATA correctly without this
				#10ns
				set_addr_mode(4'h0, {gmode, PCAPMODE}); //change addr to 0 and mode to select programmable capacitor register
				#150ns	
				set_stb(1); //register addr/mode
				#300ns	
				set_prog_cap(4'h1); //set programmable capacitor for 2ns rise
				#310ns
				set_tp_mux(TPLOCK); //set testpoint to see lockout pulse
				#320ns
				set_nowlin_mode(1); //set to fast mode
				#450ns 	
				set_stb(0); //register data
				#600ns
				set_addr_mode(4'h0, {gmode, DACMODE}); //set mode to select threshold DAC
				#750ns
				set_le_dac(6'h04); //set threshold value
				#760ns
				set_channel_enable(1); //enable channel
				#900ns
				set_stb(0);
				#1us
				set_addr_mode(4'h0, 4'h0); //set address for test point
			join end

			/* Enable control lines */
			#(NEG_TEST + 100us) begin fork
				gen(1);
				set_neg_pol(1);
			join end

			/* Hit with negative pulses 16ns rise */
			#(NEG_TEST + 500us) begin
				/* Trigger the exp pules */
					for(i = 0; i < CHANNELS; i = i + 1) begin
						delay[i] = 100ns; 
						level[i] = (i == 0) ? -0.015:level[i-1]*1.36;
					end
				
					trigger_pulse(level, delay, 2e-9);
			end

			/* Test lockout with negative pulses */
			#(NEG_TEST + 501us) begin
				/* Trigger the exp pules */
					for(i = 0; i < CHANNELS; i = i + 1) begin
						delay[i] = 100ns; 
						level[i] = -0.15;
					end
				
					trigger_pulse(level, delay, 2e-9);
			end

			/* Hit all of the channels with the same negative pulses, and hit them again
			   100us later to test how quick DC offset recovery is */
			#(NEG_TEST + 2.5ms) begin fork
				/* Trigger the exp pules */
					for(i = 0; i < CHANNELS; i = i + 1) begin
						delay[i] = 100ns; 
						level[i] = (i == 0) ? -0.015:level[i-1]*1.36;
					end
				
					trigger_pulse(level, delay, 2e-9);
					#100us
					trigger_pulse(level, delay, 2e-9);

			join end


			#(NEG_TEST + 4.5ms) begin fork
				gen(0);
			join end

			/* Configure for long negative pulses */
			#(NEG_TEST + 6.5ms) begin fork
				set_addr_mode(4'h0, {gmode, PCAPMODE}); 
				#10ns
				set_addr_mode(4'h0, {gmode, PCAPMODE});
				#150ns	
				set_stb(1);
				#300ns	
				set_prog_cap(4'hf); //set PCAP at maximum value (192 ns)
				#310ns
				set_tp_mux(TPLOCK);
				#320ns
				set_nowlin_mode(0); //set to slow mode
				#450ns 	
				set_stb(0);
				#500ns
				set_addr_mode(4'h0, 4'h0); //set address for TP
			join end

			#(NEG_TEST + 5ms) begin
				gen(1);
			end

			/* Hit channels with long negative pulses */
			#(NEG_TEST + 5.5ms) begin
				/* Trigger the exp pules */
					for(i = 0; i < CHANNELS; i = i + 1) begin
						delay[i] = 100ns; 
						level[i] = (i == 0) ? -0.015:level[i-1]*1.36;
					end
				
					trigger_pulse(level, delay, 192e-9);
			end

		`endif

		join
	end
endmodule
