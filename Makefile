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

.PHONY: release-notes
release-notes:
	@rm -rf ./release
	mkdir -p ./release
	version=$$(date +%y-%m-%d)
	last_release=$$(gh release list --limit 10 | grep Latest | cut -f1 | tr -d \")
	echo -e $${last_release}
	relfile=./release/release-$${version}.md
	rm -f $${relfile}
	echo -e "## Release Notes\r\n\r\n" >> $${relfile}
	echo -e "\`\`\`" >> $${relfile}
	git log --pretty=format:"%cd: %s" --date=short --submodule=short $${last_release}..HEAD >> $${relfile}
	echo -e "\r\n\`\`\`" >> $${relfile}

.PHONY: release
release: release-build release-notes
	@version=$$(date +%y-%m-%d)
	$(MAKE) release -C msx --no-print-directory
	$(MAKE) release -C apps-rc2014 --no-print-directory
	if gh release list | grep Draft; then
		current_draft_release=$$(gh release list | grep Draft | cut -f3 | head -n1)
		gh release delete --yes $${current_draft_release}
	fi
	gh release create --draft -F ./release/release-$${version}.md -t "$${version}" $${version} release/*

.PHONY: release-build
release-build:
	$(MAKE) -C msx clean
	RELEASE=true $(MAKE) -C msx jeds
	RELEASE=true $(MAKE) -C msx/apps -j
	RELEASE=true $(MAKE) -C msx apps
	RELEASE=true $(MAKE) -C msx VERSION=$$(date +%Y-%m-%d)
	RELEASE=true $(MAKE) -C apps-rc2014
