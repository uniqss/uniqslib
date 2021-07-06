#include<bits/stdc++.h>
using namespace std;
int main()
{
	char ch; 
	ifstream in;
	in.open("old.jpg",ios::binary);    
	if(!in)
	{
		cout<<"Cannot open file.";
		return 1;
	}
	ofstream out;
	out.open("tmp.jpg",ios::binary);
	if(!out){
	
	cout<<"Cannot open file.";
	return 1;      
	}
	while(in)
	{
		in.get(ch);
		ch=ch^char(0xFF);    
		if(in)   out.put(ch);
	}
	

	in.close();
	out.close();
	return 0;
}

