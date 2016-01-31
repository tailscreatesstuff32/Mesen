#include "stdafx.h"
#include "StandardController.h"
#include "ControlManager.h"
#include "PPU.h"
#include "EmulationSettings.h"

StandardController::StandardController(uint8_t port)
{
	ControlManager::RegisterControlDevice(this, port);
}

StandardController::~StandardController()
{
	ControlManager::UnregisterControlDevice(this);
}

void StandardController::AddKeyMappings(KeyMapping keyMapping)
{
	_keyMappings.push_back(keyMapping);
}

void StandardController::ClearKeyMappings()
{
	_keyMappings.clear();
}

ButtonState StandardController::GetButtonState()
{
	ButtonState state;

	if(!EmulationSettings::CheckFlag(EmulationFlags::InBackground) || EmulationSettings::CheckFlag(EmulationFlags::AllowBackgroundInput)) {
		for(size_t i = 0, len = _keyMappings.size(); i < len; i++) {
			KeyMapping keyMapping = _keyMappings[i];

			state.A |= ControlManager::IsKeyPressed(keyMapping.A);
			state.B |= ControlManager::IsKeyPressed(keyMapping.B);
			state.Select |= ControlManager::IsKeyPressed(keyMapping.Select);
			state.Start |= ControlManager::IsKeyPressed(keyMapping.Start);
			state.Up |= ControlManager::IsKeyPressed(keyMapping.Up);
			state.Down |= ControlManager::IsKeyPressed(keyMapping.Down);
			state.Left |= ControlManager::IsKeyPressed(keyMapping.Left);
			state.Right |= ControlManager::IsKeyPressed(keyMapping.Right);

			//Turbo buttons - need to be applied for at least 2 reads in a row (some games require this)
			uint8_t turboFreq = 1 << (4 - keyMapping.TurboSpeed);
			bool turboOn = (uint8_t)(PPU::GetFrameCount() % turboFreq) < turboFreq / 2;
			if(turboOn) {
				state.A |= ControlManager::IsKeyPressed(keyMapping.TurboA);
				state.B |= ControlManager::IsKeyPressed(keyMapping.TurboB);
				state.Start |= ControlManager::IsKeyPressed(keyMapping.TurboStart);
				state.Select |= ControlManager::IsKeyPressed(keyMapping.TurboSelect);
			}
		}

		if(!EmulationSettings::CheckFlag(EmulationFlags::AllowInvalidInput)) {
			if(state.Up && state.Down) {
				state.Down = false;
			}
			if(state.Left && state.Right) {
				state.Right = false;
			}
		}
	}

	return state;
}
