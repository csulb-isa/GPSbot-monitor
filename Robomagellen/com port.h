#ifndef COM_PORT_H
#define COM_PORT_H

int WincomInit(wchar_t*);
int ComIsAvailable(int);
int WincomClose();
void WincomSendByte(const unsigned char);
int WriteCom(wchar_t*);
int WriteCom(char*);
unsigned char WincomRcvByte();
int ReadCom(wchar_t*);


#endif


