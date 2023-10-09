SHELL := /bin/bash
.SHELLFLAGS := -eu -o pipefail -c
.ONESHELL:
MAKEFLAGS += --warn-undefined-variables
MAKEFLAGS += --no-builtin-rules

BIN = ../bin/
ZCC := zcc +cpm -compiler=sdcc --vc -Cs --Werror -lmath32 -Cc-D__MATH_MATH32 -D__MATH_MATH32 -pragma-define:CLIB_32BIT_FLOAT=1 -create-app

APPS := dots lines mbrot control msxkeyb rtctest

all: $(addsuffix .com,$(APPS))

$(BIN)dots.com: dots.c v9958.c v9958.h v9958.asm config_request.c config_request.h cpm.asm cpm.h
$(BIN)lines.com: lines.c v9958.c v9958.c v9958.asm config_request.c config_request.h cpm.asm cpm.h
$(BIN)mbrot.com: mbrot.c v9958.c v9958.c v9958.asm config_request.c config_request.h cpm.asm cpm.h
$(BIN)control.com: control.c ym2149.asm ym2149.h cpm.asm cpm.h
$(BIN)msxkeyb.com: msxkeyb.c msxkeyb.asm  cpm.asm cpm.h
$(BIN)rtctest.com: rtctest.c rp5c01.asm rp5c01.h cpm.asm cpm.h

TARGETS := $(addsuffix .com,$(addprefix $(BIN),$(APPS)))

%.com: ../bin/%.com
	@

$(TARGETS):
	@mkdir -p $(BIN)
	$(ZCC) $(filter-out %.h,$^) -o $(notdir $@)
	mv $(call uppercase,$(notdir $@)) "$@"
	rm -f *.com
	echo "Compiled $(notdir $@) from $(filter-out %.h,$^)"

release:
	@version=$$(date +%y-%m-%d)
	mkdir -p ../release
	rm -f ../release/romwbw-apps-$${version}.zip
	zip -j ../release/romwbw-apps-$${version}.zip ../bin/*
	cd ../bin && tar -czvf ../release/romwbw-apps-$${version}.tar.gz *

.PHONY: format
format: SHELL:=/bin/bash
format:
	@find \( -name "*.c" -o -name "*.h" \) -exec echo "formating {}" \; -exec clang-format -i {} \;



# A macro for converting a string to uppercase
uppercase_TABLE:=a,A b,B c,C d,D e,E f,F g,G h,H i,I j,J k,K l,L m,M n,N o,O p,P q,Q r,R s,S t,T u,U v,V w,W x,X y,Y z,Z

define uppercase_internal
$(if $1,$$(subst $(firstword $1),$(call uppercase_internal,$(wordlist 2,$(words $1),$1),$2)),$2)
endef

define uppercase
$(eval uppercase_RESULT:=$(call uppercase_internal,$(uppercase_TABLE),$1))$(uppercase_RESULT)
endef
