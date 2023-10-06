#!/usr/bin/env bash

set -e

outfile=$(mktemp)

rm -f "${1}"

function finish {
  rm "${outfile}"
}

trap finish EXIT

echo -e "L80\r\n${@:2}\rN\r\n\r\nbye"  | \
    cpm 2>&1 | \
    tee ${outfile} | \
    grep --color=always -E "\?Out of memory|$" | \
    grep --color=always -E "\?.*+Not Found|$" | \
    grep --color=always -E "\?Loading Error|$" | \
    grep -v "Sorry, terminal not found, using cooked mode." | \
    grep -v "A>bye\s*\$" | \
    grep -v "A>\s*\$" | \
    grep --color -E "[0-9]+\s+Undefined Global\(s\)|$"

if grep -q ' Not Found' ${outfile}; then
  rm -f ${1}
  exit 1
fi

if grep -q '?Out of memory' ${outfile}; then
  rm -f ${1}
  exit 1
fi

FINAL_LINK=$(awk '/\/N/{y=1;next}y' ${outfile})

if echo ${FINAL_LINK} | grep -q 'Undefined Global(s)'; then
  rm -f ${1}
  exit 1
fi

./cleancpmfile.sh "${1}"
