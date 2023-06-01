#include <iostream>
#include <fstream>
#include <string>
#include <stack>
#include <vector>
#include <map>

using namespace std;

//Преобразование в нижний регистр
string to_low(string text)
{
	for (unsigned int i = 0; i < text.length(); i++)
	{
		if (isupper((unsigned char)text[i])) text[i] = tolower(text[i]);
	}
	return text;
}

//#1
void search_comm(string filecheck, string fileout, char symbol)
{
	bool ocomm = false;
	int pcomm = 0;
	ofstream secondfile(fileout);
	ifstream firstfile(filecheck);

	while (!firstfile.eof())
	{
		firstfile.get(symbol);

		if (symbol == '/') ocomm = true; //проверка на однострочный комментарий

		if (symbol == '{' && !ocomm) pcomm++;

		if (symbol == '\n' && ocomm)
		{
			secondfile << "\n";
			ocomm = false;
		}

		if (symbol == '}')
		{
			pcomm--;
			continue;
		}

		if (pcomm < 0)
		{
			cout << "Error";
			break;
		}
		if (!ocomm && pcomm == 0) secondfile << symbol;
	}
	firstfile.close();
	secondfile.close();
}

//#2
//аддресные комманды.
void adr_com(string fileout, char symbol, int mass[])
{
	ifstream file(fileout);
	string str = "", function;
	string variable = "";
	int count = 0;

	//создание пары токенов.
	typedef vector < pair <string, string> > VectorTokens;
	VectorTokens tkn;

	multimap <string, string> m1;
	map <char, int> m2;

	string arr_com[11] = { "in", "input", "out", "output", "print", "not", "minus", "add", "mult", "xor", "move"};

	//пока файл не закончится.
	while (!file.eof())
	{
		//записываем символ из файла в переменную symbol.
		file.get(symbol);

		//Реализация сравнения symbol'а с массивом Params.
		for (int i = 0; i < 27; i++)
		{
			
			//если символ в строке - английская буква, то записываем её в str.
			if (symbol == char(mass[i]))
			{
				str.push_back(symbol);
				if (symbol == ',')
				{
					variable.push_back(str[count]);
					count += 2;
				}
				break;
			}
			//если символ - пробел и длина str != 1, то symbol - функция,
			//заносим function значения str.
			else if (symbol == ' ' && str.length() > 1 && str.find(",") == string::npos)
			{
				function = str;
				str.clear();
				break;
			}
			//если символ - пробел и длина str = 1, то symbol - переменная,
			//заносим variable значения str.
			else if (symbol == ' ' && str.length() == 1 && str.find(",") == string::npos)
			{
				variable = str[0];
				str.clear();
				break;
			}
			//если символ - пробел и длина str = 1, то symbol - переменная,
			//заносим variable значения str.
			else if (symbol == '\n' && str.length() == 1 && str.find(",") == string::npos)
			{
				variable = str[0];
				str.clear();
				tkn.push_back(make_pair(function, variable));
				break;
			}
			else if (symbol == ' ' && str.find(",") != string::npos)
			{
				variable.push_back(str[count]);
				str.clear();
				count = 0;
				break;
			}
			else if (symbol == '\n' && str.find(",") != string::npos)
			{
				variable.push_back(str[count]);
				str.clear();
				tkn.push_back(make_pair(function, variable));
				variable.clear();
				count = 0;
				break;
			}
			else if (symbol == '\n' && str.length() > 1 && str.find(",") == string::npos)
			{
				function = str;
				str.clear();
				tkn.push_back(make_pair(function, variable));
				variable.clear();
				break;
			}
		}
	}
	for (vector<pair<string, string> >::iterator it = tkn.begin(); it != tkn.end(); it++) {
		//cout << "First: " << it->first << " Second: " << it->second << std::endl;
		for (int i = 0; i < 11; i++)
		{
			//Для одноадрессных команд
			if (it->first == arr_com[i])
			{
				m1.insert(make_pair(it->first, it->second));
				//занесение в multimap m2 переменную - значение
				
				if (it->second.length() < 3)
				{
					if (it->first == "in" || it->first == "input")
					{
						int x;
						cout << "Enter the value of the variable " << it->second << ": " << endl;
						cin >> x;
						m2.insert(make_pair(it->second[0], x));
					}
					else if (it->first == "out" || it->first == "output" || it->first == "print")
					{
						auto result = m2.find(it->second[0]);
						cout << "Output the value of the variable " << it->second << ": " << result->first << " " << result->second << endl;
					}
					else if (it->first == "minus" || it->first == "not")
					{
						auto result = m2.find(it->second[0]);
						string ch = it->second;
						m2.at(ch[0]) = 0 - result->second;
					}
					else if (it->first == "add")
					{
						auto result = m2.find(it->second[0]);
						auto result2 = m2.find(it->second[1]);
						string ch = it->second;
						m2.at(ch[0]) = result->second + result2->second;
					}
					else if (it->first == "mult")
					{
						auto result = m2.find(it->second[0]);
						auto result2 = m2.find(it->second[1]);
						string ch = it->second;
						m2.at(ch[0]) = result->second * result2->second;
					}
					else if (it->first == "xor")
					{
						auto result = m2.find(it->second[0]);
						auto result2 = m2.find(it->second[1]);
						string ch = it->second;
						m2.at(ch[0]) = result->second ^ result2->second;
					}
					else if (it->first == "move")
					{
						auto result2 = m2.find(it->second[1]);
						string ch = it->second;
						m2.at(ch[0]) = result2->second;
					}
				}
				else
				{
					if (it->first == "add")
					{
						auto result = m2.find(it->second[0]);
						auto result2 = m2.find(it->second[1]);
						string ch = it->second;
						m2.at(ch[2]) = result->second + result2->second;
					}
					else if (it->first == "mult")
					{
						auto result = m2.find(it->second[0]);
						auto result2 = m2.find(it->second[1]);
						string ch = it->second;
						m2.at(ch[2]) = result->second * result2->second;
					}
					else if (it->first == "xor")
					{
						auto result = m2.find(it->second[0]);
						auto result2 = m2.find(it->second[1]);
						string ch = it->second;
						m2.at(ch[2]) = result->second ^ result2->second;
					}
				}
			}
		}
	}
	
	
	for (auto& item : m1)
	{
		//проверка заполнения multimap m1 (команда - переменная)
		cout << item.first << " : " << item.second << endl; //вывод ключей и значений
	}	
	//проверка заполнения map m2 (переменная - значение)
	for (auto& item : m2)
	{
		cout << item.first << " : " << item.second << endl; //вывод ключей и значений
	}

	file.close();
}


