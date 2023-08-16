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