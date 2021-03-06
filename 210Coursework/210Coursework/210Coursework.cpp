// 210Coursework.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <tuple>
#include <fstream>
#include <windows.h> 
#include <typeinfo>
#include <map>  
#include <random>
#include <algorithm>
#include <time.h>

using namespace std;

//global int COUNT for animation
int COUNT;

//Classes
class Board;
class Tile;
class Player;
class BagOfLetters;

//void Functions 
void ParseFile(vector<tuple<string, int>>* legalWords);
void Play(vector<tuple<string, int>>* legalWords, map<char, int>* letters, Board playBoard, bool firstPlay, BagOfLetters *LetterBag, Player *Player1, int playAmount);
void DisplayList(vector<tuple<string, int>>* legalWords);
void Merge(vector<tuple<string, int>>* legalWords, int begin, int end, int mid, int count);
void MergeSort(vector<tuple<string, int>>* legalWords, int begin, int end, int count);
void AssignWordValue(vector<tuple<string, int>>* legalWords);
void FillLetterMap(map<char, int>& letters);
void TestAdjacency(Board playBoard);

//Functions that return int
int LetterToNumber(char letter);
int BoardBinarySearch(vector<tuple<string, int>>* legalWords, string word, int begin, int end);

//Function that returns Tile
Tile CoordsToTile(Board playBoard, int x, int y);

//Function that returns Boolean
bool CheckLetterHand(Player* Player1, string word, vector<char>* intersectedLetters = NULL);

//Struct of (x,y) coordinates
struct Coords {
	int x;
	int y;
};

//Player class with PlayerHand vector of characters (each initialised at NULL), PlayerPoints int (initialised at 0) and MaxSize int (initialised at 7)
class Player {
public:
	vector<char>PlayerHand{ NULL, NULL, NULL, NULL, NULL, NULL, NULL };
	int PlayerPoints;
	int MaxSize = 7;
	Player() {
		PlayerPoints = 0;
	}
};

//BagofLetters class with a Letters vector of characters (initialised with each letter in scrabble and 2 blanks), ShuffleBag method and GiveLetter method (takes pointer to player and a index int as arguments)
class BagOfLetters {
public:
	vector<char>Letters ={
		//12 e
		'e', 'e', 'e', 'e', 'e', 'e', 'e', 'e', 'e', 'e', 'e', 'e',
		//9 a
		'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a',
		//9 i
		'i', 'i', 'i', 'i', 'i', 'i', 'i', 'i', 'i',
		//8 o
		'o', 'o', 'o', 'o', 'o', 'o', 'o', 'o',
		//6 n
		'n', 'n', 'n', 'n', 'n', 'n',
		//6 r
		'r', 'r', 'r', 'r', 'r', 'r',
		//6 t
		't', 't', 't', 't', 't', 't',
		//4 l
		'l', 'l', 'l', 'l',
		//4 s
		's', 's', 's', 's',
		//4 u
		'u', 'u', 'u', 'u',
		//4 d
		'd', 'd', 'd', 'd',
		//3 g
		'g', 'g', 'g',
		//2 b
		'b', 'b',
		//2 c
		'c', 'c',
		//2 m
		'm', 'm',
		//2 p
		'p', 'p',
		//2 f
		'f', 'f',
		//2 h
		'h', 'h',
		//2 v
		'v', 'v',
		//2 w
		'w', 'w',
		//2 y
		'y', 'y',
		//1 k
		'k',
		//1 j
		'j',
		//1 x
		'x',
		//1 q
		'q',
		//1 z
		'z',
		//2 Blanks
		char(254), char(254)
	};
	void ShuffleBag() 
	{
		auto random = default_random_engine{};
		shuffle(Letters.begin(), Letters.end(), random);
	}
	void GiveLetter(Player *player, int index) 
	{
		ShuffleBag();
		int Random = rand() % 100;
		char RandomLetter = Letters[Random];
		//cout << RandomLetter << endl;
		Letters.pop_back();
		(*player).PlayerHand[index] = RandomLetter;
	}

};

