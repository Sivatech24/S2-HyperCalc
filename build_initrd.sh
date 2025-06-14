#!/bin/bash
set -e

# Step 1: Create rootfs directory structure
mkdir -p rootfs/{dev,proc,sys}
cp init rootfs/init
chmod +x rootfs/init

# Step 2: Build initramfs using fakeroot in one shell session
fakeroot -- bash -c '
  mknod rootfs/dev/console c 5 1
  chmod 600 rootfs/dev/console

  mknod rootfs/dev/null c 1 3
  chmod 666 rootfs/dev/null

  cd rootfs
  find . | cpio -o --format=newc | gzip > ../initrd.gz
'

echo "✅ Created initrd.gz"
