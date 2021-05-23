
.PHONY: bin/telnet.com
bin/telnet.com:
	@$(MAKE) bin/telnet.com --no-print-directory -C apps
	cp -up ./apps/bin/telnet.com ./bin/

APPS := hello dots lines mbrot spike-fdd ide cpusptst fdisk vramtest extbio
APP_TARGETS := $(addsuffix .com,$(addprefix ./bin/,$(APPS)))

.PHONY: apps
apps: $(APP_TARGETS) bin/telnet.com

.PHONY: $(APP_TARGETS)
$(APP_TARGETS):
	@mkdir -p bin
	$(MAKE) "$@" --no-print-directory -C apps
	cp -up ./apps/"$@" ./bin/

.PHONY: format
format: SHELL:=/bin/bash
format:
	@cd apps
	find \( -name "*.c" -o -name "*.h" \) -exec echo "formating {}" \; -exec clang-format -i {} \;

