source ./utils/color.sh

print_color 'green' '[INFO] (qemu-run.sh) start making stellaros-kernel.elf...'

if ./build_kernel.sh; then
    print_color 'green' '[INFO] (qemu-run.sh) stellaros-kernel.elf is successfully built.'
else
    print_color 'red' '[ERROR] (qemu-run.sh) failed to build stellaros-kernel.elf.'
    exit 1
fi

cd build

# if no disk.img, create one
if [ ! -f disk.img ]; then
    print_color 'green' '[INFO] (qemu-run.sh) creating disk.img...'
    qemu-img create -f qcow2 disk.img 512M
fi

qemu_executable_path=/usr/bin/qemu-system-aarch64

print_color 'green' '[INFO] (qemu-run.sh) starting QEMU('$qemu_executable_path') with QEMU ARM Virt Machine...'
$qemu_executable_path \
    -kernel ./stellaros-kernel.elf \
    -machine virt,gic-version=2 \
    -m 1024 \
    -cpu cortex-a72 \
    -smp 4 \
    -drive file=disk.img,format=qcow2 \
    -nographic \
    -serial mon:stdio \
