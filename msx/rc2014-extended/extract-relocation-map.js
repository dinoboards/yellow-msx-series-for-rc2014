const fs = require('fs')
const util = require('util')
const childProcess = require('child_process')
const path = require('path')
const os = require('os')

function convertBitmapByte(hits, index) {
  let byte = 0
  for(let i = 0; i < 8; i++) {
    const hit = hits.some(x => x === index + i)
    if (hit)
      byte |= Math.pow(2, 7 - i)
  }

  return byte
}

function convertToBitmap(hits, dataLength) {
  const length_ = parseInt(dataLength / 8)
  const length = dataLength % 8 > 0 ? length_ + 1 : length_
  const data = Buffer.alloc(length + 2)

  for(let i = 0; i < length; i++) {
    const byte = convertBitmapByte(hits, i * 8)
    data[i + 2] = byte
  }

  data[0] = length & 255
  data[1] = length >> 8
  return data
}

async function main(fileAt100, fileAt200, relocationFileName) {
  const data1 = fs.readFileSync(fileAt100)
  const data2 = fs.readFileSync(fileAt200)

  if (data1.length != data2.length)
    throw new Error("File length are different")

  const hits = []
  for(let i=0; i < data1.length; i++) {
    if (data1[i] !== data2[i])
      hits.push(i)
  }

  const data = convertToBitmap(hits, data2.length)

  fs.writeFileSync(relocationFileName, data)
}

main(...[...process.argv].slice(2)).catch(err => {
  console.log(err.stack)
  console.log("");
  process.exit(1)
})
