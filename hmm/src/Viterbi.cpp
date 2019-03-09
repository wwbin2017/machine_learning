#include <iostream>
#include <string>
#include <vector>
#include <fstream> 

class Viterbi{
	public:
		Viterbi(std::string fname): _fname(fname) {
			init();
		}

		void init() {
			std::ifstream infile;
    		infile.open(_fname.c_str(), std::ios::in);
    		if(!infile.is_open ()) {
				std::cout << "Open file failure" << std::endl;
				exit(1);
			} else {
    		//while (!infile.eof()) {
        		infile >> _n >> _m;
				_a.resize(_n);
				_b.resize(_n);
				std::cout << _n << " " << _m << std::endl;
				for(int i = 0; i< _n; i+=1) {
					_a[i].resize(_n);
					for(int j = 0; j < _n; j+=1) {
						infile >> _a[i][j];
					}
				}
				for(int i = 0; i< _n; i+=1) {
					_b[i].resize(_m);
					for(int j = 0; j < _m; j += 1) {
						infile >> _b[i][j];
					}
				}
				_p.resize(_n);
				for(int i = 0; i< _n; i+=1) {
					infile >> _p[i];
				}
				/**
				for(int i = 0; i < _a.size();i++) {
					for(int j = 0; j < _a[i].size(); j+=1){
						std::cout << _a[i][j] << " ";
					}
					std::cout << std::endl;
				}
				for(int i = 0; i < _b.size();i++) {
					for(int j = 0; j < _b[i].size(); j+=1){
						std::cout << _b[i][j] << " ";
					}
					std::cout << std::endl;
				}
				for(int i = 0; i< _n; i++) {
					std::cout << _p[i] << " ";
				}
				std::cout << std::endl;
				**/
    		}
    		infile.close();
		}

		void predict(std::vector<int> o, float &max_p, std::vector<int> & path) {
			int T = o.size();
			std::vector< std::vector<float> > delta(T);
			std::vector< std::vector<int> > psi(T);
			for(int i = 0; i < T; i+=1){
				delta[i].resize(_n);
				psi[i].resize(_n);
			}
			for(int i = 0; i < _n; i++) {

				delta[0][i] = _p[i]*_b[i][o[0]];
				psi[0][i] = 0;
			}
			for(int t=1; t < T; t+=1) {
				for(int i=0; i < _n; i+=1) {
					float max_delta = -1;
					int max_index_psi = 0;
					for(int j = 0; j < _n; j+=1) {
						float tmp = delta[t-1][j]*_a[j][i];
						if (max_delta < tmp) {
							max_delta = tmp;
							max_index_psi = j;
						}
					}
					delta[t][i] = max_delta*_b[i][o[t]];
					psi[t][i] = max_index_psi;
				}
			}
			max_p = -1;
			int max_index = 0;
			for(int i = 0; i < _n; i+=1) {
				if (delta[T-1][i] > max_p) {
					max_p = delta[T-1][i];
					max_index = i;
				}
			}
			path.clear();
			path.resize(T);
			path[T-1] = max_index;
			for(int t = T-2 ; t >= 0 ; t-=1) {
				path[t] = psi[t+1][path[t+1]];
			}
		}

	private:
		std::string _fname;
		int _n;
		int _m;
		std::vector< std::vector<float> > _a;
		std::vector< std::vector<float> > _b;
		std::vector<float> _p;
};
