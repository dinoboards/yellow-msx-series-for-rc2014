SHELL := /bin/bash
.SHELLFLAGS := -eu -o pipefail -c
.ONESHELL:
MAKEFLAGS += --warn-undefined-variables
MAKEFLAGS += --no-builtin-rules

.PHONY: msx
msx:
	$(MAKE) -C msx

.PHONY: apps-rc2014
apps-rc2014:
	$(MAKE) -C apps-rc2014

.PHONY: release-notes
release-notes:
	@mkdir -p ./release
	version=$$(date +%y-%m-%d)
	relfile=./release/release-$${version}.md
	rm -f $${relfile}
	echo -e "## Release Notes\r\n\r\n" >> $${relfile}
	echo -e "### CBIOS:\r\n" >> $${relfile}
	(cd msx/cbios && git log --pretty=format:"%cd: %s" --date=short d714defae18cc6f3019638a8c7cf6c1d86e33ead..HEAD >> ../.$${relfile})
	echo -e "\r\n\r\n## NEXTOR/MSX-DOS:\r\n" >> $${relfile}
	(cd msx/nextor && git log --pretty=format:"%cd: %s" --date=short 5cd1f2a362671c7c2a51aa9a1564a1e620989eba..HEAD >> ../.$${relfile})
	echo -e "\r\n\r\n### MSXSYSSRC:\r\n" >> $${relfile}
	(cd msx/msxsys-build/msxsyssrc && git log --pretty=format:"%cd: %s" --date=short 7474c7eaa47379620eb029107735652af11c3270..HEAD >> ../../.$${relfile})
	echo -e "\r\n\r\n### MAIN:\r\n" >> $${relfile}
	git log --pretty=format:"%cd: %s" --date=short >> $${relfile}

.PHONY: release
release: release-notes
	version=$$(date +%y-%m-%d)
	$(MAKE) release -C msx --no-print-directory
	$(MAKE) release -C apps-rc2014 --no-print-directory
	pwd
	gh release create --draft -F ./release/release-$${version}.md -t \"$${version}\" $${version} release/*
