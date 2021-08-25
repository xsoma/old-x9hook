#include "../dependencies/utilities/csgo.hpp"
#include "features/features.hpp"
#include "menu/menu.hpp"

unsigned long WINAPI initialize(void* instance) {
	while (!GetModuleHandleA("serverbrowser.dll"))
		Sleep(200);

#ifdef _DEBUG
	console::initialize("x9hook");
#endif

	try {
		interfaces::initialize();
		render::initialize();
		menu::initialize();
		hooks::initialize();

		//interfaces::engine->execute_cmd(XorStr("echo have fun using x9hook <3"));
		//interfaces::engine->execute_cmd(XorStr("disconnect Loaded!"));
		interfaces::engine->execute_cmd(XorStr("clear"));
	//	interfaces::console->console_printf("\n [x9hook] current version: 1.0 \n");

	}
	
	catch (const std::runtime_error & error) { 
		FreeLibraryAndExitThread(static_cast<HMODULE>(instance), -1);
		/*MessageBoxA(nullptr, error.what(), "something wrong happend..", MB_OK | MB_ICONERROR);
		return;
		/*
		*/
	} 

/*	while (!GetAsyncKeyState(VK_END))
		std::this_thread::sleep_for(std::chrono::milliseconds(50));

	FreeLibraryAndExitThread(static_cast<HMODULE>(instance), 0);*/
}

unsigned long WINAPI release() {
	hooks::release();

#ifdef _DEBUG
	console::release();
#endif

	return TRUE;
}

std::int32_t WINAPI DllMain(const HMODULE instance [[maybe_unused]], const unsigned long reason, const void* reserved [[maybe_unused]] ) {
	DisableThreadLibraryCalls(instance);

	switch (reason) {
	case DLL_PROCESS_ATTACH: {
		if (auto handle = CreateThread(nullptr, NULL, initialize, instance, NULL, nullptr))
			/*CloseHandle(handle);*/

		break;
	}

	case DLL_PROCESS_DETACH: {
		release();
		break;
	}
	}

	return true;
}
