#ifndef _UTILS
#define _UTILS

#include <vector>
#include <string>
#include <sstream>
#include <iomanip>

using namespace std;

class Utils{
private:

public:
	Utils();
	vector<string> explodeString(const string& s, const char& c);
	// int32_t stoui(const string hex);
};

Utils::Utils(){

}

vector<string> Utils::explodeString(const string& s, const char& c)
{
	string buff{""};
	vector<string> v;
	
	for(auto n:s)
	{
		if(n != c) buff+=n; else
		if(n == c && buff != "") { v.push_back(buff); buff = ""; }
	}
	if(buff != "") v.push_back(buff);
	
	return v;
}

/*
int32_t Utils::stoui(const string hex) {
	int32_t x;   
	stringstream ss;
	ss << std::hex << hex;
	ss >> x;
	return x;
}
*/

#endif