//Tile class with myCoordinates (initialised as x=0 and y=0), tileCharacter (initialised as char(254)), colourNo (number of colour, intialised as 7 (white)), tileValue (letter multiplier, initialised as 1), 
//wordValue (word multiplier, initialised as 1), adjacentTile list of 4 coordinates (coordinates of adjacent tiles, each initialised as x=0 and y=0) and assignAdjacent method that assigns coordinates of adjacent tiles.
class Tile {
public:
	Coords myCoordinates;
	char tileCharacter;
	int colourNo;
	int tileValue;
	int wordValue;
	bool isUsed;
	Coords adjacentTiles[4];
	void assignAdjacent()
	{
		adjacentTiles[0].x = myCoordinates.x - 1;
		adjacentTiles[0].y = myCoordinates.y;

		adjacentTiles[1].x = myCoordinates.x;
		adjacentTiles[1].y = myCoordinates.y - 1;

		adjacentTiles[2].x = myCoordinates.x + 1;
		adjacentTiles[2].y = myCoordinates.y;

		adjacentTiles[3].x = myCoordinates.x;
		adjacentTiles[3].y = myCoordinates.y + 1;
	}
	Tile() {
		tileCharacter = char(254);
		colourNo = 7;
		tileValue = 1;
		wordValue = 1;
		isUsed = false;

		myCoordinates.x = 0;
		myCoordinates.y = 0;

		adjacentTiles[0].x = 0;
		adjacentTiles[0].y = 0;

		adjacentTiles[1].x = 0;
		adjacentTiles[1].y = 0;

		adjacentTiles[2].x = 0;
		adjacentTiles[2].y = 0;

		adjacentTiles[3].x = 0;
		adjacentTiles[3].y = 0;
	}
};

