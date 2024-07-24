// Orginally by, Shantanu Hirlekar
#include <iostream>
#include <string>
#include <cmath>
#include <map>
#include <vector>
#include <Windows.h>
#include <io.h>

using namespace std;
typedef unsigned char BYTE;

// Lists the .exe and .dll files present in a directory. 
vector<string> listFiles(const string& searchpath) {
    vector<string> names;
    WIN32_FIND_DATAA search_data;
    memset(&search_data, 0, sizeof(WIN32_FIND_DATAA));
    HANDLE handle = FindFirstFileA(searchpath.c_str(), &search_data);

    while (handle != INVALID_HANDLE_VALUE) {
        names.push_back(search_data.cFileName);
        if (FindNextFileA(handle, &search_data) == FALSE)
            break;
    }

    FindClose(handle);
    return names;
}

// Prints and returns the entropy of a file. 
double getEntropy(FILE *file) {
    long fileSize = 0;
    BYTE *fileBuf = nullptr;

    if (file) {
        int fileNum = _fileno(file);
        HANDLE hFile = (HANDLE)_get_osfhandle(fileNum);
        fileSize = GetFileSize(hFile, NULL);

        fileBuf = new BYTE[fileSize];
        fread(fileBuf, fileSize, 1, file);
    }

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

// A function that returns the files having entropy in the top 10%
void getTen(float cut_off, const map<string, float>& track) {
    cout << endl << "Cut off for the top 10% is: " << cut_off << endl;
    cout << "The files having entropy in the top 10% are: " << endl;
    for (const auto& val : track) {
        if (val.second >= cut_off)
            cout << val.first << ": " << val.second << endl;
    }
}

int main(int argc, char const *argv[]) {
    if (argc != 2) {
        cout << "Usage: " << argv[0] << " <directory_path>" << endl;
        return 1;
    }

    string filepath = argv[1];
    map<string, float> track;
    float entropy_sum = 0;
    float max_entropy = 0, min_entropy = 0;
    int total_files = 0;

    // Retrieve the .exe and .dll files
    vector<string> exe_files = listFiles(filepath + "/*.exe");
    vector<string> dll_files = listFiles(filepath + "/*.dll");

    // Combine file lists
    exe_files.insert(exe_files.end(), dll_files.begin(), dll_files.end());

    if (!SetCurrentDirectoryA(filepath.c_str())) {
        cout << "Can't go to the filepath." << endl;
        return 1;
    }

    // Calculate entropy for each file
    for (const string& filename : exe_files) {
        FILE *file = fopen(filename.c_str(), "rb");
        if (!file) {
            cout << "Could not open file: " << filename << endl;
            continue;
        }

        double entropy = getEntropy(file);
        fclose(file);

        entropy_sum += entropy;
        if (entropy > max_entropy) 
            max_entropy = entropy;
        if (total_files == 0)
            min_entropy = entropy;
        else if (entropy < min_entropy)
            min_entropy = entropy;
            
        track[filename] = entropy;
        total_files++;
    }

    // Print the average entropy
    if (total_files > 0) {
        float avg_entropy = entropy_sum / total_files;
        cout << endl << "The average entropy of all the files is: " << avg_entropy << endl;

        // Get the files with entropy in the top 10%
        float cut_off = (max_entropy - min_entropy) * 0.9 + min_entropy;
        getTen(cut_off, track);
    } else {
        cout << "No files analyzed." << endl;
    }

    return 0;
}
