#!/bin/bash

module purge
#module load oneapi/2020.12.15.005
module load oneapi

declare oneapi_version="$(module info-loaded oneapi)"
declare oneapi_version="${oneapi_version#oneapi/}"
declare cmake_build_type=debug
declare kokkos_dir="${PWD}"
declare bld="${kokkos_dir}"/build/"${oneapi_version}"/"${cmake_build_type}"
declare install="${bld}"/install
declare cmake_cxx_compiler="$(type -P icpx)"

rm -rf "${bld}" "${install}"
mkdir -p "${bld}" "${install}"

cd "${bld}"
cmake \
    -DCMAKE_CXX_COMPILER="${cmake_cxx_compiler}" \
    -DCMAKE_CXX_FLAGS="-ggdb3" \
    -DCMAKE_EXE_LINKER_FLAGS= \
    -DCMAKE_INSTALL_PREFIX="${install}" \
    -DKokkos_ENABLE_SYCL=ON \
    -DKokkos_ENABLE_TESTS=OFF \
    -DKokkos_ENABLE_EXAMPLES=OFF \
    -DKokkos_CXX_STANDARD=17 \
    -DCMAKE_VERBOSE_MAKEFILE=OFF \
    -DCMAKE_CXX_EXTENSIONS=OFF \
    -DCMAKE_BUILD_TYPE="${cmake_build_type}" \
    -DKokkos_ENABLE_DEBUG=ON \
    -DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
    -GNinja \
    ~-

#make -j install
ninja install

