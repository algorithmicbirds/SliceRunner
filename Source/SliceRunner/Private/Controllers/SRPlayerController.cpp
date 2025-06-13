// Copyright (c) 2025 algorithmicbird  -- See MIT License for details.


#include "Controllers/SRPlayerController.h"
#include "Camera/SRPlayerCameraManager.h"

ASRPlayerController::ASRPlayerController()
{
	PlayerCameraManagerClass = ASRPlayerCameraManager::StaticClass();
}