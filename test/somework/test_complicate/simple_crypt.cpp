#include "simple_crypt.h"

int simple_crypt(const string& file_in)
{
	char ch; 
	ifstream in;
	in.open(file_in.c_str(),ios::binary);    
	if(!in)
	{
		cout << "Cannot open file_in." << file_in << std::endl;
		return -1;
	}
	ofstream out;
	std::string file_out = file_in + ".crypt";
	out.open(file_out.c_str(),ios::binary);
	if(!out)
	{
		std::cout << "Cannot open output file." << file_out << std::endl;
		in.close();
		return -1;
	}
	while(in)
	{
		in.get(ch);
		ch=ch^char(0xFF);    
		if(in) 
		{
			out.put(ch);
		}
	}

	in.close();
	out.close();
	return 0;
}

#if 0
int main()
{
	std::string file_in = "old.jpg";
	simple_crypt(file_in);
	return 0;
}

#endif
