#include "base_config.h"
#ifdef FRONT
static const char* CONFIG_PATH = "Data/";
#else
static const char* CONFIG_PATH = "./conf/";
#endif

void CConfig::load()
{
	clear();
	std::string str(CONFIG_PATH);
	load_config(str);
}
