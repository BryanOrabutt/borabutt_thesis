/*	Pulse characteristics	*/
localparam PW = 100ns; 
localparam TRF = 1ns;
localparam TPD = 5ns;
localparam STBPERIOD = 1us; // time between stb pulses when creating pulse train

/*	Bit widths	*/
localparam CVBITS = 2;
localparam LOCKOUTBITS = 5;
localparam ADDRBITS = 4;
localparam DATABITS = 8;
localparam MODEBITS = 4;
localparam SBITS = 4;
localparam LEBITS = 6;
localparam TRIMBITS = 3;
localparam TPBITS = 3;
localparam CHANNELS = 16;

/*	Mode values 	*/
localparam PCAPMODE = 3'h0; //used for programmable capacitor
localparam TPMODE = 3'h0; //used for test point
localparam ONESHOTMODE = 3'h1; //used for oneshot
localparam TRIMMODE = 3'h1; //used for AGND trim
localparam LOCKOUTMODE = 3'h5; //used for lockout DAC
localparam DACMODE = 3'h6; //used for LE DAC
localparam COMMON = 4'h0; //address for common area devices

/*	TP MUX selectors	*/
localparam TPAVSS = 3'h0;
localparam TPLOCK = 3'h1;
localparam TPLE	= 3'h2;
localparam TPOSIN = 3'h3;
localparam TPOSOUT = 3'h5;
localparam TPZC = 3'h6;

/*	Simulation parameters	*/
localparam EPSILON = 1e-15;
localparam SIMDELAY = 225us;

/*	Event schedule	*/
localparam POWER = 5ns;
localparam CONTROL_SIGS = 1us;
localparam CONFIG_ALL = 5us;
localparam GEN_ON = 6ms;
localparam HIT_ALL = 9ms;
localparam TEST_LOCKOUT = 9001us;
localparam HIT_ALL_100us = 11ms;
localparam CONFIG_LONG = 12ms;
localparam GEN_OFF = 12ms;
localparam GEN_ON2 = 12.5ms;
localparam HIT_LONG = 13ms;
localparam GEN_OFF2 = 14ms;
localparam NEG_TEST = 15ms;
