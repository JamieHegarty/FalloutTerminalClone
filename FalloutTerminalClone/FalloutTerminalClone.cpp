// FalloutTerminalClone.cpp : Defines the entry point for the console application.

#include <SFML/Graphics.hpp>

#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <random>
#include <conio.h>


/*
	The minigame features four levels of dificulty with varied number and length of words
		Novie - 5 Letters - 6 Words
		Advanced - 7 Letters - 8 Words
		Expert - 9 Letters - 10 Words
		Master - 11 Letters - 12 Words
*/

const int NOVICE_LENGHT = 5, ADVANCED_LENGHT = 7, EXPERT_LENGTH = 9, MASTER_LENGTH = 11; //Word lengths for each dificulty
const int NOVICE_WORDS = 6, ADVANCED_WORDS = 8, EXPERT_WORDS = 10, MASTER_WORDS = 12; //Number of words for each dificulty
const std::string DICTIONARY_FILE = "dictionary.txt"; //Path to dictionary file
const int LIVES = 4; //Total number of attempts per word

std::vector<std::string> noviceDictionary, advancedDictionary, expertDictionary, masterDictionary; //Each dificulty has it's own set of words
std::vector<std::string> wordSubset; //This is the current subset of words randomly pulled from a dictionary
std::string currentWord; //This is the current correct word

/* SFML Variables */

const sf::Vector2f WINDOW_SIZE(900.0f, 600.0f); //Predetermined window size

sf::Texture backgroundTexture; //Variable to hold background texture
sf::Sprite backgroundSprite;
sf::Font mainFont; //Main typeface for the terminal
sf::RenderWindow gameWindow; //The main game window

/* 
	This function is used to set up SFML for the application, including loading in and assigning needed textures, fonts 
	and audio etc..
*/

