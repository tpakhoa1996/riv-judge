#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <sys/stat.h>
#include <unistd.h>
#include "rapidjson/document.h"
#include <vector>
#include <string>
#include <set>

using namespace std;

typedef rapidjson::Document Document;
typedef rapidjson::Value Value;

const int dirLen= int(1e3) + 111;
const int configLen = int(1e6) + 111;

char test_dir[dirLen], sub_dir[dirLen], config_dir[dirLen], config_json[configLen];
Document config;

bool existed_path(char dir[]) {
	struct stat buf;
	return stat(dir, &buf) == 0;
}

bool is_a_directory(char dir[]) {
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

bool is_special_letter(char x) {
	return (x == 9 || x == 10 || x == 12);
}

char* strip(char tmp[]) {
	int len = strlen(tmp);
	while (len && is_special_letter(tmp[len - 1])) len --;
	int i = 0;
	while (i < len && is_special_letter(tmp[i])) i ++;
	tmp[len] = 0;
	strcpy(tmp, tmp + i);
}

void read_in_config(char config_json[], vector<int> &config_file) {
	FILE *f = fopen(config_dir, "r");
	char tmp[510];
	while (!feof(f)) {
		tmp[0] = 0;
		fgets(tmp, 500, f);
		strcat(config_json, strip(tmp));
		config_file.push_back(strlen(tmp));
	}
	fclose(f);
}

int get_error_line(Document &config, vector<int> &config_file) {
	int line;
	int pos = config.GetErrorOffset() + 1;
	for (line = 0; line < (int) config_file.size(); line ++) {
		if (pos <= config_file[line])
			break;
		pos -= config_file[line];
	}
	return (line + 1);
}

void return_config_error(Document &config, vector<int> &config_file) {
	cerr << "Configuration file has error on line: ";
	cerr << get_error_line(config, config_file) << endl;
	string error_st;
	switch(config.GetParseError()) {
		case 1:
			error_st = "The document is empty.";
			break;
		case 2:
			error_st = "The document root must not follow by other values.";
			break;
		case 3:
			error_st = "Invalid value.";
			break;
		case 4:
			error_st = "Missing a name for object member.";
			break;
		case 5:
			error_st = "Missing a colon after a name of object member.";
			break;
		case 6:
			error_st = "Missing a comma or '}' after an object member.";
			break;
		case 7:
			error_st = "Missing a comma or ']' after an array element.";
			break;
		case 8:
			error_st = "Incorrect hex digit after \\u escape in string.";
			break;
		case 9:
			error_st = "The surrogate pair in string is invalid.";
			break;
		case 10:
			error_st = "Invalid escape character in string.";
			break;
		case 11:
			error_st = "Missing a closing quotation mark in string.";
			break;
		case 12:
			error_st = "Invalid encoding in string.";
			break;
		case 13:
			error_st = "Number too big to be stored in double.";
			break;
		case 14:
			error_st = "Miss fraction part in number.";
			break;
		case 15:
			error_st = "Miss exponent in number.";
			break;
		case 16:
			error_st = "Parsing was terminated.";
			break;
		case 17:
			error_st = "Unspecific syntax error.";
			break;
	}
	cerr << "-->" << error_st << endl;
}

void exit_mess(string mess = "", void (*mess_func)() = 0) {
	if (!mess.empty())
		cerr << mess;
	if (mess_func != 0)
		mess_func();
	exit(EXIT_FAILURE);
}

void get_config(Document &config) {
	vector<int> config_file;
	config_file.clear();
	read_in_config(config_json, config_file);
	config.Parse(config_json);
	if (config.HasParseError()) {
		return_config_error(config, config_file);
		exit(EXIT_FAILURE);
	}
	config_file.clear();
}

void config_return_field_error(string object_name, Value& x, set<string> requiredField) {
	for (Value::ConstMemberIterator it = x.MemberBegin();
			it != x.MemberEnd(); it++) {
		string field_name = it->name.GetString();
		if (!requiredField.count(field_name)) {
			requiredField.clear();
			exit_mess("Field \"" + field_name + "\" in object \"" + 
					object_name + "\" has no meaning.\n");
		}
		requiredField.erase(field_name);
	}
	if (!requiredField.empty()) {
		cerr << "The following field in object \"" +
			object_name + "\" was not provided: ";
		while (!requiredField.empty()) {
			cerr << "\"" << (*requiredField.begin()) << "\" ";
			requiredField.erase(requiredField.begin());
		}
		exit(EXIT_FAILURE);
	}
}

void config_field_verification(Document &config) {
	if (!config.HasMember("Configuration"))
		exit_mess("Config should be a json of Configuration object\n");
	Value &conf = config["Configuration"];
	config_return_field_error("Configuration", conf, {"Language", "Dir"});
	Value &lang = conf["Language"];
	for (Value::ConstMemberIterator it = lang.MemberBegin();
			it != lang.MemberEnd(); it++) {
		char* lang_name;
		strcpy(lang_name, it->name.GetString());
		config_return_field_error(lang_name,
				lang[lang_name], {"compile", "run", "clean_up"});
	}
	Value &dir = conf["Dir"];
	if (dir.HasMember("generator"))
		config_return_field_error("Dir", dir, 
				{"var", "generator", "checker", "source", "submission"});
	else if (dir.HasMember("test_input") || dir.HasMember("test_output"))
		config_return_field_error("Dir", dir,
				{"var", "test_input", "test_output", "submission"});
	else
		exit_mess("Cannot specify any type of testing.\n");
}

int main(int argNum, char* arg[]) {
	extract_judging_info(argNum, arg);
	get_config(config);
	//TODO: verify config [Done]
	config_field_verification(config);
	//TODO: testing
	return 0;
}
