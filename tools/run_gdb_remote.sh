#!/bin/bash
TARGET_IP=${1:-"192.168.1.70"}
TARGET_USER=${2:-"ark"}
TARGET_PASS="ark"

sshpass -p "$TARGET_PASS" ssh -o StrictHostKeyChecking=no "$TARGET_USER@$TARGET_IP" "
cd /roms/ports/metroid_prime

MALI_DIR=\"/tmp/metroid_prime_mali\"
rm -rf \"\$MALI_DIR\"
mkdir -p \"\$MALI_DIR\"
MALI_BLOB=\"/usr/lib/aarch64-linux-gnu/libmali-bifrost-g31-rxp0-gbm.so\"
if [ ! -f \"\$MALI_BLOB\" ]; then
  MALI_BLOB=\$(find /usr/lib -name \"libmali-bifrost-g31-*.so\" -o -name \"libmali.so\" 2>/dev/null | head -n 1)
fi
if [ -n \"\$MALI_BLOB\" ]; then
  ln -sf \"\$MALI_BLOB\" \"\$MALI_DIR/libEGL.so.1\"
  ln -sf \"\$MALI_BLOB\" \"\$MALI_DIR/libEGL.so\"
  ln -sf \"\$MALI_BLOB\" \"\$MALI_DIR/libGLESv2.so.2\"
  ln -sf \"\$MALI_BLOB\" \"\$MALI_DIR/libGLESv2.so\"
  ln -sf \"\$MALI_BLOB\" \"\$MALI_DIR/libgbm.so.1\"
  ln -sf \"\$MALI_BLOB\" \"\$MALI_DIR/libgbm.so\"
fi

export LD_LIBRARY_PATH=\"\$MALI_DIR:/roms/ports/metroid_prime/libs.aarch64:/roms/ports/metroid_prime:\$LD_LIBRARY_PATH\"
export SDL_VIDEODRIVER=\"sdl2\"
export SDL3SHIM_SDL2_VIDEODRIVER=\"kmsdrm\"
export SDL_AUDIODRIVER=\"sdl2\"
export SDL3SHIM_SDL2_AUDIODRIVER=\"alsa\"

gdb -batch -ex \"run\" -ex \"bt full\" --args ./metroid_prime_port /roms/ports/metroid_prime/GM8E01.iso
"
