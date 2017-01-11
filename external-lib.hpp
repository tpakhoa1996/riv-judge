#ifndef external_lib_hpp
#define external_lib_hpp
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
#define BOOST_NO_CXX11_SCOPED_ENUMS
#include <boost/filesystem.hpp>
#include <boost/process.hpp>
#undef BOOST_NO_CXX11_SCOPED_ENUMS
#include <set>

using namespace std;
namespace bf = ::boost::filesystem;
namespace bp = ::boost::process;

typedef rapidjson::Document Document;
typedef rapidjson::Value Value;
#endif
