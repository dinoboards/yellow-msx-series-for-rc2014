#!/usr/bin/env bash

set -e

export PATH=$(pwd)/tools:${PATH}

mkdir -p ./${WRK_DIR}

sf "./tools/M80.COM"
sf "./tools/L80.COM"
sf "./tools/m80.sh"
sf "./tools/l80.sh"
sf "./tools/cleancpmfile.sh"
