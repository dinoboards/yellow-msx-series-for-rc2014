#!/usr/bin/env bash

set -e

echo -e "" > $2
xxd -c 1 $1 | while read offset hex char; do
  echo -en "\tDB\t0${hex}H\r\n" >> $2 
done 
