# Nanos++ Runtime Library with Maxeler support


This repository provides the OmpSs programming model integrated with the Maxeler runtime system, in order to run Maxeler tasks from OmpSs, in Maxeler hardware.

**Instructions**

Clone this repository on a machine with the Maxeler hardware and software already installed.

Setup the environment variables for configure and compilation:
load Vivado and Maxcompiler modules

$ module load vivado maxcompiler

set LDFLAGS to find libslic.a during compilation

$ export LDFLAGS="-L/opt/Software/maxeler/maxcompiler-2018.3.1/lib/ -lslic -lcurl"

configure nanos++

$ <path-to-nanox-mx>/configure --prefix=<nanos++-installation-directory> --with-maxcompiler=<Maxcompiler-installation-directory> --with-maxeleros=<MaxelerOS-installation-directory>

compile nanos++

$ make

Install nanos++

$ make install

Configure Mercurium

$ <path-to-mcxx>/configure --prefix=<mcxx-installation-directory> --with-nanox=<nanos++-installation-directory> --enable-ompss --enable-openmp

Compile Mercurium

$ make 

Install Mercurium

$ make install


** Example gemm **

Directory gemm contains a sample matrix multiplication that can be built
using the Nanos++ runtime with Maxeler support

gemm/
Makefile           # compilation command lines for the CPU code
mm_test_max.c
mm_test_panels.c          # three different drivers for executing matrix mult.
mm_test_panels_nodep.c 

buildDFE.sh        # script for compilation of the DFE kernel

src/gemm/
GemmEngineParameters.maxj  # kernel parameters
GemmManagerMax5.maxj       # kernel for Max5 (Jumax) compilation
GemmManager.maxj           # kernel for Max4 compilation


