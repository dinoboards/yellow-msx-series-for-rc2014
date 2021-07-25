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
	@rm -rf ./release
	mkdir -p ./release
	version=$$(date +%y-%m-%d)
	last_release=$$(gh release list --limit 1 | grep Latest | cut -f1 | tr -d \")
	echo -e $${last_release}
	relfile=./release/release-$${version}.md
	rm -f $${relfile}
	echo -e "## Release Notes\r\n\r\n" >> $${relfile}
	echo -e "\`\`\`" >> $${relfile}
	git log --pretty=format:"%cd: %s" --date=short --submodule=short $${last_release}..HEAD >> $${relfile}
	echo -e "\r\n\`\`\`" >> $${relfile}

.PHONY: release
release: release-notes
	version=$$(date +%y-%m-%d)
	$(MAKE) release -C msx --no-print-directory
	$(MAKE) release -C apps-rc2014 --no-print-directory
	gh release create --draft -F ./release/release-$${version}.md -t \"$${version}\" $${version} release/*
