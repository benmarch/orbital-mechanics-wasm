SRCS=${wildcard src/wasm/*.cpp}
INC=include/

TARGET=build/om.js
TARGETS=$(TARGET) build/om.wasm

all: om.js

om.js: $(SRCS)
		em++ -s WASM=1 -s ALLOW_MEMORY_GROWTH=1 \
		-s DEMANGLE_SUPPORT=1 --bind -Wall \
		-O3 -DNDEBUG \
		-s EXPORTED_FUNCTIONS="[]" \
		-I$(INC) \
		-o $(TARGET) $(SRCS)
		make copy

.PHONY: clean copy
clean:
	rm $(TARGETS)

copy:
	cp -R src/web/ build/
