#!/usr/bin/python
#
#  GLE: 6 June 2017
#
# Fixed several bugs!!!
# It now finds floating point numbers correctly
# Also fixed the bug tha Po discovered
#
# Python script to convert vcd file to a series of pwl files
#
#
# This script will read and parse a VCD (Value Change Dump) file
# produced by a Verilog simulation
#
# A SPICE piece-wise-linear description is created for each signal in the VCD file
#
# Modfied on September 14, 2014 to support real variables

# Need system calls

import  sys ;

# Need command line arguments from operating system

from sys import argv ;

# Need the regular expression package 

import   re ;

# Set some electrical parameters

HI = 3.3 ;			# Electrical level for a logical 1
LO = 0.0 ;			# Electrical value for a logical 0
TRF = 1000  ;		# Rise/fall time (in ps) i.e. 1 ns
REAL_SCALE = 1.0;	# Scale factor for the real valued signals

# Creates symbol_table

symbol_table = {} ;

#
# Create a global variable token_table
# Token is the key with the pattern as the value
#

token_table = {"DATE" : "^\$date" ,
	"VER" : "^\$version" ,
	"TIME" : "^\$timescale",
	"SCOPE" : "^\$scope",
	"DUMP"  : "^\$dumpvars" ,
	"VAR" : "^\$var",
	"END" : "^\$end",
	"UPDATE" : "^\#[\d]+",
	"VCD" : "^[01]",
	"VCDR" : "^r" } ;

# Create a function that parses a line and returns the appropriate token

def parser(line):
	global token_table ;					    # token_table is a global variable
	keys = token_table.keys() ;
	token = "NULL" ;					        # The NULL token is our default
	for key in keys :
 		pattern = token_table[key] ;			# Pattern we are attempting to match
		match = re.match(pattern, line) ;	
		if (match) :
			token = key ;				        # If there is a match set token equal to the key
	return token ;	  					        # Return the token!
	
# User is expected to provide name of vcd file to read
# Expecting exactly two command line arguments

if (len(sys.argv) == 2) :
	cmd, vcd_file_name = argv ;
	print "" ;
	print "Reading file:  %s" % vcd_file_name ;
	print "" ;
else :
	print "" ;
	print "Usage: vcd2pwl <filename.vcd>" ;
	print ""
	sys.exit ;

# Open up the file for reading

try:
	vcd_fid = open(vcd_file_name, "r") ;
except IOError:
	print "Could not open file for reading!" ;

# Read one line at a time from the vcd file
# Read in first line from file

line = vcd_fid.readline() ; 

# Keep reading lines from the file until EOF reached

while line :
#
# Send line off to be parsed ... comes back with a token
#
	token = parser(line) ; 				
#
# If we have a timescale directive then read the next line and pick off the multiplier
#
	if (token == "TIME") :
		line = vcd_fid.readline() ; 			# Read in the next line
		fields = line.split() ;				# Split the line up into fields
		value = float(fields[0]) ; 			# value (first field)
		unit = fields[1] ; 				# unit ... ps, ns, us etc (second field)
		if (unit == "ns") :				# Determine what our time base multiplier is
			multiplier = 1e-9  * value ;
		elif (unit == "ps") :
			multiplier = 1e-12 * value ;
		elif (unit == "us") :
			multiplier = 1e-6 * value ;
		else :
			multiplier = 1.0 * value ;
		print "Mutliplier is %g.\n" % multiplier ;
#
# If we have a var directive then pick off signal name and symbol to be used to represent the signal
#
	elif (token == "VAR") :
		fields = line.split() ;				# Split line up into fields
		symbol = fields[3] ; 				# Symbol used to represent signal (4th field)
		signal = fields[4] ; 				# Signal name (5th field)
		symbol_table[symbol] = signal ;			# Build our dictionary of symbols and signal names
#
# If we have a dump directive then open up a bunch of files for writing
# and then get the initial conditions
#
	elif (token == "DUMP") :
		time = 0 ;					# Set time to 0.0
		keys = symbol_table.keys() ;			# The keys are the symbols
		fid = {} ;					# Create a dictionary
		for key in keys :				# Build the dictionary
			signal_name = symbol_table[key] ;
			file_name = signal_name + ".pwl" ;
			fid[key] =  open(file_name, "w") ;	# Opening a .pwl file for each signal
#
# Keep reading lines for the DUMP state until we get the END token
# For real valued signals the line will begin with a r
#
		line = vcd_fid.readline() ;			# Read next line from the file	
		while (parser(line) != "END") :
			value = line[0] ;			# First character is the value of the signal
			if (value == '0') :			# Convert to an electrical level
				voltage = LO ;
				symbol = line[1] ;		# Second character is the symbol used for the signal
			elif (value == '1') :
				voltage = HI ;
				symbol = line[1] ;		# Second character is the symbol used for the signal
			elif (value == 'r') :
				fields = line.split() ;		# Split the line up into sapce delimited fields
#				m = re.search('[\d.]+', fields[0]);	# Find the float
				m = re.search('[-+]?(\d+(\.\d*)?|\.\d+)([eE][-+]?\d+)?', fields[0]);	# Find the float
				voltage = float(m.group(0)) ;	# Convert to float
				voltage *= REAL_SCALE ;
				symbol = fields[1] ;
			else :
				pass ;
			line_out = "%g %g\n" % (time, voltage) ;
			fid[symbol].write(line_out) ;		# Write out inital values at time t=0
			line = vcd_fid.readline() ;
#
# Need to compute our new time ... UPDATE state
#	
	elif (token == "UPDATE") :	
		time = int(line[1:len(line)]) ;			# Strip off first character which is a pound sign
#
# Here is what we do when a value changes and is dumped (VCD)
#	
	elif (token == "VCD") :
		value = line[0] ;				# First character is the NEW value either a 0 or a 1
		symbol = line[1] ;				# Next character is the symbol
		if (value == '0') :				# Convert to an electrical level
			voltage = HI;				# Looks wrong but not
		else :						# Need to write out old value first
			voltage = LO ;
		line_out = "%dp %g\n" % (time, voltage) ;  	# time came from the UPDATE state
		fid[symbol].write(line_out) ;	
#		time += TRF ;					# Increment time by a rise/fall time
		if (value == '0') :				# Compute new electrical levels
			voltage = LO;			
		else :					
			voltage = HI ;	
		line_out = "%dp %g\n" % (time + TRF, voltage) ;	# Write out "new" (time, voltage) pair
		fid[symbol].write(line_out) ;	

	elif (token == "VCDR") :
		fields = line.split() ;				# Split the line up into sapce delimited fields
#		m = re.search('[\d.-]+', fields[0]);		# Find the float
		m = re.search('[-+]?(\d+(\.\d*)?|\.\d+)([eE][-+]?\d+)?', fields[0]);		# Find the float
		voltage = float(m.group(0)) ;			# Convert to float
		voltage *=  REAL_SCALE ;		
		symbol = fields[1] ;
		line_out = "%dp %g\n" % (time, voltage) ;  	# time came from the UPDATE state
		fid[symbol].write(line_out) ;	
#
# Go read the next line from the file and go back to start of while loop
#		
	line = vcd_fid.readline() ;

# *************************************************************************
#
# Close up all of the files
#
vcd_fid.close() ;
keys = symbol_table.keys() ;					# The keys are the symbols
for key in keys :
	name = symbol_table[key] ;
	name += ".pwl" ;
	print  "Successfully created: %s" % name ;
	fid[key].close() ;
print ""


 

 
	
