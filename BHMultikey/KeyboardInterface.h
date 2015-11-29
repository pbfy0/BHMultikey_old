#pragma once
#include <winsock2.h>
#include <map>

#pragma pack(push,1)
struct input_packet {
	BYTE joystick;
	FLOAT value;
	USHORT name_len;
	CHAR name[32];
};
#pragma pack(pop)
class KeyboardInterface
{
public:
	KeyboardInterface(HANDLE handle, int vjoy_id);
	virtual ~KeyboardInterface();

	void ResetJoystick();

	void HandleInput(USHORT keycode, BOOL keyPressed);


	static int n_kis;

	static void Init();
	static KeyboardInterface* Add(HANDLE handle);
	static void HandleInput(HANDLE handle, USHORT keycode, BOOL keyPressed);
	void SendInput(const char *input, FLOAT value);
	static void ToggleKeyboard(BOOL enabled);
	static KeyboardInterface* list[8];
	static SOCKET i_socket;
	static struct sockaddr_in to_addr;
	static char *order;
private:
	int vjoy_id;
	HANDLE handle;
	struct input_packet packet;
	int record_idx;

	std::map<USHORT, char> *keymap;
	BOOL up, down, left, right;
	BOOL up_jump;
	CHAR taunting;
};

