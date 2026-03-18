<p align="center">
    <img src="doc/images/cwipiNew.svg" alt="Logo" width="25%"/>
</p>


# CWIPI #

**CWIPI** (Coupling With Interpolation Parallel Interface) is a library for coupling parallel scientific codes via MPI communications to perform multi-physics simulations in massively parallel, distributed-memory environments, with interfaces in C, Python and Fortran.

## Documentation  ##

The user documentation is available [here](https://onera.github.io/cwipi/index.html).

ONERA users can also access the documentation deployed on ONERA's [internal Gitlab pages server](https://numerics.gitlab-pages.onera.net/coupling/cwipi/cwipi-1.1.0/index.html).

## Installation ##

### Dependencies

General dependencies for building **CWIPI** are:
- a C++ compiler
- [CMake](https://cmake.org/) (version 3.16 or higher)
- an MPI distribution

### Basic Installation

Follow these steps to build **CWIPI** from the sources:

First, clone the repository
  - either from [GitHub](https://github.com/onera/cwipi): `git clone git@github.com:onera/cwipi.git`
  - or from [GitLab](https://gitlab.onera.net/numerics/coupling/cwipi) (for ONERA users only): `git clone git@gitlab.onera.net:numerics/coupling/cwipi.git`

Then, use the following commands:
1. `cd cwipi`
2. `git submodule update --init` (needed for dependencies such as [**ParaDiGM**](https://github.com/onera/paradigm))
3. `mkdir build`
4. `cd build`
5. `cmake ..`
6. `make`
7. `make install`
8. `./cwp_run` (if you want to run the test cases)


**CWIPI** relies on [CMake](https://cmake.org/) for software configuration.
Please refer to the [installation guide](https://onera.github.io/cwipi/1.3.0/installation.html#installation) for more details about the CMake options.


## Quick start ##

You can find a basic example of two-way coupling using **CWIPI** in the [quick start section of the documentation](https://onera.github.io/cwipi/1.3.0/quick_start.html).
Additional examples for more advanced scenarios can be found in the ``tests`` directory.

## Issues ##

Issues can be reported directly in the [Issues](https://gitlab.onera.net/numerics/coupling/cwipi/-/issues) section.


## License ##

**CWIPI** is available under the [LGPL3 license](https://www.gnu.org/licenses/lgpl-3.0.fr.html).


## Copyright ##

Copyright 2023, ONERA The French Aerospace Lab
