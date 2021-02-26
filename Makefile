# Please run 'module load dpcpp' in your environment before executing this Makefile
# so that the correct version of clang++ is first in the PATH (among other reasons)
#
# This Makefile just sets things up so that "make foo" will build foo.cpp into foo
# with the correct command line options
#KOKKOS_SRC_PATH=/home/nliber/git/github.com/nliber/kokkos-develop
KOKKOS_SRC_PATH=/home/nliber/git/github.com/nliber/kokkos-construct-atomic-subviews
#CXX=clang++
CXX=icpx
KOKKOS_DEVICES=SYCL
KOKKOS_ARCH=SKX
CXXFLAGS=-ggdb3 -O0 -I. -DCL_TARGET_OPENCL_VERSION=210
KOKKOS_CXX_STANDARD=c++17
GTEST_PATH=/home/nliber/git/github.com/nliber/kokkos-sycl/tpls/gtest
KOKKOS_OPTIONS=disable_deprecated_code
#KOKKOS_INTERNAL_GCC_TOOLCHAIN=/soft/compilers/gcc/8.2.0/linux-rhel7-x86_64/
KOKKOS_INTERNAL_GCC_TOOLCHAIN=/soft/packaging/spack-builds/linux-rhel7-x86_64/gcc-9.3.0/gcc-9.3.0-qfmcwfbuvnpn47zxjzfjvodzjl6reerh/

#KOKKOS_PATH=/home/nliber/git/github.com/pvelesko/kokkos/build_nliber/install
#KOKKOS_PATH=/home/nliber/git/github.com/pvelesko/kokkos/build_velesko/install
#KOKKOS_PATH=/home/pvelesko/local/kokkos_liber/build_velesko/install
#KOKKOS_PATH=/home/nliber/git/github.com/nliber/kokkos-sycl/build5/install
#KOKKOS_PATH=/home/nliber/git/github.com/nliber/kokkos-sycl/build.serial/install
#KOKKOS_PATH=/home/nliber/git/github.com/nliber/kokkos-sycl/build.2020.04.30.001/install
#KOKKOS_PATH=/home/nliber/git/github.com/nliber/kokkos-sycl/build.2020.06.15.001/install
#KOKKOS_PATH=/home/nliber/git/github.com/nliber/kokkos-rebase-sycl-nliber2-develop/build-2020.07.30.001/install
#KOKKOS_PATH=/home/nliber/git/github.com/nliber/kokkos-rebase-sycl-nliber2-develop/build_icpx/install
KOKKOS_PATH=${KOKKOS_SRC_PATH}/build/2020.12.15.005/debug/install
#KOKKOS_PATH=/home/nliber/kokkos-sycl-nliber2/install
#KOKKOS_PATH=/home/nliber/git/github.com/nliber/kokkos-sycl/build_icpx/install

CXXFLAGS+=-Wall
CXXFLAGS+=-std=$(KOKKOS_CXX_STANDARD)
CXXFLAGS+=-I$(KOKKOS_PATH)/include
CXXFLAGS+=-fsycl
CXXFLAGS+=-fsycl-unnamed-lambda
CXXFLAGS+=-fsycl-targets=spir64_gen-unknown-unknown-sycldevice
CXXFLAGS+=-Xsycl-target-backend "-device skl"
CXXFLAGS+=-DKOKKOS_DEPENDENCE

LDFLAGS=-L$(KOKKOS_PATH)/lib -L$(KOKKOS_PATH)/lib64
LDLIBS=-lkokkoscore -lkokkoscontainers -lOpenCL -lsycl -ldl -lpthread

