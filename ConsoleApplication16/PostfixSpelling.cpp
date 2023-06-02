#include "PostfixSpelling.h"

void PosSpel(std::string infile, char symbol)
{
	std::ifstream file(infile);
	std::string out_str = "";
	std::stack <char> st;
	typedef std::map <char, int> Mymap;
	int priority;
	Mymap m1;
	m1.insert(Mymap::value_type('(', 0));
	m1.insert(Mymap::value_type(')', 1));
	m1.insert(Mymap::value_type('+', 2));
	m1.insert(Mymap::value_type('-', 2));
	m1.insert(Mymap::value_type('*', 3));
	m1.insert(Mymap::value_type('/', 3));

	while (!file.eof())
	{
		
		file.get(symbol);

		if (symbol <= '9' && symbol >= '1')
		{
			out_str.push_back(symbol);
		}
		else if (symbol == '(')
		{
			st.push(symbol);
		}
		else if (symbol == ')')
		{
			while (!st.empty())
			{
				if (st.top() != '(')
				{
					out_str.push_back(st.top());
				}
				st.pop();
			}
		}
		else if (symbol == '*'||symbol == '/'||symbol == '-'||symbol == '+')
		{
			for (auto& item : m1)
			{
				if (item.first == symbol)
				{
					if (st.empty())
					{
						st.push(symbol);
					}
					else
					{
						auto result = m1.find(st.top());
						while (item.second <= result->second)
						{
							out_str.push_back(st.top());
							st.pop();
							if (st.empty())
							{
								break;
								//st.push(symbol);
							}
							else
							{
								result = m1.find(st.top());
							}
						}
						st.push(symbol);
					}
					break;
				}
			}
		}
	}
	if (st.empty())
	{
		std::cout << out_str << std::endl;
	}
	/*else
	{
		for (int i = 0; i <= st.size(); i++)
		{
			out_str.push_back(st.top());
			st.pop();

		}
	}*/

	file.close();
	std::cout << out_str << std::endl;
}