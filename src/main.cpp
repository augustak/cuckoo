#include "cuckoo.hpp"

#include <iostream>
#include <cstdlib>
#include <ctime>

int main(int argc, char** argv)
{
	srand(time(0));
	cuckoo c;
	std::string s = "a";
	for(int i = 0; i < 101; ++i)
	{
		pair entry(s, "hi");
		c.insert(entry);
		s += "a";
	}
	c.print();
	return 0;
}