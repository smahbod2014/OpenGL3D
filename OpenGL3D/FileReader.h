#pragma once

class FileReader
{
public:
	static unsigned char* read(const char* path, long& outLength);
};