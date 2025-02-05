#include "SPTGameMode.h"
#include "SPTPawn.h"
#include "SPTPlayerController.h"

ASPTGameMode::ASPTGameMode()
{
	DefaultPawnClass = ASPTPawn::StaticClass();
	PlayerControllerClass = ASPTPlayerController::StaticClass();

}

