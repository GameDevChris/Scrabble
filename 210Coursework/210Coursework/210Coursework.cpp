// 210Coursework.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <tuple>
#include <fstream>
#include <windows.h> 
#include <typeinfo>
#include <map>  

using namespace std;

int BoardBinarySearch(vector<tuple<string, int>>* legalWords, string word, int begin, int end);

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
	void InsertWord(vector<tuple<string, int>>* legalWords, map<char, int>* letters, string word, int xStart, int yStart, char direction)
	{
		int wordIndex = BoardBinarySearch(legalWords, word, 0, ((*legalWords).size() - 1));
		int wordValue =0;
		int multiplierFlag = 0;
		if (direction == 'V' || direction == 'v') 
		{
			for (int i = 0; i < word.length(); i++)
			{
				tileList[(yStart - 1) + i][(xStart - 1)].tileCharacter = word[i];

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
		ShowBoard();
		cout << "The word " << word << " was placed at X= " << xStart << " and Y= " << yStart << endl;
		cout << "The value of the played word is: " << wordValue << endl;
	}

	Board() {
		console = GetStdHandle(STD_OUTPUT_HANDLE);
	}
};

void ParseFile(vector<tuple<string,int>>* legalWords);
void Play(vector<tuple<string, int>>* legalWords, map<char, int>* letters, Board playBoard);
void DisplayList(vector<tuple<string, int>>* legalWords);
void Merge(vector<tuple<string, int>>* legalWords, int begin, int end, int mid, int count);
void MergeSort(vector<tuple<string, int>>* legalWords, int begin, int end, int count);
void AssignWordValue(vector<tuple<string, int>>* legalWords);
void FillLetterMap(map<char, int>& letters);

int COUNT;
int main()
{
	COUNT = 0;
	//LoadAnimation();
	bool isSorted = false;
	map<char, int> LetterValues;
	FillLetterMap(LetterValues);
	vector<tuple<string, int>> myWords;
	ParseFile(&myWords);
	cout << "Legal word list created" << endl;

	MergeSort(&myWords, 0, myWords.size() - 1, 0);

	cout << "Legal word list sorted" << endl;
	AssignWordValue(&myWords);
	cout << "Word values assigned" << endl;
	cout << endl;
	Board playBoard;
	playBoard.FillBoard();
	Play(&myWords, &LetterValues, playBoard);
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

void Play(vector<tuple<string, int>>* legalWords, map<char, int>* letters, Board playBoard)
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
		}
	}
	system("CLS");
	playBoard.ShowBoard();
	cout << "The word is legal and at index " << wordIndex << " in the table" << endl;	
	
	cout << "What is the X start position of the word you want to play?" << endl;
	cin >> xStart;
	while((xStart<1 || xStart >15)||(typeid(xStart)!=typeid(int)))
	{
		cout << char(200) << "(" << char(248) << char(239) << char(248) << ")" << char(188)  << " Error, please choose a X between 1 and 15: " << endl;
		cin >> xStart;
		system("CLS");
		playBoard.ShowBoard();
	}
	system("CLS");
	playBoard.ShowBoard();

	cout << "What is the Y start position of the word you want to play?" << endl;
	cin >> yStart;
	while (yStart < 1 || yStart >15)
	{
		cout << char(200) << "(" << char(248) << char(239) << char(248) << ")" << char(188) << " Error, please choose a Y between 1 and 15: " << endl;
		cin >> yStart;
		system("CLS");
		playBoard.ShowBoard();
	}
	system("CLS");
	playBoard.ShowBoard();

	cout << "What direction do you want to play your word? H for horizontal and V for vertical." << endl;
	cin >> direction;
	while (!(direction == 'h' || direction == 'H' || direction == 'v' || direction == 'V'))
	{
		cout << char(200) << "(" << char(248) << char(239) << char(248) << ")" << char(188) << " Error, please type H for horizontal placement or V for vertical placement: " << endl;
		cin >> direction;
		system("CLS");
		playBoard.ShowBoard();
	}
	system("CLS");
	playBoard.ShowBoard();

	playBoard.InsertWord(legalWords, letters, playedWord, xStart , yStart, direction);
	
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

