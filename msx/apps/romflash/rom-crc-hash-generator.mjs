import fs from 'fs'
import util from 'util'
import childProcess from 'child_process'
import path from 'path'
import os from 'os'
import crc from 'crc'

async function main() {
  const romImage = fs.readFileSync('../../bin/msxsyssrc-rc2014-aust-with-rtc.rom')

  for(let i = 0; i < 1024*512; i+= 0x4000) {
    const x = crc.crc16xmodem(romImage.slice(i, i + 0x4000)).toString(16)

    console.log(x);
  }
}


main()
