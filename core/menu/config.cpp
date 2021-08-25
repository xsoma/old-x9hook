#include "config.hpp"
#include "../../dependencies/reader.hpp"



variabless_t variabless;

CSimpleIniA cfg;

const char* config_files[] = {
	"config1.x9",
	"config2.x9",
	"config3.x9",
	"config4.x9",
	"config5.x9"
};

void config::save()
{
	cfg.SetUnicode(config_files[variabless.miscc.config_file]);
	
	cfg.SetBoolValue("Misc", "infiniteduck", variabless.miscc.bhop);


	cfg.SaveFile(config_files[variabless.miscc.config_file]);

}

void config::load()
{
	cfg.SetUnicode(config_files[variabless.miscc.config_file]);
	cfg.LoadFile(config_files[variabless.miscc.config_file]);

	variabless.miscc.bhop = cfg.GetBoolValue("Misc", "Bhop", variabless.miscc.bhop);
}