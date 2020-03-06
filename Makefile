# Please run 'module load dpcpp' in your environment before executing this Makefile
# so that the correct version of clang++ is first in the PATH (among other reasons)
#
# This Makefile just sets things up so that "make foo" will build foo.cpp into foo
# with the correct command line options
KOKKOS_SRC_PATH=/home/nliber/git/github.com/nliber/kokkos-sycl
CXX=clang++
KOKKOS_DEVICES=SyCL
KOKKOS_ARCH=SKX
CXXFLAGS=-ggdb3 -O0
KOKKOS_CXX_STANDARD=c++14
GTEST_PATH=/home/nliber/git/github.com/nliber/kokkos-sycl/tpls/gtest
KOKKOS_OPTIONS=disable_deprecated_code
KOKKOS_INTERNAL_GCC_TOOLCHAIN=/soft/compilers/gcc/8.2.0/linux-rhel7-x86_64/
KOKKOS_PATH=/home/nliber/git/github.com/nliber/kokkos-sycl/build0/install

CXXFLAGS+=-Wall -std=$(KOKKOS_CXX_STANDARD) -I$(KOKKOS_PATH)/include -fsycl
LDFLAGS=-L$(KOKKOS_PATH)/lib
LDLIBS=-lkokkos -lOpenCL -lsycl -ldl

