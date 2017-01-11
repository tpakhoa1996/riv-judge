#ifndef var_hpp
#define var_hpp
#include "external-lib.hpp"

const int dirLen= int(1e3) + 111;
const int configLen = int(1e6) + 111;

char test_dir[dirLen], sub_dir[dirLen], config_dir[dirLen], config_json[configLen];
Document config;

#endif