static bool sfmlSetup() {
	//Load in required resources
	if (!backgroundTexture.loadFromFile("Sprites/terminalBackground.jpg")) {
		std::cerr << "Could not load background texture" << std::endl;
		return false;
	}
	else if (!mainFont.loadFromFile("Fonts/Share-TechMono.ttf")) {
		std::cerr << "Could not load main font" << std::endl;
		return false;
	}

	backgroundSprite.setTexture(backgroundTexture);
	backgroundSprite.setScale( //Scale the background sprite to fit the current window size
		WINDOW_SIZE.x / backgroundSprite.getLocalBounds().width,
		WINDOW_SIZE.y / backgroundSprite.getLocalBounds().height);

	gameWindow.create(sf::VideoMode(WINDOW_SIZE.x, WINDOW_SIZE.y), "Terminal", sf::Style::Titlebar | sf::Style::Close); //Create SFML Window

	/*  Testing drawing text on screen

		sf::Text text;
		text.setFont(mainFont);
		text.setString("\n\t\tFALLOUT TERMINAL CLONE");
		text.setColor(sf::Color::Green);
		text.setCharacterSize(25); //Set size (pixels)
	*/

	return gameWindow.isOpen(); //Only returns true if the game window was successfully created

}

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
				noviceDictionary.push_back(line);
				break;
			case 7:
				advancedDictionary.push_back(line);
				break;
			case 9:
				expertDictionary.push_back(line);
				break;
			case 11:
				masterDictionary.push_back(line);
				break;
			default:
				//Do nothing if the word is not of the correct length	
				break;
			}
		}

		fileStream.close(); //Close file stream when we are finished with it

		//Check that each word category has been populated correctly
		if (noviceDictionary.size() && advancedDictionary.size() && expertDictionary.size() && masterDictionary.size()) {
			//This print statement is for debugging only, to be removed at later itterations
			//std::cout << "No. Novice Words: " << noviceDictionary.size() << "\nNo. Advanced Words: " << advancedDictionary.size() << "\nNo. Expert Words: " <<  expertDictionary.size() << "\nNo. Master Words: " << masterDictionary.size() << std::endl;
			
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

/* Function to clear screen, temporarily used until project later upgraded to SFML */
void console_clear_screen() {
#ifdef _WIN32
	system("cls");
#endif
#ifdef linux
	system("clear");
#endif
}

/* This function is to compare the string input against the correct answer and return their likeness */
int compareLikeness(std::string input) {
	
	int likeness = 0;

	for (int i = 0; i < input.size(); ++i) {
		if (input[i] == currentWord[i])
			likeness++;
	}

	return likeness;
}

/*  This function is used to randomly select the words to be displayed to the user, assign the correct
	answer and list them on the screen
*/
void generateBoard(std::vector<std::string> &dictionary, int wordCount) {

	/* Random number generator set up */
	std::random_device rd; //Non-deterministic 32-bit seed, different seed everytime executed
	std::mt19937 mt(rd()); //Mersenne Twister pseudo-random number generating engine [0, 2^32]
	std::uniform_int_distribution<int> dist(0, dictionary.size()-1); //Evenly distributes across the two numbers input (note it is [inlusive, inclusive] )
	
	wordSubset.clear(); //Clear the previous subset
	std::string tempWord;

	for (int i = 0; i < wordCount; ++i) {
		tempWord = dictionary.at(dist(mt)); //Pull a random word from the selected dictionary and store in a temp string
		wordSubset.push_back(tempWord); 
		std::cout << i+1 << ".) " << tempWord << "\n";
	}

	//Randomly select a word from the subset to be the "correct" answer
	std::uniform_int_distribution<int> subsetDist(0, wordSubset.size() - 1);
	currentWord = wordSubset.at(subsetDist(mt));

	std::cout << "\nAnswer:" << currentWord << "\n" << std::endl; //Only show answer while debugging/developing

	int choice;
	int livesLeft = LIVES;
	//Loop until out of lives or correct answer provided
	for (;;) {
		if (std::cin >> choice &&  choice > 0 && choice <= wordSubset.size()) {

			int likeness = compareLikeness(wordSubset.at(choice - 1));

			if (likeness == currentWord.size()) {
				console_clear_screen();

				std::cout << "\nPassword accepted\n" << std::endl;
				break;
			}
			else if (livesLeft <= 1) {
				console_clear_screen();
				std::cout << "\nEntry Denied\n" << std::endl;
				break;
			} else {
				//Clear the screen and redraw the board, this is a temporary console solution before GUI is implemented
				console_clear_screen();

				std::cout << "Lives Remaining " << --livesLeft << "/" << LIVES << "\n" << std::endl;

				for (int i = 0; i < wordSubset.size(); ++i) {
						std::cout << i + 1 << ".) " << wordSubset.at(i) << "\n";
				}
				std::cout << "\nAnswer:" << currentWord << "\n" << std::endl; //Only show answer while debugging/developing

				std::cout << wordSubset.at(choice - 1) << std::endl;
				std::cout << "Entry Denied\nLikeness: " << likeness << "/" << currentWord.size() << "\n" <<std::endl;
			}

			
		}
		else {

			std::cin.clear(); //Clears the input buffer
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		}
	}

}

/* Menu that lists selection options - Refactor later*/
void menuLoop() {
	
	std::string menuText = "Choose difficulty level:\n\n\t 1.) Novice\n\t 2.) Advanced\n\t 3.) Expert\n\t 4.) Master\n\n> ";

	for (;;) {
		int difficulty;

		std::cout << menuText;

		//Loop if a valid option is not selected
		for (;;) {
			if (std::cin >> difficulty) {
				break;
			}
			else {
				console_clear_screen(); 
				std::cout << menuText;
				std::cin.clear(); //Clears the input buffer
				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			}
		}

		switch (difficulty) {
			case 1:
				console_clear_screen();
				std::cout << "Novice: \n" << std::endl;
				generateBoard(noviceDictionary, NOVICE_WORDS);
				break;
			case 2:
				console_clear_screen();
				std::cout << "Advanced: \n" << std::endl;
				generateBoard(advancedDictionary, ADVANCED_WORDS);
				break;
			case 3:
				console_clear_screen();
				std::cout << "Expert: \n" << std::endl;
				generateBoard(expertDictionary, EXPERT_WORDS);
				break;
			case 4:
				console_clear_screen();
				std::cout << "Master: \n" << std::endl;
				generateBoard(masterDictionary, MASTER_WORDS);
				break;

			default:
				break;
		}

		system("pause"); //System is bad, only a temp solution
		console_clear_screen();

	}
}

int main() {

	//If the SFML setup executes successfully go into SFML loop
	if (sfmlSetup()) {
		while (gameWindow.isOpen()) {
			sf::Event event;

			while (gameWindow.pollEvent(event)) {
				if (event.type == sf::Event::Closed)
					gameWindow.close();
			}

			gameWindow.clear(); //Clear and redraw the screen
			gameWindow.draw(backgroundSprite);
			//gameWindow.draw(text);
			gameWindow.display();
		}
	} else if (loadDictionary()) {
			std::cout << "Dictionary Loaded\n" << std::endl;
			console_clear_screen();
			menuLoop();
	
	}

    return 0;
}

