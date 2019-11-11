#!/bin/bash

# exit when any command fails
set -e

if [[ -z "${HIMAWARI_HOST_SDK}" ]]; then
    echo "Environment variable 'HIMAWARI_HOST_SDK' is undefined! Using predefined path."
    HIMAWARI_HOST_SDK=`cd ~ && pwd`/himawari_host_sdk
fi

echo "HIMAWARI_HOST_SDK: ${HIMAWARI_HOST_SDK}"
PKG_CONFIG_PATH=${HIMAWARI_HOST_SDK}/lib/pkgconfig:$(pkg-config --variable pc_path pkg-config)${PKG_CONFIG_PATH:+:}${PKG_CONFIG_PATH}
CFLAGS="-O2 -pipe -g -feliminate-unused-debug-types"
CXXFLAGS="-std=c++11 -O2 -pipe -g -feliminate-unused-debug-types"
LDFLAGS="-Wl,-O1 -Wl,--hash-style=gnu -Wl,--as-needed"

export PKG_CONFIG_PATH=${PKG_CONFIG_PATH}
echo ${PKG_CONFIG_PATH}
rm -rf bin
rm -rf build_host
mkdir build_host && cd build_host
cmake -DCMAKE_CXX_FLAGS="-I${HIMAWARI_HOST_SDK}/include ${CXXFLAGS}" -DCMAKE_C_FLAGS="${CFLAGS}" -DCMAKE_EXE_LINKER_FLAGS="${LDFLAGS}" -DCMAKE_FIND_ROOT_PATH="${HIMAWARI_HOST_SDK}/lib;${HIMAWARI_HOST_SDK}/lib/cmake;common-api/bin" -DBUILD_DIRECTORY="build_host" .. && make

set +e
