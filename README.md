# ompss-maxeler
OmpSs-Maxeler integration for LEGaTO

This repository provides the OmpSs programming model integrated with the Maxeler runtime system, in order to run Maxeler tasks from OmpSs, in Maxeler hardware.


Instructions

Clone this repository on a machine with the Maxeler hardware and software already installed.

Setup the environment variables for configure and compilation:

# load Vivado and Maxcompiler modules
module load vivado maxcompiler

# set LDFLAGS to find libslic.a during compilation
export LDFLAGS="-L/opt/Software/maxeler/maxcompiler-2018.3.1/lib/ -lslic -lcurl"

# configure nanos++
configure --prefix=<installation-directory> --with-maxcompiler=<path-to-maxcompiler-installation-dir> \
   --with-maxeleros=<path-to-maxeleros-installation-dir>
  
# compile nanos++
make

# install nanos++
make install

