#include <stdio.h>
#include <stdlib.h>

void run_command(const char *command) {
    FILE *fp;
    char path[1035];
    fp = _popen(command, "r");
    if (fp == NULL) {
        printf("Failed to run command: %s\n", command);
        exit(1);
    }
    while (fgets(path, sizeof(path)-1, fp) != NULL) {
        printf("%s", path);
    }
    _pclose(fp);
}

int main() {
    run_command("systeminfo | findstr /B /C:\"OS Name\" /C:\"OS Version\"");
    run_command("wmic qfe get Caption,Description,HotFixID,InstalledOn");
    run_command("wmic os get osarchitecture || echo %PROCESSOR_ARCHITECTURE%");

    return 0;
}

