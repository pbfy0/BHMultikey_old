#include "stdafx.h"
#include "KeyboardInterface.h"
 
#pragma comment(lib,"ws2_32.lib") //Winsock Library

int KeyboardInterface::n_kis = 0;
KeyboardInterface *KeyboardInterface::list[] = { NULL };
struct sockaddr_in KeyboardInterface::to_addr = {};
SOCKET KeyboardInterface::i_socket = NULL;
char *KeyboardInterface::order = "WASD HJKL";
//struct input_packet KeyboardInterface::packet = {};

KeyboardInterface::KeyboardInterface(HANDLE handle, int vjoy_id)
{
	this->handle = handle;
	this->packet.joystick = vjoy_id;
	this->vjoy_id = vjoy_id;
	this->taunting = 0;
	this->up_jump = false;
	this->record_idx = -1;
}


KeyboardInterface::~KeyboardInterface()
{
	if (keymap != NULL) {
		delete keymap;
	}
}

void KeyboardInterface::Init() {
	WSADATA wsa;
	WSAStartup(MAKEWORD(2, 2), &wsa);
	i_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	to_addr.sin_family = AF_INET;
	to_addr.sin_port = htons(13337);
	to_addr.sin_addr.S_un.S_addr = 0x0100007f;// ("127.0.0.1");
	ToggleKeyboard(TRUE);
}

KeyboardInterface* KeyboardInterface::Add(HANDLE handle) {
	int id = n_kis++;
	KeyboardInterface *n = new KeyboardInterface(handle, id);
	KeyboardInterface::list[id] = n;
	return n;
}
void KeyboardInterface::HandleInput(HANDLE handle, USHORT keycode, BOOL keyPressed) {
	for (int i = 0; i < n_kis; i++) {
		if (list[i]->handle == handle) {
			list[i]->HandleInput(keycode, keyPressed);
			return;
		}
	}
	Add(handle)->HandleInput(keycode, keyPressed);
}

void KeyboardInterface::SendInput(const char *input, FLOAT value) {
	packet.value = value;
	packet.name_len = strlen(input);
	strcpy_s(packet.name, input);
	size_t ts = sizeof(struct input_packet) - 32 + packet.name_len;
	sendto(i_socket, (const char *)&packet, ts, 0, reinterpret_cast<const sockaddr *>(&to_addr), sizeof(struct sockaddr_in));
}

void KeyboardInterface::ToggleKeyboard(BOOL disabled) {
	char x[2];
	x[0] = 0xff;
	x[1] = disabled;
	sendto(i_socket, x, 2, 0, reinterpret_cast<const sockaddr *>(&to_addr), sizeof(struct sockaddr_in));
}

void KeyboardInterface::HandleInput(USHORT keycode, BOOL keyPressed)
{
	if (record_idx != -1) {
		if (keyPressed) {
			if (keycode == VK_OEM_5) {
				record_idx = -1;
				delete keymap;
				keymap = NULL;
				return;
			}
			(*keymap)[keycode] = order[record_idx++];
			if (record_idx >= strlen(order)) record_idx = -1;
		}
		return;
	}
	if (keymap != NULL && (*keymap)[keycode]) {
		keycode = (*keymap)[keycode];
	}
	switch (keycode) {
		case ' ':
			SendInput("Y", keyPressed);
			//SetBtn(keyPressed, vjoy_id, 3);
			break;
		case 'H':
		case 'V':
			//SetBtn(keyPressed, vjoy_id, 5);
			SendInput("LB", keyPressed);
			break;
		case 'J':
		case 'C':
			//SetBtn(keyPressed, vjoy_id, 1);
			SendInput("A", keyPressed);
			break;
		case 'K':
		case 'X':
			//SetBtn(keyPressed, vjoy_id, 2);
			SendInput("X", keyPressed);
			break;
		case 'L':
		case 'Z':
		case VK_SHIFT:
			//SetBtn(keyPressed, 1, 6);
			//SetAxis(keyPressed ? 0x8000 : 0x4000, vjoy_id, HID_USAGE_RZ);
			SendInput("B", keyPressed);
			break;
		case 'W':
		case VK_UP:
			if(up_jump)
				SendInput("Y", keyPressed);
			up = keyPressed;
			//SetAxis((1 + down - up) * 0x4000, vjoy_id, HID_USAGE_Y);
			SendInput("A_LSY", up - down);
			break;
		case 'S':
		case VK_DOWN:
			down = keyPressed;
			//SetAxis((1 + down - up) * 0x4000, vjoy_id, HID_USAGE_Y);
			SendInput("A_LSY", up - down);
			break;
		case 'A':
		case VK_LEFT:
			left = keyPressed;
			//SetAxis((1 + right - left) * 0x4000, vjoy_id, HID_USAGE_X);
			SendInput("A_LSX", right - left);
			break;
		case 'D':
		case VK_RIGHT:
			right = keyPressed;
			//SetAxis((1 + right - left) * 0x4000, vjoy_id, HID_USAGE_X);
			SendInput("A_LSX", right - left);
			break;
		case VK_ESCAPE:
			SendInput("START", keyPressed);
			break;
		case '\t':
			SendInput("BACK", keyPressed);
			break;
		case VK_OEM_1: // semicolon
			if(keyPressed)
				up_jump = !up_jump;
			break;
		case VK_OEM_5: // backslash
			if (!keyPressed) break;
			if (keymap == NULL) {
				keymap = new std::map<USHORT, char>;
				record_idx = 0;
			} else {
				delete keymap;
				keymap = NULL;
				record_idx = -1;
			}
			break;
	}
	if (!taunting || (taunting == keycode && keyPressed == !taunting)) {
		taunting = keyPressed ? keycode : 0;
		switch (keycode) {
			case '1':
				SendInput("A_RSY", keyPressed);
				break;
			case '2':
				SendInput("A_RSX", -keyPressed);
				break;
			case '3':
				SendInput("A_RSX", keyPressed);
				break;
			case '4':
				SendInput("A_RSY", -keyPressed);
				break;
		}
	}

}