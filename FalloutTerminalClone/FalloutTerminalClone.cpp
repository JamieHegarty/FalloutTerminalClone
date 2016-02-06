// FalloutTerminalClone.cpp : Defines the entry point for the console application.
#include <stdlib.h>
#include <fstream>
#include <iostream>

#include <string>
#include <vector>

const std::string DICTIONARY_FILE = "dictionary.txt";
std::vector<std::string> wordList;

static bool loadDictionary() {

	std::ifstream fileStream(DICTIONARY_FILE); //Open dictionary file using input file stream
	std::string line = "";

	if (fileStream.good()) {
		std::cout << "Reading in dictionary file..." << std::endl;
	
		//Read each line (word) from the dictionary
		while (std::getline(fileStream, line)) {
			wordList.push_back(line);
			std::cout << line << std::endl;
		}

		fileStream.close();

		return true;

	} else {

		std::cout << "Could not find file " << DICTIONARY_FILE << "\nExiting..."<<std::endl;
		return false;
	}
	
}

int main() {

	if (loadDictionary()) {
		std::cout << "Dictionary Loaded" << std::endl;
	}


    return 0;
}

