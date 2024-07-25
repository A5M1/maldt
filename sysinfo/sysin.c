#include <stdio.h>
#include <stdlib.h>

void run_command(const char* command) {
	FILE* fp;
	char path[4096];
	int buffer_size = sizeof(path);

	fp = _popen(command, "r");
	if (fp == NULL) {
		fprintf(stderr, "Failed to run command: %s\n", command);
		exit(EXIT_FAILURE);
	}

	while (fgets(path, buffer_size - 1, fp) != NULL) {
		if (strlen(path) >= buffer_size - 1) {
			fprintf(stderr, "Output exceeds buffer size for command: %s\n", command);
			_pclose(fp);
			exit(EXIT_FAILURE);
		}
		printf("%s", path);
	}

	if (_pclose(fp) == -1) {
		fprintf(stderr, "Failed to close command stream: %s\n", command);
		exit(EXIT_FAILURE);
	}
}

int main() {
	run_command("systeminfo | findstr /B /C:\"OS Name\" /C:\"OS Version\"");
	run_command("wmic qfe get Caption,Description,HotFixID,InstalledOn");
	run_command("wmic os get osarchitecture || echo %PROCESSOR_ARCHITECTURE%");

	return 0;
}
