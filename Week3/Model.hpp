#ifndef _MODEL_
#define _MODEL_

#include <vector>
#include <string>

class Model {
private:
	vector<string> vertices;
	vector<string> textures;
	char* image;

public:
	Model();

	vector<string> getVertices();
	vector<string> getTextures();
	char* getImage();
};

Model::Model(){
	vertices.push_back("car_vertices.txt");
	// colors.push_back("cube_color.txt");
	textures.push_back("car_textures.txt");
	image = "car_image.jpg";
}

vector<string> Model::getVertices(){
	return vertices;
}

vector<string> Model::getTextures(){
	return textures;
}

char* Model::getImage(){
	return image;
}

#endif