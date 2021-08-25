#pragma once

#define tick_interval			(interfaces::globals->interval_per_tick)
#define time_to_ticks( dt )		( (int)( 0.5f + (float)(dt) / tick_interval ) )
#define ticks_to_time( t )		( tick_interval *( t ) )

namespace variables {
	inline int menu_key = VK_INSERT;
	inline int menu_key2 = VK_F11; // todo add dumb nigger

	inline bool mp = false;

	inline bool chams = false;
	inline bool backtrack_chams = false;
	inline int chams_material = 0;

	inline bool desync_chams = false;

	inline int chams_arms_mat = 0;
	inline int chams_weapon_mat = 0;
	inline int chams_sleeve_mat = 0;
	inline bool chams_arms = false;
	inline bool chams_weapon = false;
	inline bool chams_sleeve = false;
	inline bool chams_arms_wire = false;
	inline bool chams_weapon_wire = false;
	inline bool chams_sleeve_wire = false;
	inline int transparency_arm = 0.f;
	inline int transparency_weapon = 0.f;
	inline int transparency_sleeve = 0.f;
	inline float Chams_Color_Arm[3] = { 1.f, 1.f, 1.f };
	inline float Chams_Color_Weapon[3] = { 1.f, 1.f, 1.f };
	inline float Chams_Color_Sleeve[3] = { 1.f, 1.f, 1.f };

	inline bool colorsexe = false;
	inline int sex = 0.f;

	//inline float desyncclr2[3] = { 1.f, 1.f, 1.f };
	//inline float desyncclr1[3] = { 1.f, 1.f, 1.f };

	inline bool remove_hands = false;
	inline bool remove_sleeves = false;
	inline bool leftknife = false;

	inline bool nigger = false;
	inline int niggernigger = 0;
	inline bool chams1 = false;
	inline bool radar = false;
	inline bool noflash = false;
	inline bool ignore_z = false;
	inline float transparency = 0.f;
	inline float bttransparency = 1.f;
	
	inline int viewmodel_fov = 0.f;
	inline int fov = 0.f;
	inline int nightmode = 0.f;
	inline bool nightmodesex= false;
	inline bool fullbright = false;
	inline bool regionchanger = false;
	inline int region = 0;
	inline bool post = false;
	inline bool glow = false;
	inline bool glow_over = false;
	inline bool glow_over_arm = false;
	inline bool flat = false;
	inline bool removescope = false;
	inline bool recoil_crosshair = false;

	inline int weapon_config = 0;

	inline bool rcs = false;
	inline bool aimbot = false;
	inline bool backtrack = false;
	inline float aim_fov = 0;
	inline float smooth = 0;

	inline float aim_fov_rifle = 0;
	inline float smooth_rifle = 0;

	inline float aim_fov_pistol = 0;
	inline float smooth_pistol = 0;

	inline float aim_fov_sniper = 0;
	inline float smooth_sniper = 0;

	inline bool antiaim = false;
	inline bool lby = true;
	inline bool micro = false;

	inline bool triggerbot = false;
	inline int triggerbot_bind = -1;
	inline bool triggerbotmsc = false;
	inline bool triggerbotsc = false;
	inline bool triggerbotfc = false;
	inline bool trigger_magnet = false;
	inline int trigger_fov = 0; // float
	inline int trigger_delay = 0;
	inline int trigger_min_damage = 0;
	inline int trigger_smooth = 0;

	inline bool esp = false;
	inline bool box_esp = false;
	inline bool ammo_esp = false;
	inline bool name_esp = false;
	inline bool hp_esp = false;
	inline bool weapon_esp = false;
	inline bool thirdperson = false;
	inline int thirdperson_key = -1;

	inline int jump_bug_key = -1;
	
	inline int edge_bug_key = -1;
	inline int race = 0;

