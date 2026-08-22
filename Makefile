# Makefile — CSE-LinAlg: vectors over a component field, generic over cse_field. The library depends only
# on spine + CSE-DSA + the CSE-Field interface; the test wires CSE-Arith.
#
# Build the deps first: make -C ../CardinalSlate lib && make -C ../CSE-DSA lib && make -C ../CSE-Arith lib

SPINE ?= ../CardinalSlate
DSA   ?= ../CSE-DSA
FIELD ?= ../CSE-Field
ARITH ?= ../CSE-Arith
CC    ?= clang
CFLAGS := -std=c11 -Iinclude -I$(FIELD)/include -I$(DSA)/include -I$(SPINE)/include -O2 -Wall -Wextra
TFLAGS := $(CFLAGS) -I$(ARITH)/include -I$(ARITH)/kernel

OUT     := build
HDRS    := include/cse/linalg.h
SRCS    := $(wildcard src/*.c)
OBJS    := $(patsubst src/%.c,$(OUT)/%.o,$(SRCS))

.PHONY: all check clean lib
all: check lib

$(OUT):
	@mkdir -p $(OUT)

$(OUT)/types.stamp: $(HDRS) $(SRCS) | $(OUT)
	@bad=$$(grep -rnE '\b(int|long|short|size_t|unsigned|char|bool|float|double)\b|void[[:space:]]*\*|stdint' include src 2>/dev/null || true); \
	  if [ -n "$$bad" ]; then printf "  %-10s C TYPE FOUND\n" "linalg:"; printf '%s\n' "$$bad" | sed 's/^/    /'; exit 1; \
	  else printf "  %-10s only slate · generic over a field\n" "linalg:"; fi; touch $@

$(OUT)/standalone.stamp: $(HDRS) | $(OUT)
	@for h in $(HDRS); do rel=$${h#include/}; printf '#include "%s"\nint main(void){return 0;}\n' "$$rel" > $(OUT)/one.c; \
	  $(CC) $(CFLAGS) -fsyntax-only $(OUT)/one.c || exit 1; done; touch $@

$(OUT)/%.o: src/%.c $(HDRS) | $(OUT)
	@$(CC) $(CFLAGS) -c $< -o $@

lib: $(OUT)/libcse-linalg.a
$(OUT)/libcse-linalg.a: $(OBJS) | $(OUT)
	@ar rcs $@ $(OBJS)

$(OUT)/test_linalg: tests/linalg.c $(OBJS) | $(OUT)
	@$(CC) $(TFLAGS) tests/linalg.c $(OBJS) $(ARITH)/build/libcse-arith.a $(DSA)/build/libcse-dsa.a -o $@

check: $(OUT)/types.stamp $(OUT)/standalone.stamp $(OUT)/test_linalg
	@echo "== cse-linalg =="; out=$$($(OUT)/test_linalg 2>&1); st=$$?; \
	  if [ $$st -ne 0 ] || printf '%s' "$$out" | grep -q FAIL; then printf '%s\n' "$$out" | sed 's/^/  /'; exit 1; \
	  else printf '%s\n' "$$out" | sed 's/^/  /'; echo "== ALL PASS =="; fi

clean:
	@rm -rf $(OUT)
