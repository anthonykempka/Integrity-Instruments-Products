#if !defined _EZTERM_DEFS_
#define _EZTERM_DEFS_

#define DATASOURCE_KEYBOARD		1
#define DATASOURCE_SERIAL		2

#define DISPLAY_ASCII			0
#define DISPLAY_ASCIIHEX		1
#define ASCII_POSITION			550
#define CARRIAGERETURN			0x000D
#define NEWLINE					0x000A
#define BACKSPACE				0x0008

#define RX_BUFSIZE				4096
#define WM_CHAR_RECEIVED		WM_USER + 100

#endif