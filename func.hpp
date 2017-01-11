#ifndef func_hpp
#define func_hpp

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

void exit_mess(string mess = "", void (*mess_func)() = 0) {
	if (!mess.empty())
		cerr << mess;
	if (mess_func != 0)
		mess_func();
	exit(EXIT_FAILURE);
}
#endif
