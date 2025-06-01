const std = @import("std");
const this = @This();
const rl = @import("raylib");

pub fn build(b: *std.Build) void {
    const target = b.standardTargetOptions(.{});
    const optimize = b.standardOptimizeOption(.{});

    const raylib = b.dependency("raylib", .{
        .target = target,
        .optimize = optimize,
    });

    const raylib_artifact = raylib.artifact("raylib");

    const exe_mod = b.createModule(.{
        .root_source_file = b.path("src/zigmain.zig"),
        .target = target,
        .optimize = optimize,
    });

    const exe = b.addExecutable(.{
        .name = "baurifontmaker",
        .root_module = exe_mod,
    });

    if (optimize == .Debug and target.result.os.tag != .windows) {
        exe.use_lld = false;
    }

    exe.addIncludePath(b.path("src/include/"));
    exe.addCSourceFiles(.{ .files = &.{
        "src/gui.c",
        "src/fontvalue.c",
    } });
    exe_mod.linkLibrary(raylib_artifact);
    exe_mod.link_libc = true;
    exe.linkLibC();
    exe.linkLibrary(raylib_artifact);

    b.installArtifact(exe);

    const run_cmd = b.addRunArtifact(exe);

    run_cmd.step.dependOn(b.getInstallStep());

    if (b.args) |args| {
        run_cmd.addArgs(args);
    }

    const run_step = b.step("run", "Run the app");
    run_step.dependOn(&run_cmd.step);
}
