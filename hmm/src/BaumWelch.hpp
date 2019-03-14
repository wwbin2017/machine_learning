#include <iostream>
#include <string>
#include <vector>

using namespace std;
class BaumWelch {
	public:
		BaumWelch(int N, int M):_N{N}, _M(M) {
		
		}
		void init() {
			_A.resize(_N);
			_B.resize(_N);
			_Pi.resize(_N);
			for( int i = 0; i< _N; i+=1) {
				_A[i].resize(_N);
				_B[i].resize(_M);
				_Pi[i] = 1.0/_N;
				for(j = 0; j < _N; j+=1) {
					_A[i][j] = 1.0/_N;
				}
				for(j = 0; j < _M; j+=1) {
					_B[i][j] = 1.0/_M;
				}
			}
			// 初始化参数
		}
	    void forward(vector< vector<int> > &A, vector< vector<float> > &B, 
				vector<float> &Pi, vector<int> &observe, float &p, vector< vector<float> > &alpha) {
			int N = B.size();
			int M = B[0].size();
			int T = observe.size();
			alpha.resize(T);
			for(int i = 0; i < T; i+=1) {
				alpha[i].resize(N);
			}
			for(int i = 0; i < N; i+=1) {
				alpha[0][i] = Pi[i]*B[i][observe[0]];
			}
			for(int t = 0; t < T -1; t+=1) {
				for(int i = 0; i < N; i+=1) {
					tmp = 0;
					for(int j = 0; j < N; j+=1) {
						tmp += alpha[t][j]*A[j][i]
					}
					alpha[t+1] = tmp*B[i][observe[t+1]];
				}
			}
			p = 0;
			for(int i = 0; i < N; i+=1) {
				p += alpha[T-1][i]
			}
		}

	    void backward(vector< vector<int> > &A, vector< vector<float> > &B, 
				vector<float> &Pi, vector<int> &observe, float &p, vector< vector<float> > &belta) {
			int N = B.size();
			int M = B[0].size();
			int T = observe.size();
			belta.resize(T);
			for(int i = 0; i < T; i+=1) {
				belta[i].resize(N);
			}
			for(int i = 0; i < N; i+=1) {
				belta[T-1][i] = 1;
			}
			for(int t = T-2; t >= 0; t-=1) {
				for(int i = 0; i < N; i+=1) {
					belta[t][i] = 0;
					for(int j = 0; j < N; j+=1) {
						belta[t][i] += A[i][j]*B[j][observe[t+1]]*belta[t+1][j];
					}
				}
			}
			p = 0;
			for(int i = 0; i < N; i+=1) {
				p += Pi[i]*B[i][observe[0]]*belta[0][i]
			}
		}
		
		void compute_gamma(vector< vector<float> >&alpha, vector< vector<float> > &belta, float p, vector< vector<float> > &gamma) {
			int T = alha.size();
			int N = alpha[0].size();
			gamma.resize(T);
			for(int t = 0; t < T; t += 1) {
				gamma[t].resize(N);
			}
			for(int t = 0; t < T; t+=1) {
				for(int i = 0; i < N; i+=1) {
					gamma[t][i] = alpha[t][i]*belta[t][i]/p;
				}
			}
		}

		void compute_sigma(vector< vector<float> > &A, vector< vector<float> > B, vector< vector<float> >&alpha, 
				vector< vector<float> > &belta, vector<int> observe, float p, vector< vector< vector<float> > > &sigma) {
			int T = alha.size();
			int N = alpha[0].size();
			sigma.resize(T);
			for(int t = 0; t < T; t += 1) {
				sigma[t].resize(N);
				for(int i=0; i< N; i += 1) {
					sigma[t][i].resize(N);
				}
			}
			for(int t = 0; t < T; t+=1) {
				for(int i = 0; i < N; i+=1) {
					for(int j = 0; j < N; j+=1) {
						sigma[t][i][j] = alpha[t][i]*A[i][j]*B[j][observe[t+1]]*belta[t+1][j]/p;
					}
				}

			}
		}

		void train(vector<int> observe) {
			int T = observe.size();
			vector< vector<float> > alpha;
			vector< vector<float> > belta;
			float p;
			forward(_A, _B, _Pi, observe, p, alha);
			backward(_A, _B, _Pi, observe, p, belta);
			vector< vector<float> >gamma;
			gamma = compute_gamma(alpha, belta, p, gamma);
			vector< vector< vector<float> > > sigma;
			compute_sigma(_A, _B, alpha, belta, observe, p, sigma);
			for(int i = 0; i < _N; i+=1) {
				float tmp1 = 0;
				_Pi[i] = gamma[0][i];
				for(int j = 0; j < _N; j+=1) {
					tmp1 += gamma[t][i];
					float tmp2 = 0;
					for(int t = 0; t < observe.size() -1; t+=1) {
						tmp2 += sigma[t][i][j];
					}
					_A[i][j] = tmp2/tmp1;
				}
			}
			for(int j = 0; j < _N; j+=1) {
				for(int k = 0; k < _M; k+=1) {
					float tmp1 = 0;
					float tmp2 = 0;
					for(int t = 0; t < T; t += 1) {
						tmp1 += gamma[t][j];
						if (observe[t] == k)
							tmp2 += gamma[t][j];
					}
					_B[j][k] = tmp2/tmp1;
				}
			}
					
		}
	private:
		vector< vector<float> > _A;
		vector< vector<float> > _B;
		vector< float > _Pi;
		int _N;
		int _M;
};
