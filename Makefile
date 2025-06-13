BIN:=baurifontmaker
CMAKE_OUTPUT:=build/$(BIN)/$(BIN)
ZIG_OUTPUT:=zig-out/bin/$(BIN)

all: crun

.PHONY: zbuild
zbuild:
	zig build

.PHONY: zrun
zrun:
	zig build run

.PHONY: cbuild
cbuild:
	cmake --build build

.PHONY: cbuild_rls
cbuild_rls:
	cmake --build build --config Release

.PHONY: crun
crun: cbuild
	./$(CMAKE_OUTPUT)

.PHONY: csetup
csetup:
	cmake -S . -B build
	make cbuild

.PHONY: fmt
fmt:
	@clang-format -i -style=file --verbose src/*.c src/windows/*.c src/include/*.h src/include/windows/*.h

.PHONY:valgrind
valgrind: cbuild
	valgrind --leak-check=full --track-origins=yes --show-leak-kinds=all $(CMAKE_OUTPUT) > valgrind.log 2>&1

.PHONY: massif
massif: cbuild
	valgrind --tool=massif $(CMAKE_OUTPUT)

.PHONY: perf
perf:
	perf record -g -F 999 ./$(CMAKE_OUTPUT)
	perf script -F +pid > baurifontmaker.perf

.PHONY:valgrind_z
valgrind_z: zbuild
	valgrind --leak-check=full --track-origins=yes --show-leak-kinds=all $(ZIG_OUTPUT) > valgrind.log 2>&1

.PHONY: infer
infer: clean
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


.PHONY: cmake_clean
cmake_clean:
	make clean -C build

.PHONY: clean
clean: cmake_clean
	rm -rf .zig-cache
	rm -rf zig-out
