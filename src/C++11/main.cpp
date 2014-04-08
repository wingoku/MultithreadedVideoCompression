// Multithreaded Video Compression/Decompression & trimming of unwanted bytes from Video files



// written using C++11
// April 7, 2014
// http://www.wingoku.com

#include "stdafx.h"
#include <iostream>
#include <thread>
#include <conio.h>
#include <string>
#include <direct.h> // for _chdir() and _getcwd()
#include <fstream>
#include <stdio.h>


using namespace std;

void asyncTasks(float i, int start, int end)
{
	string orignalFileName = "Kimono1_1920x1080x24x420.yuv";

	string cmd = "";

	string fileName;

	//for (int j = start; j<end; j++)
	{

		cout << "Compression Phase\n";
		cmd = "kdu_v_compress -i " + orignalFileName + " -o compressed" + to_string(i) + ".mj2 -rate " + to_string(i);

		system(cmd.c_str());

		cout << "Decompression Phase\n";

		cmd = "kdu_v_expand -i compressed" + to_string(i) + ".mj2 -o unComp" + to_string(i) + ".vix";

		system(cmd.c_str());

	}

}


void trimFile()
{
	ifstream file;
	ofstream trimmedFile;
	string fileName;
	string orignalFileName = "Kimono1_1920x1080x24x420.yuv";

	float i = 0.1;

	file.open(orignalFileName.c_str(), ios::binary | ios::ate);
	int orgFileSize = file.tellg();
	file.close();

	for (int j = 0; j < 10; j++)
	{

		cout << "reading uncomp " << i << "\n";

		char *trimmedData = new char[orgFileSize];

		fileName = "unComp" + to_string(i) + ".vix";
		file.open(fileName.c_str(), ios::binary | ios::ate);

		cout << "seeking" << (int)file.tellg() - orgFileSize << " \n";

		file.seekg((int)file.tellg() - orgFileSize, ios::beg);
		file.read(trimmedData, orgFileSize);
		file.close();

		cout << "writing trimmed file\n";

		fileName = "trimmed" + to_string(i) + ".yuv";

		trimmedFile.open(fileName.c_str(), ios::binary);
		trimmedFile.write(trimmedData, orgFileSize);
		trimmedFile.close();
		cout << to_string(i) + " done\n";
		delete trimmedData;

		i += 0.1;
	}

}

int _tmain(int argc, _TCHAR* argv[])
{
	float i = 0.1;

	string path = "C:\\Program Files (x86)\\Kakadu";

	char temp[256] = "";

	_chdir(path.c_str());

	_getcwd(temp, 256);

	cout << temp;

	thread *tid = new thread[10];

	// compress/decompress
	for (int j = 0; j < 10; j++)
	{
		tid[0] = thread(asyncTasks, i, 0, 2);
		i += 0.1;
	}

	/*
	tid[0] = thread(asyncTasks, 0.1,0,2);
	tid[1] = thread(asyncTasks, 0.3, 2, 4);
	tid[2] = thread(asyncTasks, 0.5, 4, 6);
	tid[3] = thread(asyncTasks, 0.7, 6, 8);
	tid[4] = thread(asyncTasks, 0.9, 8, 10);
	*/

	for (int x = 0; x < 10; x++)
	{
		tid[x].join();
	}
	
	// trim unwanted bytes from uncompressed video files
	// performing read/writing operations in a single thread. Performing disk operations in multithreaded envoirnment results in
	// abnormal termination of the program
	thread(trimFile).join();

	_getch();
	return 0;
}

