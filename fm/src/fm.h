#include <iostream>
#include <vector>

# ifndef SRC_FM_
# define SRC_FM_

using std::vector;
using std::string;

class FM {
	public:
		FM();
		~FM();
		void Init(string file_name, int n_sample, int n, int k);
		void Fit(int epoch);
		float Predict(vector<float> &sample);
	private:
		float GradYToTheta(int sample, int i, int f);
		void CalcVDot();
	private:
		vector<vector<float> > data_;
		vector<float> y_;
		vector<float> w_;
		vector<vector<float> > v_;
		vector<vector<float> > v_dot_;
};

#endif
