SHELL := /bin/bash
.SHELLFLAGS := -eu -o pipefail -c
.ONESHELL:
MAKEFLAGS += --warn-undefined-variables
MAKEFLAGS += --no-builtin-rules

.PHONY: msx
msx:
	@$(MAKE) -C msx --no-print-directory

.PHONY: apps-rc2014
apps-rc2014:
	@$(MAKE) -C apps-rc2014 --no-print-directory

.PHONY: clean
clean:
	$(MAKE) -C msx clean

.PHONY: release-notes
release-notes:
	@rm -rf ./release
	mkdir -p ./release
	version=$$(date +%y-%m-%d)
	last_release=$$(git tag -l | sort | tail -n1)
	echo -e $${last_release}
	relfile=./release/release-$${version}.md
	rm -f $${relfile}
	echo -e "## Release Notes\r\n\r\n" >> $${relfile}
	echo -e "\`\`\`" >> $${relfile}
	git log --pretty=format:"%ad: %s" --date=short --submodule=short $${last_release}..HEAD >> $${relfile}
	echo -e "\r\n\`\`\`" >> $${relfile}

.PHONY: release-build-jeds
release-build-jeds:
	@version=$$(date +%y-%m-%d)
	RELEASE=true $(MAKE) -C msx jeds

.PHONY: release-build-msx-apps
release-build-msx-apps:
	@version=$$(date +%y-%m-%d)
	RELEASE=true $(MAKE) -C msx apps

.PHONY: release-build-msx-roms
release-build-msx-roms:
	@version=$$(date +%y-%m-%d)
	RELEASE=true $(MAKE) -C msx VERSION=$$(date +%Y-%m-%d)

.PHONY: release-build-rc2014-app
release-build-rc2014-apps:
	@version=$$(date +%y-%m-%d)
	RELEASE=true $(MAKE) -C apps-rc2014

.PHONY: release-build-package
release-build-package:
	@version=$$(date +%y-%m-%d)
	$(MAKE) release -C msx --no-print-directory

.PHONY: release
release:
	@$(MAKE) clean
	$(MAKE) release-notes
	$(MAKE) release-build-jeds
	$(MAKE) release-build-msx-apps
	$(MAKE) release-build-msx-roms
	$(MAKE) release-build-rc2014-apps
	$(MAKE) release-build-package
	echo gh release create --draft -F ./release/release-$${version}.md -t "$${version}" $${version} release/*

# Mark documents with DRAFT
.PHONY: draft-release
draft-release:
	@DRAFT=true $(MAKE) clean
	DRAFT=true $(MAKE) release-notes
	DRAFT=true $(MAKE) release-build-jeds
	DRAFT=true $(MAKE) release-build-msx-apps
	DRAFT=true $(MAKE) release-build-msx-roms
	DRAFT=true $(MAKE) release-build-package
	version=$$(date +%y-%m-%d)
	echo gh release create --draft -F ./release/release-$${version}.md -t "$${version}" $${version} release/*
