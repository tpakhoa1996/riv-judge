#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <sys/stat.h>
#include <unistd.h>

using namespace std;

const int maxN = int(1e3) + 111;

char test_dir[maxN], sub_dir[maxN], config_dir[maxN];

bool is_a_directory(char* dir) {
	struct stat *buf;
	if (stat(dir, buf) != 0)
		return false;
	return S_ISDIR(buf->st_mode);
}

bool is_a_file(char* dir) {
	struct stat *buf;
	if (stat(dir, buf) != 0)
		return false;
	return S_ISREG(buf->st_mode);
}

void dir_cat(char dest[], const char* first, const char* second) {
	strcpy(dest, first);
	int len = strlen(dest);
	if ((dest[len - 1] == '/') == (second[0] == '/')) {
		if (second[0] == '/')
			dest[len - 1] = 0;
		else {
			dest[len] = '/';
			dest[len + 1] = 0;
		}
	}
	strcat(dest, second);
}

void extract_judging_info(int argNum, char* arg[]) {
	dir_cat(config_dir, get_current_dir_name(), "default_config.json");
	for (int i = 1; i < argNum;) {
		if (strcmp(arg[i], "-t") == 0) /* test directory */ {
			if (i + 1 >= argNum) {
				cerr << "Test directory was not provided!" << endl;
				exit(EXIT_FAILURE);
			}
			if (!is_a_directory(arg[i + 1])) {
				cerr << "Test directory does not exists!" << endl;
				exit(EXIT_FAILURE);
			}
			strcpy(test_dir, arg[i + 1]);
			i += 2;
		} else if (strcmp(arg[i], "-s") == 0) /* submission directory */ {
			if (i + 1 >= argNum) {
				cerr << "Submission directory was not provided!" << endl;
				exit(EXIT_FAILURE);
			}
			struct stat *buf;
			if (stat(arg[i + 1], buf) != 0) {
				cerr << "Submission directory does not exists!" << endl;
				exit(EXIT_FAILURE);
			}
			strcpy(sub_dir, arg[i + 1]);
			i += 2;
		} else if (strcmp(arg[i], "-c") == 0) /* config directory*/ {
			if (i + 1 >= argNum || arg[i + 1][0] == '-') {
				dir_cat(config_dir, test_dir, "config.json");
				i++;
			} else  {
				strcpy(config_dir, arg[i + 1]);
				i += 2;
			}
		}
	}
	if (!is_a_file(config_dir)) {
		cerr << "Directory for configuration does not exist!" << endl;
		exit(EXIT_FAILURE);
	}
}

int main(int argNum, char* arg[]) {
	extract_judging_info(argNum, arg);
	return 0;
}
