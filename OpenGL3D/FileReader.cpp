#include "FileReader.h"
#include <stdio.h>
#include <iostream>

unsigned char* FileReader::read(const char* path, long& outLength)
{
	FILE* file = fopen(path, "rb");

	if (file == nullptr)
	{
		std::cout << "Couldn't find file " << path << std::endl;
		return nullptr;
	}

	fseek(file, 0, SEEK_END);
	outLength = ftell(file);
	rewind(file);
	
	unsigned char* data = new unsigned char[outLength + 1];
	data[outLength] = '\0';
	fread(data, 1, outLength, file);
	fclose(file);

	return data;
}