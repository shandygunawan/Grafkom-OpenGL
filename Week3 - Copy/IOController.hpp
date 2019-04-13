#ifndef _IOCONTROLLER
#define _IOCONTROLLER

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

class IOController {
private:

public:
	IOController();
	vector<string> readFile(string filename);
    void writeFile(string filename, vector<string> inputs);
};

IOController::IOController(){

}

vector<string> IOController::readFile(string filename){
	
    // Open file
    ifstream infile;
    infile.open(filename);

    vector<string> outputs;
    
    // Check if file is open
    if(infile.is_open()){
        string line;
        while(getline(infile, line)){
            outputs.push_back(line);
        }

        infile.close();   
    }
    else {
        cout << "File is not open" << endl;
    }

    return outputs;
}

void IOController::writeFile(string filename, vector<string> inputs){
    
    // Open File
    ofstream outfile;
    outfile.open(filename);

    // Check if file is ready to be written
    if(outfile.is_open()){

        for(unsigned int i = 0; i < inputs.size(); i++){
            outfile << inputs.at(i) << endl;
        }

        outfile.close();
    }
    else {
        cout << "File is not ready to be written" << endl;
    }
}


#endif