#include "../features.hpp"
#include  "engine_prediction.hpp"
#include  "../../../dependencies/xor.h"
#include "../../menu/config.hpp"

#define D3DX_PI 3.1415926535897932384626

void misc::movement::bunny_hop(c_usercmd* cmd) {
	if (!variables::bhop)
		return;

	if (variables::jump_bug && GetAsyncKeyState(variables::jump_bug_key))
		return;

	const int move_type = csgo::local_player->move_type();

	if (move_type == movetype_ladder || move_type == movetype_noclip || move_type == movetype_observer)
		return;

	if (!(csgo::local_player->flags() & fl_onground))
		cmd->buttons &= ~in_jump;
};

void misc::movement::auto_strafe(c_usercmd* cmd) {

	if (!variables::auto_strafe)
		return;

	if ((csgo::local_player->flags() & fl_onground))
		return;

	static auto side = 1.f;
	side = -side;

	auto velocity = csgo::local_player->velocity();

	vec3_t wish_angle = cmd->viewangles;

	auto speed = velocity.length_2d();
	auto ideal_strafe = std::clamp(RAD2DEG(atan(15.f / speed)), 0.f, 90.f);

	cmd->forwardmove = 0.f;

	static auto cl_sidespeed =interfaces::console->get_convar(XorStr("cl_sidespeed"));

	static float old_y = 0.f;
	auto y_delta = std::remainderf(wish_angle.y - old_y, 360.f);
	auto abs_y_delta = abs(y_delta);
	old_y = wish_angle.y;

	const auto cl_sidespeed_value = cl_sidespeed->get_float();

	if (abs_y_delta <= ideal_strafe || abs_y_delta >= 30.f)
	{
		vec3_t velocity_direction;
		math::VectorAngles(velocity, velocity_direction);
		auto velocity_delta = std::remainderf(wish_angle.y - velocity_direction.y, 360.0f);
		auto retrack = std::clamp(RAD2DEG(atan(30.f / speed)), 0.f, 90.f) * 2.f;
		if (velocity_delta <= retrack || speed <= 15.f)
		{
			if (-retrack <= velocity_delta || speed <= 15.0f)
			{
				wish_angle.y += side * ideal_strafe;
				cmd->sidemove = cl_sidespeed_value * side;
			}
			else
			{
				wish_angle.y = velocity_direction.y - retrack;
				cmd->sidemove = cl_sidespeed_value;
			}
		}
		else
		{
			wish_angle.y = velocity_direction.y + retrack;
			cmd->sidemove = -cl_sidespeed_value;
		}

		math::CorrectMovement(cmd, wish_angle, cmd->viewangles);
	}
	else if (y_delta > 0.f)
		cmd->sidemove = -cl_sidespeed_value;
	else
		cmd->sidemove = cl_sidespeed_value;
}

bool unduck;

