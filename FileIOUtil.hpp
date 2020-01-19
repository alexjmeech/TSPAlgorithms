#ifndef FILEIOUTIL_HPP_
#define FILEIOUTIL_HPP_

#include <string>
#include <cstring>
#include <sstream>
#include <sys/stat.h>
#include <iostream>
using namespace std;

template<typename T>
class FileIOUtil
{
private:
	FileIOUtil();

	static void ensureDirectory(string&);
public:
	typedef T(*StrToObj)(istream&);
	typedef void(*ObjToStr)(ostream&, T&);

	static void readData(string, string, T*&, unsigned long&, StrToObj);

	static void writeData(string, string, T*, unsigned long, ObjToStr);

	static void writeBuffer(string, string, ostringstream&);
};

template<typename T>
void FileIOUtil<T>::readData(string directoryPath, string fileName, T*& data, unsigned long& total, StrToObj converter)
{
	if (directoryPath.empty() && directoryPath[directoryPath.length() - 1] != '/')
	{
		directoryPath.append("/");
	}

	FILE* input = fopen(((directoryPath + fileName).c_str()), "rb");

	if (input)
	{
		total = 0;

		char* buffer = nullptr;
		long fileLength = 0;
		fseek(input, 0, SEEK_END);
		fileLength = ftell(input);

		fseek(input, 0, SEEK_SET);
		buffer = (char*) malloc(fileLength * sizeof(char));
		fread(buffer, sizeof(char), fileLength, input);
		fclose(input);

		for (unsigned long i = 0; i <= fileLength * sizeof(char); i++)
		{
			if (buffer[i] == '\n' || buffer[i] == '\0' || i == (fileLength * sizeof(char)))
			{
				total++;
			}
		}

		istringstream processor(buffer);
		free(buffer);

		data = new T[total];

		for (unsigned long i = 0; i < total; i++)
		{
			data[i] = (*converter)(processor);
		}
	}
}

template<typename T>
void FileIOUtil<T>::ensureDirectory(string& path)
{
	if (path.empty())
	{
		return;
	}

	if (path[path.length() - 1] == '/')
	{
		path.pop_back();
	}

	mkdir(path.c_str(), 0777);

	path.append("/");
}

template<typename T>
void FileIOUtil<T>::writeData(string directoryPath, string fileName, T* data, unsigned long total, ObjToStr converter)
{
	if (data == nullptr)
	{
		return;
	}

	ensureDirectory(directoryPath);

	ostringstream buffer;

	for (unsigned long i = 0; i < total; i++)
	{
		(*converter)(buffer, data[i]);

		if (i + 1 < total)
		{
			buffer << '\n';
		}
		else
		{
			buffer << '\0';
		}
	}

	FILE* output = fopen((directoryPath + fileName).c_str(), "wb");
	fwrite(buffer.str().c_str(), 1, ((long)buffer.tellp()) - 1, output);
	fclose(output);
}

template<typename T>
void FileIOUtil<T>::writeBuffer(string directoryPath, string fileName, ostringstream& buffer)
{
	if (buffer.tellp() == 0)
	{
		return;
	}

	ensureDirectory(directoryPath);

	FILE* output = fopen((directoryPath + fileName).c_str(), "wb");
	fwrite(buffer.str().c_str(), 1, ((long)buffer.tellp()) - 1, output);
	fclose(output);
}

#endif /* FILEIOUTIL_HPP_ */
