#!/bin/bash
TARGET_IP=${1:-"192.168.1.70"}
TARGET_USER=${2:-"ark"}
TARGET_PASS="ark"

sshpass -p "$TARGET_PASS" ssh -o StrictHostKeyChecking=no "$TARGET_USER@$TARGET_IP" "
cd /roms/ports/metroid_prime

MALI_DIR=\"/tmp/metroid_prime_mali\"
export LD_LIBRARY_PATH=\"\$MALI_DIR:/roms/ports/metroid_prime/libs.aarch64:/roms/ports/metroid_prime:\$LD_LIBRARY_PATH\"
export SDL_VIDEODRIVER=\"sdl2\"
export SDL3SHIM_SDL2_VIDEODRIVER=\"kmsdrm\"
export SDL_AUDIODRIVER=\"sdl2\"
export SDL3SHIM_SDL2_AUDIODRIVER=\"alsa\"

gdb -q -batch -ex \"handle SIGABRT stop print pass\" -ex \"handle SIGSEGV stop print pass\" -ex \"run\" -ex \"bt 30\" --args ./metroid_prime_port GM8E01.iso
"
