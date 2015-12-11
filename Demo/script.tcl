############################################################
## This file is generated automatically by Vivado HLS.
## Please DO NOT edit it.
## Copyright (C) 2013 Xilinx Inc. All rights reserved.
############################################################
open_project HLS
set_top wbfm
add_files wbfm.cpp
add_files cordic.cpp
add_files atan.cpp
add_files xilly_debug.cpp
#add_files -tb myinput.txt
#add_files -tb golden.txt
#add_files -tb WBFM_test.cpp
open_solution "solution1"
set_part {xc7z020clg484-1}
create_clock -period 10 -name default
config_interface -all ap_fifo -expose_global
source "./directives.tcl"
#csim_design
#csynth_design
