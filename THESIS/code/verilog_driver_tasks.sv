
// #########################################################################################
//
// Initialization task
// 
// #########################################################################################
task init;
	integer i;

	AVDD = 0; 	//Analog supply
	AVSS = 0; 	//Analog reference
	DVDD = 0;	//Digital supply
	DGND = 0; 	//Digital reference

	GEN = 0; //Global enable
	NEG_POL = 0; //Negative polarity enable
	AGND_INT_DISABLE = 0; //Internal AGND generator enable
	RST_L = 0; //Low active RST
	STB  = 0; //Data/addr/mode latch signal
	
	RISE = 0; //Rise time constant line

	/* Set data bus low */
	for(i = 0; i < DATABITS; i = i + 1) begin
		DATA[i] = 0;
	end

	/* Set pulse trigger and peak lines low */
	for(i = 0; i < CHANNELS; i = i + 1) begin
		TRIG[i] = 0;
		PEAK[i] = 0;
	end
endtask

// #########################################################################################
//
// Global Enable task
// 
// #########################################################################################
task gen(input state);
	if(state > 1 || state < 0) 
		state = 0;
	
	GEN = state;
endtask

// #########################################################################################
//
// Power on task
// 
// #########################################################################################
task power_on;

	#(TRF)
	AVDD = 3.3;
	DVDD = 3.3;	

endtask

// #########################################################################################
//b
// Set ADDR and MODE on the shared 8-bit bus
// 
// #########################################################################################
task set_addr_mode(input[ADDRBITS-1:0]addr, input[MODEBITS-1:0] mode);

	#(TRF)
	DATA[7:4] = addr;
	DATA[3:0] = mode;
	
endtask

// #########################################################################################
//
// Set the One Shot lockout enable
// 
// #########################################################################################
task set_lockout_en(input data);
	
	if(data > 1 || data < 0)
		data = 0;	

	#(TRF)
	DATA[5] = data;

endtask

// #########################################################################################
//
// Set the lockout mode (0 long; 1 short)
// 
// #########################################################################################
task set_lockout_mode(input data);
	
	if(data > 1 || data < 0)
		data = 0;	

	#(TRF)
	DATA[5] = data;

endtask

// #########################################################################################
//
// Set the One Shot lockout pulse width control voltage DAC.
// 
// #########################################################################################
task set_lockout_cv(input[LOCKOUTBITS-1:0] data);

	#(TRF)
	DATA[4:0] = data;

endtask

// #########################################################################################
//
// Set the One Shot pulse width control bus
// 
// #########################################################################################
task set_os_width(input[CVBITS-1:0] data);

	#(TRF)
	DATA[1:0] = data;

endtask

// #########################################################################################
//
// Set the Leading Edge descriminator threshold DAC
// 
// #########################################################################################
task set_le_dac(input[LEBITS-1:0] data);	

	#(TRF)
	DATA[LEBITS-1:0] = data;

endtask

// #########################################################################################
//
// Set the AGND trim bits
// 
// #########################################################################################
task set_trim(input[TRIMBITS-1:0] data);	

	#(TRF)
	DATA[4:2] = data;

endtask

// #########################################################################################
//
// Set the AGND_INT_DISABLE line (internal AGND generator disable)
// 
// #########################################################################################
task set_agnd_int_disable(input val);	

	if(val > 1 || val < 0)
		val = 0;

	#(TRF)
	AGND_INT_DISABLE = val;

endtask

// #########################################################################################
//
// Set the channel enable bit for a given channel
// 
// #########################################################################################
task set_channel_enable(input val);	
	
	if(val > 1 || val < 0)	
		val = 0;

	#(TRF)
	DATA[6] = val;

endtask


// #########################################################################################
//
// Set the test point select mux
// 
// #########################################################################################
task set_tp_mux(input[TPBITS-1:0] data);	

	#(TRF)
	DATA[6:4] = data;

endtask


// #########################################################################################
//
// Set the programmable capacitor bus
// 
// #########################################################################################
task set_prog_cap(input[SBITS-1:0] data);

	#(TRF)
	DATA[SBITS-1:0] = data;

endtask

// #########################################################################################
//
// Set the Nowlin Circuit mode (1 = fast, 0 = slow)
// 
// #########################################################################################
task set_nowlin_mode(input data);
	
	if(data > 1 || data < 0)
		data = 0;	

	#(TRF)
	DATA[7] = data;

endtask

// #########################################################################################
//
// Set the negative polarity bit
// 
// #########################################################################################
task set_neg_pol(input val);
	
	if(val > 1 || val < 0)
		val = 0;
	
	#(TRF)
	NEG_POL = val;

