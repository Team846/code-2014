#include <iostream>
#include <cstdio>
#include <algorithm>
#include <cstring>
#include <string>
#include <cctype>
#include <stack>
#include <queue>
#include <list>
#include <vector>
#include <map>
#include <sstream>
#include <cmath>
#include <bitset>
#include <utility>
#include <set>
#include <numeric>
#include <ctime>
#include <climits>
#include <cstdlib>
#include <fstream>

using namespace std;

typedef struct
{
	string type;
	string name;
	int size;
	int arrayLength;
} Field;

FILE* file;
ofstream fout;

template <typename T> T swap_endian(T u)
{
	union
	{
		T u;
		unsigned char u8[sizeof(T)];
	} source, dest;

	source.u = u;

	for (size_t k = 0; k < sizeof(T); k++)
		dest.u8[k] = source.u8[sizeof(T) - k - 1];

	return dest.u;
}

template<typename T> void Output(T *v, string name, size_t size, int count)
{
	fread(v, size / count, count, file);
	for (int i = 0; i < count; i++)
	{
		fout << swap_endian(v[i]);
		if (i != count - 1)
			fout << ',';
	}
	delete v;
}

int main (int argc, char ** argv)
{
	string filename;
	if (argc != 2)
	{
		printf("Enter filename of binary log file: ");
		getline(cin, filename);
	}
	else
		filename = argv[1];
	printf("Decoding binary file: %s\n", filename.c_str());
	ifstream fin((filename + ".header").c_str());
	if (!fin.is_open())
	{
		cout << "Log header file not found" << endl;
		if (argc != 2)
			cin.ignore();
		return 0;
	}
	fout.open((filename.substr(0, filename.find('.')) + ".csv").c_str());
	fout.precision(20);
	fout.setf(std::ios::fixed, std:: ios::floatfield);
	int iN;
	int totalSize = 0;
	vector<Field> fields;
	fin >> iN;
	fout << "Cycle,";
	for (int i = 0; i < iN; i++)
	{
		Field field;
		fin >> field.type >> field.name >> field.size;
		totalSize += field.size;
		if (field.type[0] == 'A')
		{
			string arrayLength = field.type.substr(1, field.type.find('_') - 1);
			istringstream(arrayLength) >> field.arrayLength;
			field.type = field.type.substr(field.type.find('_') + 1);
			for (int j = 0; j < field.arrayLength; j++)
			{
				fout << field.name << '[' << j << ']';
				if (j != field.arrayLength - 1)
					fout << ',';
			}
		}
		else
		{
			fout << field.name;
			field.arrayLength = 1;
		}
		if (i == iN - 1)
			fout << endl;
		else
			fout << ',';
		fields.push_back(field);
	}
	file = fopen(filename.c_str(), "rb");
	fseek(file, 0, SEEK_END);
	int cnt = ftell(file) / totalSize;
	rewind(file);
	for (int i = 0; i < cnt; i++)
	{
		fout << i << ",";
		for (int i = 0; i < iN; i++)
		{
			if (fields[i].type == "i") // int
			{
				int *v = new int[fields[i].arrayLength];
				Output(v, fields[i].name, fields[i].size, fields[i].arrayLength);
			}
			else if (fields[i].type == "f") // float
			{
				float *v = new float[fields[i].arrayLength];
				Output(v, fields[i].name, fields[i].size, fields[i].arrayLength);
			}
			else if (fields[i].type == "d") // double
			{
				double *v = new double[fields[i].arrayLength];
				Output(v, fields[i].name, fields[i].size, fields[i].arrayLength);
			}
			else if (fields[i].type == "s") // short
			{
				short *v = new short[fields[i].arrayLength];
				Output(v, fields[i].name, fields[i].size, fields[i].arrayLength);
			}
			else if (fields[i].type == "l") // long
			{
				long *v = new long[fields[i].arrayLength];
				Output(v, fields[i].name, fields[i].size, fields[i].arrayLength);
			}
			else if (fields[i].type == "x") // long long
			{
				long long *v = new long long[fields[i].arrayLength];
				Output(v, fields[i].name, fields[i].size, fields[i].arrayLength);
			}
			else if (fields[i].type == "b") // bool
			{
				bool *v = new bool[fields[i].arrayLength];
				Output(v, fields[i].name, fields[i].size, fields[i].arrayLength);
			}
			else if (fields[i].type == "h") // unsigned char
			{
				unsigned char *v = new unsigned char[fields[i].arrayLength];
				Output(v, fields[i].name, fields[i].size, fields[i].arrayLength);
			}
			else if (fields[i].type == "j") // unsigned int
			{
				unsigned int *v = new unsigned int[fields[i].arrayLength];
				Output(v, fields[i].name, fields[i].size, fields[i].arrayLength);
			}
			else if (fields[i].type == "t") // unsigned short
			{
				unsigned short *v = new unsigned short[fields[i].arrayLength];
				Output(v, fields[i].name, fields[i].size, fields[i].arrayLength);
			}
			else if (fields[i].type == "m") // unsigned long
			{
				unsigned long *v = new unsigned long[fields[i].arrayLength];
				Output(v, fields[i].name, fields[i].size, fields[i].arrayLength);
			}
			else if (fields[i].type == "y") // unsigned long long
			{
				unsigned long long *v = new unsigned long long[fields[i].arrayLength];
				Output(v, fields[i].name, fields[i].size, fields[i].arrayLength);
			}
			else if (fields[i].type == "a") // signed char
			{
				signed char *v = new signed char[fields[i].arrayLength];
				Output(v, fields[i].name, fields[i].size, fields[i].arrayLength);
			}
			else // enum
			{
				int *v = new int[fields[i].arrayLength];
				Output(v, fields[i].name, fields[i].size, fields[i].arrayLength);
			}
			if (i == iN - 1)
				fout << endl;
			else
				fout << ',';
		}
	}
	fin.close();
	fclose(file);
	fout.close();
	cout << "Successfully decoded " << cnt << " cycles of logged data, " << totalSize << " bytes per cycle." << endl << endl;
	if (argc != 2)
	{
		cout << "Press ENTER to exit..." << endl;
		cin.ignore();
	}
	return 0;
}

