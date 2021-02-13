SHELL := /bin/bash
.SHELLFLAGS := -eu -o pipefail -c
.ONESHELL:
MAKEFLAGS += --warn-undefined-variables
MAKEFLAGS += --no-builtin-rules

BIN = ../bin/
ZCC := zcc +cpm -clib=sdcc_iy -SO3 -lmath32 -Cc-D__MATH_MATH32 -D__MATH_MATH32 -pragma-define:CLIB_32BIT_FLOAT=1 --max-allocs-per-node200000 -create-app

APPS := dots lines mbrot control

all: $(addsuffix .com,$(APPS))

$(BIN)dots.com: dots.c v9958.c v9958.h v9958.asm config_request.c config_request.h
$(BIN)lines.com: lines.c v9958.c v9958.c v9958.asm config_request.c config_request.h
$(BIN)mbrot.com: mbrot.c v9958.c v9958.c v9958.asm config_request.c config_request.h
$(BIN)control.com: control.c ym2149.asm ym2149.h

TARGETS := $(addsuffix .com,$(addprefix $(BIN),$(APPS)))

%.com: ../bin/%.com
	@

$(TARGETS):
	@mkdir -p $(BIN)
	$(ZCC) $(filter-out %.h,$^) -o $(notdir $@)
	mv $(call uppercase,$(notdir $@)) "$@"
	rm *.bin
	echo "Compiled $(notdir $@) from $(filter-out %.h,$^)"






# A macro for converting a string to uppercase
uppercase_TABLE:=a,A b,B c,C d,D e,E f,F g,G h,H i,I j,J k,K l,L m,M n,N o,O p,P q,Q r,R s,S t,T u,U v,V w,W x,X y,Y z,Z

define uppercase_internal
$(if $1,$$(subst $(firstword $1),$(call uppercase_internal,$(wordlist 2,$(words $1),$1),$2)),$2)
endef

define uppercase
$(eval uppercase_RESULT:=$(call uppercase_internal,$(uppercase_TABLE),$1))$(uppercase_RESULT)
endef
