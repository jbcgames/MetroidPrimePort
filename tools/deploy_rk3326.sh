#!/bin/bash
TARGET_IP=${1:-"192.168.1.70"}
TARGET_USER=${2:-"ark"}
TARGET_PASS="ark"
TARGET_PATH="/roms/ports"

echo "=== Deploying Metroid Prime PortMaster package to $TARGET_USER@$TARGET_IP:$TARGET_PATH ==="

sshpass -p "$TARGET_PASS" ssh -o StrictHostKeyChecking=no "$TARGET_USER@$TARGET_IP" "mkdir -p $TARGET_PATH/metroid_prime"

if [ -f "portmaster/Metroid Prime.sh" ]; then
  echo "Copying Metroid Prime.sh..."
  sshpass -p "$TARGET_PASS" scp -o StrictHostKeyChecking=no "portmaster/Metroid Prime.sh" "$TARGET_USER@$TARGET_IP:$TARGET_PATH/"
fi

if [ -f "portmaster/metroid_prime.gptk" ]; then
  echo "Copying metroid_prime.gptk..."
  sshpass -p "$TARGET_PASS" scp -o StrictHostKeyChecking=no "portmaster/metroid_prime.gptk" "$TARGET_USER@$TARGET_IP:$TARGET_PATH/metroid_prime/"
fi

if [ -f "portmaster/port.json" ]; then
  echo "Copying port.json..."
  sshpass -p "$TARGET_PASS" scp -o StrictHostKeyChecking=no "portmaster/port.json" "$TARGET_USER@$TARGET_IP:$TARGET_PATH/metroid_prime/"
fi

if [ -f "build/linux-aarch64-gles/metroid_prime_port" ]; then
  echo "Copying binary metroid_prime_port..."
  sshpass -p "$TARGET_PASS" scp -o StrictHostKeyChecking=no "build/linux-aarch64-gles/metroid_prime_port" "$TARGET_USER@$TARGET_IP:$TARGET_PATH/metroid_prime/"
  sshpass -p "$TARGET_PASS" ssh -o StrictHostKeyChecking=no "$TARGET_USER@$TARGET_IP" "chmod +x $TARGET_PATH/Metroid\ Prime.sh $TARGET_PATH/metroid_prime/metroid_prime_port"
else
  echo "WARNING: metroid_prime_port binary not found in build dir!"
fi

if [ -f "/mnt/d/Laptop/Port/GM8E01.iso" ]; then
  echo "Checking if ROM GM8E01.iso exists on target..."
  ROM_EXISTS=$(sshpass -p "$TARGET_PASS" ssh -o StrictHostKeyChecking=no "$TARGET_USER@$TARGET_IP" "[ -f $TARGET_PATH/metroid_prime/GM8E01.iso ] && echo 'yes' || echo 'no'")
  if [ "$ROM_EXISTS" != "yes" ]; then
    echo "Copying ROM GM8E01.iso..."
    sshpass -p "$TARGET_PASS" scp -o StrictHostKeyChecking=no "/mnt/d/Laptop/Port/GM8E01.iso" "$TARGET_USER@$TARGET_IP:$TARGET_PATH/metroid_prime/"
  else
    echo "ROM GM8E01.iso already exists on target device."
  fi
fi

echo "=== Deployment complete ==="
