#include "DFA.h"
#include "DFA_MIN.cpp"
using namespace std;

int main()
{
  int i, j, k;
  cout<<"NUMBER OF STATES : ";
  cin>>num_states;
  cout<<"NUMBER OF SYMBOLS : ";
  cin>>num_symbols;
  cout<<"FINAL STATES : ";
  cin>>final_states;
  cout<<"DEFINE RELATIONS : "<<endl;
  char ch;
  for(int i= 0; i< num_states;i++)
  {
    for(int j = 0; j< num_symbols;j++)
    {
      cout<<"RELATION FOR Q("<<char('A'+i)<<") -> "<<j;
      cin>>ch;
      DFAMat[i][j]=ch;
    }
  }
  DisplayDFA_Matrix(DFAMat, num_states, num_symbols, final_states);
  new_states = optimize_DFA(DFAMat, num_states,num_symbols, final_states, StateName, Upd_DFA);
  get_NEW_finals(NEW_finals, final_states, StateName, new_states);
  DisplayDFA_Matrix(Upd_DFA, new_states, num_symbols, NEW_finals);
  return 0;
}
