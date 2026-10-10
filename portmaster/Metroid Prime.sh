#!/bin/bash

sudo killall -9 metroid_prime_port gptokeyb gptokeyb2 2>/dev/null || true

XDG_DATA_HOME=${XDG_DATA_HOME:-$HOME/.local/share}

if [ -d "/opt/system/Tools/PortMaster/" ]; then
  controlfolder="/opt/system/Tools/PortMaster"
elif [ -d "/opt/tools/PortMaster/" ]; then
  controlfolder="/opt/tools/PortMaster"
elif [ -d "$XDG_DATA_HOME/PortMaster/" ]; then
  controlfolder="$XDG_DATA_HOME/PortMaster"
else
  controlfolder="/roms/ports/PortMaster"
fi

source "$controlfolder/control.txt"
[ -f "${controlfolder}/mod_${CFW_NAME}.txt" ] && source "${controlfolder}/mod_${CFW_NAME}.txt"
get_controls

GAMEDIR="/roms/ports/metroid_prime"
cd "$GAMEDIR" || exit 1

> "$GAMEDIR/log.txt" && exec > >(tee "$GAMEDIR/log.txt") 2>&1

amixer -c 0 sset 'Playback Path' 'SPK' >/dev/null 2>&1 || true
# Do not force volume to 95% so device system volume is preserved

MALI_DIR="/tmp/metroid_prime_mali"
rm -rf "$MALI_DIR"
mkdir -p "$MALI_DIR"

if [ -f "$GAMEDIR/libs.aarch64/libmali-bifrost-g31-rxp0-gbm.so" ]; then
  MALI_BLOB="$GAMEDIR/libs.aarch64/libmali-bifrost-g31-rxp0-gbm.so"
elif [ -f "/usr/lib/aarch64-linux-gnu/libmali-bifrost-g31-rxp0-gbm.so" ]; then
  MALI_BLOB="/usr/lib/aarch64-linux-gnu/libmali-bifrost-g31-rxp0-gbm.so"
else
  MALI_BLOB=$(find /usr/lib -name "libmali-bifrost-g31-*.so" -o -name "libmali.so" 2>/dev/null | head -n 1)
fi

if [ -n "$MALI_BLOB" ]; then
  ln -sf "$MALI_BLOB" "$MALI_DIR/libEGL.so.1"
  ln -sf "$MALI_BLOB" "$MALI_DIR/libEGL.so"
  ln -sf "$MALI_BLOB" "$MALI_DIR/libGLESv2.so.2"
  ln -sf "$MALI_BLOB" "$MALI_DIR/libGLESv2.so"
  ln -sf "$MALI_BLOB" "$MALI_DIR/libgbm.so.1"
  ln -sf "$MALI_BLOB" "$MALI_DIR/libgbm.so"
fi

export LD_LIBRARY_PATH="$MALI_DIR:$GAMEDIR/libs.aarch64:$GAMEDIR:$GAMEDIR/libs:$LD_LIBRARY_PATH"
unset LD_PRELOAD

export SDL_VIDEODRIVER="sdl2"
export SDL3SHIM_SDL2_VIDEODRIVER="kmsdrm"
export SDL_AUDIODRIVER="sdl2"
export SDL3SHIM_SDL2_AUDIODRIVER="alsa"
export SDL_NO_SIGNAL_HANDLERS="1"
ulimit -c unlimited

for cpu in /sys/devices/system/cpu/cpu[0-3]/online; do
  [ -f "$cpu" ] && echo 1 | sudo tee "$cpu" >/dev/null 2>&1 || true
done

if [ -f /sys/devices/system/cpu/cpu0/cpufreq/scaling_governor ]; then
  echo performance | sudo tee /sys/devices/system/cpu/cpu*/cpufreq/scaling_governor >/dev/null 2>&1 || true
fi

for gpu_gov in /sys/devices/platform/*.gpu/devfreq/*.gpu/governor /sys/class/devfreq/*gpu*/governor; do
  [ -f "$gpu_gov" ] && echo performance | sudo tee "$gpu_gov" >/dev/null 2>&1 || true
done

for dmc_gov in /sys/devices/platform/dmc/devfreq/dmc/governor /sys/class/devfreq/*dmc*/governor; do
  [ -f "$dmc_gov" ] && echo performance | sudo tee "$dmc_gov" >/dev/null 2>&1 || true
done

MAP_GO="190000004b4800000011000000010000,GO-Super Gamepad,a:b1,b:b0,back:b12,dpdown:b9,dpleft:b10,dpright:b11,dpup:b8,guide:b16,leftshoulder:b4,leftstick:b14,lefttrigger:b6,leftx:a0,lefty:a1,rightshoulder:b5,rightstick:b15,righttrigger:b7,rightx:a2,righty:a3,start:b13,x:b2,y:b3,platform:Linux,"
MAP_GO_DEB="1900bb3e4b4800000011000000010000,GO-Super Gamepad,a:b1,b:b0,back:b12,dpdown:b9,dpleft:b10,dpright:b11,dpup:b8,guide:b16,leftshoulder:b4,leftstick:b14,lefttrigger:b6,leftx:a0,lefty:a1,rightshoulder:b5,rightstick:b15,righttrigger:b7,rightx:a2,righty:a3,start:b13,x:b2,y:b3,platform:Linux,"

if [ -n "$sdl_controllerconfig" ]; then
  cfg_deb=$(echo "$sdl_controllerconfig" | sed 's/190000004b4800000011000000010000/1900bb3e4b4800000011000000010000/g')
  export SDL_GAMECONTROLLERCONFIG="${sdl_controllerconfig}"$'\n'"${cfg_deb}"$'\n'"${MAP_GO}"$'\n'"${MAP_GO_DEB}"
else
  export SDL_GAMECONTROLLERCONFIG="${MAP_GO}"$'\n'"${MAP_GO_DEB}"
fi

BIN="$GAMEDIR/metroid_prime_port"
chmod +x "$BIN"

# if [ -f "$GAMEDIR/metroid_prime.gptk" ]; then
#   if [ -n "$GPTOKEYB2" ]; then
#     $GPTOKEYB2 "$(basename "$BIN")" -c "$GAMEDIR/metroid_prime.gptk" &
#   elif [ -n "$GPTOKEYB" ]; then
#     $GPTOKEYB "$(basename "$BIN")" -c "$GAMEDIR/metroid_prime.gptk" &
#   fi
# fi

pm_platform_helper "$BIN"

ISO_PATH=""
for iso in "$GAMEDIR"/*.iso "$GAMEDIR"/*.ISO "$GAMEDIR"/*.gcm "$GAMEDIR"/*.GCM "$GAMEDIR"/*.rvz "$GAMEDIR"/*.RVZ; do
  if [ -f "$iso" ]; then
    ISO_PATH="$iso"
    break
  fi
done

if [ -z "$ISO_PATH" ]; then
  echo "ERROR: No disc image (.iso / .gcm / .rvz) found in $GAMEDIR"
  pm_finish
  exit 1
fi

export MP_DISC="$ISO_PATH"
"$BIN" "$ISO_PATH"
ret=$?

pm_finish
