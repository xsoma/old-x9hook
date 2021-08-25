#include "menu.hpp"
#include "../../dependencies/xor.h"
#include "../features/skinchanger/skin_list.h"
#include "config.hpp"
#include "../features/features.hpp"

std::vector<const char*> search_results = { "Skins" };
void skin_input(std::string& search_str, int& skinid, const char* control_name) {
	zgui::text_input(control_name, search_str, 5);
	if (search_str.size() > 0) {
		std::string::size_type sz;
		int i_dec = std::stoi(search_str, &sz);
		if (i_dec >= 0 && i_dec <= 920)
			skinid = i_dec;
	}
}

struct hud_weapons_t {
	std::int32_t* get_weapon_count() {
		return reinterpret_cast<std::int32_t*>(std::uintptr_t(this) + 0x80);
	}
};

template<class T>
static T* FindHudElement(const char* name)
{
	static auto pThis = *reinterpret_cast<DWORD**>(utilities::pattern_scan("client.dll", "B9 ? ? ? ? E8 ? ? ? ? 8B 5D 08") + 1);

	static auto find_hud_element = reinterpret_cast<DWORD(__thiscall*)(void*, const char*)>(utilities::pattern_scan("client.dll", "55 8B EC 53 8B 5D 08 56 57 8B F9 33 F6 39 77 28"));
	return (T*)find_hud_element(pThis, name);
}



