#ifndef _IMAGE_
#define _IMAGE_

#include <vector>
#include <string>

class Image {
private:
	vector<string> vertices;
	vector<string> colors;

public:
	Image();

	vector<string> getVertices();
	vector<string> getColors();
};

Image::Image(){
	vertices.push_back("cube_vertices.txt");

	colors.push_back("cube_color.txt");

}

vector<string> Image::getVertices(){
	return vertices;
}

vector<string> Image::getColors(){
	return colors;
}

#endif