#!/usr/bin/env tclsh

#
# Tcl script to go in and re-analyze the tvc.dat files in the data directory
#

#
# Determine the project directory
# We will do the linearity analysis in the
# psd_tvc_linearity sub-directory
#

set		phome	$env(PHOME)
cd		${phome}/psd_tvc_linearity

set  	pwd_dir     [pwd]
set     data_dir    ${pwd_dir}/data
set     data_files  [glob -dir ${data_dir} *]

#
# Run a linearity analysis on each of the files in the
# data subdirectory
#

foreach filename $data_files {
    set      name    [file tail $filename]
    set      name    [file root $name]
	set		octave	"linearity  ${name}  6  12 250"
    puts 	$octave
	flush	stdout
	if 		{ [catch {eval	exec ${octave}} errmsg ] } {
				puts "Calling Octave returned => $errmsg"
	}
}

exit