//Board class with console handle (to manipulate console colours, initialised to standard output handle), tileList 2d [15][15] array of Tiles, 
//FillBoard method that fills the tileList with tiles of different colour and value depending on their coordinates, ShowBoard method that displays the board (takes as arguments a pointer to Player1 and a pointer to LetterBag),
//InsertWord method that inserts a word into the tileList (takes as arguments pointer to legalWords, pointer to letters map, word string, xStart and yStart ints, direction character, pointer to Player 1 and pointer to LetterBag),
//CheckIntersection method that returns a boolean if a word is corectly intersection other words in tileList (takes as arguments word string, xStart and yStart ints, direction character, and pointer to a vector of intersectedLetters),
//AssignAdjacency method that calls assignAdjacent for each tile in tileList.
class Board {
public:
	HANDLE console;
	Tile tileList[15][15];
	void FillBoard()
	{
		for (int i = 0; i < 15; i++)
		{
			for (int j = 0; j < 15; j++)
			{
				Tile tile;
				tile.myCoordinates.x = i;
				tile.myCoordinates.y = j;

				//double word
				if (((i == j) || (j == 14 - i)) && ((i != 0) && (i != 5) && (i != 6) && (i != 8) && (i != 9) && (i != 14)))
				{
					tile.colourNo = 13;
					tile.tileValue = 1;
					tile.wordValue = 2;
				}

				//double letter
				else if ((i == 0 && j == 3) || (i == 0 && j == 11)
					|| (i == 2 && j == 6) || (i == 2 && j == 8)
					|| (i == 3 && j == 0) || (i == 3 && j == 7) || (i == 3 && j == 14)
					|| (i == 6 && j == 2) || (i == 6 && j == 6) || (i == 6 && j == 8) || (i == 6 && j == 12)
					|| (i == 7 && j == 3) || (i == 7 && j == 11)
					|| (i == 8 && j == 2) || (i == 8 && j == 6) || (i == 8 && j == 8) || (i == 8 && j == 12)
					|| (i == 11 && j == 0) || (i == 11 && j == 7) || (i == 11 && j == 14)
					|| (i == 12 && j == 6) || (i == 12 && j == 8)
					|| (i == 14 && j == 3) || (i == 14 && j == 11))
				{
					tile.colourNo = 11;
					tile.tileValue = 2;
					tile.wordValue = 1;
				}

				//triple letter
				else if ((i == 1 && j == 5) || (i == 1 && j == 9)
					|| (i == 5 && j == 0) || (i == 5 && j == 5) || (i == 5 && j == 9) || (i == 5 && j == 13)
					|| (i == 9 && j == 0) || (i == 9 && j == 5) || (i == 9 && j == 9) || (i == 9 && j == 13)
					|| (i == 13 && j == 5) || (i == 13 && j == 9))
				{
					tile.colourNo = 9;
					tile.tileValue = 3;
					tile.wordValue = 1;
				}

				//triple word
				else if ((i == 0 && j == 0) || (i == 0 && j == 7) || (i == 0 && j == 14) || (i == 7 && j == 0) || (i == 0 && j == 14) || (i == 14 && j == 0) || (i == 14 && j == 7) || (i == 14 && j == 14))
				{
					tile.colourNo = 4;
					tile.tileValue = 1;
					tile.wordValue = 3;
				}

				tileList[i][j] = tile;
			}
		}
	};
	void ShowBoard(Player *Player1, BagOfLetters* LetterBag)
	{
		SetConsoleTextAttribute(console, 14);
		cout << "   A B C D E F G H I J K L M N O" << endl;
		

		for (int i = 0; i < 15; i++)
		{
			SetConsoleTextAttribute(console, 14);
			if  (i < 9)
			{
				cout << " " <<  i +1 << " ";
			}

			else 
			{
					cout << i + 1 << " " ;
			}

			for (int j = 0; j < 15; j++)
			{
				SetConsoleTextAttribute(console, tileList[i][j].colourNo);
				//if(tileList[i][j].isUsed == true){ SetConsoleTextAttribute(console, 222); } colour code +16 to cycle through
				cout << tileList[i][j].tileCharacter << " ";
			}
			cout << endl;
		}
		SetConsoleTextAttribute(console, 15);
		cout << endl;
		cout << "Player Tiles: " << "       " << "P1 Score: " << (*Player1).PlayerPoints << endl;
		
		for (int i = 0; i < (*Player1).PlayerHand.size(); i++) {
			cout << (*Player1).PlayerHand[i] << " ";
		}
		cout << "       Amount of Tiles left in bag: " << (*LetterBag).Letters.size() << endl;
	}
	void InsertWord(vector<tuple<string, int>>* legalWords, map<char, int>* letters, string word, int xStart, int yStart, char direction, Player* Player1, BagOfLetters* LetterBag)
	{
		int wordIndex = BoardBinarySearch(legalWords, word, 0, ((*legalWords).size() - 1));
		int wordValue =0;
		int multiplierFlag = 0;
		if (direction == 'V' || direction == 'v') 
		{
			for (int i = 0; i < word.length(); i++)
			{
				tileList[(yStart - 1) + i][(xStart - 1)].tileCharacter = word[i];
				tileList[(yStart - 1) + i][(xStart - 1)].isUsed = true;

				if ((tileList[(yStart - 1) + i][(xStart - 1)].tileValue != 1) || (tileList[(yStart - 1) + i][(xStart - 1)].wordValue != 1))
				{
					multiplierFlag += 1;
				}
			}

			if(multiplierFlag != 0)
			{
				int wordMult = 1;
				for (int i = 0; i < word.length(); i++)
				{
					wordValue += (*letters)[word[i]] * tileList[(yStart - 1) + i][(xStart - 1)].tileValue;
					if (tileList[(yStart - 1) + i][(xStart - 1)].wordValue == 2) 
					{
						wordMult *= 2;
					}
					else if (tileList[(yStart - 1) + i][(xStart - 1)].wordValue == 3)
					{
						wordMult *= 3;
					}
				}
				wordValue *= wordMult;
			}

			else 
			{
				wordValue = get<1>((*legalWords)[wordIndex]);
			}
		}

		else 
		{
			for (int i = 0; i < word.length(); i++)
			{
				tileList[(yStart-1)][(xStart-1) + i].tileCharacter = word[i];
				tileList[(yStart - 1)][(xStart - 1) + i].isUsed = true;

				if ((tileList[(yStart - 1)][(xStart - 1) + i].tileValue != 1) || (tileList[(yStart - 1)][(xStart - 1) + i].wordValue != 1))
				{
					multiplierFlag += 1;
				}
			}


			if (multiplierFlag != 0)
			{
				int wordMult = 1;
				for (int i = 0; i < word.length(); i++)
				{
					wordValue += (*letters)[word[i]] * tileList[(yStart - 1)][(xStart - 1) + i].tileValue;
					if (tileList[(yStart - 1)][(xStart - 1) + i].wordValue == 2)
					{
						wordMult *= 2;
					}
					else if (tileList[(yStart - 1)][(xStart - 1) + i].wordValue == 3)
					{
						wordMult *= 3;
					}
				}
				wordValue *= wordMult;
			}

			else
			{
				wordValue = get<1>((*legalWords)[wordIndex]);
			}
		}

		system("CLS");
		(*Player1).PlayerPoints += wordValue;
		ShowBoard(Player1, LetterBag);
		cout << "The word " << word << " was placed at X= " << xStart << " and Y= " << yStart << endl;
		cout << "The value of the played word is: " << wordValue << endl;
	}
	bool CheckIntersection(string word, int xStart, int yStart, char direction, vector<char>*intersectedLetters)
	{
		int intersectionCount = 0;
		if (direction == 'v' || direction == 'V')
		{
			for (int i = 0; i < word.length(); i++)
			{
				if(tileList[(yStart - 1) + i][xStart - 1].isUsed == true)
				{
					intersectionCount++;
					if (tileList[(yStart - 1) + i][xStart - 1].tileCharacter != word[i])
					{
						return false;
					}
					else
					{
						(*intersectedLetters).push_back(tileList[(yStart - 1) + i][xStart - 1].tileCharacter);
					}
				}
			}
		}

		else
		{
			for (int i = 0; i < word.length(); i++)
			{
				if (tileList[yStart - 1][(xStart - 1) + i].isUsed == true)
				{
					intersectionCount++;
					if (tileList[yStart - 1][(xStart - 1) + i].tileCharacter != word[i])
					{
						return false;
					}
					else
					{
						(*intersectedLetters).push_back(tileList[yStart - 1][(xStart - 1) + i].tileCharacter);
					}
				}
			}
		}

		if (intersectionCount == 0)
		{
			return false;
		}

		return true;
	}
	void AssignAdjacency()
	{
		for (int i = 0; i < 15; i++)
		{
			for (int j = 0; j < 15; j++)
			{
				tileList[i][j].assignAdjacent();
			}
		}
	}
	Board() {
		console = GetStdHandle(STD_OUTPUT_HANDLE);
	}
};

