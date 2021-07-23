# To create the image:
#   $ (cd docker-tool-chain && docker build -t yellowmsx .)
# To run the container:
#   $ docker run -v ${PWD}:/src/ --privileged=true -u $(id -u ${USER}):$(id -g ${USER}) -it yellowmsx

FROM ubuntu:20.04

LABEL Version="0.1" \
      Date="2021-07-22" \
      Maintainer="Dean Netherton" \
      Description="Tool chain to build units for the yellow msx for rc2014 kits"

ENV Z88DK_PATH="/opt/z88dk" \
    SDCC_PATH="/tmp/sdcc" \
    BUILD_SDCC=1 \
    DEBIAN_FRONTEND=noninteractive

RUN apt update -y && \
    apt dist-upgrade -y && \
    apt install -y build-essential dos2unix libboost-all-dev texinfo texi2html libxml2-dev subversion bison \
                   flex zlib1g-dev m4 git wget pasmo sudo dosfstools curl && \
    curl -sL https://deb.nodesource.com/setup_16.x | sudo -E bash - && \
    apt install -y nodejs

RUN mv /usr/bin/svn /usr/bin/org-svn
COPY ./svnshim.sh /usr/bin/svn

RUN git clone https://github.com/z88dk/z88dk.git ${Z88DK_PATH} && \
    cd ${Z88DK_PATH} && \
    git checkout 8d70c5aaa6a6de544d66731015aaaa0d8f0253da && \
    git submodule update --init --recursive && \
    rm -rf .git* && \
    chmod 777 build.sh && \
    ./build.sh && \
    rm -fR ${SDCC_PATH}

RUN mkdir -p /linuxtools/  && \
	cd /linuxtools && \
	wget https://sourceforge.net/projects/sdcc/files/sdcc-linux-amd64/4.0.0/sdcc-4.0.0-amd64-unknown-linux2.5.tar.bz2/download -O "sdcc-4.0.0-amd64-unknown-linux2.5.tar.bz2" && \
	tar -xjf  sdcc-4.0.0-amd64-unknown-linux2.5.tar.bz2 && \
	rm sdcc-4.0.0-amd64-unknown-linux2.5.tar.bz2

RUN cd /linuxtools && \
    git clone --depth 1 https://github.com/E3V3A/hex2bin.git && \
	cd hex2bin && \
	make

RUN cd /linuxtools && \
	git clone --depth 1 https://github.com/jhallen/cpm.git && \
	cd cpm && \
	OS=linux MAKEFLAGS= make -B --trace

ENV PATH="${Z88DK_PATH}/bin:/linuxtools/sdcc-4.0.0/bin:/linuxtools/hex2bin:/linuxtools/cpm:${PATH}" \
    ZCCCFG="${Z88DK_PATH}/lib/config/"

RUN apt install -y mtools

WORKDIR /src/
