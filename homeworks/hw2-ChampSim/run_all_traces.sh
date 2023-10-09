#!/bin/bash

###############################################################
# Run ChampSim version IPC1 (first competition) with 
# cababilities to print conditional branch addresses taken 
# not taken and predicted mispredicted. This is for all traces 
# of CPU2017 generated in uni under the directory 
# /home/students/cs/benchmarks/champsim/
################################################################

./run_champsim.sh bimodal-no-no-no-no-lru-1core 100 500 Gcc.trace.gz;

./run_champsim.sh bimodal-no-no-no-no-lru-1core 100 500 Blender.trace.gz;

./run_champsim.sh bimodal-no-no-no-no-lru-1core 100 500 Bwaves.trace.gz;

./run_champsim.sh bimodal-no-no-no-no-lru-1core 100 500 cactuBSSN.trace.gz;

./run_champsim.sh bimodal-no-no-no-no-lru-1core 100 500 Cam4.trace.gz;

./run_champsim.sh bimodal-no-no-no-no-lru-1core 100 500 Exchange.trace.gz;

./run_champsim.sh bimodal-no-no-no-no-lru-1core 100 500 Fotonik3d.trace.gz;

./run_champsim.sh bimodal-no-no-no-no-lru-1core 100 500 Imagick.trace.gz;

./run_champsim.sh bimodal-no-no-no-no-lru-1core 100 500 Lbm.trace.gz;

./run_champsim.sh bimodal-no-no-no-no-lru-1core 100 500 Leela.trace.gz;

./run_champsim.sh bimodal-no-no-no-no-lru-1core 100 500 Mcf.trace.gz;

./run_champsim.sh bimodal-no-no-no-no-lru-1core 100 500 Omnetpp.trace.gz;

./run_champsim.sh bimodal-no-no-no-no-lru-1core 100 500 Parest.trace.gz;

./run_champsim.sh bimodal-no-no-no-no-lru-1core 100 500 Perlbench.trace.gz;

./run_champsim.sh bimodal-no-no-no-no-lru-1core 100 500 Povray.trace.gz;

./run_champsim.sh bimodal-no-no-no-no-lru-1core 100 500 Roms.trace.gz;

./run_champsim.sh bimodal-no-no-no-no-lru-1core 100 500 Wrf.trace.gz;

./run_champsim.sh bimodal-no-no-no-no-lru-1core 100 500 x264.trace.gz;

./run_champsim.sh bimodal-no-no-no-no-lru-1core 100 500 Xalancbmk.trace.gz;

./run_champsim.sh bimodal-no-no-no-no-lru-1core 100 500 Xz.trace.gz;