//main function
int main()
{
	//variables
	Board playBoard;
	BagOfLetters LetterBag;
	Player Player1;
	COUNT = 0;
	bool isSorted = false;
	map<char, int> LetterValues;
	FillLetterMap(LetterValues);
	vector<tuple<string, int>> myWords;
	int plays;

	//seeding random
	srand(time(NULL));
	ParseFile(&myWords);
	MergeSort(&myWords, 0, myWords.size() - 1, 0);
	AssignWordValue(&myWords);
	playBoard.FillBoard();
	playBoard.AssignAdjacency();

	cout << "How many plays do you want?:" << endl;
	cin >> plays;
	Play(&myWords, &LetterValues, playBoard, true, &LetterBag, &Player1, plays);
}

//Utility function that desplays each item in function, used for debug
void DisplayList(vector<tuple<string, int>>* legalWords) 
{
	for (int i = 0; i < (*legalWords).size(); i++) 
	{
		cout << get<0>((*legalWords)[i]) << " " << get<1>((*legalWords)[i]) << endl;
	}
}

//Function that parses list of words file
void ParseFile(vector<tuple<string, int>>* legalWords)
{
	int amountOfWords = 0;
	string word;
	ifstream wordFile;
	wordFile.open("words.txt");
	while (wordFile >> word) 
	{
		int ilegal = 0;
		tuple<string, int> result;
		for (int i = 0; i < (int)word.length(); i++) 
		{
			if (word[i] < 97 || word[i]>122) {
				ilegal++;
			}
		}
		if (ilegal == 0) 
		{
			result = make_tuple(word, 0);
			legalWords->push_back(result);
			amountOfWords++;
		}
	}
	cout << "There are: " << amountOfWords << " legal words"<< endl;
	wordFile.close();
}


