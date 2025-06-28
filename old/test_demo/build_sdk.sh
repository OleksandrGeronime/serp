#!/bin/bash

if [[ -z "${BT_SDK_ENV_PATH}" ]]; then
    echo "Environment variable 'BT_SDK_ENV_PATH' is undefined!"
    exit
fi

source ${BT_SDK_ENV_PATH}
rm -rf build
mkdir build && cd build
${BASH_ALIASES[cmake]} .. && make -j8
