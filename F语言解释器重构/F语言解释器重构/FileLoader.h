#pragma once
class FileLoader
{
private:
	FILE *fp;
	errno_t err;
public:
	FileLoader();
	~FileLoader();
	errno_t FileReader(char *Filename);
	void LoadProgram(char *pp);
};