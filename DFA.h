#define STATES  50
#define SYMBOLS 10

int num_symbols;
int num_states;
char final_states[STATES];
int DFAMat[STATES][SYMBOLS];
char StateName[STATES][STATES+1];
int new_states;
int Upd_DFA[STATES][SYMBOLS];
char NEW_finals[STATES+1];

void DisplayDFA_Matrix(int **, int ,int , char *);
void Next_State(char *, char *, int **, int );
char Class_Index(char , char **, int );
char is_one_nextstate(char *);
int state_index(char *, char **, int , int *, int);
int Divide_Class(char **, int , char *);
int Update_DFA(char **, int , int **, int , int **);
void append(char *, char );
void sort(char **, int );
int Split_Class(char **, int , int , int , int );
int combine_class(char **, int , int **, int , int );
void print_equiv_classes(char **, int );
int optimize_DFA(int **, int , int , char *, char **, int **);
int is_subset(char *, char *);
void get_NEW_finals(char *,char *, char **, int );