void menu::render()
{
	zgui::poll_input("Counter-Strike: Global Offensive");

	static int MenuTab = 1;
	static int VisualsTab = 0;
	static int RageTab = 0;
	static int SkinTab = 0;
	variables::menu::opened = false;
	

	if (zgui::begin_window("", { 601.5, 401.5 }, render::fonts::watermark_font, zgui::zgui_window_flags_no_ontoggle_animation)) // 500, 365
	{
		variables::menu::opened = true;

		zgui::begin_titlebox(XorStr("x9hook/aimwhere [alpha build]"), { 170, 90 }); //  385, 90
		{
			/*if (zgui::tab_button(XorStr("combat#tab"), { 100, 20 })) MenuTab = 1; 
			zgui::next_column(147.5, 17.5); // 140?
			if (zgui::tab_button(XorStr("visuals#tab"), { 100, 20 })) MenuTab = 2;
			if (MenuTab == 2)
			{
				VisualsTab = 0;
			}
			zgui::next_column(280, 17.5);
			if (zgui::tab_button(XorStr("misc#tab"), { 100, 20 })) MenuTab = 3; */

			zgui::next_column(0, 13.79);
			if (zgui::tab_button("combat", { 191,29 }, MenuTab == 1)) MenuTab = 1; // 129, 32
			zgui::next_column(195.5, 13.79); // 147.5
			if (zgui::tab_button("visuals", { 191,29 }, MenuTab == 2)) MenuTab = 2; //128, 20
			zgui::next_column(390, 13.79); // 17.5
			if (zgui::tab_button("misc", { 191,29 }, MenuTab == 3)) MenuTab = 3;
			//zgui::next_column(-9 + 99 * 3, -9);

			zgui::end_titlebox();
		}



		zgui::next_column(0, 45); // 0, 45
		if (MenuTab == 0)
		{if (RageTab == 0) {}}
		else if (MenuTab == 1) // COMBAT TAB
		{
			zgui::begin_groupbox(XorStr("triggerbot"), { 191, 327 }); // aimbot
			{
				zgui::checkbox(XorStr("enable triggerbot"), variables::triggerbot);
				if (variables::triggerbot) {
					zgui::key_bind(XorStr("#tb"), variables::triggerbot_bind);
					//	zgui::checkbox(XorStr("flash check"), variables::triggerbotfc); // still need some work (triggerbot.cpp)
					//	zgui::checkbox(XorStr("smoke check"), variables::triggerbotsc); // still need some work (triggerbot.cpp)
					zgui::slider_int(XorStr("           delay"), 0, 350, variables::trigger_delay);
					//	zgui::slider_float(XorStr("minimum dmg"), 0.f, 100.f, variables::trigger_min_damage);

		//		zgui::checkbox(XorStr("flashbang check"), variables::triggerbotfc); 

					zgui::checkbox(XorStr("magnetise"), variables::trigger_magnet);
					if (variables::trigger_magnet) {
						//		zgui::checkbox(XorStr("smoke check "), variables::triggerbotmsc); // working perfectly fine uwu
						zgui::slider_int(XorStr("             fov"), 0, 90, variables::trigger_fov); // slider_float
						zgui::slider_int(XorStr(" smoothness"), 0, 30, variables::trigger_smooth);

					}
				}


				zgui::end_groupbox();
			}

			zgui::next_column(195, 45);

			zgui::begin_groupbox(XorStr("other combats"), { 191, 327 });
			{
				zgui::checkbox(XorStr("backtrack"), variables::backtrack);
			
				zgui::checkbox(XorStr("recoil control system"), variables::rcs);
				zgui::end_groupbox();

			}
			

			zgui::next_column(390, 45);

			zgui::begin_groupbox(XorStr("anti aim"), { 191, 327 });
			{
				zgui::checkbox(XorStr("enable anti aim"), variables::antiaim);

				//zgui::colorpicker(XorStr("          arm color#nigger"), variables::desyncclr1);
				//zgui::colorpicker(XorStr("          arm color#nigus"), variables::desyncclr2);

				/*const std::vector<zgui::multi_select_item> AACombo{{XorStr("LBY"), &variables::lby},
				{ XorStr("Micromovements"), &variables::micro } };

				zgui::multi_combobox(XorStr("AA Additions"), AACombo);*/

				zgui::end_groupbox();
			}

			
		}
		else if (MenuTab == 2)
		{
			if (VisualsTab == 0)
			{
				zgui::begin_groupbox(XorStr("player chams"), { 191, 327 });
				{
					zgui::checkbox(XorStr("enable chams"), variables::chams);
					if (variables::chams) {
						zgui::colorpicker(XorStr("       visible color"), variables::Chams_Color);
						zgui::checkbox(XorStr("invisible chams"), variables::ignore_z);
						if (variables::ignore_z) {
							zgui::colorpicker(XorStr("     invisible color"), variables::Chams_Color_Z);
						}
						
					//	zgui::slider_float(XorStr("transparency"), 0, 100, variables::transparency);
						
						
					//	zgui::slider_float(XorStr("transparency#chamsex"), 1, 100, variables::bttransparency);
						zgui::combobox("           material#chamssex", { "textured", "flat", "blinking", "crystal", "dark chrome", "glass", "glow", "gold" }, variables::chams_material);

						//  XorStr("none"), XorStr("flat"), XorStr("bark"), XorStr("glossy")
					}

					zgui::checkbox(XorStr("enable backtrack chams"), variables::backtrack_chams);
					if (variables::backtrack_chams) {

						zgui::colorpicker(XorStr("  backtrack color#bt"), variables::Chams_Color_BT);
					}
					



				
					


					
					zgui::end_groupbox();
				}

				zgui::next_column(195, 45);

				zgui::begin_groupbox(XorStr("player esp"), { 191, 327 });
				{
					zgui::checkbox(XorStr("enable esp"), variables::esp);
					if (variables::esp) {
						zgui::checkbox(XorStr("name esp"), variables::name_esp);
						/*zgui::checkbox(XorStr("box esp"), variables::box_esp);*/

						//	zgui::checkbox(XorStr("Ammo ESP"), variables::ammo_esp);

						zgui::checkbox(XorStr("health esp"), variables::hp_esp); // rework needed

						//	zgui::checkbox(XorStr("Weapon ESP"), variables::weapon_esp);
					}

					zgui::end_groupbox();
				}
				

				zgui::next_column(390, 45);

				
				zgui::begin_groupbox(XorStr("other visuals"), { 191, 327 });
				{




					zgui::checkbox(XorStr("enable arm chams"), variables::chams_arms);
					if (variables::chams_arms) {

						zgui::combobox(XorStr("           material#arm"), { "textured", "flat", "blinking",  "crystal" }, variables::chams_arms_mat);
						zgui::colorpicker(XorStr("          color"), variables::Chams_Color_Arm);
						zgui::slider_int(XorStr("transparency#arm"), 0, 100, variables::transparency_arm);

					}
					/*	zgui::checkbox(XorStr("enable sleeve chams"), variables::chams_sleeve);
						if (variables::chams_sleeve) {

							zgui::combobox(XorStr("sleeve material#arm"), { "textured", "flat", "blinking",  "crystal" }, variables::chams_sleeve_mat);
							zgui::colorpicker(XorStr("sleeve color"), variables::Chams_Color_Sleeve);
							zgui::slider_float(XorStr("sleeve transparency#arm"), 0.f, 100.f, variables::transparency_sleeve);

						}*/


					zgui::checkbox(XorStr("enable viewmodel changer"), variables::v);
					if (variables::v) {

						zgui::slider_int(XorStr("              fov"), 0, 45, variables::fov);
						zgui::checkbox("fov while scoping", variables::nigger);
						zgui::slider_int(XorStr("    debug fov"), 0, 45, variables::viewmodel_fov); // -101.f
						zgui::slider_int("          x axis", -25, 25, variables::viewmodelvv[0]);
						zgui::slider_int("          y axis", -25, 25, variables::viewmodelvv[1]);
						zgui::slider_int("          z axis", -25, 25, variables::viewmodelvv[2]);


					}

					zgui::checkbox("enable nightmode", variables::niggerrr);
					if (variables::niggerrr) {
						zgui::slider_int("           value#nightmode", 0, 100, variables::nightmode);
					}

					/*		zgui::checkbox("ambient light sex", variables::al);
							if (variables::al) {
								zgui::slider_float("r", 0, 1, variables::ambientlight[0]);
								zgui::slider_float("g", 0, 1, variables::ambientlight[1]);
								zgui::slider_float("b", 0, 1, variables::ambientlight[2]);
							}*/


							//	zgui::checkbox("rem sleeve", variables::remove_sleeves);
							//	zgui::checkbox("rem hands", variables::remove_hands);

							//	zgui::checkbox(XorStr("Model Glow#arm"), variables::glow_over_arm);
							//	zgui::colorpicker(XorStr("Model Glow Color#arm"), variables::Chams_Color_G_Arm);
							//	zgui::checkbox(XorStr("Wireframe"), variables::chams_arms_wire);
							//	
							//	
					zgui::end_groupbox();
				}
				/*
				zgui::begin_groupbox(XorStr("world"), { 191, 125 });
				{
					//	zgui::checkbox("light presets", variables::niggerrr);
					//	if (variables::niggerrr) {
					//		zgui::checkbox(XorStr("nightmode"), variables::nightmode);
					zgui::checkbox("enable nightmode", variables::nightmodesex);
					if (variables::nightmodesex) {
						
					}

					//	}
	//			
					//	if (variables::r) {
						//	zgui::slider_float(XorStr("value"), 0.f, 100.f, variables::nightmode);
					//	}
					zgui::checkbox("enable skybox changer", variables::skybox);
					if (variables::skybox) {
						zgui::combobox("            skybox", { "baggage", "daylight", "lunacy", "night"}, variables::skybox2);
					}


					zgui::end_groupbox();
				} */

				

				

				

				

			}

			

				
		}
			else if (VisualsTab == 1)
			{
				zgui::begin_groupbox(XorStr("shut the fuck up"), { 190, 325 });
				{
					zgui::end_groupbox();
				}
				zgui::next_column(289, 0);

				zgui::begin_groupbox(XorStr("Misc"), { 191, 325 });
				{
					zgui::end_groupbox();
				}
			}
	// deleted a }

			else if (MenuTab == 3)
		{
			zgui::begin_groupbox(XorStr("misc"), { 191, 260.5 });
			{

				/*
				zgui::checkbox(XorStr("thirdperson"), variables::thirdperson);
				if (variables::thirdperson) {
					zgui::key_bind(XorStr("#tp"), variables::thirdperson_key);
				} */ // [NEED TO FIX!!! READD IF NEEDED]




				//	zgui::checkbox(XorStr("FPS Boost"), variables::fpsboost); // 175
				
				zgui::checkbox(XorStr("rank revealer"), variables::ranks);
			//	zgui::checkbox(XorStr("radar revealer"), variables::radar);
				//	zgui::checkbox(XorStr("sniper crosshair"), variables::sniper_crosshair); // NO WORKY:ccc
			//	zgui::checkbox("left hand knife", variables::leftknife);
				zgui::checkbox(XorStr("grenade prediction"), variables::nade);


				//zgui::checkbox(XorStr("crosshair modifier"), variables::w);
			//	if (variables::w) {

				//	zgui::checkbox(XorStr("recoil crosshair"), variables::recoil_crosshair);


//				}


				 // -101.f// 175
			//	zgui::checkbox(XorStr("ragdoll modifier"), variables::r);
			//	if (variables::r) {
			//		zgui::combobox("gravity", { "default", "froze", "light", "sky force", "ground force" }, variables::r2);
//
	//			}


			//	zgui::checkbox(XorStr("Watermark"), variables::watermark); // 175
			//	if (variables::watermark)
			//		zgui::colorpicker(XorStr("Watermark Color"), variables::Watermark_Color);
			//	zgui::checkbox(XorStr("Chatspam"), variables::chatspam); // 175
				zgui::checkbox(XorStr("clantag changer"), variables::clantag); // 175
				if (variables::clantag) {
					zgui::combobox("            clantag", { "none", "x9hook", "aimwhere", "nn user"/*, ">_<"*/ /*".noclips", "Big Hopper", "BoxESP)", "Hopped", "Kreedz", "movement", "movemint", "SpaceHolding",*/ /*+ weed /*+ cheater*/}, variables::clantagg);
					/*switch (variables::clantagg) {
					case 4:
						zgui::text_input("");*/

				}
				//	zgui::checkbox(XorStr("Spectator List"), variables::spectator_list); // 175
				//	zgui::checkbox(XorStr("Resolver"), variables::resolver); // 175


				

				
				
				

				

				zgui::end_groupbox();
			}

			zgui::begin_groupbox("knife changer", { 191, 63 });
			{
				zgui::checkbox("enable knife changer", variables::skinchanger);
				if (variables::skinchanger) {
					zgui::combobox("             model", { "default", "bayonet", "bowie", "butterfly", "classic", "falchion", "flip", "gut", "huntsman", "karambit", "m9 bayonet", "navaja", "nomad", "paracord", "shadow daggers", "skeleton", "stiletto", "survival", "talon", "ursus" }, variables::knife_model);

				}

				zgui::end_groupbox();
			}

			
			zgui::next_column(195, 45);

			zgui::begin_groupbox("movement", { 191, 327 }); // 191, 240.75
			{
				
				zgui::checkbox("automatic jump", variables::bhop); // 175
			//	if (variables::bhop) {
			//		zgui::slider_float(XorStr("hitchance"), 0.f, 100.f, variables::bhop_chance);
			//	}
				zgui::checkbox("automatic strafe", variables::auto_strafe); // 175
		//		zgui::checkbox("automatic edge jump", variables::edge_jump); // 175
				zgui::checkbox("infinite duck", variables::bullrush); // 175
		//		zgui::checkbox("duck in da air", variables::duckinair);

				
				zgui::checkbox("edge bug", variables::edge_bug); // 175
				if (variables::edge_bug) {
					zgui::key_bind("#eb", variables::edge_bug_key);
				}

				zgui::checkbox("jump bug", variables::jump_bug); // 175
				if (variables::jump_bug) {
					zgui::key_bind("#jb", variables::jump_bug_key);
				}

				
				
				
				
				
				
				zgui::end_groupbox();
			}
			/*
			zgui::begin_groupbox("glove changer", { 191, 75 });
			{
				zgui::checkbox("glove changer", variables::glovechanger);
				if (variables::glovechanger) {
					zgui::combobox("             model", { "default", "bloodhound", "driver", "hand wraps", "hydra", "moto", "specialist", "sport" }, variables::glove_model);
					switch (variables::glove_model) {
					case 1:
						zgui::combobox("                 skin", { "bronzed", "charred", "guerrilla", "snakebite" }, variables::bloodhound_skin);
						break;
					case 2:
						zgui::combobox("                 skin", { "convoy", "crimson weave", "diamondback", "imperial plaid", "king snake", "lunar weave", "overtake", "racing green" }, variables::driver_skin); // +4
						break;
					case 3:
						zgui::combobox("                 skin", { "arboreal", "badlands", "cobalt skulls", "duct tape", "leather", "overprint", "slaughter", "spruce ddpat" }, variables::handwrap_skin); // +4
						break;
					case 4:
						zgui::combobox("                 skin", { "case hardened", "emerald", "mangrove", "rattler" }, variables::hydra_skin);
						break;
					case 5:
						zgui::combobox("                 skin", { "boom!", "cool mint", "eclipse", "polygon", "pow!", "spearmint", "transport", "turtle" }, variables::moto_skin); // +4
						break;
					case 6:
						zgui::combobox("                 skin", { "buckshot", "crimson kimono", "crimson web", "emerald web", "fade", "forest ddpat", "foundation", "mogul" }, variables::specialist_skin); // +4
						break;
					case 7:
						zgui::combobox("                 skin", { "amphibious", "arid", "bronze", "hedge maze",  "omega", "pandora's box", "superconductor", "vice" }, variables::sport_skin); // +4
						break;
					}

				}
			
				zgui::end_groupbox();
			} */

			zgui::next_column(390, 45);

			zgui::begin_groupbox("removals", { 191, 159.5 });
			{

				//	zgui::checkbox(XorStr("remove bloom"), variables::remove_bloom);
				
				//zgui::checkbox(XorStr("disable ragdolls"), variables::noragdoll);
			//	zgui::checkbox(XorStr("remove 3d sky"), variables::remove_3dsky);
			
				//	zgui::checkbox(XorStr("wireframe smoke"), variables::nosmoke); // no worky:C (pasted from aristois ffs)
				
				zgui::checkbox(XorStr("disable panorama blur"), variables::disable_panorama);
				zgui::checkbox(XorStr("remove scope overlay"), variables::removescope);
				zgui::checkbox(XorStr("disable post-processing"), variables::post);
				zgui::checkbox(XorStr("disable shadow rendering"), variables::disable_shadow);
				zgui::checkbox(XorStr("disable flashbang rendering"), variables::noflash); // 175
				zgui::end_groupbox();



			}

			zgui::begin_groupbox(XorStr("config"), { 191, 163.5 }); // 191, 81.25
			{
				
			//	zgui::checkbox("key incicator", variables::keyindicators);
				zgui::checkbox("keybind incicator", variables::keybindindicators);
				zgui::checkbox("velocity indicator", variables::velocity); // 175
			
		
					
				
				zgui::checkbox("custom menu key", variables::menu::yesmenukey);
				if (variables::menu::yesmenukey) {
					zgui::key_bind("#mk", variables::menu::menukey);
				}
				//zgui::colorpicker("          color", variables::sex);
	
			//	zgui::checkbox("region changer", variables::regionchanger);
			//	if (variables::regionchanger) {
			//		zgui::combobox("region", { "amsterdam", "atlanta", "london", "los angeles", "madrid", "saopaulo", "seattle", "sydney", "tokyo" }, variables::region);
			//	}

			
				if (zgui::button("force update", { 180, 15 })) {

					static auto clear_hud_weapon_icon_fn =
						reinterpret_cast<std::int32_t(__thiscall*)(void*, std::int32_t)>(
							utilities::pattern_scan("client.dll", "55 8B EC 51 53 56 8B 75 08 8B D9 57 6B FE 2C 89 5D FC"));

					auto element = FindHudElement<std::uintptr_t*>("CCSGO_HudWeaponSelection");

					if (element)
					{
						auto hud_weapons = reinterpret_cast<hud_weapons_t*>(std::uintptr_t(element) - 0xa0);
						if (hud_weapons != nullptr)
						{

							if (*hud_weapons->get_weapon_count())
							{
								for (std::int32_t i = 0; i < *hud_weapons->get_weapon_count(); i++)
									i = clear_hud_weapon_icon_fn(hud_weapons, i);

								typedef void(*ForceUpdate) (void);
								static ForceUpdate FullUpdate = (ForceUpdate)utilities::pattern_scan("engine.dll", "A1 ? ? ? ? B9 ? ? ? ? 56 FF 50 14 8B 34 85");
								FullUpdate();

								interfaces::clientstate->full_update();

							}
						}
					}

				}

			//	if (zgui::button("get steamid64's", { 180, 15 })) {
			//		misc::dump();
			//	}
			//	if (zgui::button("invisible name", { 180, 13.7 })) {
					// utilities::change_name("\n\xAD\xAD\xAD");
			//	}
				if (zgui::button("release hooks", { 180,15 })) {
			
				hooks::release();
				}
			
			}
		}

		
		zgui::end_window();
	}
	
}

