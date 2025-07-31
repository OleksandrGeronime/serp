#!/bin/bash

SERP_PATH="../../"
DIAG_PATH="./project/AppComponentDiagram.puml"
GEN_PATH="./"

"$SERP_PATH/tools/serp_app_generator" -s "$SERP_PATH" -d "$DIAG_PATH" -o "$GEN_PATH" || exit 1

cd ExampleProject || exit 1
mkdir -p build
cd build || exit 1

cmake .. && make -j"$(nproc)"

if [ $? -eq 0 ] && [ -f "./ExampleProject" ]; then
    FULL_PATH=$(realpath "./ExampleProject")
    echo "ExampleProject generated and built $FULL_PATH"
fi