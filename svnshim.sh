#!/usr/bin/env /bin/bash

# checkout -r 12419 https://svn.code.sf.net/p/sdcc/code/trunk/sdcc -q /opt/z88dk/src/sdcc-build

set -x

targetpath=${@: -1}

if [ "$1" == "checkout" ]; then

  org-svn checkout --depth=empty ${@:2}
  cd $targetpath
  until org-svn update -r $3 --set-depth infinity
  do
    org-svn cleanup
  done

else
  set -e
  org-svn $@
fi