//Play recursive function, the function takes as parameter a pointer to the list of legal words, a pointer to a map that gives lettervalue for a given character, a playBoard, a pointer to LetterBag, a pointer to Player1 and a int playAmount.
void Play(vector<tuple<string, int>>* legalWords, map<char, int>* letters, Board playBoard, bool firstPlay, BagOfLetters *LetterBag, Player *Player1, int playAmount)
{
	//runs as long as there are still playAmounts
	if(playAmount>0)
	{
		(*LetterBag).ShuffleBag();
		//replaces NULL letters by random letters from the bag
		for(int index = 0; index < (*Player1).PlayerHand.size(); index ++)
			{ 
			if((*Player1).PlayerHand[index] == NULL)
			{
				(*LetterBag).GiveLetter(Player1, index);
			}
		}
		system("CLS");
		playBoard.ShowBoard(Player1, LetterBag);
		string playedWord;
		int wordIndex = -1;
		int xStart;
		char xStartLetter;
		int yStart;
		char direction;
		
		cout << endl << "You have " << playAmount << " plays left!" << endl;

		//Ask for word until the player enters a legal word
		while (wordIndex == -1)
		{
			cout << endl;
			cout << "What is the word that you want to play? (Or say PASS if unable to play)" << endl;
			cin >> playedWord;

			//to pass turn and reshuffle cards
			if (playedWord == "PASS")
			{
				for (int i = 0; i < (*Player1).PlayerHand.size(); i++) 
				{
					(*LetterBag).Letters.push_back((*Player1).PlayerHand[i]);
					(*Player1).PlayerHand[i] = NULL;
				}
				playAmount--;
				Play(legalWords, letters, playBoard, firstPlay, LetterBag, Player1, playAmount);
			}

			wordIndex = BoardBinarySearch(legalWords, playedWord, 0, ((*legalWords).size() - 1));
			if (wordIndex == -1)
			{
				cout << "The given word is illegal" << endl;
			}
		}

		system("CLS");
		playBoard.ShowBoard(Player1, LetterBag);
		cout << "The word is legal and at index " << wordIndex << " in the table" << endl;

		//If this isn't the player's first turn
		if (firstPlay == false) 
		{
			cout << "What is the Horizontal start position of the word you want to play? (A to O)" << endl;
			cin >> xStartLetter;
			xStart = LetterToNumber(xStartLetter);

			while ((xStart < 1 || xStart >15) || (typeid(xStart) != typeid(int)))
			{
				cout << char(200) << "(" << char(248) << "^" << char(248) << ")" << char(188) << " Error, please choose a Horizontal position between A and O: " << endl;
				cin >> xStartLetter;
				xStart = LetterToNumber(xStartLetter);

				system("CLS");
				playBoard.ShowBoard(Player1, LetterBag);
			}
			system("CLS");
			playBoard.ShowBoard(Player1, LetterBag);

			cout << "What is the Vertical start position of the word you want to play? (1 to 15)" << endl;
			cin >> yStart;
			while (yStart < 1 || yStart >15)
			{
				cout << char(200) << "(" << char(248) << "^" << char(248) << ")" << char(188) << " Error, please choose a Vertical position between 1 and 15: " << endl;
				cin >> yStart;
				system("CLS");
				playBoard.ShowBoard(Player1, LetterBag);
			}
			system("CLS");
			playBoard.ShowBoard(Player1, LetterBag);

			cout << "What direction do you want to play your word? H for horizontal and V for vertical." << endl;
			cin >> direction;
			while (!(direction == 'h' || direction == 'H' || direction == 'v' || direction == 'V'))
			{
				cout << char(200) << "(" << char(248) << "^" << char(248) << ")" << char(188) << " Error, please type H for horizontal placement or V for vertical placement: " << endl;
				cin >> direction;
				system("CLS");
				playBoard.ShowBoard(Player1, LetterBag);
			}
			system("CLS");
			playBoard.ShowBoard(Player1, LetterBag);

			if ((xStart + playedWord.length() > 15) && ((direction == 'h') || (direction == 'H')) || ((yStart + playedWord.length() > 15) && ((direction == 'v') || (direction == 'V'))))
			{
				if ((direction == 'h') || (direction == 'H'))
				{
					cout << char(200) << "(" << char(248) << "^" << char(248) << ")" << char(188) << " The word won't fit horizontally at (" << xStart << ";" << yStart << ")" << endl;
				}
				else
				{
					cout << char(200) << "(" << char(248) << "^" << char(248) << ")" << char(188) << " The word won't fit vertically at (" << xStart << ";" << yStart << ")" << endl;
				}

				Play(legalWords, letters, playBoard, firstPlay, LetterBag, Player1, playAmount);
			}

			vector<char>intersectedLetters;
			bool checkBool = playBoard.CheckIntersection(playedWord, xStart, yStart, direction, &intersectedLetters);
			if (checkBool == false)
			{
				cout << "You can't place that there, your word must have a valid intersection wih a previously played word." << endl;
			}
			else
			{
				bool testFlag = CheckLetterHand(Player1, playedWord, &intersectedLetters);
				if (testFlag == true)
				{
					int amountOfLettersFound = 0;

					for (int i = 0; i < playedWord.size(); i++)
					{
						for (int j = 0; j < (*Player1).PlayerHand.size(); j++)
						{
							if (playedWord[i] == (*Player1).PlayerHand[j])
							{
								(*Player1).PlayerHand[j] = NULL;
								amountOfLettersFound++;
								break;
							}
						}
					}
					while (amountOfLettersFound != (playedWord.size()-intersectedLetters.size()))
					{
						for (int i = 0; i < (*Player1).PlayerHand.size(); i++)
						{
							if ((*Player1).PlayerHand[i] == char(254))
							{
								(*Player1).PlayerHand[i] = NULL;
								amountOfLettersFound++;
								break;
							}
						}
					}
					for (int i = 0; i < intersectedLetters.size(); i++) 
					{
						for(int j = 0; j < (*Player1).PlayerHand.size(); j++)
						{
							if((*Player1).PlayerHand[j] == NULL)
							{
								(*Player1).PlayerHand[j] = intersectedLetters[i];
								break;
							}
						}
					}
					
					playBoard.InsertWord(legalWords, letters, playedWord, xStart, yStart, direction, Player1, LetterBag);
				}
			}

		}
		
		//If this is the player's first turn
		else
		{ 
			xStart = 8;
			yStart = 8;
			cout << "What direction do you want to play your word? H for horizontal and V for vertical." << endl;
			cin >> direction;
			while (!(direction == 'h' || direction == 'H' || direction == 'v' || direction == 'V'))
			{
				cout << char(200) << "(" << char(248) << "^" << char(248) << ")" << char(188) << " Error, please type H for horizontal placement or V for vertical placement: " << endl;
				cin >> direction;
				system("CLS");
				playBoard.ShowBoard(Player1, LetterBag);
			}
			system("CLS");
			playBoard.ShowBoard(Player1, LetterBag);

			if ((xStart + playedWord.length() > 15) && ((direction == 'h') || (direction == 'H')) || ((yStart + playedWord.length() > 15) && ((direction == 'v') || (direction == 'V'))))
			{
				if ((direction == 'h') || (direction == 'H'))
				{
					cout << char(200) << "(" << char(248) << "^" << char(248) << ")" << char(188) << " The word won't fit horizontally at (" << xStart << ";" << yStart << ")" << endl;
				}
				else
				{
					cout << char(200) << "(" << char(248) << "^" << char(248) << ")" << char(188) << " The word won't fit vertically at (" << xStart << ";" << yStart << ")" << endl;
				}

				Play(legalWords, letters, playBoard, firstPlay, LetterBag, Player1, playAmount);
			}

			bool testFlag = CheckLetterHand(Player1, playedWord);
			if (testFlag == true)
			{
				int amountOfLettersFound = 0;

				for(int i= 0; i < playedWord.size(); i++)
				{
					for(int j = 0; j < (*Player1).PlayerHand.size(); j++)
					{
						if (playedWord[i] == (*Player1).PlayerHand[j])
						{
							(*Player1).PlayerHand[j] = NULL;
							amountOfLettersFound ++;
							break;
						}
					}
				}
				while (amountOfLettersFound != playedWord.size()) 
				{
					for(int i = 0; i < (*Player1).PlayerHand.size(); i++)
					{
						if((*Player1).PlayerHand[i] == char(254))
						{
							(*Player1).PlayerHand[i] = NULL;
							amountOfLettersFound++;
							break;
						}	
					}
				}		
				playBoard.InsertWord(legalWords, letters, playedWord, xStart, yStart, direction, Player1, LetterBag);
			}

			else
			{
				Play(legalWords, letters, playBoard, firstPlay, LetterBag, Player1, playAmount);
			}
		}
		firstPlay = false;
		playAmount -= 1;
		Play(legalWords, letters, playBoard, firstPlay, LetterBag, Player1, playAmount);
	}
	//end
	else
	{
		string randomSentence;
		cout << endl << "You are out of plays, thanks for playing! What did you think? (This will not be recorded)" << endl;
		cin >> randomSentence;
		exit(0);
	}	
}

