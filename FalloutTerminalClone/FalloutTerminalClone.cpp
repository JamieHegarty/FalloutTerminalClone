// FalloutTerminalClone.cpp : Defines the entry point for the console application.
#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

/*
	The minigame features four levels of dificulty with varied number and length of words
		Novie - 5 Letters - 6 Words
		Advanced - 7 Letters - 8 Words
		Expert - 9 Letters - 10 Words
		Master - 11 Letters - 12 Words
*/

//Each dificulty has it's own set of words
std::vector<std::string> noviceWords, advancedWords, expertWords, masterWords;

const std::string DICTIONARY_FILE = "dictionary.txt";

/* 
	The loadDictionary function reads in words from a specified text file
	and then arranges them into the correct category (explained above)
*/
static bool loadDictionary() {

	std::ifstream fileStream(DICTIONARY_FILE); //Open dictionary file using input file stream
	std::string line = "";

	if (fileStream.good()) {
		std::cout << "Reading in dictionary file..." << std::endl;
	
		//Read each line (word) from the dictionary
		while (std::getline(fileStream, line)) {

			//Switch statement to sort our words into the corresponding vector based on size
			switch (line.length()) {
			case 5:
				noviceWords.push_back(line);
				break;
			case 7:
				advancedWords.push_back(line);
				break;
			case 9:
				expertWords.push_back(line);
				break;
			case 11:
				masterWords.push_back(line);
				break;
			default:
				//Do nothing if the word is not of the correct length	
				break;
			}
		}

		fileStream.close(); //Close file stream when we are finished with it

		//Check that each word category has been populated correctly
		if (noviceWords.size() && advancedWords.size() && expertWords.size() && masterWords.size()) {
			//This print statement is for debugging only, to be removed at later itterations
			std::cout << "No. Novice Words: " << noviceWords.size() << "\nNo. Advanced Words: " << advancedWords.size() << "\nNo. Expert Words: " <<  expertWords.size() << "\nNo. Master Words: " << masterWords.size() << std::endl;
			return true;

		} else {
			std::cout << "Error reading from dictionary, word lists couldn't be populated!" << std::endl;
			return false;
		}

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

