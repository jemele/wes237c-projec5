############################################################
## This file is generated automatically by Vivado HLS.
## Please DO NOT edit it.
## Copyright (C) 2012 Xilinx Inc. All rights reserved.
############################################################
open_project hls
set_top wbfm
add_files wbfm.c
add_files xilly_debug.c
open_solution "solution1"
set_part  {xc7z020clg484-1}
create_clock -period 10
config_interface -all ap_fifo -expose_global
source "directives.tcl"
csynth_design
