#include <iostream>
#include <string>
#include <cmath>
#include <map>
#include <cstdio>
#include <Windows.h>
#include <io.h>

using namespace std;
typedef unsigned char BYTE;

double getEntropy(FILE *file) {
    long fileSize = 0;
    BYTE *fileBuf = nullptr;
    if (file) {
        int fileNum = _fileno(file);
        HANDLE hFile = (HANDLE)_get_osfhandle(fileNum);
        fileSize = GetFileSize(hFile, NULL);
    }
    fileBuf = new BYTE[fileSize];
    fread(fileBuf, fileSize, 1, file);

    double entropy = 0;
    map<char, int> freq;
    cout << "Calculating the entropy of the file, please wait....." << endl;
    for (int i = 0; i < fileSize; i++) {
        freq[fileBuf[i]]++;
    }
    delete[] fileBuf;
    for (const auto& p : freq) {
        double frequency = static_cast<double>(p.second) / fileSize;
        entropy += frequency * log2(frequency);
    }
    entropy *= -1;
    cout << "Entropy is: " << entropy << endl;
    return entropy;
}

int main(int argc, char const *argv[]) {
    if (argc != 2) {
        cout << "Usage: " << argv[0] << " <file_path>" << endl;
        return 1;
    }
    const char *filepath = argv[1];
    FILE *file = fopen(filepath, "rb");
    if (!file) {
        cout << "Could not open file: " << filepath << endl;
        return 1;
    }
    double entropy = getEntropy(file);
    fclose(file);
    cout << "The entropy of the file " << filepath << " is: " << entropy << endl;
    return 0;
}
