#include <iostream>

using namespace std;

int main (int argc, char *argv[])
{
	if (argc != 3) {
		cout << "Usage : input two numbers." << endl;
		return -1;
	}

	int operatorA = atoi(argv[1]);
	int operatorB = atoi(argv[2]);
	cout << "result :  " << operatorA + operatorB << endl;

	return 0;
}
