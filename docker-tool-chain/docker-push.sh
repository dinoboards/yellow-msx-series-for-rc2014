#!/usr/bin/env /bin/bash

set -e

source ./version.sh

docker push vipoo/yellow-msx-rc2014-tool-chain:${VERSION}
docker push vipoo/yellow-msx-rc2014-tool-chain:latest
