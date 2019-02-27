# mitsuba2-vdb-converter
A small utility to convert OpenVDB files to the Mitsuba 2 volume format. 

## Compiling
The converter uses a recursive CMake setup to compile OpenVDB and its dependencies. The repository should be cloned recursively, i.e. 
```
git clone --recursive https://github.com/mitsuba-renderer/mitsuba2-vdb-converter.git
```
Then, the build process is the regular CMake process:
```
cd mitsuba2-vdb-converter 
mkdir build 
cd build 
cmake ..
make -j
```
This should then build all the dependencies as well as the converter utility. The setup was tested on Arch Linux and macOS 10.14. 

## Usage

Currently, the usage is limited to extracting a single float grid from an OpenVDB file. 
In the simplest case, the converter can be used as
```
./convertvdb myvolumefile.vdb
```
The output is then written in the same path with the suffix `.vol`. If the OpenVDB file contains multiple grids, one of them can be selected:
```
./convertvdb myvolumefile.vdb gridName
```
To get a list of grids in a file, one can use the `vdb_print` program, which is part of OpenVDB. In this CMake setup, the `vdb_print` executable is built and written to a subfolder in `build/ext_build`.
