#include "stdafx.h"
#include "windows.h"

#include "Header.h"


int OpenDevice(wchar_t* DeviceName,HANDLE* phDevice)
{
	if(!DeviceName) return -1;
	if(!phDevice) return -2;

	*phDevice = INVALID_HANDLE_VALUE;

	HANDLE hFile = 0;
	_UNICODE_STRING Uni  = {0};
	Uni.Buffer = DeviceName; 
	Uni.Length = wcslen(DeviceName) * 2;
	Uni.MaxLength = Uni.Length + 2;


	_OBJECT_ATTRIBUTES Obj = {0};
	Obj.Length = sizeof(Obj);
	Obj.ObjectName = &Uni;
	Obj.Attributes = OBJ_CASE_INSENSITIVE;

	_IO_STATUS_BLOCK IOB = {0};



	int ret = ZwCreateFile(&hFile,
						FILE_GENERIC_READ|FILE_GENERIC_WRITE /*0xC0100080*/,
						&Obj,
						&IOB,
						0 /*AllocationSize*/,
						FILE_ATTRIBUTE_NORMAL /*FileAttributes*/,
						FILE_SHARE_READ|FILE_SHARE_WRITE,
						FILE_OPEN  /*CreateDisposition*/,
						FILE_NON_DIRECTORY_FILE|FILE_SYNCHRONOUS_IO_ALERT /*CreateOptions*/,
						0 /*EaBuffer*/,
						0 /*EaLength*/);
	if(hFile && hFile!=INVALID_HANDLE_VALUE)
	{
		*phDevice = hFile;
	}
	return ret;
}