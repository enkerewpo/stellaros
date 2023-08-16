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

# https://github.com/usatenko/qemu has a patch on raspi4b support on qemu
qemu_executable_path=/opt/qemu-raspi4b/qemu/build/qemu-system-aarch64

print_color 'green' '[INFO] (qemu-run.sh) starting qemu('$qemu_executable_path')...'
# store the run command in a variable, print it out and then run it
run_cmd=''$qemu_executable_path' -serial mon:stdio -nographic -m 4096 -smp 4 -machine raspi4b4g -cpu cortex-a72 -kernel ./stellaros-kernel.elf -drive file=disk.img,format=qcow2'
echo $run_cmd
$run_cmd