void zgui_line(int x, int y, int x2, int y2, zgui::color c) noexcept
{
	render::draw_line(x, y, x2, y2, color(c.r, c.g, c.b, c.a));
}

void zgui_outline(int x, int y, int w, int h, zgui::color c) noexcept
{
	render::draw_outline(x, y, w, h, color(c.r, c.g, c.b, c.a));
}

void zgui_filled_rect(int x, int y, int w, int h, zgui::color c) noexcept
{
	render::draw_filled_rect(x, y, w, h, color(c.r, c.g, c.b, c.a));
}

void zgui_text(int x, int y, zgui::color c, int font, bool center, const char* text) noexcept
{
	render::draw_text_string(x, y, font, text, center, color(c.r, c.g, c.b, c.a));
}

void zgui_get_text_size(unsigned long font, const char* text, int& width, int& height) noexcept
{
	render::get_text_size(font, text, width, height);
}

float zgui_get_frametime() noexcept
{
	return interfaces::globals->frame_time;
}

float zgui_get_curtime() noexcept
{
	return interfaces::globals->cur_time;
}

void menu::initialize()
{
	variables::menu::opened = false;
	
	zgui::functions.draw_line = zgui_line;
	zgui::functions.draw_rect = zgui_outline;
	zgui::functions.draw_filled_rect = zgui_filled_rect;
	zgui::functions.draw_text = zgui_text;
	zgui::functions.get_text_size = zgui_get_text_size;
	zgui::functions.get_frametime = zgui_get_frametime;
	zgui::functions.get_curtime = zgui_get_curtime;

	//console::log("[setup] menu initialized!\n"); gay fucking nigger no needed
}