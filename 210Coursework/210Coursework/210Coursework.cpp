// 210Coursework.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <tuple>
#include <fstream>
#include <windows.h> 

using namespace std;

void ParseFile(vector<tuple<string,int>>* legalWords);
void CreateBoard(HANDLE console);
void Play(vector<tuple<string, int>>* legalWords);
int BoardBinarySearch(vector<tuple<string, int>>* legalWords, string word, int begin, int end);

class Tile {
	public:
	int colourNo;
	int tileValue;
	int wordValue;
};
int main()
{
	HANDLE  hConsole;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	vector<tuple<string, int>> myWords;
	ParseFile(&myWords);
	CreateBoard(hConsole);
}

void ParseFile(vector<tuple<string, int>>* legalWords)
{
	int amountOfWords = 0;
	string word;
	ifstream wordFile;
	wordFile.open("words.txt");
	if (wordFile.is_open()) 
	{
		cout << "OPEN" << endl;
	}

	while (wordFile >> word) 
	{
		int ilegal = 0;
		int wordValue = 0;
		tuple<string, int> result;
		for (int i = 0; i < (int)word.length(); i++) 
		{
			if (word[i] < 97 || word[i]>122) {
				ilegal++;
			}
		}
		if (ilegal == 0) 
		{
			for (int i = 0; i < (int)word.length(); i++)
			{
				if (word[i] == 'q' || word[i] == 'z')
				{
					wordValue += 10;
				}
				else if (word[i] == 'd' || word[i] == 'g')
				{
					wordValue += 2;
				}
				else if (word[i] == 'b' || word[i] == 'c' || word[i] == 'm' || word[i] == 'p')
				{
					wordValue += 3;
				}
				else if (word[i] == 'f' || word[i] == 'h' || word[i] == 'v' || word[i] == 'w' || word[i] == 'y')
				{
					wordValue += 4;
				}
				else if (word[i] == 'k')
				{
					wordValue += 5;
				}
				else if (word[i] == 'j' || word[i] == 'x')
				{
					wordValue += 8;
				}
				else
				{
					wordValue += 1;
				}
			}
			result = make_tuple(word, wordValue);
			legalWords->push_back(result);
			//cout << word << wordValue << endl;
			amountOfWords++;
		}
	}
	cout << "There are: " << amountOfWords << " legal words"<< endl;
	wordFile.close();
}

void CreateBoard(HANDLE console)
{

	Tile boardDim[15][15];

	for (int i = 0; i < 15; i++)
	{
		for (int j = 0; j < 15; j++)
		{
			Tile tile;
			

			//double word
			if (( (i == j) || (j == 14 - i) ) && ((i != 0) && (i != 5) && (i != 6) && (i != 8) && (i != 9) && (i != 14)))
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
			else if ((i == 0 && j==0) || (i == 0 && j == 7) || (i == 0 && j == 14) || (i == 7 && j == 0) || (i == 0 && j == 14) || (i == 14 && j == 0) || (i == 14 && j == 7) || (i == 14 && j == 14)) 
			{
				tile.colourNo = 4;
				tile.tileValue = 1;
				tile.wordValue = 3;
			}

			//generic tile
			else 
			{
				tile.colourNo = 7;
				tile.tileValue = 1;
				tile.wordValue = 1;
			}
			boardDim[i][j] = tile;
			SetConsoleTextAttribute(console, tile.colourNo);
			cout << char(254) << " ";
		}
		cout << endl;
	}
	SetConsoleTextAttribute(console, 15);
}

void Play(vector<tuple<string, int>>* legalWords)
{
	int playedWord;
	cout << "What is the word that you want to play?" << endl;
	cin >> playedWord;
	int listSize = (*legalWords).size;
	
}

int BoardBinarySearch(vector<tuple<string, int>>* legalWords, string word, int begin, int end) 
{
	int middle = begin + (begin - end) / 2;
	
	if (get<0>((*legalWords)[middle]) == word)
	{
		return middle;
	}

	else if (get<0>((*legalWords)[middle]) > word)
	{
		return BoardBinarySearch((*legalWords), word, begin, middle - 1);
	}

	else
	{
		return BoardBinarySearch((*legalWords), word, middle+1, end);
	}
		
}