	inline bool nosmoke = false;
	inline bool bullrush = false;
	inline bool bhop = false;
	inline bool jump_bug = false;
	inline bool edge_bug = false;
	inline bool edge_jump = false;
	inline bool auto_strafe = false;
	inline bool velocity = false;
	inline bool keyindicators = false;
	inline bool keybindindicators = false;
	inline bool fpsboost = false;
	inline bool disable_shadow = false;
	inline bool ranks = false;
	inline bool w = false;
	inline bool sniper_crosshair = false;
	inline bool remove_3dsky = false;
	inline bool remove_bloom = false;
	inline bool disable_panorama = false;
	inline bool r = false;
	inline int r2 = 0;
	inline int viewmodelvv[3];
	inline bool al = false;
	inline int ambientlight[3];
	inline bool skybox = false; 
	inline int skybox2 = false;
	inline bool niggerrr = false;
	inline bool noragdoll = false;
	inline bool v = false;
	inline bool duckinair = false;

	inline bool nade = false;
	inline bool clantag = false;
	inline int clantagg = 0;
	inline bool chatspam = false;
	inline bool watermark = false;
	inline float bhop_chance = 100;
	inline bool spectator_list = false;
	inline bool resolver = false;

	inline float Chams_Color[3] = { 1.f, 0.60f, 0.f };
	inline float Chams_Color_Z[3] = { 0.50f, 0.50f, 1.f };
	inline float Chams_Color_BT[3] = { 0.30f, 0.60f, 1.f };
	inline float Chams_Color_G[3] = { 1.f, 0.5f, 0.6f };
	inline float Chams_Color_G_Arm[3] = { 1.f, 1.f, 0.6f };
	
	inline int Chams_Color_Desync[3] = { 0.5f, 0.1f, 1.f };
	inline int Glow_Color[3] = { 0.5f, 0.1f, 1.f };
	inline int Watermark_Color[3] = { 0.35f, 0.47f, 0.94f };

	inline int Glow_A = 100.f;

	inline bool skinchanger;
	inline int knife_model = 0;
	inline int knife_skin = 0;

	inline bool glovechanger;
	inline int glove_model = 0;
	inline int hydra_skin = 0;
	inline int bloodhound_skin = 0;
	inline int driver_skin = 0;
	inline int handwrap_skin = 0;
	inline int moto_skin = 0;
	inline int specialist_skin = 0;
	inline int sport_skin = 0;
	inline int sex_skin = 0;

	inline int paint_kit_index_usp = 0;
	inline int paint_kit_index_p2000 = 0;
	inline int paint_kit_index_glock = 0;
	inline int paint_kit_index_p250 = 0;
	inline int paint_kit_index_fiveseven = 0;
	inline int paint_kit_index_tec = 0;
	inline int paint_kit_index_cz = 0;
	inline int paint_kit_index_duals = 0;
	inline int paint_kit_index_deagle = 0;
	inline int paint_kit_index_revolver = 0;
	inline int paint_kit_index_famas = 0;
	inline int paint_kit_index_galil = 0;
	inline int paint_kit_index_m4a4 = 0;
	inline int paint_kit_index_m4a1 = 0;
	inline int paint_kit_index_ak47 = 0;
	inline int paint_kit_index_sg553 = 0;
	inline int paint_kit_index_aug = 0;
	inline int paint_kit_index_ssg08 = 0;
	inline int paint_kit_index_awp = 0;
	inline int paint_kit_index_scar = 0;
	inline int paint_kit_index_g3sg1 = 0;
	inline int paint_kit_index_sawoff = 0;
	inline int paint_kit_index_m249 = 0;
	inline int paint_kit_index_negev = 0;
	inline int paint_kit_index_mag7 = 0;
	inline int paint_kit_index_xm1014 = 0;
	inline int paint_kit_index_nova = 0;
	inline int paint_kit_index_bizon = 0;
	inline int paint_kit_index_mp5sd = 0;
	inline int paint_kit_index_mp7 = 0;
	inline int paint_kit_index_mp9 = 0;
	inline int paint_kit_index_mac10 = 0;
	inline int paint_kit_index_p90 = 0;
	inline int paint_kit_index_ump45 = 0;

	namespace menu {
		inline bool opened = false;
		inline bool yesmenukey = false;
		inline int menukey = 45;
		inline bool combat = false;
		inline bool visuals = false;
		inline bool misc = false;

	/*	inline int x = 210, y = 210;
		inline int w = 400, h = 450; // 400, 300*/
	}
}