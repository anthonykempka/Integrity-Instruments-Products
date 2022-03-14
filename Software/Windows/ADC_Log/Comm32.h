




HANDLE Open_Comport (DWORD Comport, DWORD BaudRate);
int Close_Comport (HANDLE DriverHandle);
int Write_Comport (HANDLE DriverHandle, DWORD NumBytes, void *Buffer);
int Read_Comport (HANDLE DriverHandle, DWORD *BytesRead, DWORD BufferSize, void *Buffer);