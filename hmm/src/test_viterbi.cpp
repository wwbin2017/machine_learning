#include<string>
#include<vector>
#include "Viterbi.cpp"
using namespace std;
int main()
{
    string file_name = "../data/test";
    Viterbi viterbi_model(file_name);
	int  v1[3] = {0,1,0};
	vector<int> o(v1, &v1[2]);
	float max_p = 0.0;
	vector<int> path;
    viterbi_model.predict(o, max_p, path);
	cout << "概率为：" << max_p <<endl;
	cout << "预测值：" << " ";
	for(int i = 0; i < path.size(); i+=1) {
		cout <<path[i] << " ";
	}
	cout << endl;
    return 0;
}
