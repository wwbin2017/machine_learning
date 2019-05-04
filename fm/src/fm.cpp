#include<fstream>
#include<iomanip>
#include "fm.h"

using std::ifstream;

FM::FM() {}

FM::~FM() {}

void FM::Init(string file_name, int n_sample, int n, int k) {
	ifstream fin(file_name.c_str());
	if(!fin.is_open()) {
		std::cerr << "opep file failure" << std::endl;
	}
	
}
void FM::Fit(int epoch) {
	for(int ep = 0; ep < epoch; ep++) {
		for(int i = 0; i < data_.size(); i++) {
			float y_p = Predict(data_[i]);
			float resid = y_p - y_[i];
			w_[0] += resid;
		    for(int j = 1; j < w_.size(); j++) {
				w_[j] = resid*data_[i][j-1];
			}
			for(int iter_i = 0; iter_i < v_.size(); iter_i++) {
				for (int f = 0; f < v_[iter_i].size(); f++) {
					v_[iter_i][f] += resid*GradYToTheta(i, iter_i, f);
				}
			}
			CalcVDot();
		}
	
	}

}

float FM::Predict(vector<float>& sample) {
	float result = 0;
	for(int i=0; i < w_.size(); i++) {
		result += sample[i]*w_[i];
	}
	for(int i = 0; i < sample.size(); i++) {
		for(int j= i+1; j < sample.size(); j++) {
			result += v_dot_[i][j]*sample[i]*sample[j];
		}
	}
	return result;
}

void FM::CalcVDot() {
	for(int i = 0; i < v_.size(); i++) {
		for(int j = 0; j < v_.size(); j++) {
			float temp = 0;
			for(int f = 0; f < v_[i].size(); f++) {
				temp += v_[i][f]*v_[j][f];
			}
			v_dot_[i][j] = temp;
		}
	}
}
float FM::GradYToTheta(int sample, int i, int f) {
		float sum = 0;
		for (int iter_j; iter_j < v_.size(); iter_j++) {
			sum += v_[iter_j][f]*data_[sample][iter_j];
		}

		return data_[sample][i]*(sum - v_[i][f]*data_[sample][i]);
}

