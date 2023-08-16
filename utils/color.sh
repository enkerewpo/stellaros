# color terminal
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[0;33m'
PURPLE='\033[0;35m'
CYAN='\033[0;36m'
BOLD='\033[1m'
NC='\033[0m' # No Color

# function to print colored text
function print_color() {
    if [ "$1" == "red" ]; then
        echo -e "${RED}$2${NC}"
    elif [ "$1" == "green" ]; then
        echo -e "${GREEN}$2${NC}"
    elif [ "$1" == "yellow" ]; then
        echo -e "${YELLOW}$2${NC}"
    elif [ "$1" == "purple" ]; then
        echo -e "${PURPLE}$2${NC}"
    elif [ "$1" == "cyan" ]; then
        echo -e "${CYAN}$2${NC}"
    elif [ "$1" == "bold" ]; then
        echo -e "${BOLD}$2${NC}"
    else
        echo "[ERROR] (build_kernel.sh) print_color() function error"
    fi
}