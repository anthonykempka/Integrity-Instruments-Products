
#include "stdafx.h"
#include <windows.h>            // Standard Windows include for system defines
#include <stdlib.h>             // Standard library include
#include <winbase.h>            // Has WIN32 Comm library and structure DEFS


#include <stdio.h>              // I/O include for printf() function
#include <conio.h>              // Include for the getch() function


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



HANDLE ComHandle;


