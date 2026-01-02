#include "NeonAscendant.h"
#include "MissionGenerator.h"
#include "Modules/ModuleManager.h"

IMPLEMENT_PRIMARY_GAME_MODULE(FNeonAscendantModule, NeonAscendant, "NeonAscendant");

void FNeonAscendantModule::StartupModule()
{
}

void FNeonAscendantModule::ShutdownModule()
{
	// Clean up singleton instances
	UMissionGeneratorSingleton::Shutdown();
}