void misc::movement::DoJumpBug(c_usercmd* cmd)
{
	if (!variables::jump_bug || !GetAsyncKeyState(variables::jump_bug_key))
		return;

	if (!csgo::local_player || !csgo::local_player->is_alive())
		return;

	float max_radias = D3DX_PI * 2;
	float step = max_radias / 128; // normal is 128
	float xThick = 25;


	if (variables::jump_bug && GetAsyncKeyState(variables::jump_bug_key) && (csgo::local_player->flags() & (1 << 0)))
	{
		int screenWidth, screenHeight;
		interfaces::engine->get_screen_size(screenWidth, screenHeight);
		if (unduck)
		{
			cmd->buttons &= ~in_duck; // duck
			cmd->buttons |= in_jump; // jump
			unduck = false;
		}
		vec3_t pos = csgo::local_player->origin();
		for (float a = 0.f; a < max_radias; a += step)
		{
			vec3_t pt;
			pt.x = (xThick * cos(a)) + pos.x;
			pt.y = (xThick * sin(a)) + pos.y;
			pt.z = pos.z;


			vec3_t pt2 = pt;
			pt2.z -= 6;

			trace_t fag;

			ray_t ray;
			ray.initialize(pt, pt2);

			trace_filter flt;
			flt.skip = csgo::local_player;
			interfaces::trace_ray->trace_ray(ray, MASK_SOLID_BRUSHONLY, &flt, &fag);

			if (fag.flFraction != 1.f && fag.flFraction != 0.f)
			{
				cmd->buttons |= in_duck; // duck
				cmd->buttons &= ~in_jump; // jump
				unduck = true;
			}
		}
		for (float a = 0.f; a < max_radias; a += step)
		{
			vec3_t pt;
			pt.x = ((xThick - 2.f) * cos(a)) + pos.x;
			pt.y = ((xThick - 2.f) * sin(a)) + pos.y;
			pt.z = pos.z;

			vec3_t pt2 = pt;
			pt2.z -= 6;

			trace_t fag;

			ray_t ray;
			ray.initialize(pt, pt2);

			trace_filter flt;
			flt.skip = csgo::local_player;
			interfaces::trace_ray->trace_ray(ray, MASK_SOLID_BRUSHONLY, &flt, &fag);

			if (fag.flFraction != 1.f && fag.flFraction != 0.f)
			{
				cmd->buttons |= in_duck; // duck
				cmd->buttons &= ~in_jump; // jump
				unduck = true;
			}
		}
		for (float a = 0.f; a < max_radias; a += step)
		{
			vec3_t pt;
			pt.x = ((xThick - 20.f) * cos(a)) + pos.x;
			pt.y = ((xThick - 20.f) * sin(a)) + pos.y;
			pt.z = pos.z;

			vec3_t pt2 = pt;
			pt2.z -= 6;

			trace_t fag;

			ray_t ray;
			ray.initialize(pt, pt2);

			trace_filter flt;
			flt.skip = csgo::local_player;
			interfaces::trace_ray->trace_ray(ray, MASK_SOLID_BRUSHONLY, &flt, &fag);

			if (fag.flFraction != 1.f && fag.flFraction != 0.f)
			{
				cmd->buttons |= in_duck; // duck
				cmd->buttons &= ~in_jump; // jump
				unduck = true;
			}
		}
	}
}



void misc::regionchanger() {
	if (!variables::regionchanger)
		return;

	switch (variables::region) {

	case 0:
		interfaces::engine->execute_cmd("sdr SDRClient_ForceRelayCluster ams");
		break;
	case 1:
		interfaces::engine->execute_cmd("sdr SDRClient_ForceRelayCluster atl");
		break;
	case 2:
		interfaces::engine->execute_cmd("sdr SDRClient_ForceRelayCluster lhr");
		break;
	case 3:
		interfaces::engine->execute_cmd("sdr SDRClient_ForceRelayCluster lax");
		break;
	case 4:
		interfaces::engine->execute_cmd("sdr SDRClient_ForceRelayCluster mad");
		break;
	case 5:
		interfaces::engine->execute_cmd("sdr SDRClient_ForceRelayCluster gru");
		break;
	case 6:
		interfaces::engine->execute_cmd("sdr SDRClient_ForceRelayCluster sea");
		break;
	case 7:
		interfaces::engine->execute_cmd("sdr SDRClient_ForceRelayCluster syd");
		break;
	case 8:
		interfaces::engine->execute_cmd("sdr SDRClient_ForceRelayCluster tyo");
		break;

	}
}


void misc::movement::edgebug(c_usercmd* cmd)
{
	if (!variables::edge_bug || !GetAsyncKeyState(variables::edge_bug_key))
		return;

	if (variables::jump_bug == true && GetAsyncKeyState(variables::jump_bug_key))
		return;

	if (!csgo::local_player || !csgo::local_player->is_alive())
		return;

	if (!(prediction::getFlags() & fl_onground) && csgo::local_player->flags() & fl_onground)
		cmd->buttons |= in_duck;
}

void misc::movement::edgeJump(c_usercmd* cmd) {
	if (!(variables::edge_jump)) return;

	player_t* localPlayer = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(interfaces::engine->get_local_player()));


	static int moveTipo = localPlayer->move_type();


	if (moveTipo == movetype_ladder) return;

	if ((prediction::getFlags() & 1) && !(localPlayer->flags() & 1))
		cmd->buttons |= in_jump;
}

void misc::visual::radar() {

	if (!variables::radar)
		return;

	for (int i = 1; i <= interfaces::globals->max_clients; i++) {

		entity_t* ent = reinterpret_cast<entity_t*>(interfaces::entity_list->get_client_entity(i));

		if (!ent)
			return;

		if (ent->team() != csgo::local_player->team() && ent->index() != csgo::local_player->index()) {
			ent->spotted() = true;
		}
	}
}

