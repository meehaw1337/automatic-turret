#include <iostream>
#include "SerialPort.h"
#include <windows.h>
using namespace std;

HANDLE serialHandle;

void init() {
    serialHandle = CreateFile("\\\\.\\COM3", GENERIC_READ | GENERIC_WRITE, 0,
                                NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    
    DCB serialParams = {0};

    GetCommState(serialHandle, &serialParams);

    serialParams.BaudRate = CBR_9600;
    serialParams.ByteSize = 8;
    serialParams.StopBits = ONESTOPBIT;
    serialParams.Parity = NOPARITY;
    serialParams.fDtrControl = DTR_CONTROL_ENABLE;

    SetCommState(serialHandle, &serialParams);
}


int main() {

    init();

    DWORD bytesSend;
    char* info = (char*)malloc(sizeof(char)*10);

    while(true) {
            if(GetKeyState(VK_UP) & 0x8000) {
                strcpy(info, "D");
                WriteFile(serialHandle, (void*) info, 2, &bytesSend, 0);
                Sleep(50);
            }

            if(GetKeyState(VK_DOWN) & 0x8000) {
                strcpy(info, "U");
                WriteFile(serialHandle, (void*) info, 2, &bytesSend, 0);
                Sleep(50);
            }

            if(GetKeyState(VK_LEFT) & 0x8000) {
                strcpy(info, "L");
                WriteFile(serialHandle, (void*) info, 2, &bytesSend, 0);
                Sleep(50);
            }

            if(GetKeyState(VK_RIGHT) & 0x8000) {
                strcpy(info, "R");
                WriteFile(serialHandle, (void*) info, 2, &bytesSend, 0);
                Sleep(50);
            }
    }
}