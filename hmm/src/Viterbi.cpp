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
				for(int i = 0; i< _n; i+=1) {
					std::vector<float> tmp_a(_n);
					float t;
					for(int j = 0; j < _n; j+=1) {
						infile >> t;
						tmp_a.push_back(t);
					}
					_a.push_back(tmp_a);
				}
				for(int i = 0; i< _n; i+=1) {
					std::vector<float> tmp_a(_m);
					float t;
					for(int j = 0; j < _m; j += 1) {
						infile >> t;
						tmp_a.push_back(t);
					}
					_b.push_back(tmp_a);
				}
				float t;
				for(int i = 0; i< _n; i+=1) {
					infile >> t;
					_p.push_back(t);
				}

    		}
    		infile.close();
			}

			void predict(std::vector<int> o, float &max_p, std::vector<int> & path) {
			int T = o.size();
			std::vector< std::vector<float> > delta(T);
			std::vector< std::vector<int> > psi(T);
			std::vector<float> delta_1(_n);
			std::vector<int> psi_1(_n);
			for(int i = 0; i < _n; i++) {
				delta_1.push_back(_p[i]*_b[i][o[0]]);
				psi_1.push_back(0);
			}
			delta.push_back(delta_1);
			psi.push_back(psi_1);
			for(int t=1; t < T; t+=1) {
				std::vector<float> delta_tmp(_n);
				std::vector<int> psi_tmp(_n);
				for(int i=0; i < _n; i+=1) {
					float max_delta = 0.0;
					int max_index_psi = 0;
					float max_psi = 0.0;
					for(int j = 0; j < _n; j+=1) {
						float tmp = delta[t-1][j]*_a[j][i];
						if (max_delta < tmp) {
							max_delta = tmp;
						}
						if (tmp > max_index_psi) {
							max_index_psi = j;
							max_psi = tmp;
						}
					}
					delta_tmp.push_back(max_delta*_b[i][o[t]]);
					psi_tmp.push_back(max_index_psi);
				}
				delta.push_back(delta_tmp);
			}
			max_p = 0.0;
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
