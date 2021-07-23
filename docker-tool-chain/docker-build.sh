#!/usr/bin/env /bin/bash

set -e

source ./version.sh

docker build -t vipoo/yellow-msx-rc2014-tool-chain:${VERSION} .
docker tag vipoo/yellow-msx-rc2014-tool-chain:${VERSION} vipoo/yellow-msx-rc2014-tool-chain:latest
