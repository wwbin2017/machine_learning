#include<string>
#include "svm.h"
using namespace std;
int main()
{
    float C = 5.0;
    string kernel = "None";
    float precision = 0.000001;
    string file_name = "./data/test.txt";
    SVM svm_model(file_name, C, kernel, precision);
    svm_model.smo(100, precision);
    float feature[3]={0,1,0};
    svm_model.predict(feature);
    float feature1[3]={1,0,1};
    svm_model.predict(feature1);
    float feature2[3]={1,1,1};
    svm_model.predict(feature2);
    return 0;
}