endtask

// #########################################################################################
//
// Set RST_L line
// 
// #########################################################################################
task set_rst_l(input val);
	
	if(val > 1 || val < 0)
		val = 0;
	
	#(TRF)
	RST_L = val;

endtask

// #########################################################################################
//
// Produces n clock cycles on the STB line
// 
// #########################################################################################
task pulse_stb(integer npulses);
	integer i;
	for(i = 0; i < npulses; i = i + 1) begin
		#(STBPERIOD/2.0)
		STB = 1;
		#(STBPERIOD/2.0)
		STB = 0;
	end	
endtask

// #########################################################################################
//
// Set STB line either high or low
// 
// #########################################################################################
task set_stb(input val);

	if(val > 1 || val < 0)
		val = 0;

	#(TRF)
	STB = val;	

endtask

// #########################################################################################
//
// Sets the exponential pulse peak line, and creates a trigger pulse to begin the output
// 
// #########################################################################################
task trigger_pulse(real level[CHANNELS-1:0], real delay[CHANNELS-1:0], real rise);
	integer i;
	begin
		fork
			begin
				RISE = EPSILON;
				#(TRF)
				RISE = rise;
				#(PW*2)
				RISE = rise + EPSILON;
				#(TRF)
				RISE = 0;
			end

			begin
				/* Create exp pulse peak voltage pulse */
				for(i = 0; i < CHANNELS; i = i + 1) begin
					PEAK[i] = EPSILON;
				end
				
				#(TRF)
				for(i = 0; i < CHANNELS; i = i + 1) begin
					PEAK[i] = level[i];
				end

				#(PW)
				for(i = 0; i < CHANNELS; i = i + 1) begin
					PEAK[i] = level[i] + EPSILON;
				end

				#(TRF)
				for(i = 0; i < CHANNELS; i = i + 1) begin
					PEAK[i] = 0;					
				end
			end

			/* Trigger an exp pulse on each channel if a peak voltage is set */
			begin
				if(level[0] != 0) begin
				#(delay[0]) TRIG[0] = 1;
				#(PW)	    TRIG[0] = 0;
				end
			end

			begin
				if(level[1] != 0) begin
				#(delay[1]) TRIG[1] = 1;
				#(PW)	    TRIG[1] = 0;
				end
			end

			begin
				if(level[2] != 0) begin
				#(delay[2]) TRIG[2] = 1;
				#(PW)	    TRIG[2] = 0;
				end
			end

			begin
				if(level[3] != 0) begin
				#(delay[3]) TRIG[3] = 1;
				#(PW)	    TRIG[3] = 0;
				end
			end

			begin
				if(level[4] != 0) begin
				#(delay[4]) TRIG[4] = 1;
				#(PW)	    TRIG[4] = 0;
				end
			end

			begin
				if(level[5] != 0) begin
				#(delay[5]) TRIG[5] = 1;
				#(PW)	    TRIG[5] = 0;
				end
			end

			begin
				if(level[6] != 0) begin
				#(delay[6]) TRIG[6] = 1;
				#(PW)	    TRIG[6] = 0;
				end
			end

			begin
				if(level[7] != 0) begin
				#(delay[7]) TRIG[7] = 1;
				#(PW)	    TRIG[7] = 0;
				end
			end

			begin
				if(level[8] != 0) begin
				#(delay[8]) TRIG[8] = 1;
				#(PW)	    TRIG[8] = 0;
				end
			end

			begin
				if(level[9] != 0) begin
				#(delay[9]) TRIG[9] = 1;
				#(PW)	    TRIG[9] = 0;
				end
			end

			begin
				if(level[10] != 0) begin
				#(delay[10]) TRIG[10] = 1;
				#(PW)	    TRIG[10] = 0;
				end
			end

			begin
				if(level[11] != 0) begin
				#(delay[11]) TRIG[11] = 1;
				#(PW)	    TRIG[11] = 0;
				end
			end

			begin
				if(level[12] != 0) begin
				#(delay[12]) TRIG[12] = 1;
				#(PW)	    TRIG[12] = 0;
				end
			end

			begin
				if(level[13] != 0) begin
				#(delay[13]) TRIG[13] = 1;
				#(PW)	    TRIG[13] = 0;
				end
			end

			begin
				if(level[14] != 0) begin
				#(delay[14]) TRIG[14] = 1;
				#(PW)	    TRIG[14] = 0;
				end
			end

			begin
				if(level[15] != 0) begin
				#(delay[15]) TRIG[15] = 1;
				#(PW)	    TRIG[15] = 0;
				end
			end
		join
	end
endtask
