#include<iostream>
#include<fstream>
#include<cassert>
#include<string>
using namespace std;

int load_file(const string& file_name, float*** sample, float** label, int& sample_num, int& fields_num)
{
    ifstream f_s(file_name.c_str());
    assert(f_s.is_open());
    f_s>>sample_num>>fields_num;
    
    *label = new float[sample_num];
    *sample = new float*[sample_num];
    for(int i=0;i<sample_num;i++)
    {
        (*sample)[i] = new float[fields_num];
    }
    for(int i=0;i<sample_num;i++)
    {
        f_s>>(*label)[i];
        
        for(int j=0;j<fields_num;j++)
        {
            f_s>>(*sample)[i][j];
        }
    }
    return 0;
}
