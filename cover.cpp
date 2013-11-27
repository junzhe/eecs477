#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <sstream>
using namespace std;

struct Base{
  int seq;
  int ref_count;
  Base(int seq): seq(seq),ref_count(0){}
};

struct Antenna {
  int seq;
  vector<Base *> base;
  bool in_sol;
  Antenna(int seq):seq(seq),in_sol(true){}
};

vector<Base *> B;
vector<Antenna *> A;

int get_val(const Antenna *a){
  int val = 0;
  for(Base *b : a->base){
    if(b->ref_count == 1) val--;
    else if(b->ref_count == 2) val++;
  }

  return val;
}

struct AntennaComp{
  bool operator() (const Antenna *a1, const Antenna *a2){
    int v1 = get_val(a1);
    int v2 = get_val(a2);

    return v1 < v2;
  }
};

int main(){
  int m, n;
  cin >> m >> n;
  
  for(int i = 0; i<m; i++){
    Antenna *a = new Antenna(i);
    A.push_back(a);
  }

  for(int i = 0; i<n; i++){
    Base *b = new Base(i);
    B.push_back(b);
  }

  for(int i = 0; i<m; i++){
    int num;
    cin >> num;
    Antenna *a = A[i];
    for(int j = 0; j < num; j++){
      int bi;
      cin>>bi;
      a->base.push_back(B[bi]);
      B[bi]->ref_count++;
    }
  }
  
  int cur_sum = 0;

  for(Base *b : B){
    if(b->ref_count == 1) cur_sum++;
  }
  
  priority_queue<Antenna *, vector<Antenna *>, AntennaComp> q(A.begin(), A.end());

  int sol_size = m;

  while(!q.empty()){
    Antenna *a = q.top();

    int val = get_val(a);

    if(val < 0) break;

    cur_sum += val;

    q.pop();

    for(Base *b : a->base){
      b->ref_count--;
    }

    a->in_sol = false;

    sol_size--;
  }

  stringstream ss;

  ss<<sol_size<<" ";

  for(Antenna *a: A){
    if(a->in_sol)
      ss<<a->seq<<" ";
  }

  ss<<endl;

  ss<<cur_sum<<endl;

  cout<<ss.str();

  for(Antenna *a: A) delete a;
  for(Base *b: B) delete b;
}
  
 
