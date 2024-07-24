#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    FILE *fp;
    char path[1035];

    // Open the command for reading
    fp = popen("whoami /groups | find \"Label\"", "r");
    if (fp == NULL) {
        printf("Failed to run command\n");
        exit(1);
    }
    while (fgets(path, sizeof(path)-1, fp) != NULL) {
        printf("%s", path);
    }

    pclose(fp);
    return 0;
}

