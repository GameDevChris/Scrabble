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

class Tile {
	public:
	int colourNo;
};
int main()
{
	HANDLE  hConsole;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	vector<tuple<string, int>> myWords;
	ParseFile(&myWords);
	cout << 'test' << endl;
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

			if (i == 0 && j==0) {
				tile.colourNo = 4;
			}

			else 
			{
				tile.colourNo = 7;
			}
			SetConsoleTextAttribute(console, tile.colourNo);
			cout << "+ ";
		}
		cout << endl;
	}

}