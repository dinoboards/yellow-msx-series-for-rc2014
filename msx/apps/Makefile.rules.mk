define assemble
	@mkdir -p $(dir $@)
	$(ZCC) --compile-only $< -o $@
	echo "Assembled $(notdir $@) from $(notdir $<)"
endef

define compile
	@mkdir -p $(dir $@)
	$(ZCC) --c-code-in-asm  --assemble-only $< -o $@
	echo "Compiled $(notdir $@) from $(notdir $<)"
endef

define link
	@mkdir -p $(dir $@)
	$(ZCC) $^ -o $@
	echo "Compiled $(notdir $@) from $(notdir $^)"
endef

define objects
 $(patsubst %.asm,%.o,$(patsubst %.c,%.c.o,$(1)))

endef

define sources
 $(wildcard $1/*.c) $(wildcard $1/*.asm)

endef

app_files = $(call objects,$(2) $(call sources,$(1)))

.PRECIOUS: $(BIN)%.c.asm
$(BIN)%.c.asm: %.c; $(compile)
$(BIN)%.o: $(BIN)%.asm; $(assemble)
$(BIN)%.o: %.asm; $(assemble)
$(BIN)%.m4.o: %.asm.m4; $(assemble)

$(BIN)%.com:
	$(link)
