
APPS := dots lines mbrot cpusptst fdisk vramtest extbio rs232tst fosiltst term fossilon xrecv rtccalb esp8266 chip8 chip8asm romflash muflash
APP_TARGETS := $(addsuffix .com,$(addprefix ./bin/,$(APPS)))

.PHONY: apps
apps: $(APP_TARGETS)

.PHONY: $(APP_TARGETS)
$(APP_TARGETS):
	@mkdir -p bin
	$(MAKE) "$@" --no-print-directory -j -C apps
	cp -up ./apps/$@ ./bin/

.PHONY: format
format:
	$(MAKE) -C apps format
