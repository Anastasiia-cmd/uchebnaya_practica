#include <iostream>
#include <fstream> //класс
#include <string>
#include <list>
#include <vector>
#include <Windows.h>
#include <chrono>
#include <algorithm>

using namespace std;

list<string> dicision_into_words(string text)
{
	list<string> words;
	vector<char> symbols_of_word;
	for (char symbol : text)
	{
		if (symbol != ' ')
		{
			symbols_of_word.push_back(symbol);
		}
		else
		{
			string word(symbols_of_word.begin(), symbols_of_word.end());
			words.push_back(word);
			symbols_of_word.clear();

		}
	}
	return words;
}

list<string> clear_of_excess(list<string> words)
{
	list<string> updated_words;
	vector<char> symbols_of_updated_word;
	for (string word : words)
	{
		for (char symbol : word)
		{
			if (symbol >= 'а' && symbol <= 'я') symbols_of_updated_word.push_back(symbol);

		}
		string updated_word(symbols_of_updated_word.begin(), symbols_of_updated_word.end());
		updated_words.push_back(updated_word);
		symbols_of_updated_word.clear();
	}
	return updated_words;
}

vector <string> bubble_sort(vector<string> words)
{
	for (unsigned int i = 0; i < words.size(); i++)
	{
		for (unsigned int j = 0; j < words.size() - 1; j++)
		{
			if (words[j] > words[j + 1])
			{
				string temp = words[j];
				words[j] = words[j + 1];
				words[j + 1] = temp;
			}
		}
	}
	return words;
}

int getNumofWords(string& s) {
	int i = 0;
	int count = 0;
	while (i != s.size())
	{    // проходим с начала до конца строки
		while (isspace(s[i]) && i != s.size()) // если это пробел и не конец строки
			++i;								// шаг вперёд, выходим из цикла когда нашли не пробел
		int j = i;							 // т.е. позицию первой буквы записываем в j
		while (!isspace(s[j]) && j != s.size())  // ещё один цикл, но уже: если это НЕ пробел и не конец строки
			++j;          // шаг вперёд
		if (i != j)
		{       // если позиция первой буквы не равна последней
			++count;          // то увеличиваем счётчик слов на 1
			i = j;            // теперь i равно позиции последней буквы найденного слова
		}
	}
	return count;
}

void Statistics(vector<string> sort_words, ofstream& file)
{
	file << "Вариант 13: кириллица, по алфавиту, по возрастанию, игнорировать числа, сортировка пузырьком." << endl;
	cout << "Статистика (количество слов на каждую букву алфавита)" << endl;
	file << "Статистика (количество слов на каждую букву алфавита)" << endl;
	int alpha[32] = { 0 };
	for (int i = 0; i < sort_words.size(); i++)
	{
		if (sort_words[i][0] >= 'а' && sort_words[i][0] <= 'я')
			alpha[sort_words[i][0] - 'а']++;
	}
	for (int i = 0; i < 32; i++)
	{
		cout << (char)(i + 'а') << " - " << alpha[i] << "\n";
		file << (char)(i + 'а') << " - " << alpha[i] << "\n";
	}
}


float  sort_and_sorttime(vector<string> sort)
{
	auto  start = chrono::steady_clock::now();
	bubble_sort(sort);
	auto end = chrono::steady_clock::now();
	chrono::duration<float> duraction = end - start;
	return duraction.count();
}


int main(int argc, char* argv[])
{
	setlocale(LC_ALL, "rus");
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	int count_words = 0;
	float time = 0;
	string line, file_in, file_analis,file_sorted;
	cout << "Введите название файла, откуда взять текст: " << endl;
	getline(cin, file_in);
	file_in += ".txt";
	cout << "Введите название файла для отправки анализа: " << endl;
	getline(cin, file_analis);
	file_analis += ".txt";
	cout << "Введите название файла для отправки отсортированного текста: " << endl;
	getline(cin, file_sorted);
	file_sorted += ".txt";
	ifstream in(file_in); //открытие файла для чтения
	ofstream result(file_sorted, ios::out), analysis(file_analis, ios::out);
	list<string> mywords;
	vector <string> sort_words;
	
	if (in.is_open())
	{
		while (getline(in, line))
		{
			cout << "Исходный текст: \n" << line << endl;
			analysis << "Исходный текст: \n" << line << endl;
			transform(line.begin(), line.end(), line.begin(), tolower); //весь текст преобразовываем к нижнему регистру
			count_words = getNumofWords(line);
			cout << "Вариант 13: кириллица, по алфавиту, по возрастанию, игнорировать числа, сортировка пузырьком." << endl;
			analysis << "Количество слов: " << count_words << endl;
			cout << "Количество слов: " << count_words << endl;
			mywords = dicision_into_words(line);
			mywords = clear_of_excess(mywords);
		}
	}
	else
	{
		cout << "Error!";
	}
	sort_words.reserve(mywords.size());
	copy(mywords.begin(), mywords.end(), back_inserter(sort_words)); // перемещаем элементы вектора mywords в вектор sort_words
	time = sort_and_sorttime(sort_words);
	analysis << "Время сортировки: " << time << "сек" << endl;
	cout << "Время сортировки: " << time << "сек" << endl;
	sort_words = bubble_sort(sort_words);
	auto is_empty = [](const string& s)
	{
		return s.find_first_not_of(" \t");
	};
	sort_words.erase(remove_if(begin(sort_words), end(sort_words), is_empty), end(sort_words));
	for (auto el:sort_words)
	{
		result << el << endl;
	}
	Statistics(sort_words, analysis);
	result.close();
	analysis.close();
	in.close();
}

