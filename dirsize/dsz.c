#include <stdio.h>
#include <windows.h>

unsigned long long get_dir_size(const char *dir_path) {
    WIN32_FIND_DATA find_data;
    HANDLE hFind;
    unsigned long long total_size = 0;
    char search_path[MAX_PATH];
    snprintf(search_path, MAX_PATH, "%s\\*", dir_path);
    hFind = FindFirstFile(search_path, &find_data);
    if (hFind == INVALID_HANDLE_VALUE) {
        //fprintf(stderr, "FindFirstFile error: %lu\n", GetLastError());
        return 0;
    }
    do {
        if (strcmp(find_data.cFileName, ".") == 0 || strcmp(find_data.cFileName, "..") == 0) {
            continue;
        }
        char full_path[MAX_PATH];
        snprintf(full_path, MAX_PATH, "%s\\%s", dir_path, find_data.cFileName);
        if (!(find_data.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN) &&
            !(find_data.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM)) {
            if (find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
                total_size += get_dir_size(full_path);
            } else {
                LARGE_INTEGER file_size;
                file_size.LowPart = find_data.nFileSizeLow;
                file_size.HighPart = find_data.nFileSizeHigh;
                total_size += file_size.QuadPart;
            }
        }
    } while (FindNextFile(hFind, &find_data) != 0);
    if (GetLastError() != ERROR_NO_MORE_FILES) {
        fprintf(stderr, "FindNextFile error: %lu\n", GetLastError());
    }
    FindClose(hFind);
    return total_size;
}

void format_size(unsigned long long size, char *output) {
    const char *sizes[] = { "B", "KB", "MB", "GB", "TB" };
    int order = 0;
    double readable_size = (double)size;

    while (readable_size >= 1024 && order < (sizeof(sizes) / sizeof(sizes[0])) - 1) {
        order++;
        readable_size /= 1024;
    }
    snprintf(output, 20, "%.2f %s", readable_size, sizes[order]);
}

int main() {
    char current_dir[MAX_PATH];
    if (GetCurrentDirectory(MAX_PATH, current_dir) == 0) {
        fprintf(stderr, "GetCurrentDirectory error: %lu\n", GetLastError());
        return 1;
    }

    unsigned long long dir_size = get_dir_size(current_dir);
    char formatted_size[20];
    format_size(dir_size, formatted_size);
    printf("Current directory size: %s\n", formatted_size);

    return 0;
}
