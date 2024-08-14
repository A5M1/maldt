#include <iostream>
#include <fstream>
#include <windows.h>

#ifndef IMAGE_FILE_MACHINE_ARM64
#define IMAGE_FILE_MACHINE_ARM64 0xAA64
#endif

#ifndef IMAGE_FILE_MACHINE_ARMNT
#define IMAGE_FILE_MACHINE_ARMNT 0x01C4
#endif

void checkFileArchitecture(const std::string& filePath) {
    std::ifstream file(filePath.c_str(), std::ios::binary);

    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filePath << std::endl;
        return;
    }

    IMAGE_DOS_HEADER dosHeader;
    file.read(reinterpret_cast<char*>(&dosHeader), sizeof(dosHeader));

    if (dosHeader.e_magic != IMAGE_DOS_SIGNATURE) {
        std::cerr << "Not a valid PE file: " << filePath << std::endl;
        file.close();
        return;
    }

    file.seekg(dosHeader.e_lfanew, std::ios::beg);

    DWORD peSignature;
    file.read(reinterpret_cast<char*>(&peSignature), sizeof(peSignature));

    if (peSignature != IMAGE_NT_SIGNATURE) {
        std::cerr << "Not a valid PE file: " << filePath << std::endl;
        file.close();
        return;
    }

    IMAGE_FILE_HEADER fileHeader;
    file.read(reinterpret_cast<char*>(&fileHeader), sizeof(fileHeader));

    switch (fileHeader.Machine) {
        case IMAGE_FILE_MACHINE_I386:
            std::cout << filePath << " is a 32-bit (x86) executable." << std::endl;
            break;
        case IMAGE_FILE_MACHINE_AMD64:
            std::cout << filePath << " is a 64-bit (x64) executable." << std::endl;
            break;
        case IMAGE_FILE_MACHINE_ARMNT:
            std::cout << filePath << " is an ARM executable." << std::endl;
            break;
        case IMAGE_FILE_MACHINE_ARM64:
            std::cout << filePath << " is an ARM64 executable." << std::endl;
            break;
        default:
            std::cout << filePath << " has an unknown or unsupported architecture." << std::endl;
            break;
    }

    file.close();
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <path_to_executable>" << std::endl;
        return 1;
    }

    checkFileArchitecture(argv[1]);
    return 0;
}