//Binary search for word in legalWords list
int BoardBinarySearch(vector<tuple<string, int>>* legalWords, string word, int begin, int end) 
{
	int middle = begin + (end - begin) / 2;

	if (begin > end) 
	{
		return -1;
	}
	else if (get<0>((*legalWords)[middle]) == word)
	{
		return middle;
	}

	else if (get<0>((*legalWords)[middle]) > word)
	{
		return BoardBinarySearch(legalWords, word, begin, middle - 1);
	}

	else if (get<0>((*legalWords)[middle]) < word)
	{
		return BoardBinarySearch(legalWords, word, middle+1, end);
	}
		
}

//Merge function coplimentary to mergeSort
void Merge(vector<tuple<string, int>>* legalWords, int begin, int end, int mid, int count) 
{
	int i, j, k;
	int leftLength = mid - begin + 1;
	int rightLength = end - mid;

	vector<string> Left, Right;

	for (i = 0; i < leftLength; i++)
	{
		string word = get<0>((*legalWords)[begin + i]);
		Left.push_back(word);
		
	}
	for (j = 0; j < rightLength; j++)
	{
		string word = get<0>((*legalWords)[mid + 1 + j]);
		Right.push_back(word);
		
	}

	i = 0; j = 0; k = begin;
	while (i < leftLength && j < rightLength)
	{
		if (Left[i] <= Right[j])
		{
			get<0>((*legalWords)[k]) = Left[i];
			i++;
		}
		else 
		{
			get<0>((*legalWords)[k]) = Right[j];
			j++;
		}
		k++;
	}

	while (i < leftLength) 
	{
		get<0>((*legalWords)[k]) = Left[i];
		i++;
		k++;
	}

	while (j < rightLength)
	{
		get<0>((*legalWords)[k]) = Right[j];
		j++;
		k++;
	}
	
}

