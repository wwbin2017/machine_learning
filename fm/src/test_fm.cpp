#include<string>
#include<vector>
#include "fm.cpp"
using namespace std;
int main()
{
    string file_name = "../data/test";
    FM fm;
	fm.Init(file_name, 100, 20, 10);
	fm.Fit(200);
	return 0;
}
