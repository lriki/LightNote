//==============================================================================
// LFInput
//==============================================================================

#pragma once

#include "stdafx.h"
#include "LFInternal.h"
#include "LFInput.h"

//==============================================================================
// LNInput
//==============================================================================

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNInput_IsPress(int button, LNBool* state)
	{
		LNHandle h;
		LNVirtualPad_GetVirtualPad(0, &h);
		return LNVirtualPad_IsPress(h, button, state);
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNInput_IsTrigger(int button, LNBool* state)
	{
		LNHandle h;
		LNVirtualPad_GetVirtualPad(0, &h);
		return LNVirtualPad_IsTrigger(h, button, state);
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNInput_IsOffTrigger(int button, LNBool* state)
	{
		LNHandle h;
		LNVirtualPad_GetVirtualPad(0, &h);
		return LNVirtualPad_IsOffTrigger(h, button, state);
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNInput_IsRepeat(int button, LNBool* state)
	{
		LNHandle h;
		LNVirtualPad_GetVirtualPad(0, &h);
		return LNVirtualPad_IsRepeat(h, button, state);
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNInput_GetPower(int button, float* state)
	{
		LNHandle h;
		LNVirtualPad_GetVirtualPad(0, &h);
		return LNVirtualPad_GetPower(h, button, state);
	}

	//----------------------------------------------------------------------
	///
	//----------------------------------------------------------------------
	LNResult LNInput_GetAxisState(int axis, LNVector2* state)
	{
		LNHandle h;
		LNVirtualPad_GetVirtualPad(0, &h);
		return LNVirtualPad_GetAxisState(h, axis, state);
	}

#if 0
	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNInput_IsPress(int ctrlIndex, int button, LNBool* state)
	{
		LN_FUNC_TRY_BEGIN;
		*state = LNOTE_BOOL_TO_LNBOOL(
			FuncLibManager::InputManager->getGameController(ctrlIndex)->isPress((Core::Input::LNGameControllerButton)button));
		LN_FUNC_TRY_END_RETURN;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNInput_IsTrigger(int ctrlIndex, int button, LNBool* state)
	{
		LN_FUNC_TRY_BEGIN;
		*state = LNOTE_BOOL_TO_LNBOOL(
			FuncLibManager::InputManager->getGameController(ctrlIndex)->isOnTrigger((Core::Input::LNGameControllerButton)button));
		LN_FUNC_TRY_END_RETURN;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNInput_IsOffTrigger(int ctrlIndex, int button, LNBool* state)
	{
		LN_FUNC_TRY_BEGIN;
		*state = LNOTE_BOOL_TO_LNBOOL(
			FuncLibManager::InputManager->getGameController(ctrlIndex)->isOffTrigger((Core::Input::LNGameControllerButton)button));
		LN_FUNC_TRY_END_RETURN;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNInput_IsRepeat(int ctrlIndex, int button, LNBool* state)
	{
		LN_FUNC_TRY_BEGIN;
		*state = LNOTE_BOOL_TO_LNBOOL(
			FuncLibManager::InputManager->getGameController(ctrlIndex)->isRepeat((Core::Input::LNGameControllerButton)button));
		LN_FUNC_TRY_END_RETURN;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNInput_GetPower(int ctrlIndex, int button, float* state)
	{
		LN_FUNC_TRY_BEGIN;
		*state = FuncLibManager::InputManager->getGameController(ctrlIndex)->getPower((Core::Input::LNGameControllerButton)button);
		LN_FUNC_TRY_END_RETURN;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNInput_GetAxisState(int ctrlIndex, int axis, LNVector2* state)
	{
		LN_FUNC_TRY_BEGIN;
		(*(LVector2*)state) = FuncLibManager::InputManager->getGameController( ctrlIndex )->getAxisState( axis );
		LN_FUNC_TRY_END_RETURN;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNInput_GetPressedDeviceButton(int ctrlIndex, int* deviceButton)
	{
		LN_FUNC_TRY_BEGIN;
		*deviceButton = FuncLibManager::InputManager->getGameController(ctrlIndex)->getPressedButton();
		LN_FUNC_TRY_END_RETURN;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNInput_GetDeviceButtonType(int deviceButton, LNDeviceButtonType* type)
	{
		if ( Input::LN_VBUTTON_KEYBOARD_BEGIN <= deviceButton && deviceButton <= Input::LN_VBUTTON_KEYBOARD_END )
			*type = LN_DEVICEBUTTONTYPE_KEYBOARD;
		if ( Input::LN_VBUTTON_MOUSE_BEGIN <= deviceButton && deviceButton <= Input::LN_VBUTTON_MOUSE_END )
			*type = LN_DEVICEBUTTONTYPE_MOUSE;
		if ( Input::LN_VBUTTON_JOYPAD_BUTTON_BEGIN <= deviceButton && deviceButton <= Input::LN_VBUTTON_JOYPAD_BUTTON_END )
			*type = LN_DEVICEBUTTONTYPE_JOYPAD_BUTTON;
		if ( Input::LN_VBUTTON_JOYPAD_POV_BEGIN <= deviceButton && deviceButton <= Input::LN_VBUTTON_JOYPAD_POV_END )
			*type = LN_DEVICEBUTTONTYPE_JOYPAD_POV;
		if ( Input::LN_VBUTTON_JOYPAD_AXIS_BEGIN <= deviceButton && deviceButton <= Input::LN_VBUTTON_JOYPAD_AXIS_END )
			*type = LN_DEVICEBUTTONTYPE_JOYPAD_AXIS;
		*type = LN_DEVICEBUTTONTYPE_UNKNOWN;
		return ::LN_OK;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNInput_AttachVirtualButton(int ctrlIndex, int virtualButton, int deviceButton)
	{
		LN_FUNC_TRY_BEGIN;
		FuncLibManager::InputManager->getGameController( ctrlIndex )->attachVirtualButton( (Core::Input::LNGameControllerAttachTarget)virtualButton, deviceButton, true );
		LN_FUNC_TRY_END_RETURN;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNInput_DetachVirtualButton(int ctrlIndex, int virtualButton, int deviceButton)
	{
		LN_FUNC_TRY_BEGIN;
		FuncLibManager::InputManager->getGameController(ctrlIndex)->detachVirtualButton((Core::Input::LNGameControllerAttachTarget)virtualButton, deviceButton);
		LN_FUNC_TRY_END_RETURN;
	}
#endif

//==============================================================================
// LNVirtualPad
//==============================================================================

	LNOTE_TYPE_INFO_IMPL(Input::GameController, LNVirtualPad);

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNVirtualPad_GetVirtualPad(int index, LNHandle* virtualPad)
	{
		LN_CHECK_ARG(0 <= index && index < 4);

		*virtualPad = FuncLibManager::GameControllers[index];
		return ::LN_OK;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNVirtualPad_IsPress(LNHandle virtualPad, int button, LNBool* state)
	{
		LN_FUNC_TRY_BEGIN;
		*state = LNOTE_BOOL_TO_LNBOOL(TO_REFOBJ(Input::GameController, virtualPad)->isPress((Input::LNGameControllerButton)button));
		LN_FUNC_TRY_END_RETURN;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNVirtualPad_IsTrigger(LNHandle virtualPad, int button, LNBool* state)
	{
		LN_FUNC_TRY_BEGIN;
		*state = LNOTE_BOOL_TO_LNBOOL(TO_REFOBJ(Input::GameController, virtualPad)->isOnTrigger((Input::LNGameControllerButton)button));
		LN_FUNC_TRY_END_RETURN;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNVirtualPad_IsOffTrigger(LNHandle virtualPad, int button, LNBool* state)
	{
		LN_FUNC_TRY_BEGIN;
		*state = LNOTE_BOOL_TO_LNBOOL(TO_REFOBJ(Input::GameController, virtualPad)->isOffTrigger((Input::LNGameControllerButton)button));
		LN_FUNC_TRY_END_RETURN;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNVirtualPad_IsRepeat(LNHandle virtualPad, int button, LNBool* state)
	{
		LN_FUNC_TRY_BEGIN;
		*state = LNOTE_BOOL_TO_LNBOOL(TO_REFOBJ(Input::GameController, virtualPad)->isRepeat((Input::LNGameControllerButton)button));
		LN_FUNC_TRY_END_RETURN;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNVirtualPad_GetPower(LNHandle virtualPad, int button, float* state)
	{
		LN_FUNC_TRY_BEGIN;
		*state = TO_REFOBJ(Input::GameController, virtualPad)->getPower((Core::Input::LNGameControllerButton)button);
		LN_FUNC_TRY_END_RETURN;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNVirtualPad_GetAxisState(LNHandle virtualPad, int axis, LNVector2* state)
	{
		LN_FUNC_TRY_BEGIN;
		(*(LVector2*)state) = TO_REFOBJ(Input::GameController, virtualPad)->getAxisState(axis);
		LN_FUNC_TRY_END_RETURN;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNVirtualPad_GetPressedDeviceButton(LNHandle virtualPad, int* deviceButton)
	{
		LN_FUNC_TRY_BEGIN;
		*deviceButton = TO_REFOBJ(Input::GameController, virtualPad)->getPressedButton();
		LN_FUNC_TRY_END_RETURN;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNVirtualPad_GetDeviceButtonType(int deviceButton, LNDeviceButtonType* type)
	{
		if (Input::LN_VBUTTON_KEYBOARD_BEGIN <= deviceButton && deviceButton <= Input::LN_VBUTTON_KEYBOARD_END)
			*type = LN_DEVICEBUTTONTYPE_KEYBOARD;
		if (Input::LN_VBUTTON_MOUSE_BEGIN <= deviceButton && deviceButton <= Input::LN_VBUTTON_MOUSE_END)
			*type = LN_DEVICEBUTTONTYPE_MOUSE;
		if (Input::LN_VBUTTON_JOYPAD_BUTTON_BEGIN <= deviceButton && deviceButton <= Input::LN_VBUTTON_JOYPAD_BUTTON_END)
			*type = LN_DEVICEBUTTONTYPE_JOYPAD_BUTTON;
		if (Input::LN_VBUTTON_JOYPAD_POV_BEGIN <= deviceButton && deviceButton <= Input::LN_VBUTTON_JOYPAD_POV_END)
			*type = LN_DEVICEBUTTONTYPE_JOYPAD_POV;
		if (Input::LN_VBUTTON_JOYPAD_AXIS_BEGIN <= deviceButton && deviceButton <= Input::LN_VBUTTON_JOYPAD_AXIS_END)
			*type = LN_DEVICEBUTTONTYPE_JOYPAD_AXIS;
		*type = LN_DEVICEBUTTONTYPE_UNKNOWN;
		return ::LN_OK;
	}

	//----------------------------------------------------------------------
	///
	//----------------------------------------------------------------------
	LNResult LNVirtualPad_AttachVirtualButton(LNHandle virtualPad, int virtualButton, int deviceButton)
	{
		LN_FUNC_TRY_BEGIN;
		TO_REFOBJ(Input::GameController, virtualPad)->attachVirtualButton((Input::LNGameControllerAttachTarget)virtualButton, deviceButton, true);
		LN_FUNC_TRY_END_RETURN;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNVirtualPad_DetachVirtualButton(LNHandle virtualPad, int virtualButton, int deviceButton)
	{
		LN_FUNC_TRY_BEGIN;
		TO_REFOBJ(Input::GameController, virtualPad)->detachVirtualButton((Core::Input::LNGameControllerAttachTarget)virtualButton, deviceButton);
		LN_FUNC_TRY_END_RETURN;
	}

