const gui = @cImport({
    @cInclude("gui.h");
});

pub fn main() !void {
    _ = gui.GuiMain();
}
