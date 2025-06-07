BIN:=baurifontmaker
CMAKE_OUTPUT:=build/$(BIN)/$(BIN)
ZIG_OUTPUT:=zig-out/bin/$(BIN)

.PHONY: zbuild
zbuild:
	zig build

.PHONY: zrun
zrun:
	zig build run

.PHONY: cbuild
cbuild:
	cmake --build build

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

.PHONY:valgrind_z
valgrind_z: zbuild
	valgrind --leak-check=full --track-origins=yes --show-leak-kinds=all $(ZIG_OUTPUT) > valgrind.log 2>&1
