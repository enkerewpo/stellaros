source ./utils/color.sh

print_color 'green' '[INFO] (qemu-run.sh) start making stellaros-kernel.elf...'

./build_kernel.sh

cd build

# if no disk.img, create one
if [ ! -f disk.img ]; then
    print_color 'green' '[INFO] (qemu-run.sh) creating disk.img...'
    qemu-img create -f qcow2 disk.img 512M
fi

# qemu_executable_path=/usr/bin/qemu-system-aarch64
qemu_executable_path=/opt/qemu/build/qemu-system-aarch64 # with raspi4b machine

print_color 'green' '[INFO] (qemu-run.sh) starting QEMU('$qemu_executable_path')...'
$qemu_executable_path \
    -serial mon:stdio \
    -nographic -m 4096 -smp 4 \
    -machine raspi4b4g \
    -cpu cortex-a72 \
    -kernel ./stellaros-kernel.elf \
    -drive file=disk.img,format=qcow2 \
    -s -S # this will start QEMU in debug mode, and wait for gdb to connect