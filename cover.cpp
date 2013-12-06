#include <iostream>
#include <utility>
#include <vector>
#include <queue>
#include <algorithm>
#include <sstream>
#include <cmath>
#include <cstdlib>
#include <random>
#include <chrono>
using namespace std;

struct Base{
  int seq;
  int ref_count;
  int init_count;
  Base(int seq): seq(seq),ref_count(0),init_count(0){}
};

struct Antenna {
  int seq;
  int e;
  vector<Base *> base;
  bool in_sol;
  Antenna(int seq):seq(seq),in_sol(true),e(0){}
};

vector<Base *> B;
vector<Antenna *> A;

int get_val(const Antenna *a){
  int val1 = 0, val2=0;
  for(Base *b : a->base){
    if(b->ref_count == 1) val2--;
    else if(b->ref_count == 2) val1++;
  }

  return val2 + val1;
}

pair<int, int> get_val_pair(const Antenna *a){
  int val1 = 0, val2=0;
  for(Base *b : a->base){
    if(b->ref_count == 1) val2--;
    else if(b->ref_count == 2) val1++;
  }

  return pair<int, int>(val1, val2);
}

struct AntennaComp{
  bool operator() (const Antenna *a1, const Antenna *a2){
    int v1 = get_val(a1);
    int v2 = get_val(a2);
    
    if(v1 != v2) return v1 < v2;

    return a1->e < a2->e;
  }
};

int main(){
  int m, n;
  cin >> m >> n;

  srand (time(NULL));
  
  int iter_num = 0;

  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();

  vector<int> e(m, 0);

  for(int i = 0; i<m; i++){
    e[i] = i;
  }

  for(int i = 0; i<m; i++){
    Antenna *a = new Antenna(i);
    A.push_back(a);
  }

  for(int i = 0; i<n; i++){
    Base *b = new Base(i);
    B.push_back(b);
  }

  int max_bases = 0;

  for(int i = 0; i<m; i++){
    int num;
    cin >> num;
    
    if(num > max_bases) max_bases = num;

    Antenna *a = A[i];
    for(int j = 0; j < num; j++){
      int bi;
      cin>>bi;
      a->base.push_back(B[bi]);
      B[bi]->ref_count++;
    }
  }

  int upper = (m * max_bases) > n? (m * max_bases):n;

  iter_num = (int) (log(upper));
  
  int total_sum = 0;

  for(Base *b : B){
    b->init_count = b->ref_count;
    if(b->ref_count == 1) total_sum++;
  }

  int max_sum = 0;

  int C = 1;

  if(iter_num <= 10) C = 10;

  vector<int> max_sol_rm;

  //helper(q, cur_sum, max_sum, sol_rm, max_sol_rm);

  for(int i = 0; i < iter_num * iter_num * C; i++){
 
    double T = 50;
    int ei = A.size();
    int cur_sum = total_sum;
    vector<int> sol_rm;
    
    priority_queue<Antenna *, vector<Antenna *>, AntennaComp> q(A.begin(), A.end());

    while(!q.empty()){
      Antenna *a = q.top();

      a->e = ei--;

      int val = get_val(a);

      if(val < 0){
	double r = ((double) rand() / (RAND_MAX));

	double diff = a->base.size() + 1;

	double p = exp(((double)(val)) * diff / T);

	T *= 0.5;

	if(r > p){
	  q.pop();
	  continue;
	}
      }

      cur_sum += val;
      
      sol_rm.push_back(a->seq);
      
      if(cur_sum > max_sum){
	max_sum = cur_sum;
	max_sol_rm = sol_rm;
      }
      
      q.pop();
      
      for(Base *b : a->base){
	b->ref_count--;
      }
    }

    for(Base *b : B){
      b->ref_count = b->init_count;
    }
  }
    
  stringstream ss;
  
  ss<<m-max_sol_rm.size()<<" ";
  
  for(int v:max_sol_rm) A[v]->in_sol = false;
  
  for(Antenna *a: A){
    if(a->in_sol)
      ss<<a->seq<<" ";
  }
  
  ss<<endl;

  ss<<max_sum<<endl;

  cout<<ss.str();

  for(Antenna *a: A) delete a;
  for(Base *b: B) delete b;
}
  
 
