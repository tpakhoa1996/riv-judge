#include "base.hpp"
#include "arg-prc.hpp"
#include "config-prc.hpp"

//TODO: research about boost
void testing() {
	Value &conf = config["Configuration"];

}

int main(int argNum, char* arg[]) {
	extract_judging_info(argNum, arg);
	get_config(config);
	config_field_verification(config);
	//TODO: testing
	testing();
	return 0;
}
