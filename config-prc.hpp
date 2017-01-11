#ifndef config_prc_hpp
#define config_prc_hpp
#include "base.hpp"

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
#endif