//MergeSort of list
void MergeSort(vector<tuple<string, int>>* legalWords, int begin, int end, int count) 
{
	COUNT++;
	if (COUNT == 10000) { cout << " Sorting List.    /-_(^o^)_-\\ " << "\r";}
	else if (COUNT == 20000) { cout << " Sorting List..   \\_-(*o*)-_/ " << "\r";}
	else if (COUNT == 30000) { cout << " Sorting List...  /-_(^o^)_-\\ " << "\r";}
	else if (COUNT == 40000) { cout << " Sorting List.... \\_-(*o*)-_/ " << "\r"; COUNT = 0;}
	
	if(begin<end)
	{
		int mid = (begin + (end-1)) / 2;
		MergeSort(legalWords, begin, mid,count);
		MergeSort(legalWords, mid + 1, end, count);
		Merge(legalWords, begin, end, mid, count);
	}
}

//Assign word value to each tuple 
void AssignWordValue(vector<tuple<string, int>>* legalWords) 
{
	for (int i = 0; i < (*legalWords).size(); i++) 
	{ 
		string word = get<0>((*legalWords)[i]);
		int wordValue = 0;
		tuple<string, int> result;

		for (int j = 0; j < (int)word.length(); j++)
		{
			if (word[j] == 'q' || word[j] == 'z')
			{
				wordValue += 10;
			}
			else if (word[j] == 'd' || word[j] == 'g')
			{
				wordValue += 2;
			}
			else if (word[j] == 'b' || word[j] == 'c' || word[j] == 'm' || word[j] == 'p')
			{
				wordValue += 3;
			}
			else if (word[j] == 'f' || word[j] == 'h' || word[j] == 'v' || word[j] == 'w' || word[j] == 'y')
			{
				wordValue += 4;
			}
			else if (word[j] == 'k')
			{
				wordValue += 5;
			}
			else if (word[j] == 'j' || word[j] == 'x')
			{
				wordValue += 8;
			}
			else
			{
				wordValue += 1;
			}
		}

		result = make_tuple(word, wordValue);
		(*legalWords)[i] = result;
	}
}

//Fill map of letter values
void FillLetterMap(map<char,int> &letters) 
{
	//1 value letters A, E, I, O, U, L, N, S, T, R
	(letters)['a'] = 1;
	(letters)['A'] = 1;

	(letters)['e'] = 1;
	(letters)['E'] = 1;

	(letters)['i'] = 1;
	(letters)['I'] = 1;

	(letters)['o'] = 1;
	(letters)['O'] = 1;

	(letters)['u'] = 1;
	(letters)['U'] = 1;

	(letters)['l'] = 1;
	(letters)['L'] = 1;

	(letters)['n'] = 1;
	(letters)['N'] = 1;

	(letters)['s'] = 1;
	(letters)['S'] = 1;

	(letters)['t'] = 1;
	(letters)['T'] = 1;

	(letters)['r'] = 1;
	(letters)['R'] = 1;

	//2 value letters D, G
	(letters)['d'] = 2;
	(letters)['D'] = 2;

	(letters)['g'] = 2;
	(letters)['G'] = 2;

	//3 value letters B, C, M, P
	(letters)['b'] = 3;
	(letters)['B'] = 3;

	(letters)['c'] = 3;
	(letters)['C'] = 3;

	(letters)['m'] = 3;
	(letters)['M'] = 3;

	(letters)['p'] = 3;
	(letters)['P'] = 3;

	//4 value letters F, H, V, W, Y
	(letters)['f'] = 4;
	(letters)['F'] = 4;

	(letters)['h'] = 4;
	(letters)['H'] = 4;

	(letters)['v'] = 4;
	(letters)['V'] = 4;

	(letters)['w'] = 4;
	(letters)['W'] = 4;

	(letters)['y'] = 4;
	(letters)['Y'] = 4;

	//5 value letters K
	(letters)['k'] = 5;
	(letters)['K'] = 5;

	//8 value letters J, X
	(letters)['j'] = 8;
	(letters)['J'] = 8;

	(letters)['x'] = 8;
	(letters)['X'] = 8;

	//10 value letters Q, Z
	(letters)['q'] = 10;
	(letters)['Q'] = 10;

	(letters)['z'] = 10;
	(letters)['Z'] = 10;
}

