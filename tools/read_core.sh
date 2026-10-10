#!/bin/bash
TARGET_IP=${1:-"192.168.1.70"}
TARGET_USER=${2:-"ark"}
TARGET_PASS="ark"

sshpass -p "$TARGET_PASS" ssh -o StrictHostKeyChecking=no "$TARGET_USER@$TARGET_IP" "
cd /roms/ports/metroid_prime
CORE_FILE=\$(ls core* 2>/dev/null | head -n 1)
if [ -n \"\$CORE_FILE\" ]; then
  gdb -batch -ex \"bt full\" ./metroid_prime_port \"\$CORE_FILE\"
else
  echo \"No core file found!\"
fi
"
