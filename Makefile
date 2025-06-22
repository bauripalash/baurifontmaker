BIN:=bauriglyphmaker
CMAKE_OUTPUT:=build/$(BIN)/$(BIN)
ZIG_OUTPUT:=zig-out/bin/$(BIN)
HEADERS:= $(shell find src/include -path 'src/external' -prune -o -path 'src/include/exported' -prune -o -name '*.h' -print)
SOURCES:= $(shell find src/ -path 'src/external' -prune -o -name '*.c' -print)

all: run

.PHONY: build
build:
	cmake --build build

.PHONY: build_rls
build_rls:
	cmake --build build --config Release

.PHONY: run
run: build
	./$(CMAKE_OUTPUT)

.PHONY: cmake_setup
cmake_setup:
	cmake -S . -B build
	make build

.PHONY: zbuild
zbuild:
	zig build

.PHONY: zrun
zrun:
	zig build run


.PHONY: fmt
fmt:
	@clang-format -i -style=file --verbose $(SOURCES) $(HEADERS)

.PHONY:valgrind
valgrind: build
	valgrind --leak-check=full --track-origins=yes --show-leak-kinds=all $(CMAKE_OUTPUT) > valgrind.log 2>&1

.PHONY: massif
massif: build
	valgrind --tool=massif $(CMAKE_OUTPUT)

.PHONY: perf
perf:
	perf record -g -F 999 ./$(CMAKE_OUTPUT)
	perf script -F +pid > bauriglyphmaker.perf

.PHONY:valgrind_z
valgrind_z: zbuild
	valgrind --leak-check=full --track-origins=yes --show-leak-kinds=all $(ZIG_OUTPUT) > valgrind.log 2>&1

.PHONY: infer
infer: cmake_clean
	infer run --compilation-database build/compile_commands.json

.PHONY: cppcheck
cppcheck:
	cppcheck \
		--project=build/compile_commands.json \
		--template=gcc\
		--enable=all --force --inline-suppr \
		-Dfalse=0 \
		--suppressions-list=cppcheck-suppress.txt \
		-ibuild/_deps -isrc/include/ext 2>cppcheck-result.txt


.PHONY: print_sources
print_sources:
	@echo $(SOURCES)

.PHONY: cmake_clean
cmake_clean:
	make clean -C build

.PHONY: clean
clean: cmake_clean
	rm -rf .zig-cache
	rm -rf zig-out
