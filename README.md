# S2-HyperCalc
A minimal 64-bit bootable Linux ISO that runs a custom C program as init. Built using a precompiled Linux kernel, GRUB bootloader, and custom initramfs without a full Linux distro. Designed to run standalone in QEMU or VirtualBox.

---

```md
# 🐧 Custom Linux Bootable ISO

This project demonstrates how to build a **minimal 64-bit bootable Linux ISO** that runs a **custom C program as `init`**. It uses:
- A **precompiled Linux kernel (`bzImage`)**
- A **custom `init` program** written in C
- A minimal **initramfs**
- The **GRUB bootloader**
- Works in **QEMU** and **VirtualBox**

---

## 📁 Project Structure

```

.
├── init.c               # Your main C program (runs as PID 1)
├── init                # Statically compiled binary
├── rootfs/             # Root filesystem for initramfs
│   ├── dev/
│   ├── proc/
│   ├── sys/
│   └── init
├── initrd.gz           # Compressed initramfs
├── iso/
│   └── boot/
│       ├── grub/
│       │   └── grub.cfg
│       ├── vmlinuz     # Precompiled Linux kernel (bzImage)
│       └── initrd.gz
└── mylinux.iso         # Final bootable ISO

````

---

## 🛠️ Requirements

- Linux host (Ubuntu recommended)
- `gcc`, `cpio`, `grub-pc-bin`, `xorriso`
- Precompiled Linux kernel (`bzImage`)
- `fakeroot` (for safe initramfs creation)

---

## 🧪 Step-by-Step Instructions

### 1. Write Your Init Program (`init.c`)

```c
#include <unistd.h>
#include <fcntl.h>

int main() {
    int fd = open("/dev/console", O_WRONLY);
    write(fd, "✅ Hello from custom init!\n", 27);
    while (1); // Infinite loop to keep the kernel alive
    return 0;
}
````

### 2. Compile Statically

```bash
gcc -static -o init init.c
chmod +x init
```

---

### 3. Create Initramfs (initrd.gz)

```bash
mkdir -p rootfs/{dev,proc,sys}
cp init rootfs/init

# Create device nodes
sudo mknod rootfs/dev/console c 5 1
sudo mknod rootfs/dev/null c 1 3
chmod 600 rootfs/dev/console
chmod 666 rootfs/dev/null

# Create initramfs
cd rootfs
find . | cpio -o --format=newc | gzip > ../initrd.gz
cd ..
```

---

### 4. Create GRUB Boot Config

```bash
mkdir -p iso/boot/grub
nano iso/boot/grub/grub.cfg
```

Paste this:

```cfg
set timeout=0
set default=0

menuentry "Boot Custom Linux" {
    linux /boot/vmlinuz console=tty0 video=vesafb:mtrr:3,ywrap,1024x768-32
    initrd /boot/initrd.gz
}
```

---

### 5. Add Kernel & Initrd to ISO Structure

```bash
cp initrd.gz iso/boot/initrd.gz
cp bzImage iso/boot/vmlinuz
```

---

### 6. Build ISO

```bash
grub-mkrescue -o S2 HyperCalc.iso iso
```

---

## ▶️ Run in QEMU

```bash
qemu-system-x86_64 -cdrom S2 HyperCalc.iso -boot d -m 512
```

---

## ▶️ Run in VirtualBox

1. Create a new VM:

   * Type: Linux
   * Version: Other Linux (64-bit)
   * RAM: 512 MB
   * No hard disk needed

2. Mount `S2 HyperCalc.iso` as optical drive.

3. Boot and watch the console for your custom `init` output.

---

## 📺 Set Default Resolution (e.g. 480)

Edit `grub.cfg`:

```cfg
linux /boot/vmlinuz console=tty0 video=vesafb:mtrr:3,ywrap,480 - 80 25
```

Optional for EFI VMs:

```bash
VBoxManage modifyvm "YourVMName" --firmware efi
VBoxManage setextradata "YourVMName" "VBoxInternal2/EfiGraphicsResolution" "1920x1080"
```

---

## 📦 Credits

* Linux kernel: [https://kernel.org](https://kernel.org)
* GRUB bootloader
* [cpio](https://www.gnu.org/software/cpio/) for archive creation
* `fakeroot` for safe permissions during build

---
