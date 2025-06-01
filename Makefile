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
