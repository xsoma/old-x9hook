#pragma once


struct variabless_t {

	struct {
	
		int config_file = 0;
		bool bhop = false;

	} miscc;

};

extern variabless_t variabless;

namespace config {
	void save();
	void load();
}