//Give value for any letter
int LetterToNumber(char letter)
{
	int numberValue = 0;
	if((letter == 'A') || (letter == 'a'))
	{
		numberValue = 1;
	}
	if ((letter == 'B') || (letter == 'b'))
	{
		numberValue = 2;
	}
	if ((letter == 'C') || (letter == 'c'))
	{
		numberValue = 3;
	}
	if ((letter == 'D') || (letter == 'd'))
	{
		numberValue = 4;
	}
	if ((letter == 'E') || (letter =='e'))
	{
		numberValue = 5;
	}
	if ((letter == 'F') || (letter == 'f'))
	{
		numberValue = 6;
	}
	if ((letter == 'G') || (letter == 'g'))
	{
		numberValue = 7;
	}
	if ((letter == 'H') || (letter == 'h'))
	{
		numberValue = 8;
	}
	if ((letter == 'I') || (letter == 'i'))
	{
		numberValue = 9;
	}
	if ((letter == 'J') || (letter == 'j'))
	{
		numberValue = 10;
	}
	if ((letter == 'K') || (letter == 'k'))
	{
		numberValue = 11;
	}
	if ((letter == 'L') || (letter == 'l'))
	{
		numberValue = 12;
	}
	if ((letter == 'M') || (letter == 'm'))
	{
		numberValue = 13;
	}
	if ((letter == 'N') || (letter == 'n'))
	{
		numberValue = 14;
	}
	if ((letter == 'O') || (letter == 'o'))
	{
		numberValue = 15;
	}
	return numberValue;
}

//Utility function to test adjacency
void TestAdjacency (Board playBoard)
{
	int x, y;
	cout << "What is the x of the tile you want to test?" << endl;
	cin >> x;
	cout << "What is the y of the tile you want to test?" << endl;
	cin >> y;
	
	Tile playTile = playBoard.tileList[x][y];
	playTile.assignAdjacent();

	cout << "left: " << playTile.adjacentTiles[0].x << ";" << playTile.adjacentTiles[0].y << endl;
	cout << "up: " << playTile.adjacentTiles[1].x << ";" << playTile.adjacentTiles[1].y << endl;
	cout << "right: " << playTile.adjacentTiles[2].x << ";" << playTile.adjacentTiles[2].y << endl;
	cout << "down: " << playTile.adjacentTiles[3].x << ";" << playTile.adjacentTiles[3].y << endl;
}

//Give tile for any given coordinates
Tile CoordsToTile(Board playBoard, int x, int y)
{
	return (playBoard.tileList[x][y]);
}

//Check to see if the played word is in the players hand
bool CheckLetterHand(Player* Player1, string word, vector<char>* intersectedLetters)
{
	int blankAmount = 0;
	int flag = 0;
	for(int i = 0; i< (*Player1).PlayerHand.size(); i++)
	{
		if((*Player1).PlayerHand[i] == char(254))
		{
			blankAmount++;
		}
	}

	for(int i=0; i<word.size(); i++)
	{
		bool found = false;
		for(int j = 0; j < (*Player1).PlayerHand.size(); j++)
		{
			if((word[i]==(*Player1).PlayerHand[j]))
			{
				found = true;
				break;
			}
		}
		if (found == false)
		{
			for (int k = 0; k < (*intersectedLetters).size(); k++)
			{
				if (word[i] == (*intersectedLetters)[k])
				{
					found++;
					break;
				}
			}
		}
		if (found == true) 
		{
			flag++;
		}
	}
	if (flag == word.size())
	{
		return true;
	}
	
	else if(flag + blankAmount >= word.size())
	{
		char check;
		cout << "To place this word you will need to use " << word.size() - flag << " Blanks, is this ok? (Y/N)" << endl;
		cin >> check;
		if (check == 'y' || check == 'Y')
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		cout << "You don't have the letters to play that word" << endl;
		return false;
	}
}

