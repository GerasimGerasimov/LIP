#include "OSResources.h"
#include "framework.h"

#include <string>

OSResources::OSResources() {
	res = nullptr;
	init();
}

OSResources::~OSResources() {
	if (res) {
		delete[] res;
		res = nullptr;
	}
}

void OSResources::init() {
	int fSuccess = 0;
	HANDLE fHandle = NULL;
	fHandle = CreateFile(L"Drivers\\Resources\\res.bin", GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (fHandle != INVALID_HANDLE_VALUE) {
		DWORD FileSize = 0;
		fSuccess = GetFileSize(fHandle, 0);
		FileSize = fSuccess;
		res = new u8[fSuccess];
		DWORD BytesRead = 0;
		fSuccess = ReadFile(fHandle, res, FileSize, &BytesRead, NULL);
		std::string str;
		//std::memcpy(str, res, BytesRead);
		fSuccess = CloseHandle(fHandle);
		
	}
	

}

OSResources& OSResources::getInstance() {
	static OSResources resource;
	return resource;
}

const u8* OSResources::getRes() {
	return res;
}
