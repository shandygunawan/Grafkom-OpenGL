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
	vertices.push_back("kuadran1_vertices.txt");
	vertices.push_back("kuadran2_vertices.txt");
	vertices.push_back("kuadran3_vertices.txt");
	// vertices.push_back("kuadran4_vertices.txt");

	colors.push_back("kuadran1_color.txt");
	colors.push_back("kuadran2_color.txt");
	colors.push_back("kuadran3_color.txt");
}

vector<string> Image::getVertices(){
	return vertices;
}

vector<string> Image::getColors(){
	return colors;
}

#endif