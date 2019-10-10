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


			if (( (i == j) || (j == 14 - i) ) && ((i != 0) && (i != 5) && (i != 6) && (i != 8) && (i != 9) && (i != 14)))
			{
				tile.colourNo = 13;
			}

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
			}	

			else if ((i == 1 && j == 5) || (i == 1 && j == 9)
				|| (i == 5 && j == 0) || (i == 5 && j == 5) || (i == 5 && j == 9) || (i == 5 && j == 13)
				|| (i == 9 && j == 0) || (i == 9 && j == 5) || (i == 9 && j == 9) || (i == 9 && j == 13)
				|| (i == 13 && j == 5) || (i == 13 && j == 9))
			{
				tile.colourNo = 9;
			}

			else if ((i == 0 && j==0) || (i == 0 && j == 7) || (i == 0 && j == 14) || (i == 7 && j == 0) || (i == 0 && j == 14) || (i == 14 && j == 0) || (i == 14 && j == 7) || (i == 14 && j == 14)) 
			{
				tile.colourNo = 4;
			}


			else 
			{
				tile.colourNo = 7;
			}
			SetConsoleTextAttribute(console, tile.colourNo);
			cout << char(254) << " ";
		}
		cout << endl;
	}
	SetConsoleTextAttribute(console, 15);
}