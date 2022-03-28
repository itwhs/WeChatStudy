#include "WinApi.h"
#include "Strings.h"

bool Ð´ÄÚ´æ_HEX(DWORD hProcess, size_t addr, std::string hex)
{
	std::vector<unsigned char> bin = HexStrToBin(hex);
	return WriteProcessMemory((HANDLE)hProcess, (LPVOID)addr, bin.data(), bin.size(), 0);
}