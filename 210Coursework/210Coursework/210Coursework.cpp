// 210Coursework.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <tuple>
#include <fstream>
#include <windows.h> 

using namespace std;

class Tile {
public:
	char tileCharacter;
	int colourNo;
	int tileValue;
	int wordValue;
	Tile() {
		tileCharacter = char(254);
		colourNo = 7;
		tileValue = 1;
		wordValue = 1;
	}
};
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
	void ShowBoard()
	{
		for (int i = 0; i < 15; i++)
		{
			for (int j = 0; j < 15; j++)
			{
				SetConsoleTextAttribute(console, tileList[i][j].colourNo);
				cout << tileList[i][j].tileCharacter << " ";
			}
			cout << endl;
		}
		SetConsoleTextAttribute(console, 15);
	}
	void InsertWord(string word, int xStart, int yStart, char direction)
	{
		if (direction == 'V' || direction == 'v') {
			for (int i = 0; i < word.length(); i++)
			{
				tileList[(yStart-1) + i][(xStart-1)].tileCharacter = word[i];
			}
		}
		else {
			for (int i = 0; i < word.length(); i++)
			{
				tileList[(yStart-1)][(xStart-1) + i].tileCharacter = word[i];
			}
		}
		system("CLS");
		ShowBoard();
		cout << "The word " << word << " was placed at X= " << xStart << " and Y= " << yStart << endl;
	}
	Board() {
		console = GetStdHandle(STD_OUTPUT_HANDLE);
	}
};

void ParseFile(vector<tuple<string,int>>* legalWords);
void Play(vector<tuple<string, int>>* legalWords, Board playBoard);
int BoardBinarySearch(vector<tuple<string, int>>* legalWords, string word, int begin, int end);
void DisplayList(vector<tuple<string, int>>* legalWords);
void Merge(vector<tuple<string, int>>* legalWords, int begin, int end, int mid);
void MergeSort(vector<tuple<string, int>>* legalWords, int begin, int end);
void AssignWordValue(vector<tuple<string, int>>* legalWords);
void LoadAnimation();


int main()
{
	//LoadAnimation();
	bool isSorted = false;
	vector<tuple<string, int>> myWords;
	ParseFile(&myWords);
	cout << "Legal word list created" << endl;

	MergeSort(&myWords, 0, myWords.size() - 1);

	cout << "Legal word list sorted" << endl;
	AssignWordValue(&myWords);
	cout << "Word values assigned" << endl;
	cout << endl;
	Board playBoard;
	playBoard.FillBoard();
	Play(&myWords, playBoard);
}

void DisplayList(vector<tuple<string, int>>* legalWords) 
{
	for (int i = 0; i < (*legalWords).size(); i++) 
	{
		cout << get<0>((*legalWords)[i]) << " " << get<1>((*legalWords)[i]) << endl;
	}
}

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

void Play(vector<tuple<string, int>>* legalWords, Board playBoard)
{
	string playedWord;
	int wordIndex = -1;
	int xStart;
	int yStart;
	char direction;
	while (wordIndex == -1) 
	{
		cout << endl;
		cout << "What is the word that you want to play?" << endl;
		cin >> playedWord;
		wordIndex = BoardBinarySearch(legalWords, playedWord, 0, ((*legalWords).size() - 1));
		if (wordIndex == -1)
		{
			cout << "The given word is illegal" << endl;
			system("CLS");
			playBoard.ShowBoard();
		}
	}
	system("CLS");
	playBoard.ShowBoard();
	cout << "The word is legal and at index " << wordIndex << " in the table" << endl;	
	cout << "What is the X start position of the word you want to play?" << endl;
	cin >> xStart;
	system("CLS");
	playBoard.ShowBoard();
	cout << "What is the Y start position of the word you want to play?" << endl;
	cin >> yStart;
	system("CLS");
	playBoard.ShowBoard();
	cout << "What direction do you want to play your word? H for horizontal and V for vertical." << endl;
	cin >> direction;
	playBoard.InsertWord(playedWord, xStart , yStart, direction);
	
}

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

void Merge(vector<tuple<string, int>>* legalWords, int begin, int end, int mid) 
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

void MergeSort(vector<tuple<string, int>>* legalWords, int begin, int end) 
{
	if(begin<end)
	{
		
		int mid = (begin + (end-1)) / 2;
		cout << "  /-_(^o^)_-\\ " << "\r";
		cout << "  \\_-(*o*)-_/ " << "\r";
		MergeSort(legalWords, begin, mid);
		MergeSort(legalWords, mid + 1, end);

		Merge(legalWords, begin, end, mid);
	}
}

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

void LoadAnimation() 
{
	bool isSorted = false;
	int i = 0;
	while (i<10000000) 
	{
		cout << "  /-_(^o^)_-\\ " << "\r";
		Sleep(400);
		//cout << " \--(^=^)-- "  << "\r";
		//Sleep(400);
		cout << "  \\_-(*o*)-_/ "  << "\r";
		Sleep(400);
		i++;
	}
}