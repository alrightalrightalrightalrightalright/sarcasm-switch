#include <Windows.h>
#include <conio.h>
#include <iostream>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

#include "enums.hpp"

#define CAPS_CHANCE 45
#define K_TO_Q_CHANCE 65
#define K_TO_KK_CHANCE 12
#define Þ_TO_SH_CHANCE 85
#define V_TO_W_CHANCE 95
#define G_TO_Q_CHANCE 65

#define L_CAPS_CHANCE 98
#define R_CAPS_CHANCE 75
#define N_CAPS_CHANCE 80
#define M_CAPS_CHANCE 71

using namespace std;
void SendKey(WORD VKCode) {
	INPUT input = { 0 };
	//ZeroMemory(input, sizeof(input));
	input.type = INPUT_KEYBOARD;
	input.ki.wVk = VKCode;
	//input.ki.time = 0;
	SendInput(1, &input, sizeof input);

	input.type = INPUT_KEYBOARD;
	input.ki.wVk = VKCode;
	input.ki.dwFlags = KEYEVENTF_KEYUP;
	SendInput(1, &input, sizeof input);
}

void ToggleCaps() {
	SendKey(VK_CAPITAL);
}
void ChangeCapsTo(bool isOn) {
	short  nVirtKey = GetKeyState(VK_CAPITAL);
	if (nVirtKey & 0x8000)//caps is on
	{
		if (isOn)return;
		else { ToggleCaps(); return; }
	}
	else {//caps is off
		if (isOn) { ToggleCaps(); return; }
		else  return;
	}
}
bool CalcChance(int chance) { return rand() % chance < rand() % 100; }
void SendNewKeys(WORD pressedKey) {
	if (CalcChance (CAPS_CHANCE))ToggleCaps();
	switch (pressedKey)
	{
	case VK_KEYS::K:
		if (CalcChance(K_TO_Q_CHANCE)) SendKey(VK_KEYS::Q);
		else  SendKey(VK_KEYS::K);
		return;
	case VK_KEYS::Þ:
		if(CalcChance(Þ_TO_SH_CHANCE)){
			SendKey(VK_KEYS::S);
			ToggleCaps();
			SendKey(VK_KEYS::H);
		}//yawaSh mý yawasH
		else SendKey(VK_KEYS::Þ);
		return;
	case VK_KEYS::V:
		if (CalcChance(V_TO_W_CHANCE))SendKey(VK_KEYS::W);
		else SendKey(VK_KEYS::V);
		return;
	case VK_KEYS::G:
		if (CalcChance(G_TO_Q_CHANCE))SendKey(VK_KEYS::Q);
		else SendKey(VK_KEYS::G);
		return;
	case VK_KEYS::L:
		if (CalcChance(L_CAPS_CHANCE))  ChangeCapsTo(true); 
		 SendKey(VK_KEYS::L);
		return;
	case VK_KEYS::R:
		if (CalcChance(R_CAPS_CHANCE))  ChangeCapsTo(true); 
		 SendKey(VK_KEYS::R); 
		return;
	case VK_KEYS::N:
		if (CalcChance(N_CAPS_CHANCE))  ChangeCapsTo(true); 
		 SendKey(VK_KEYS::N);
		return;
	case VK_KEYS::M:
		if (CalcChance(M_CAPS_CHANCE))  ChangeCapsTo(true);
		 SendKey(VK_KEYS::M); 
		return;
	default:
		SendKey(pressedKey);
		return;
	}
}
HHOOK keyboardHook;

//BU BÝR HOOK PROSEDÜRÜ
//HOOK YAPISI GEREÐÝ DÖNÜÞ DEÐERLERÝ BÖYLE OLMALI
//nCode PARAMETRESÝ DEÐERÝ 0IN ALTINDAYSA VERÝ YOK, DÝÐER PARAMETRELER BOÞ
//WPARAM DEMEKTÝR KÝ KLAVYE MESAJI. KEYDOWN, KEYUP GÝBÝ EVENTLERDÝR
//LPARAM HOOKSTRUCK DEÐÝÞKENÝ, YANÝ KEYBOARD INPIUT EVENTÝ HAKKINDA GENÝÞ BÝLGÝ
LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	PKBDLLHOOKSTRUCT key = (PKBDLLHOOKSTRUCT)lParam;
	PKBDLLHOOKSTRUCT wara = (PKBDLLHOOKSTRUCT)wParam;
	if (nCode < 0) return CallNextHookEx(keyboardHook, nCode, wParam, lParam);
	//bir tuþa basýldýysa
	if (wParam == WM_KEYDOWN && nCode == HC_ACTION)
	{
		//maybe unefficient but works xd
		UnhookWindowsHookEx(keyboardHook);
		cout << (key->vkCode) << endl;
		SendNewKeys(key->vkCode);
		keyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, 0, 0);
		return -1;//dont type
	}
	return 0;
}


int main()
{
	srand(time(NULL));
	
	_getch(); cout << "aaha:";
	//Sleep(1111);

	//Here we set the low level hook
	keyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, 0, 0);


	MSG msg{ 0 };
	while (GetMessage(&msg, NULL, 0, 0) != 0);
	UnhookWindowsHookEx(keyboardHook);








	return 0;
	cout << "Hello World!\n"; int asd;
	_getch();
	ShowWindow(GetConsoleWindow(), SW_HIDE);
	system("pause");
	//  while (1) ;
}

