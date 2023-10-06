# To create the image:
#   $ (cd docker-tool-chain && ./docker-build.sh)
# To run the container:
#   $ docker run -v ${PWD}:/src/ --privileged=true -u $(id -u ${USER}):$(id -g ${USER}) -it vipoo/yellow-msx-rc2014-tool-chain:latest

FROM ubuntu:jammy-20230916 as basebuilder

LABEL Maintainer="Dean Netherton" \
      Description="Tool chain to build units for the yellow msx for rc2014 kits"

ENV Z88DK_PATH="/opt/z88dk" \
    SDCC_PATH="/tmp/sdcc" \
    BUILD_SDCC=1 \
    DEBIAN_FRONTEND=noninteractive

RUN sed -i 's|http://archive.ubuntu.com/ubuntu/|http://mirror.aarnet.edu.au/pub/ubuntu/archive/|g' /etc/apt/sources.list && \
    dpkg --add-architecture i386 && \
    apt update -y && \
    apt dist-upgrade -y && \
    apt install -y --no-install-recommends ca-certificates mtools build-essential dos2unix libboost-all-dev texinfo texi2html libxml2-dev subversion bison flex zlib1g-dev m4 git wget dosfstools curl

RUN mv /usr/bin/svn /usr/bin/org-svn
COPY ./svnshim.sh /usr/bin/svn

FROM basebuilder as z88dkbuild

RUN git clone https://github.com/z88dk/z88dk.git ${Z88DK_PATH}
WORKDIR /opt/z88dk
RUN git checkout 35abc0ed24564f0295ed8c2a2483e40c122172b0
RUN git submodule update --init --recursive
RUN rm -rf .git*
RUN chmod 777 build.sh
RUN ./build.sh
RUN rm -fR ${SDCC_PATH}

FROM basebuilder as sdccbuild

RUN cd /opt && \
	wget https://sourceforge.net/projects/sdcc/files/sdcc-linux-amd64/4.3.0/sdcc-4.3.0-amd64-unknown-linux2.5.tar.bz2/download -O "sdcc-4.3.0-amd64-unknown-linux2.5.tar.bz2" && \
	tar -xjf sdcc-4.3.0-amd64-unknown-linux2.5.tar.bz2 && \
	rm sdcc-4.3.0-amd64-unknown-linux2.5.tar.bz2

FROM basebuilder as hex2binbuild

RUN cd /opt && \
    git clone --depth 1 https://github.com/vipoo/hex2bin.git && \
	cd hex2bin && \
	make

FROM basebuilder as cpmbuild

RUN cd /opt && \
	git clone --depth 1 https://github.com/jhallen/cpm.git && \
	cd cpm && \
	OS=linux MAKEFLAGS= make -B

FROM basebuilder as nodejsbuild

RUN wget -O /tmp/node.tar.gz https://nodejs.org/dist/v16.5.0/node-v16.5.0-linux-x64.tar.gz
RUN tar -xzf "/tmp/node.tar.gz" -C /opt && \
    mv /opt/node-v16.5.0-linux-x64 /opt/nodejs && \
    chmod +x /opt/nodejs/bin/node && \
    /opt/nodejs/bin/node --version

FROM basebuilder as sjasmplusbuild

RUN cd /opt && \
    wget https://github.com/z00m128/sjasmplus/archive/refs/tags/v1.18.3.tar.gz -O sjasmplus.tar.gz && \
    tar -xvf sjasmplus.tar.gz && \
    rm sjasmplus.tar.gz && \
    mv sjasmplus-1.18.3 sjasmplus && \
    cd sjasmplus && \
    make clean && \
    make

FROM ubuntu:jammy-20230916

RUN sed -i 's|http://archive.ubuntu.com/ubuntu/|http://mirror.aarnet.edu.au/pub/ubuntu/archive/|g' /etc/apt/sources.list && \
    apt update -y && \
    apt dist-upgrade -y && \
    apt install -y --no-install-recommends ca-certificates mtools dos2unix pasmo dosfstools clang-format m4 ssh xxd gnupg2 software-properties-common zip wget && \
    wget -qO- https://dl.winehq.org/wine-builds/Release.key | apt-key add - && \
    apt-key adv --keyserver hkp://keyserver.ubuntu.com:80 --recv F987672F && \
    apt-add-repository 'deb https://dl.winehq.org/wine-builds/ubuntu/ focal main' && \
    dpkg --add-architecture i386 && \
    apt update -y && \
    apt install -y --no-install-recommends wine32 build-essential && \
    ln -s /usr/lib/wine/wine /usr/bin/wine && \
    rm -rf /var/lib/apt/lists/* && \
    adduser --disabled-password --gecos "" builder

COPY --link Wincupl/Shared /opt/wincupl
COPY --link --from=nodejsbuild /opt/nodejs /opt/nodejs
COPY --link --from=sdccbuild /opt/sdcc-4.3.0 /opt/sdcc-4.3.0
COPY --link --from=cpmbuild /opt/cpm /opt/cpm
COPY --link --from=hex2binbuild /opt/hex2bin /opt/hex2bin
COPY --link --from=sjasmplusbuild /opt/sjasmplus /opt/sjasmplus
COPY --link --from=z88dkbuild /opt/z88dk/bin /opt/z88dk/bin
COPY --link --from=z88dkbuild /opt/z88dk/include /opt/z88dk/include
COPY --link --from=z88dkbuild /opt/z88dk/lib /opt/z88dk/lib
COPY --link --from=z88dkbuild /opt/z88dk/libsrc/_DEVELOPMENT/sdcc_peeph*.* /opt/z88dk/libsrc/_DEVELOPMENT/
COPY --link --from=z88dkbuild /opt/z88dk/libsrc/_DEVELOPMENT/sdcc_opt*.* /opt/z88dk/libsrc/_DEVELOPMENT/
COPY --link --from=z88dkbuild /opt/z88dk/libsrc/_DEVELOPMENT/lib/sdcc_iy /opt/z88dk/libsrc/_DEVELOPMENT/lib/sdcc_iy
COPY --link --from=z88dkbuild /opt/z88dk/libsrc/_DEVELOPMENT/lib/sdcc_ix /opt/z88dk/libsrc/_DEVELOPMENT/lib/sdcc_ix
COPY --link --from=z88dkbuild /opt/z88dk/libsrc/_DEVELOPMENT/lib/sccz80 /opt/z88dk/libsrc/_DEVELOPMENT/lib/sccz80
COPY --link bashrc /home/builder/.bashrc

ENV Z88DK_PATH="/opt/z88dk" \
    PATH="/opt/z88dk/bin:/opt/nodejs/bin:/opt/sdcc-4.3.0/bin:/opt/hex2bin:/opt/cpm:/opt/sjasmplus:${PATH}" \
    ZCCCFG="/opt/z88dk/lib/config/"

WORKDIR /src/

LABEL Maintainer="Dean Netherton" \
      Description="Tool chain to build units for the yellow msx for rc2014 kits"
