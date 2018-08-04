#include<string>
using namespace std;

class SVM
{
private:
  float* alpha;
  float* E; //E值的计算
  float** sample; //样本
  float* label;   //标签
  float C;
  float b;
  float theta;
  float precision;
  int sample_n; //样本总数
  int field_n;
  string kernel;
public:
  SVM();
  SVM(const string& file_name,float C, const string& kernel,float precision);
  void init(const string& file_name,float C, const string& kernel, float precision);
  ~SVM();
  void smo(int iter_num, float precision);
  void solve_min(int var_1, int var_2);
  void clop_alpha(int var_1, int var_2, float new_alpha_2);
  float compute_v(int index, int var_1, int var_2);
  void compute_E();
  int select_var2(int index);
  int min_E(int id);
  int max_E(int id);
  bool check_KKT(int& index_2);
  float compute_gxi(int index);
  float compute_kernel(int index_1, int index_2);
  float compute_kernel_feature(float* index_1, float* index_2);
  void save_model(const string& path);
  void load_model(const string& path);
  float predict(float* feature);
};
