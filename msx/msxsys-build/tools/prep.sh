#!/usr/bin/env bash

set -e

export WRK_DIR=./working
export PATH=$(pwd)/tools:${PATH}

mkdir -p ./${WRK_DIR}

echo "PATH=", ${PATH}
sf "./tools/M80.COM"
sf "./tools/L80.COM"
sf "./tools/m80.sh"
sf "./tools/l80.sh"
sf "./tools/cleancpmfile.sh"
