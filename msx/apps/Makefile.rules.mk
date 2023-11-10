
include depends.d

define assemble
	@mkdir -p $(dir $@)
	$(ZCC) --compile-only $< -o $@
	echo "Assembled $(notdir $@) from $(notdir $<)"
endef

define buildlib
	@mkdir -p $(dir $@)
	z88dk-z80asm -x$@ $<
	echo "Packaged $(notdir $@) from $(notdir $<)"
endef

define compile
	@mkdir -p $(dir $@)
	$(ZCC) --c-code-in-asm --assemble-only $< -o $@
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
app_file = $(call objects, $(call sources,$(1))) $(call ifexists,$1.c)

.PRECIOUS: $(BIN)%.c.asm
.PRECIOUS: %.o
.PRECIOUS: $(BIN)%.o
$(BIN)%.c.asm: %.c; $(compile)
$(BIN)%.o: $(BIN)%.asm; $(assemble)
$(BIN)%.o: %.asm; $(assemble)
$(BIN)%.m4.o: %.asm.m4; $(assemble)
%.lib: %.o; $(buildlib)

$(BIN)%.com:
	@mkdir -p $(dir $@)
	$(eval $(notdir $@).crt_enable_commandline ?= 0)
	$(ZCC) -pragma-define:CRT_ENABLE_COMMANDLINE=$($(notdir $@).crt_enable_commandline) $(filter-out %.reloc,$(filter-out %.sys,$(filter-out %.inc,$(filter-out %.lib,$^)))) $(patsubst %,-l%,$(filter %.lib,$^)) -o $@
	filesize=$$(stat -c%s "$@")
	echo "Linked $(notdir $@) ($$filesize bytes)"

$(BIN)%.sys:
	@mkdir -p $(dir $@)
	$(eval $(notdir $@).crt_enable_commandline ?= 0)
	$(ZCC) -pragma-define:CRT_ENABLE_COMMANDLINE=0 $(filter-out %.init,$(filter-out %.reloc,$(filter-out %.body,$(filter-out %.inc,$(filter-out %.lib,$^))))) $(patsubst %,-l%,$(filter %.lib,$^)) -o $@
	filesize=$$(stat -c%s "$@")
	echo "Linked $(notdir $@) ($$filesize bytes)"

define relocatable =
$(BIN)sys/$1/body/$1.crtbase1000.asm: sys/$1/body/crt.asm
	@mkdir -p $$(dir $$@)
	echo -e "\tORG\t\$$$$1000\n\n" > $$@
	cat $$^ >> $$@

$1.sys.crt_enable_commandline:=1

$(BIN)sys/$1/body/$1.reloc: $(BIN)sys/$1/body/$1.body

$(BIN)$1.sys: $(addprefix $(BIN)sys/$1/body/, import.asm $1.body $1.reloc $1.def.lib) $($1.init)

$(BIN)sys/$1/body/$1.body: $($1.body)

endef


$(BIN)%.body: bin/%.crtbase1000.asm
	@$(MAKE) $(BIN)extract-reloc-table.js --no-print-directory -s
	mkdir -p $(dir $@)
	$(ZCC_FOR_sys) -crt0 ./sys/$(notdir $(basename $@))/body/crt.asm $(filter-out %.crtbase1000.asm,$(filter-out %/crt.o,$(filter-out %.reloc,$(filter-out %.sys,$(filter-out %.inc,$(filter-out %.lib,$^))))) $(patsubst %,-l%,$(filter %.lib,$^))) -o $@
	$(ZCC_FOR_sys) -crt0 $(firstword $^) $(filter-out %.crtbase1000.asm,$(filter-out %/crt.o,$(filter-out %.reloc,$(filter-out %.sys,$(filter-out %.inc,$(filter-out %.lib,$^))))) $(patsubst %,-l%,$(filter %.lib,$^))) -o $@.base1000
	node $(BIN)extract-reloc-table.js $@ $@.base1000 "$(basename $@).reloc"
	reloc_filesize=$$(stat -c%s "$(basename $@).reloc")
	echo "Reloc info $(basename $@).reloc ($$reloc_filesize bytes)"
	filesize=$$(stat -c%s "$@")
	echo "Linked $(notdir $@) ($$filesize bytes)"

.PRECIOUS: $(BIN)sys/%.def.asm
$(BIN)sys/%.def.asm: $(BIN)sys/%.body sys/%.exports
	@mkdir -p $(dir $@)
	echo "" > $@
	while IFS= read -r line
	do
		name=$$(echo "$$line" | cut -f2 -d' ')
		if grep -Fxq "$$name" $(lastword $^); then
			echo -e "\tPUBLIC\t$$name" >> $@
			echo -e $$line >> $@
		fi
	done < "$(patsubst %.body,%.def,$<)"
	echo "Exported symbols from $(notdir $^) to $(notdir $@)"

