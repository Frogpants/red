#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"

install_packages() {
    if ! command -v apt-get >/dev/null 2>&1; then
        echo "Unsupported package manager. Install g++, pkg-config, libglew-dev, libglfw3-dev, and libgl1-mesa-dev manually."
        return 1
    fi

    local missing_packages=()

    for package_name in build-essential pkg-config libglew-dev libglfw3-dev libgl1-mesa-dev; do
        if ! dpkg -s "$package_name" >/dev/null 2>&1; then
            missing_packages+=("$package_name")
        fi
    done

    if [[ ${#missing_packages[@]} -eq 0 ]]; then
        return 0
    fi

    if [[ $EUID -eq 0 ]]; then
        apt-get update
        apt-get install -y "${missing_packages[@]}"
        return 0
    fi

    if command -v sudo >/dev/null 2>&1; then
        sudo apt-get update
        sudo apt-get install -y "${missing_packages[@]}"
        return 0
    fi

    echo "Missing packages: ${missing_packages[*]}"
    echo "Run this script with sudo or install them manually."
    return 1
}

main() {
    install_packages
    mkdir -p "$ROOT_DIR/build"
    echo "Init complete."
}

main "$@"