void misc::visual::noflash() {

	player_t* local = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(interfaces::engine->get_local_player()));

	if (variables::noflash) {
		local->flash_alpha() = 0.f;
	}
	else {
		local->flash_alpha() = 255.f;
	}
}

void set_clantag(const char* tag)
{
	static auto ret = (int(__fastcall*)(const char*, const char*))utilities::pattern_scan(XorStr("engine.dll"), XorStr("53 56 57 8B DA 8B F9 FF 15"));

	ret(tag, tag);
}

void duckinair(c_usercmd* cmd) {

	
	
	if (!csgo::local_player || !csgo::local_player->is_alive() || !variables::duckinair)
		return;

	if (variables::duckinair) {
		if (cmd->buttons = ~in_jump) {

			cmd->buttons = in_duck;

		}
	}

	

}

void misc::clantag() {
	static bool reset = false;
	static float old_curtime = interfaces::globals->cur_time;

	if (!variables::clantag && reset == false) {
		reset = true;
		set_clantag(XorStr(" "));
	}
	else if (variables::clantag) {

		switch (variables::clantagg) {
		case 0:
			set_clantag(XorStr(" "));
			break;
		case 1:
			set_clantag(XorStr("x9hook"));
			/*if (interfaces::globals->cur_time > old_curtime + 0.25f) {
				switch (int(old_curtime) % 19) {
				case 0:set_clantag(XorStr("x9hook")); break;
				case 1:set_clantag(XorStr("x9hook")); break;
				case 2:set_clantag(XorStr("x9hook")); break;
				case 3:set_clantag(XorStr("x9hook")); break;
				case 4:set_clantag(XorStr("x9hook")); break;
				case 5:set_clantag(XorStr("x9hook")); break;
				case 6:set_clantag(XorStr("x9hook")); break;
				case 7:set_clantag(XorStr("x9hook")); break;
				case 8:set_clantag(XorStr("x9hook")); break;
				case 9:set_clantag(XorStr("x9hook")); break;
				case 10:set_clantag(XorStr("x9hook")); break;
				case 11:set_clantag(XorStr("x9hook")); break;
				case 12:set_clantag(XorStr("x9hook")); break;
				case 13:set_clantag(XorStr("x9hook")); break;
				case 14:set_clantag(XorStr("x9hook")); break;
				case 15:set_clantag(XorStr("x9hook")); break;
				case 16:set_clantag(XorStr("x9hook")); break;
				case 17:set_clantag(XorStr("x9hook")); break;
				case 18:set_clantag(XorStr("x9hook")); break;
				case 19:set_clantag(XorStr("x9hook")); break;
				}
				old_curtime = interfaces::globals->cur_time;
			}*/
			reset = false;
			break;
		case 2:
			set_clantag(XorStr("aimwhere"));
			/*if (interfaces::globals->cur_time > old_curtime + 0.25f) {
				switch (int(old_curtime) % 19) {
				case 0:set_clantag(XorStr("boxesp")); break;
				case 1:set_clantag(XorStr("boxesp")); break;
				case 2:set_clantag(XorStr("boxesp")); break;
				case 3:set_clantag(XorStr("boxesp")); break;
				case 4:set_clantag(XorStr("boxesp")); break;
				case 5:set_clantag(XorStr("boxesp")); break;
				case 6:set_clantag(XorStr("boxesp")); break;
				case 7:set_clantag(XorStr("boxesp")); break;
				case 8:set_clantag(XorStr("boxesp")); break;
				case 9:set_clantag(XorStr("boxesp")); break;
				case 10:set_clantag(XorStr("boxesp")); break;
				case 11:set_clantag(XorStr("boxesp")); break;
				case 12:set_clantag(XorStr("boxesp")); break;
				case 13:set_clantag(XorStr("boxesp")); break;
				case 14:set_clantag(XorStr("boxesp")); break;
				case 15:set_clantag(XorStr("boxesp")); break;
				case 16:set_clantag(XorStr("boxesp")); break;
				case 17:set_clantag(XorStr("boxesp")); break;
				case 18:set_clantag(XorStr("boxesp")); break;
				case 19:set_clantag(XorStr("boxesp")); break;
				}
				old_curtime = interfaces::globals->cur_time;
			}*/
			reset = false;
			break;
		case 3:
			set_clantag(XorStr("nn x9 user // "));
		/*	if (interfaces::globals->cur_time > old_curtime + 0.25f) {
				switch (int(old_curtime) % 19) {
				case 0:set_clantag(XorStr(">_<")); break;
				case 1:set_clantag(XorStr(">_<")); break;
				case 2:set_clantag(XorStr(">_<")); break;
				case 3:set_clantag(XorStr(">_<")); break;
				case 4:set_clantag(XorStr(">_<")); break;
				case 5:set_clantag(XorStr(">_<")); break;
				case 6:set_clantag(XorStr(">_-")); break;
				case 7:set_clantag(XorStr(">_-")); break;
				case 8:set_clantag(XorStr(">_-")); break;
				case 9:set_clantag(XorStr(">_<")); break;
				case 10:set_clantag(XorStr(">_<")); break;
				case 11:set_clantag(XorStr(">_<")); break;
				case 12:set_clantag(XorStr("-_<")); break;
				case 13:set_clantag(XorStr("-_<")); break;
				case 14:set_clantag(XorStr("-_<")); break;
				case 15:set_clantag(XorStr("-_<")); break;
				case 16:set_clantag(XorStr(">_<")); break;
				case 17:set_clantag(XorStr(">_<")); break;
				case 18:set_clantag(XorStr(">_<")); break;
				case 19:set_clantag(XorStr(">_<")); break;
				}
				old_curtime = interfaces::globals->cur_time;
			}*/
			reset = false;
			break;
		
		/*case 5:
			set_clantag(XorStr("Hopped"));
			reset = false;
			break;
		case 6:
			set_clantag(XorStr("Kreedz"));
			reset = false;
			break;
		case 7:
			set_clantag(XorStr("movement"));
			reset = false;
			break;
		case 8:
			set_clantag(XorStr("movemint"));
			reset = false;
			break;
		case 9:
			set_clantag(XorStr("SpaceHolding"));
			reset = false;
			break;
		case 10:
			set_clantag(XorStr("weed"));
			reset = false;
			break;
			*/
		}
		/*if (interfaces::globals->cur_time > old_curtime + 0.25f) {
			switch (int(old_curtime) % 19) {
			case 0:set_clantag(XorStr("x9hook")); break;
			case 1:set_clantag(XorStr("x9hook")); break;
			case 2:set_clantag(XorStr("x9hook")); break;
			case 3:set_clantag(XorStr("x9hook")); break;
			case 4:set_clantag(XorStr("x9hook")); break;
			case 5:set_clantag(XorStr("x9hook")); break;
			case 6:set_clantag(XorStr("x9hook")); break;
			case 7:set_clantag(XorStr("x9hook")); break;
			case 8:set_clantag(XorStr("x9hook")); break;
			case 9:set_clantag(XorStr("x9hook")); break;
			case 10:set_clantag(XorStr("x9hook")); break;
			case 11:set_clantag(XorStr("x9hook")); break;
			case 12:set_clantag(XorStr("x9hook")); break;
			case 13:set_clantag(XorStr("x9hook")); break;
			case 14:set_clantag(XorStr("x9hook")); break;
			case 15:set_clantag(XorStr("x9hook")); break;
			case 16:set_clantag(XorStr("x9hook")); break; 
			case 17:set_clantag(XorStr("x9hook")); break;
			case 18:set_clantag(XorStr("x9hook")); break;
			case 19:set_clantag(XorStr("x9hook")); break;
			}
			old_curtime = interfaces::globals->cur_time;
		}
		reset = false;*/
	}
}

