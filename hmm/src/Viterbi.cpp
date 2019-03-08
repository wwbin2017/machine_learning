#include <string>
#include <vector>
#include <fstream> 
#include <fiostream.h>
#include <iostream.h>
#include <stdlib.h>

class Viterbi{
	public:
		Viterbi(string fname)_fname(fname) {
			init();
		}

		void init() {
			ifstream infile;
    		infile.open(fname, ios::in);
    		if(!infile.is_open ()) {
        		cout << "Open file failure" << endl;
				exit(1);
			}
    		while (!infile.eof()) {
        		infile >> _n >> _m;
				_a.resize(_n);
				_b.resize(_m);
				for(int i < 0; i< _n; i+=1) {
					vector<float> tmp_a;
					float t;
					for(int j = 0; j < _m; j+=1) {
						infile >> t;
						tmpa.push(t);
					}
					_a.push(tmp_a)
				}
				for(int i < 0; i< _m; i+=1) {
					vector<float> tmp_a;
					float t;
					for(int j = 0; j < _n; j += 1) {
						infile >> t;
						tmpa.push(t);
					}
					_b.push(tmp_a)
				}
				float t;
				for(int i < 0; i< _m; i+=1) {
					infile >> t;
					_p.push(t);
				}

    		}
    		infile.close();
		}

	    vector<int> predict(vector<int> o) {
			int T = o.size();
						
		}

	private:
		string _fname;
		int _n;
		int _m;
		vector<vector<float>> _a;
	    vector<vector<float>> _b;
	    vector<float> _p;
};
