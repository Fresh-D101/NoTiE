#include "Notie.h"
#include "TestApp.h"
#include "Core/Log.h"

//#ifdef _DEBUG
//#define EngineMain main
//#else
//#define EngineMain WinMain
//#endif

#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080

int main()
{
	notie::Log::Init();
	NTE_CORE_INFO("Initialized No Title Engine!");

	auto notie = std::make_unique<Notie>(SCREEN_WIDTH, SCREEN_HEIGHT, "NTTP_Game", false);;

	TestApp testApp;
	notie->RunApplication(testApp);

	return 0;
}