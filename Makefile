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
	./build/baurifontmaker/baurifontmaker

.PHONY: csetup
csetup:
	cmake -S . -B build
	make cbuild

.PHONY: fmt
fmt:
	@clang-format -i -style=file --verbose src/*.c src/windows/*.c src/include/*.h src/include/windows/*.h

.PHONY:valgrind
valgrind: cbuild
	valgrind --leak-check=full --show-leak-kinds=all ./build/baurifontmaker/baurifontmaker > valgrind.log 2>&1
