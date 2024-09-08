#include "logger.h"

#include <windows.h>
#include <stdio.h>

void log_message(message_type msgType, const char* msg) {

	HANDLE hConsole;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	switch(msgType) {
		case LINFO: 
			SetConsoleTextAttribute(hConsole, 1);
			printf("INFO : %s\n", msg);
			SetConsoleTextAttribute(hConsole, 7);
			break;
		case LWARN: 
			SetConsoleTextAttribute(hConsole, 6);
			printf("WARN : %s\n", msg);
			SetConsoleTextAttribute(hConsole, 7);
			break;
		case LERROR:
			SetConsoleTextAttribute(hConsole, 4);
			printf("ERROR: %s\n", msg);
			SetConsoleTextAttribute(hConsole, 7);
			break;
	}
}
