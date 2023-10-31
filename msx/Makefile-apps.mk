

.PHONY: apps
apps:
	@mkdir -p bin
	$(MAKE) --no-print-directory -j -C apps
	cp -up ./apps/bin/*.com ./bin/
	cp -up ./apps/bin/*.sys ./bin/

.PHONY: format
format:
	$(MAKE) -C apps format
