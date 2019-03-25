#ifndef _IMAGE_
#define _IMAGE_

#include <vector>
#include <string>

class Image {
private:
	vector<string> filenames;

public:
	Image();

	vector<string> getFilenames();

};

Image::Image(){
	filenames.push_back("kuadran1.txt");
	// filenames.push_back("kuadran2.txt");
	// filenames.push_back("kuadran3.txt");
	// filenames.push_back("kuadran4.txt");
}

vector<string> Image::getFilenames(){
	return filenames;
}

#endif