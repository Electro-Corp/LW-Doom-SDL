{ pkgs }: {
    deps = [
        pkgs.openbox
        pkgs.xorg.libX11
        pkgs.xorg.libXext
        pkgs.libnsl
        pkgs.gdb
        pkgs.SDL
    ];
}