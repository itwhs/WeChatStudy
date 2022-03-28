#include "Public.h"
#include <fstream>
#include <sstream>

std::string ReadFileToString(const char* filepath)
{
	std::string ret;
	FILE* hFile = NULL;
	if (fopen_s(&hFile, filepath, "rb")) {
		return ret;
	}
	fseek(hFile, 0, SEEK_END);
	unsigned int fileSize = ftell(hFile);
	if (!fileSize) {
		fclose(hFile);
		return ret;
	}
	fseek(hFile, 0, SEEK_SET);
	ret.resize(fileSize);
	fread(&ret[0],fileSize,1,hFile);
	fclose(hFile);
	return ret;
}

unsigned int ReadUint(void* lp)
{
	return *(unsigned int*)lp;
}

