#!/usr/bin/env bash

set +x

NAME="${1%.pld}"

SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )

WINCUPLPATH=/opt/wincupl/

mkdir -p ./bin

wine ${WINCUPLPATH}cupl.exe -e -x -f - -ju ${WINCUPLPATH}\cupl.dl $1 2>>./bin/wincuplerror.log

# Removed the location entry - the TL866II Plus programmer seems to have loading file issue
sed -i '/Location/d' ./${NAME}.jed

mv ${NAME}.jed ./bin
mv ${NAME}.doc bin/${NAME}.doc
mv ${NAME}.mx bin/${NAME}.mx