void misc::chatspam() {
	static float old_curtime = interfaces::globals->cur_time;
	if (variables::chatspam) {
		if (interfaces::globals->cur_time > old_curtime + 2.f) {
			interfaces::engine->execute_cmd(XorStr("say Get Good Get DogeSense"));
			old_curtime = interfaces::globals->cur_time;
		}
	}
}



/*void misc::fpsboost() {
	static auto postprocess = interfaces::console->get_convar(XorStr("mat_postprocess_enable"));
	static auto blur = interfaces::console->get_convar(XorStr("@panorama_disable_blur"));
	static auto postprocess_enable = interfaces::console->get_convar("cl_csm_enabled");

	if (variables::fpsboost && !once) {
		postprocess->set_value(0);
		postprocess_enable->set_value(0);
		blur->set_value(1);
		once = true;
	}
	else if (once && !variables::fpsboost)
	{
		postprocess->set_value(1);
		postprocess_enable->set_value(1);
		blur->set_value(0);
		once = false;
	}
}*/

bool once2 = false;

void misc::disable_shadow() {
	static auto shadows = interfaces::console->get_convar("cl_csm_enabled");
	//mat_postprocess_enable->set_value(variables::disable_shadowvv ? 0 : 1);
	/*if (variables::disable_shadow) {
		interfaces::engine->execute_cmd(XorStr("cl_csm_enabled 0"));
	}
	else if (!variables::disable_shadow) {
		interfaces::engine->execute_cmd(XorStr("cl_csm_enabled 1"));
	}*/

	if (variables::disable_shadow && !once2) {
		//	postprocess->set_value(0);
		//	postprocess_enable->set_value(0);
		shadows->set_value(0);
		once2 = true;
	}
	else if (once2 && !variables::disable_shadow)
	{
		//	postprocess->set_value(1);
		//	postprocess_enable->set_value(1);
		shadows->set_value(1);
		once2 = false;
	}

}

