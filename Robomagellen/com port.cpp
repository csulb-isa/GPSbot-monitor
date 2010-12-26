/* This implementation of the three com routines
	is for a WIN32 execution environment.
	Replace as necessary for your particular environment. */

#include "com port.h"

// Windows Header Files:
#include <windows.h>
#include <commctrl.h>
// C RunTime Header Files
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

static HANDLE hComm;
wchar_t com_name[64] = {L""};

int WincomClose()
{
	DeleteFile(com_name);
	CloseHandle(hComm);
	return 1;
}

int ComIsAvailable(int port)
{
	HANDLE tmp_hCom = INVALID_HANDLE_VALUE;
	wchar_t txt[64] = {NULL};
	wsprintf(txt, L"COM%d", port);
	tmp_hCom = CreateFile(txt, GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, 0, NULL);
	if (tmp_hCom == INVALID_HANDLE_VALUE)
		return -1;
	DeleteFile(txt);
	CloseHandle(tmp_hCom);
	return 1;
}

int WincomInit(wchar_t* port)
{
	COMMTIMEOUTS timeouts;
	DCB          dcb;
  
	// initialise the comm port
	// was TEXT("COM8")
	hComm = CreateFile(port, GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, 0, NULL);

	if(hComm == INVALID_HANDLE_VALUE)
	    return -1;

	wmemcpy(com_name, port, wcslen(port));
	// set com port params
	FillMemory(&dcb, sizeof(dcb), 0);
	dcb.DCBlength = sizeof(dcb);

	if(GetCommState(hComm, &dcb))
	{
		dcb.BaudRate = (CBR_57600);
		dcb.Parity   = NOPARITY;
		dcb.StopBits = ONESTOPBIT;
		dcb.ByteSize = 8;
		SetCommState(hComm, &dcb);
	}
  
	// set communication timeouts
	timeouts.ReadIntervalTimeout         = 0;
	timeouts.ReadTotalTimeoutMultiplier  = 0;
	timeouts.ReadTotalTimeoutConstant    = 1;
	timeouts.WriteTotalTimeoutMultiplier = 0;
	timeouts.WriteTotalTimeoutConstant   = 0;

	SetCommTimeouts(hComm, &timeouts);

	return 1;
}

/* WincomSendByte(char) sends one byte of data to the com port
*/
void WincomSendByte(const unsigned char chr)
{
   DWORD written;
   WriteFile(hComm, &chr, 1, &written, NULL);
}

int WriteCom(wchar_t* data)
{
	int r_loc = 0x00;
	char small_data[1024] = {""};
	while(data[r_loc] != 0x00)
	{
		small_data[r_loc] = (char)data[r_loc];
		r_loc ++;
	}
	DWORD written;
	if (WriteFile(hComm, small_data, r_loc, &written, NULL) == 0)
			return -1;
	return 1;
}

int WriteCom(char* data)
{
	int r_loc = strlen(data);
	DWORD written = 0x0;
	if (WriteFile(hComm, data, r_loc, &written, NULL) == 0)
			return -1;
	return 1;
}

/* unsigned char WincomRcvByte() receives one byte of data from the com port
   it should time out and return 0x00 after 100ms
*/
unsigned char WincomRcvByte()
{
	unsigned char tmpchr=0;
	DWORD read = 0;
	ReadFile(hComm, &tmpchr, 1, &read, NULL);
	return tmpchr;
}

int ReadCom(wchar_t* data)
{
	char tmp_data[4096] = {0x00};
	memset(tmp_data, 0x00, 4096);
	DWORD read = 0;
	ReadFile(hComm, tmp_data, 4096, &read, NULL);
	if (read > 0){
		int cnt = 0x00;
		while (tmp_data[cnt] != 0x00){
			data[cnt] = tmp_data[cnt];
			cnt++;
		}
		return TRUE;
	}
	return FALSE;
}


