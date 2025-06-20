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
        .name = "bauriglyphmaker",
        .root_module = exe_mod,
    });

    if (optimize == .Debug and target.result.os.tag != .windows) {
        exe.use_lld = false;
    }

    if (target.result.os.tag == .windows) {
        exe_mod.addCMacro("BFM_OS_WIN", "");
        //exe_mod.addCMacro("UNICODE", "");
        //exe_mod.addCMacro("_UNICODE", "");
        exe_mod.linkSystemLibrary("comdlg32", .{});
        exe_mod.linkSystemLibrary("ole32", .{});
    } else if (target.result.os.tag == .linux) {
        exe_mod.addCMacro("BFM_OS_LINUX", "");
    } else if (target.result.os.tag == .macos) {
        exe_mod.addCMacro("BFM_OS_WIN", "");
    }

    exe.addIncludePath(b.path("src/include/"));
    exe.addIncludePath(b.path("src/external/"));
    exe.addIncludePath(b.path("src/external/stb"));
    exe.addIncludePath(b.path("src/external/tinyfiledialogs/"));

    exe.addCSourceFiles(.{
        .files = &.{
            "src/config.c",
            "src/glyph.c",
            "src/glyphitem.c",
            "src/gui.c",
            "src/saver.c",
            "src/uiconfig.c",
            "src/uistates.c",
            "src/utils.c",

            //codegen
            "src/codegen/cheader.c",

            //converters
            "src/converters/bdf.c",

            //widgets
            "src/widgets/canvas.c",
            "src/widgets/filedialog.c",
            "src/widgets/itemselector.c",
            "src/widgets/toolbar.c",

            //windows
            "src/windows/edititem.c",
            "src/windows/export.c",
            "src/windows/infoedit.c",
            "src/windows/newitem.c",
            "src/windows/settings.c",

            //externals / third_party
            "src/external/tinyfiledialogs/tinyfiledialogs.c",
        },
        .flags = &.{"-g3"},
    });

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
