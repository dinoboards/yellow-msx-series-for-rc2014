#!/usr/bin/env bash

set -e

outfile=$(mktemp)

rm -f "${1}"

function finish {
  rm "${outfile}"
}

trap finish EXIT

if [[ "${@:2}" == *.l80 ]]; then
  L80_COMMANDS=$(cat "${@:2}")
  L80_COMMANDS="L80\r\n${L80_COMMANDS}\r\nbye\r\n"
else
  L80_COMMANDS="L80\r\n${@:2}\rN\r\n\r\nbye"
fi

echo -e "${L80_COMMANDS}" | cpm 2>&1 | \
    tee ${outfile} | \
    grep -v "Overlapped record detected" | \
    grep -v "Overlaying Data area" | \
    grep -v "Overlaying Program area" | \
    grep -v "Sorry, terminal not found, using cooked mode." | \
    grep -v "hex2bin v2\.5\.1" | \
    grep -v "A>bye\s*\$" | \
    grep -v "A>N\s*\$" | \
    grep -v "N?\s*\$" | \
    grep -v "A>\s*\$" | \
    grep --color=always -e "Undefined Global" -e ^ | \
    grep --color=always -e "\?Out of memory|$" -e ^ | \
    grep --color=always -e "\?.*+Not Found|$" -e ^ | \
    grep --color=always -e "\?Loading Error|$" -e ^

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