int com_input(char variable)
{
	int x;
	cout << "Enter the value of the variable " << variable << ':' << endl;
	cin >> x;
	return x;
}

//
//int func()
//{
//	switch (switch_on)
//	{
//	default:
//		break;
//	}
//}

//void address_command(string fileout, char symbol)
//{
//	ifstream file(fileout);
//	vector<char> s;
//	string str, text, list[7] = {"in", "input", "out", "output", "print", "not", "minus"};
//
//	while (getline(file, str))
//	{
//		text = to_low(str);
//		for (int i = 0; i < text.length()+1; i++)
//		{
//			s.push_back(text[i]);
//			if (text[i] == ' ' || i == text.length())
//			{
//				s.pop_back();
//				if (s.size() == 1)
//				{
//					for (char elem : s)
//					{
//						cout << elem << " perem";
//					}
//					cout << "\n";
//					s.clear();
//				}
//				else
//				{
//					for (char elem : s)
//					{
//						cout << elem << " ";
//					}
//					cout << "\n";
//					s.clear();
//				}
//			}
//			else
//			{
//				continue;
//			}
//		}
//	}
//	file.close();
//
//}

int main()
{
	//массив с английскими буквами в ascii code.
	int Params[27] = { 44,97,98,99,100,101,102,103,104,105,106,107,108,109,110,111,112,113,114,115,116,117,118,119,120,121,122 };
	char ch = ' ';

	//search_comm("C:\\Test1.txt", "C:\\Test2.txt", ch);
	//address_command("C:\\Test2.txt", ch);
	adr_com("C:\\Test2.txt", ch, Params);


}
