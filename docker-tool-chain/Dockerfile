# To create the image:
#   $ (cd docker-tool-chain && ./docker-build.sh)
# To run the container:
#   $ docker run -v ${PWD}:/src/ --privileged=true -u $(id -u ${USER}):$(id -g ${USER}) -it vipoo/yellow-msx-rc2014-tool-chain:latest

FROM ubuntu:20.04 as STAGE1

LABEL Version="0.0.12" \
      Date="2021-12-17" \
      Maintainer="Dean Netherton" \
      Description="Tool chain to build units for the yellow msx for rc2014 kits"

ENV Z88DK_PATH="/opt/z88dk" \
    SDCC_PATH="/tmp/sdcc" \
    BUILD_SDCC=1 \
    DEBIAN_FRONTEND=noninteractive

RUN apt update -y && \
    apt dist-upgrade -y && \
    apt install -y  mtools build-essential dos2unix libboost-all-dev texinfo texi2html libxml2-dev subversion bison \
                   flex zlib1g-dev m4 git wget dosfstools curl

RUN mv /usr/bin/svn /usr/bin/org-svn
COPY ./svnshim.sh /usr/bin/svn

RUN git clone https://github.com/z88dk/z88dk.git ${Z88DK_PATH}
WORKDIR /opt/z88dk
RUN git checkout 8d70c5aaa6a6de544d66731015aaaa0d8f0253da
RUN git submodule update --init --recursive
RUN rm -rf .git*
RUN chmod 777 build.sh
RUN ./build.sh
RUN rm -fR ${SDCC_PATH}

RUN cd /opt && \
	wget https://sourceforge.net/projects/sdcc/files/sdcc-linux-amd64/4.0.0/sdcc-4.0.0-amd64-unknown-linux2.5.tar.bz2/download -O "sdcc-4.0.0-amd64-unknown-linux2.5.tar.bz2" && \
	tar -xjf  sdcc-4.0.0-amd64-unknown-linux2.5.tar.bz2 && \
	rm sdcc-4.0.0-amd64-unknown-linux2.5.tar.bz2

RUN cd /opt && \
    git clone --depth 1 https://github.com/E3V3A/hex2bin.git && \
	cd hex2bin && \
	make

RUN cd /opt && \
	git clone --depth 1 https://github.com/jhallen/cpm.git && \
	cd cpm && \
	OS=linux MAKEFLAGS= make -B --trace

RUN wget -O /tmp/node.tar.gz https://nodejs.org/dist/v16.5.0/node-v16.5.0-linux-x64.tar.gz
RUN tar -xzf "/tmp/node.tar.gz" -C /opt && \
    ls /opt/ -lartsh  && \
    mv /opt/node-v16.5.0-linux-x64 /opt/nodejs && ls /opt/nodejs/bin/node -lartsh && chmod +x /opt/nodejs/bin/node && \
    /opt/nodejs/bin/node --version

FROM ubuntu:20.04

ARG DEBIAN_FRONTEND=noninteractive
RUN sed -i 's|http://archive.ubuntu.com/ubuntu/|http://mirror.aarnet.edu.au/pub/ubuntu/archive/|g' /etc/apt/sources.list && \
    dpkg --add-architecture i386 && \
    apt update -y && \
    apt dist-upgrade -y && \
    apt install -y --no-install-recommends mtools dos2unix pasmo build-essential dosfstools clang-format git m4 ssh xxd wget ca-certificates gnupg2 software-properties-common zip && \
    wget -qO- https://dl.winehq.org/wine-builds/Release.key | apt-key add - && \
    apt-key adv --keyserver hkp://keyserver.ubuntu.com:80 --recv F987672F && \
    apt-add-repository 'deb https://dl.winehq.org/wine-builds/ubuntu/ focal main' && \
    apt-key adv --keyserver keyserver.ubuntu.com --recv-key C99B11DEB97541F0 && \
    apt-add-repository https://cli.github.com/packages && \
    apt update -y && \
    apt install -y wine32 gh && \
    rm -rf /var/lib/apt/lists/*

COPY Wincupl/Shared /opt/wincupl

COPY --from=STAGE1 /opt/nodejs /opt/nodejs
COPY --from=STAGE1 /opt/sdcc-4.0.0 /opt/sdcc-4.0.0
COPY --from=STAGE1 /opt/cpm /opt/cpm
COPY --from=STAGE1 /opt/hex2bin /opt/hex2bin
COPY --from=STAGE1 /opt/z88dk/bin /opt/z88dk/bin
COPY --from=STAGE1 /opt/z88dk/include /opt/z88dk/include
COPY --from=STAGE1 /opt/z88dk/lib /opt/z88dk/lib
COPY --from=STAGE1 /opt/z88dk/libsrc/_DEVELOPMENT/sdcc_peeph*.* /opt/z88dk/libsrc/_DEVELOPMENT/
COPY --from=STAGE1 /opt/z88dk/libsrc/_DEVELOPMENT/sdcc_opt*.* /opt/z88dk/libsrc/_DEVELOPMENT/
COPY --from=STAGE1 /opt/z88dk/libsrc/_DEVELOPMENT/lib/sdcc_iy /opt/z88dk/libsrc/_DEVELOPMENT/lib/sdcc_iy
COPY --from=STAGE1 /opt/z88dk/libsrc/_DEVELOPMENT/lib/sdcc_ix /opt/z88dk/libsrc/_DEVELOPMENT/lib/sdcc_ix
COPY --from=STAGE1 /opt/z88dk/libsrc/_DEVELOPMENT/lib/sccz80 /opt/z88dk/libsrc/_DEVELOPMENT/lib/sccz80

ENV Z88DK_PATH="/opt/z88dk" \
    PATH="/opt/z88dk/bin:/opt/nodejs/bin:/opt/sdcc-4.0.0/bin:/opt/hex2bin:/opt/cpm:${PATH}" \
    ZCCCFG="/opt/z88dk/lib/config/"

RUN adduser --disabled-password --gecos "" builder

COPY bashrc /home/builder/.bashrc

RUN su builder -c "wine cmd.exe /C dir"

WORKDIR /src/