bool once3 = false;

void misc::remove_3dsky() {
	static auto d3 = interfaces::console->get_convar("r_3dsky");
	/*if (variables::remove_3dsky) {
		interfaces::engine->execute_cmd(XorStr("r_3dsky 0"));
	}
	else if (!variables::remove_3dsky) {
		interfaces::engine->execute_cmd(XorStr("r_3dsky 1"));
	}*/
	// mat_postprocess_enable->set_value(variables::remove_3dskyvv ? 0 : 1);

	if (variables::remove_3dsky && !once3) {
		//	postprocess->set_value(0);
		//	postprocess_enable->set_value(0);
		d3->set_value(0);
		once3 = true;
	}
	else if (once3 && !variables::remove_3dsky)
	{
		//	postprocess->set_value(1);
		//	postprocess_enable->set_value(1);
		d3->set_value(1);
		once3 = false;
	}
}

void misc::remove_bloom() {
	static auto mat_postprocess_enable = interfaces::console->get_convar("mat_disable_bloom");
	/*if (variables::remove_bloom) {
		mat_postprocess_enable->set_value(0);
	}
	else if (!variables::remove_bloom) {
		mat_postprocess_enable->set_value(1);
	}*/
	//mat_postprocess_enable->set_value(variables::remove_bloomvv ? 0 : 1);

	
}

bool once = false;

void misc::disable_panorama() {
	static auto postprocess = interfaces::console->get_convar(XorStr("mat_postprocess_enable"));
	static auto blur = interfaces::console->get_convar(XorStr("@panorama_disable_blur"));
	static auto postprocess_enable = interfaces::console->get_convar("cl_csm_enabled");

	if (variables::disable_panorama && !once) {
	//	postprocess->set_value(0);
	//	postprocess_enable->set_value(0);
		blur->set_value(1);
		once = true;
	}
	else if (once && !variables::disable_panorama)
	{
	//	postprocess->set_value(1);
	//	postprocess_enable->set_value(1);
		blur->set_value(0);
		once = false;
	}
}

void misc::dump() {

	if (!interfaces::engine->is_connected() && !interfaces::engine->is_in_game()) {
	//	utilities::console_warning("[dump id] ");
		interfaces::console->console_printf("[x9hook] didn't find any players. you must be in a game to dump player steamid64's. \n");
		return;
	}

	static constexpr auto steam_id64_constant = 0x0110000100000000ULL;

	for (int i = 1; i <= interfaces::globals->max_clients; i++) {
		auto entity = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(i));

		if (!entity)
			continue;

		player_info_t info;
		interfaces::engine->get_player_info(i, &info);

		auto steam_id64 = steam_id64_constant + info.friendsid;

		std::stringstream output;

		std::string player_name = info.name;
		std::transform(player_name.begin(), player_name.end(), player_name.begin(), ::tolower);

		if (!info.fakeplayer) {
		//	utilities::console_warning("[dump id] ");
			output << "[x9hook dump] display name: " << player_name.c_str() << "; id: " << steam_id64 << "\n";
		}
		else {
			
		//	utilities::console_warning("[dump id] ");
		//	output << "name: " << player_name.c_str() << ", id: " << "n/a" << "\n";
		}

		interfaces::console->console_printf("%s", output.str());
		interfaces::console->console_printf("\n [x9hook] all steamid64's were succesfully dumped. \n");
	}

}


