#include <unordered_map>
#include <iostream>
#include <typeindex>
#include <memory>
#include <string>

using namespace std;

struct A {
	virtual ~A() {}
};

struct B : A {};
struct C : A {};

void ppp(A* pInput)
{
	std::type_index tpid = std::type_index(typeid(*pInput));

	static std::type_index ta = std::type_index(typeid(A));
	static std::type_index tb = std::type_index(typeid(B));
	static std::type_index tc = std::type_index(typeid(C));

	if (tpid == ta)
	{
		cout << "ppp A" << endl;
	}

	if (tpid == tb)
	{
		cout << "ppp B" << endl;
	}
	if (tpid == tc)
	{
		cout << "ppp C" << endl;
	}

}

int main()
{
	std::unordered_map<std::type_index, std::string> type_names;

	type_names[std::type_index(typeid(int))] = "int";
	type_names[std::type_index(typeid(double))] = "double";
	type_names[std::type_index(typeid(A))] = "A";
	type_names[std::type_index(typeid(B))] = "B";
	type_names[std::type_index(typeid(C))] = "C";

	int i;
	double d;
	A a;

	std::unique_ptr<A> b(new B);
	std::unique_ptr<A> c(new C);

	A* pB = new B();
	A* pC = new C();
	B* pB1 = new B();
	C* pC1 = new C();

	std::cout << "i is " << type_names[std::type_index(typeid(i))] << '\n';
	std::cout << "d is " << type_names[std::type_index(typeid(d))] << '\n';
	std::cout << "a is " << type_names[std::type_index(typeid(a))] << '\n';
	std::cout << "b is " << type_names[std::type_index(typeid(*b))] << '\n';
	std::cout << "c is " << type_names[std::type_index(typeid(*c))] << '\n';

	ppp(pB);
	ppp(pC);
	ppp(pB1);
	ppp(pC1);

	delete pB;
	delete pC;
	delete pB1;
	delete pC1;
}

