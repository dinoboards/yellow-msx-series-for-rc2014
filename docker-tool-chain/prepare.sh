#!/usr/bin/env /bin/bash

set -x
set -e

sed -i 's/http:\/\/archive\.ubuntu\.com\/ubuntu/http:\/\/au.archive.ubuntu.com\/ubuntu/g' /etc/apt/sources.list

apt update
apt upgrade -y

DEBIAN_FRONTEND="noninteractive" apt install -y --no-install-recommends \
  tzdata ca-certificates mtools dos2unix pasmo dosfstools clang-format m4 xxd gnupg2 software-properties-common zip \
  wget texlive-latex-base texlive-fonts-recommended texlive-latex-extra gpp lmodern git
wget -qO- https://dl.winehq.org/wine-builds/Release.key | apt-key add -
apt-key adv --keyserver hkp://keyserver.ubuntu.com:80 --recv F987672F
apt-add-repository 'deb https://dl.winehq.org/wine-builds/ubuntu/ focal main'

dpkg --add-architecture i386
apt update -y
apt install -y --no-install-recommends wine32 build-essential

ln -s /usr/lib/wine/wine /usr/bin/wine

wget https://github.com/jgm/pandoc/releases/download/3.1.11.1/pandoc-3.1.11.1-1-amd64.deb
dpkg -i ./pandoc-3.1.11.1-1-amd64.deb
rm ./pandoc-3.1.11.1-1-amd64.deb

apt remove -y --purge wget
apt autoremove -y
apt clean -y
rm -rf /var/lib/apt/lists/*

adduser --disabled-password --gecos "" builder
