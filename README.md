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
