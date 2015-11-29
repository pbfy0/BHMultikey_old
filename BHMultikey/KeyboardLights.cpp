#include "stdafx.h"
#include "KeyboardLights.h"
#define DECLSPEC

int DECLSPEC FlashKeyboardLight(HANDLE hKbdDev, UINT led_flag, BOOL on)
{
	KEYBOARD_INDICATOR_PARAMETERS input_buffer;    // Input buffer for DeviceIoControl
	KEYBOARD_INDICATOR_PARAMETERS output_buffer;   // Output buffer for DeviceIoControl
	UINT                LedFlagsMask;
	BOOL                Toggle;
	ULONG               buf_len = sizeof(KEYBOARD_INDICATOR_PARAMETERS);
	ULONG               ReturnedLength; // Number of bytes returned in output buffer
	int             i;
	input_buffer.unit_id = 0;
	output_buffer.unit_id = 0;
	// Preserve current indicators' state
	//
	if (!DeviceIoControl(hKbdDev, IOCTL_KEYBOARD_QUERY_INDICATORS,
		&input_buffer, buf_len,
		&output_buffer, buf_len,
		&ReturnedLength, NULL))
		return GetLastError();
	// Mask bit for light to be manipulated
	//
	//LedFlagsMask = (OutputBuffer.LedFlags & (~LedFlag));
	UINT mask = on ? output_buffer.led_flags | led_flag : output_buffer.led_flags & ~led_flag;
	// Set toggle variable to reflect current state.
	//
	//Toggle = (OutputBuffer.LedFlags & LedFlag);
	//for (i = 0; i < 2; i++)
	//{
		//Toggle ^= 1;
	input_buffer.led_flags = mask;// LedFlagsMask(LedFlagsMask | (LedFlag * Toggle));
	if (!DeviceIoControl(hKbdDev, IOCTL_KEYBOARD_SET_INDICATORS,
		&input_buffer, buf_len,
		NULL, 0, &ReturnedLength, NULL))
		return GetLastError();
		//Sleep(Duration);
	//}
	return 0;
}
