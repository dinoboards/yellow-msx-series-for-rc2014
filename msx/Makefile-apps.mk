
APPS := dots lines mbrot cpusptst fdisk vramtest extbio rs232tst fosiltst term fossil xrecv2 rtccalb helloworld
APP_TARGETS := $(addsuffix .com,$(addprefix ./bin/,$(APPS)))

.PHONY: apps
apps: $(APP_TARGETS)

.PHONY: $(APP_TARGETS)
$(APP_TARGETS):
	@mkdir -p bin
	$(MAKE) "$@" --no-print-directory -C apps
	cp -up ./apps/$@ ./bin/

.PHONY: format
format:
	$(MAKE) -C apps format
