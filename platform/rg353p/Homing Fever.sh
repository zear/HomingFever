#!/bin/bash
# Local ArkOS test package: keep paths relative to this launcher.
GAMEDIR="$(cd -- "$(dirname -- "$0")" && pwd)/homingfever"
cd "$GAMEDIR" || exit 1
exec > "$GAMEDIR/log.txt" 2>&1
echo '[launcher r4] Starting Homing Fever'
printf '[launcher] Game directory: %s\n' "$GAMEDIR"
controlfolder=""
for candidate in "$(dirname -- "$GAMEDIR")/../tools/PortMaster" /roms2/tools/PortMaster /roms/tools/PortMaster /opt/system/Tools/PortMaster /opt/tools/PortMaster; do
    if [ -f "$candidate/control.txt" ]; then
        controlfolder="$candidate"
        break
    fi
done
if [ -z "$controlfolder" ]; then
    echo 'PortMaster control.txt not found.'
    exit 1
fi
printf '[launcher] Loading %s/control.txt\n' "$controlfolder"
source "$controlfolder/control.txt"
printf '[launcher] control.txt loaded; effective controlfolder=%s\n' "$controlfolder"
if [ -f "$controlfolder/mod_${CFW_NAME}.txt" ]; then
    echo '[launcher] Loading firmware settings'
    source "$controlfolder/mod_${CFW_NAME}.txt"
    echo '[launcher] Firmware settings loaded' 
fi
echo "[launcher] Detecting controls"
get_controls
echo "[launcher] Controls detected"
export SDL_GAMECONTROLLERCONFIG="$sdl_controllerconfig"
if [ -z "$GPTOKEYB" ]; then
    echo 'PortMaster did not define GPTOKEYB.'
    exit 1
fi
cleanup() {
    echo "[launcher] Cleaning up"
    kill "$mapper_pid" 2>/dev/null || true
    wait "$mapper_pid" 2>/dev/null || true
    if declare -F pm_finish >/dev/null; then pm_finish; fi
}
# Word splitting is intentional: PortMaster may include command arguments.
echo "[launcher] Starting controller mapper"
$GPTOKEYB "fever.aarch64" -c "$GAMEDIR/homingfever.gptk" &
mapper_pid=$!
trap cleanup EXIT
if declare -F pm_platform_helper >/dev/null; then
    echo '[launcher] Preparing display through PortMaster'
    pm_platform_helper "$GAMEDIR/fever.aarch64"
    echo '[launcher] PortMaster display preparation complete' 
fi
cd "$GAMEDIR" || exit 1
printf '[launcher] SDL_VIDEODRIVER=%s DISPLAY=%s\n' "${SDL_VIDEODRIVER:-auto}" "${DISPLAY:-unset}"
if [ ! -f "$GAMEDIR/libs.aarch64/libSDL-1.2.so.0" ]; then
    echo '[launcher] Missing bundled SDL compatibility library; extract the full r4 ZIP.'
    exit 1
fi
echo '[launcher] Executing game with bundled sdl12-compat 1.2.68' 
LD_LIBRARY_PATH="$GAMEDIR/libs.aarch64${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}" \
SDL12COMPAT_DEBUG_LOGGING=1 ./fever.aarch64 -s 2
result=$?
printf '[launcher] Game exited with status %s\n' "$result"
exit "$result"
