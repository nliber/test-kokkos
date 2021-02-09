#!/bin/bash
set -xv

module purge
module load oneapi

export CMAKE_BUILD_TYPE="debug"
export KOKKOS_DIR="${PWD}"
export BLD="${KOKKOS_DIR}"/build
export INSTALL="${BLD}"/install
rm -r "${BLD}"
mkdir -p "${INSTALL}"
cd "${BLD}"

cmake \
    -DCMAKE_CXX_COMPILER="$(type -P icpx)" \
    -DCMAKE_CXX_FLAGS="-ggdb3" \
    -DCMAKE_EXE_LINKER_FLAGS= \
    -DCMAKE_INSTALL_PREFIX="${INSTALL}" \
    -DKokkos_ENABLE_SYCL=ON \
    -DKokkos_ENABLE_TESTS=ON \
    -DKokkos_ENABLE_EXAMPLES=OFF \
    -DKokkos_CXX_STANDARD=17 \
    -DCMAKE_VERBOSE_MAKEFILE=ON \
    -DCMAKE_CXX_EXTENSIONS=OFF \
    -DCMAKE_BUILD_TYPE="${CMAKE_BUILD_TYPE}" \
    -DKokkos_ENABLE_DEBUG=ON \
    ~-

make install -j

