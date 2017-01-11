#ifndef arg_prc
#define arg_prc

#include "base.hpp"

bool existed_path(char* dir) {
	struct stat buf;
	return stat(dir, &buf) == 0;
}

bool is_a_directory(char* dir) {
	struct stat buf;
	if (stat(dir, &buf) != 0)
		return false;
	return S_ISDIR(buf.st_mode);
}

bool is_a_file(char* dir) {
	struct stat buf;
	if (stat(dir, &buf) != 0)
		return false;
	return S_ISREG(buf.st_mode);
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

void get_absolute_path(char res[], char path[]) {
	if (strlen(path) == 0)
		return;
	if (path[0] == '/') 
		dir_cat(res, get_current_dir_name(), path);
	else
		strcpy(res, path);
}

//TODO: checking for existence of -t and -s option [Done]
void extract_judging_info(int argNum, char* arg[]) {
	char tmp[510];
	set<string> requiredOpt {"-t", "-s"};
	dir_cat(config_dir, "/home/brian/CODE/Testing_Soft", "default_config.json");
	for (int i = 1; i < argNum;) {
		if (strcmp(arg[i], "-t") == 0) /* test directory */ {
			requiredOpt.erase("-t");
			if (i + 1 >= argNum) {
				cerr << "Test directory was not provided!" << endl;
				exit(EXIT_FAILURE);
			}
			get_absolute_path(tmp, arg[i + 1]);
			if (!is_a_directory(tmp)) {
				cout << tmp << endl;
				cerr << "Test directory does not exists!" << endl;
				exit(EXIT_FAILURE);
			}
			strcpy(test_dir, tmp);
			i += 2;
		} else if (strcmp(arg[i], "-s") == 0) /* submission directory */ {
			requiredOpt.erase("-s");
			if (i + 1 >= argNum) {
				cerr << "Submission directory was not provided!" << endl;
				exit(EXIT_FAILURE);
			}
			get_absolute_path(tmp, arg[i + 1]);
			if (!existed_path(tmp)) {
				cerr << "Submission directory does not exists!" << endl;
				exit(EXIT_FAILURE);
			}
			strcpy(sub_dir, tmp);
			i += 2;
		} else if (strcmp(arg[i], "-c") == 0) /* config directory*/ {
			if (i + 1 >= argNum || arg[i + 1][0] == '-') {
				if (strlen(test_dir) == 0) {
					cerr << "Test directory was not provided or was listed after -c" << endl;
					exit(EXIT_FAILURE);
				}
				dir_cat(config_dir, test_dir, "config.json");
				i++;
			} else  {
				get_absolute_path(tmp, arg[i + 1]);
				i += 2;
			}
		}
	}
	if (!is_a_file(config_dir)) {
		cerr << "Directory for configuration does not exist!" << endl;
		exit(EXIT_FAILURE);
	}
	if (!requiredOpt.empty()) {
		cerr << "The following options was not provided: ";
		while (!requiredOpt.empty()) {
			cerr << (*requiredOpt.begin()) << " ";
			requiredOpt.erase(requiredOpt.begin());
		}
		exit(EXIT_FAILURE);
	}
}

#endif
