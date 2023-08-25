source ./utils/color.sh

# create a banner of StellarOS
print_color "bold" "+-----------------------------------------+"
print_color "bold" "|                                         |"
print_color "bold" "|      StellarOS Kernel Build Script      |"
print_color "bold" "|                                         |"
print_color "bold" "+-----------------------------------------+"
print_color "bold" "|              wheatfox 2023              |"
print_color "bold" "+-----------------------------------------+"

# if exits build folder, delete it
if [ -d build ]; then
    print_color "yellow" "[WARNING] (build_kernel.sh) build folder exists, deleted it."
    rm -rf build
fi

# create build folder
mkdir build
print_color "green" "[INFO] (build_kernel.sh) build folder created on path $(pwd)/build"

print_color "green" "[INFO] (build_kernel.sh) building kernel..."
make

# objdump the kernel to see the assembly code
print_color "green" "[INFO] (build_kernel.sh) objdumping kernel..."
/opt/gcc-arm/gcc-arm-10.3-2021.07-x86_64-aarch64-none-elf/bin/aarch64-none-elf-objdump -d build/stellaros-kernel.elf > build/stellaros-kernel.s

if make -j4; then
    print_color "green" "[INFO] (build_kernel.sh) kernel build success."
else
    print_color "red" "[ERROR] (build_kernel.sh) kernel build failed."
    exit 1
fi
