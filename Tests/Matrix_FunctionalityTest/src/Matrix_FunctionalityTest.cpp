#include <fstream>
#include <cstdlib>

extern "C"
int GameMain()
{
	std::ofstream fout = std::ofstream("out.txt");

	return 0;
}
