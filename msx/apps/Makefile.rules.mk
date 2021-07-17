
include depends.d

define assemble
	@mkdir -p $(dir $@)
	$(ZCC) --compile-only $< -o $@
	echo "Assembled $(notdir $@) from $(notdir $<)"
endef

define compile
	@mkdir -p $(dir $@)
	$(ZCC) --assemble-only $< -o $@
	echo "Compiled $(notdir $@) from $(notdir $<)"
endef

define objects
 $(patsubst %.asm,%.o,$(patsubst %.c,%.c.o,$(1)))

endef

define sources
 $(wildcard $1/*.c) $(wildcard $1/*.asm)

endef

ifexists = $(if $(wildcard $1),$(patsubst %.c,%.c.o,$1),)

app_files = $(call objects,$(2) $(call sources,$(1))) $(call ifexists,$1.c)

.PRECIOUS: $(BIN)%.c.asm
$(BIN)%.c.asm: %.c; $(compile)
$(BIN)%.o: $(BIN)%.asm; $(assemble)
$(BIN)%.o: %.asm; $(assemble)
$(BIN)%.m4.o: %.asm.m4; $(assemble)

$(BIN)%.com:
	@mkdir -p $(dir $@)
	echo $(eval $(notdir $@).crt_enable_commandline ?= 0)
	$(ZCC) -pragma-define:CRT_ENABLE_COMMANDLINE=$($(notdir $@).crt_enable_commandline) $^ -o $@
	echo "Linked $(notdir $@) from $(notdir $^)"


ZSDCPP_FLAGS=-iquote"." -isystem"${ZCCCFG}/../../include/_DEVELOPMENT/sdcc" -I./fdisk/ -I./telnet/ -I./fusion/

deps:
	@echo "" > ./depends.d
	# C Dependencies
	find -name "*.c" | while read -r file; do
		file_no_ext="$${file%.*}"
		file_no_ext=$${file_no_ext#./}
		filename=$$(basename $$file_no_ext)
		from="$$filename.rel"
		to="$(BIN)$$file_no_ext.c.asm"
		zsdcpp ${ZSDCPP_FLAGS} -MM -MF /tmp/deps.deps $$file
		sed "s+$$from+$$to+g" /tmp/deps.deps >> ./depends.d
	done

	# ASM include dependencies
	find -name "*.asm" -not -path "./bin/*" | while read -r file; do
		file_no_ext="$${file%.*}"
	 	file_no_ext=$${file_no_ext#./}
		printf "$(BIN)$$file_no_ext.o:" >> ./depends.d
		printf " $$file" >> ./depends.d
		sed -n '/include/s/^[\t ]*include[\t ]*"\{0,1\}\(.*\)".*$$/\1/p' < $$file | while read -r depfile; do
			depfilepath=$$(find -iname $$depfile)
			printf " $$depfilepath" >> ./depends.d
		done
		echo "" >> ./depends.d
	done

	# ASM include dependencies
	find -name "*.asm.m4" | while read -r file; do
		file_no_ext="$${file%.*}"
	 	file_no_ext=$${file_no_ext#./}
		file_no_ext="$(BIN)$${file_no_ext%.*}"
		printf "$$file_no_ext.o:" >> ./depends.d
		printf " $$file" >> ./depends.d
		sed  -n '/include/s/^[\t ]*include(.\(.*\)'\'')/\1/p' < $$file | while read -r depfile; do
			depfilepath=$$(find -iname $$depfile)
			printf " $$depfilepath" >> ./depends.d
		done
		sed -n '/include/s/^[\t ]*include[\t ]*"\{0,1\}\(.*\)".*$$/\1/p' < $$file | while read -r depfile; do
			depfilepath=$$(find -iname $$depfile)
			printf " $$depfilepath" >> ./depends.d
		done
		echo "" >> ./depends.d
	done

	echo "./depends.d created"
