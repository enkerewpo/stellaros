source ./utils/color.sh

print_color 'green' '[INFO] (qemu-run.sh) Start making stellaros-kernel.elf...'

./build_kernel.sh

cd build

# if no disk.img, create one
if [ ! -f disk.img ]; then
    print_color 'green' '[INFO] (qemu-run.sh) Creating disk.img...'
    qemu-img create -f qcow2 disk.img 512M
fi

qemu_executable_path=/usr/bin/qemu-system-aarch64

print_color 'green' '[INFO] (qemu-run.sh) Starting QEMU('$qemu_executable_path')...'
$qemu_executable_path \
    -serial mon:stdio \
    -nographic -m 1024 -smp 4 \
    -machine raspi3b \
    -cpu cortex-a72 \
    -kernel ./stellaros-kernel.elf \
    -drive file=disk.img,format=qcow2