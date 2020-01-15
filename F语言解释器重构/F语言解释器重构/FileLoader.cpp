#include "stdafx.h"
#include "FileLoader.h"

FileLoader::~FileLoader()
{
	fclose(fp);
}
FileLoader::FileLoader()
{
	fp = nullptr;
	err = 0;
}

errno_t FileLoader::FileReader(char *Filename)
{
	err = fopen_s(&fp, Filename, "rb");
	return err;
}

void FileLoader::LoadProgram(char * pp)
{
	char *t = pp;
	do {
		*t = getc(fp);
		++t;
	} while (!feof(fp));
	*(t - 1) = 0;
	return;
}