$(BIN)sys/%/body/import.asm:
	@mkdir -p  $(dir $@)
	echo "	SECTION	DATA" >$@
	echo "	PUBLIC	_relocation_table_start" >>$@
	echo "	PUBLIC	_relocation_table_end" >>$@
	echo "	PUBLIC	_sys_start" >>$@
	echo "	PUBLIC	_sys" >>$@
	echo "	PUBLIC  _sys_end" >>$@
	echo "_relocation_table_start:" >>$@
	echo "	BINARY \"./$(*F).reloc\"" >>$@
	echo "_relocation_table_end:" >>$@
	echo "_sys_start:" >>$@
	echo "_sys:" >>$@
	echo "	BINARY \"./$(*F).body\"" >>$@
	echo "_sys_end:" >>$@

ZSDCPP_FLAGS=-iquote"." -isystem"${ZCCCFG}/../../include/_DEVELOPMENT/sdcc" $(LIBS)

deps:
	@echo "" > ./depends.d
	# C Dependencies
	find -name "*.c" -not -path "./chip8/*" | while read -r file; do
		file_no_ext="$${file%.*}"
		file_no_ext=$${file_no_ext#./}
		filename=$$(basename $$file_no_ext)
		from="$$filename.o"
		to="$(BIN)$$file_no_ext.c.asm"
		sdcpp ${ZSDCPP_FLAGS} -MM -MF /tmp/deps.deps $$file
		sed "s+$$from+$$to+g" /tmp/deps.deps >> ./depends.d
	done

	# ASM include dependencies
	find -name "*.asm" -not -path "./bin/*" -not -path "./chip8/*" | while read -r file; do
		file_no_ext="$${file%.*}"
	 	file_no_ext=$${file_no_ext#./}
		printf "$(BIN)$$file_no_ext.o:" >> ./depends.d
		printf " $$file" >> ./depends.d
		sed -n '/include/s/^[\t ]*include[\t ]*"\{0,1\}\(.*\)".*$$/\1/p' < $$file | while read -r depfile; do
			depfilepath=$$(find -iname $$depfile -not -path "./chip8/*")
			printf " $$depfilepath" >> ./depends.d
		done
		echo "" >> ./depends.d
	done

	# ASM include dependencies
	find -name "*.asm.m4" -not -path "./chip8/*" | while read -r file; do
		file_no_ext="$${file%.*}"
	 	file_no_ext=$${file_no_ext#./}
		file_no_ext="$(BIN)$${file_no_ext%.*}"
		printf "$$file_no_ext.o:" >> ./depends.d
		printf " $$file" >> ./depends.d
		sed  -n '/include/s/^[\t ]*include(.\(.*\)'\'')/\1/p' < $$file | while read -r depfile; do
			depfilepath=$$(find -iname $$depfile -not -path "./chip8/*")
			printf " $$depfilepath" >> ./depends.d
		done
		sed -n '/include/s/^[\t ]*include[\t ]*"\{0,1\}\(.*\)".*$$/\1/p' < $$file | while read -r depfile; do
			depfilepath=$$(find -iname $$depfile -not -path "./chip8/*")
			printf " $$depfilepath" >> ./depends.d
		done
		echo "" >> ./depends.d
	done

	echo "./depends.d created"

$(BIN)extract-reloc-table.js:
	@cat <<EOT > $(BIN)extract-reloc-table.js
	const fs = require('fs')
	const util = require('util')
	const childProcess = require('child_process')
	const path = require('path')
	const os = require('os')

	function convertBitmapByte(hits, index) {
		let byte = 0
		for(let i = 0; i < 8; i++) {
			const hit = hits.some(x => x === index + i)
			if (hit)
				byte |= Math.pow(2, 7 - i)
		}

		return byte
	}

	function convertToArrayOfUint16(hits) {
		const data = Buffer.alloc(hits.length * 2)

		for(let i = 0; i < hits.length; i++) {
			data[i*2] = hits[i] & 255
			data[(i*2) + 1] = hits[i] >> 8
		}

		return data
	}

	async function main(fileAt100, fileAt200, relocationFileName) {
		const data1 = fs.readFileSync(fileAt100)
		const data2 = fs.readFileSync(fileAt200)

		if (data1.length != data2.length)
			throw new Error("File length are different")

		const hits = []
		for(let i=0; i < data1.length; i++) {
			if (data1[i] !== data2[i]) {
				hits.push(i)
			}
		}

		const data = convertToArrayOfUint16(hits, data2.length)

		fs.writeFileSync(relocationFileName, data)
	}

	main(...[...process.argv].slice(2)).catch(err => {
		console.log(err.stack)
		console.log("");
		process.exit(1)
	})
	EOT

