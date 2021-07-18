
APPS := dots lines mbrot cpusptst fdisk vramtest extbio rs232tst fosiltst telnet fossil xrecv2 helloworld
APP_TARGETS := $(addsuffix .com,$(addprefix ./bin/,$(APPS)))

.PHONY: apps
apps: $(APP_TARGETS)

.PHONY: $(APP_TARGETS)
$(APP_TARGETS):
	@mkdir -p bin
	$(MAKE) "$@" --no-print-directory -C apps
	cp -up ./apps/$@ ./bin/

.PHONY: format
format: SHELL:=/bin/bash
format:
	@cd apps
	find \( -name "*.c" -o -name "*.h" \) -exec echo "formating {}" \; -exec clang-format -i {} \;