void misc::nosmoke() {

	if (variables::nosmoke)
		return;

	if (!interfaces::engine->is_connected() && !interfaces::engine->is_in_game())
		return;

	static auto smoke_count = *reinterpret_cast<uint32_t**>(utilities::pattern_scan("client.dll", "A3 ? ? ? ? 57 8B CB") + 1);
	

	static std::vector<const char*> smoke_materials = {
	/*	"particle/vistasmokev1/vistasmokev1_fire",
		"particle/vistasmokev1/vistasmokev1_smokegrenade",
		"particle/vistasmokev1/vistasmokev1_emods",
		"particle/vistasmokev1/vistasmokev1_emods_impactdust"*/
	"particle/vistasmokev1/vistasmokev1",
	"particle/vistasmokev1/vistasmokev1_emods",
	"particle/vistasmokev1/vistasmokev1_emods_impactdust",
	"particle/vistasmokev1/vistasmokev1_fire",
	"particle/vistasmokev1/vistasmokev1_fire2",
	"particle/vistasmokev1/vistasmokev1_fire2_fogged",
	"particle/vistasmokev1/vistasmokev1_min_depth_nearcull",
	"particle/vistasmokev1/vistasmokev1_nearcull",
	"particle/vistasmokev1/vistasmokev1_nearcull_fog",
	"particle/vistasmokev1/vistasmokev1_nearcull_nodepth",
	"particle/vistasmokev1/vistasmokev1_no_rgb",
	"particle/vistasmokev1/vistasmokev1_smokegrenade",
	"particle/vistasmokev1/vistasmokev4_addself_nearcull",
	"particle/vistasmokev1/vistasmokev4_emods_nocul",
	"particle/vistasmokev1/vistasmokev4_nearcull",
	"particle/vistasmokev1/vistasmokev4_nocull",
	};

	for (auto material_name : smoke_materials) {
		i_material* smoke = interfaces::material_system->find_material(material_name, TEXTURE_GROUP_OTHER);
	//	smoke->increment_reference_count();
		smoke->set_material_var_flag(MATERIAL_VAR_WIREFRAME, true);

		*(int*)smoke_count = 0;
	}

}

void misc::noragdoll() {

	static auto gay = interfaces::console->get_convar(XorStr("cl_disable_ragdolls"));

	if (variables::noragdoll) {
		interfaces::engine->execute_cmd(XorStr("cl_disable_ragdolls 1"));
	}
	else if (!variables::noragdoll) {
		interfaces::engine->execute_cmd(XorStr("cl_disable_ragdolls 0"));
	}

}

void misc::visual::sniper_crosshair() {
	if (interfaces::engine->is_in_game() && interfaces::engine->is_connected() && csgo::local_player) {
		static convar* weapon_debug_spread_show = interfaces::console->get_convar("weapon_debug_spread_show");
		if (variables::sniper_crosshair && csgo::local_player->is_alive() && !csgo::local_player->is_scoped()) {
			if (weapon_debug_spread_show->get_int() != 69)
				weapon_debug_spread_show->set_value(69);
		}
		else {
			if (weapon_debug_spread_show->get_int() != 0)
				weapon_debug_spread_show->set_value(0);
		}
	}
}

void misc::r() {

	if (!variables::r) {
		interfaces::engine->execute_cmd(XorStr("cl_ragdoll_gravity 600"));
	}

	else if (variables::r) {

		switch (variables::r2) {
		case 0:
			interfaces::engine->execute_cmd(XorStr("cl_ragdoll_gravity 600"));
			break;
		case 1:
			interfaces::engine->execute_cmd(XorStr("cl_ragdoll_gravity 1000000000000000000000000000"));
			break;
		case 2:
			interfaces::engine->execute_cmd(XorStr("cl_ragdoll_gravity -600"));
			break;
		case 3:
			interfaces::engine->execute_cmd(XorStr("cl_ragdoll_gravity -10000000"));
			break;
		case 4:
			interfaces::engine->execute_cmd(XorStr("cl_ragdoll_gravity 10000000"));
			break;
		
		
		}

	}

}