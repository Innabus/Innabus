#include <fstream>
#include <cstdlib>

extern "C"
int GameMain()
{
	std::ofstream fout = std::ofstream("out.txt");

	return 0;
}

extern "C"
int GameUpdate(float)
{
	return 0;
}
