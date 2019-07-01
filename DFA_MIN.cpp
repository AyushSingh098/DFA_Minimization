#include <stdio.h>
#include <string.h>
#include<stdio.h>
#include<string.h>
#include <iostream>
#include <bits/stdc++.h>
using namespace std;

void Next_State(char *nextstates, char *cur_states, int dfa[STATES][SYMBOLS], int symbol)
{
  int i, ch;
  for (i = 0; i < strlen(cur_states); i++)
  {
    *nextstates++ = dfa[cur_states[i]-'A'][symbol];
  }
  *nextstates = '\0';
}

char Class_Index(char ch, char stnt[][STATES+1], int n)
{
  int i;
    for (i = 0; i < n; i++)
        if (strchr(stnt[i], ch)) return i+'0';
    return -1;  /* next state is NOT defined */
}

char is_one_nextstate(char *s)
{
    /* first equiv. class */
    char equiv_class;
    while (*s == '@')
      s++;
    /* index of equiv. class */
    equiv_class = *s++;
    while (*s)
    {
        if (*s != '@' && *s != equiv_class) return 0;
        s++;
    }
    /* next state: char type */
    return equiv_class;
}

int state_index(char *state, char stnt[][STATES+1], int n, int *pn, int cur)
{
    /* 'cur' in argument is added only for 'printf()' */
    int i;
    /* next state info. */
    char state_flags[STATES+1];
    /* no next state */
    if (!*state)
      return -1;
    for (i = 0; i < strlen(state); i++)
    {
        state_flags[i] = Class_Index(state[i], stnt, n);
    }
    state_flags[i] = '\0';

    cout<<"   "<<cur<<":["<<stnt[cur]<<"]\t ["<<state<<"]  ("<<state_flags<<")\n";
    if (i=is_one_nextstate(state_flags))
    {
        /* deterministic next states */
        return i-'0';
    }
    else
    {
        /* state-division info */
        strcpy(stnt[*pn], state_flags);
        return (*pn)++;
    }
}

int Divide_Class(char statename[][STATES+1], int n, char *finals)
{
    int i, j;
    if (strlen(finals) == n)
    {
        /* all states are final states */
        strcpy(statename[0], finals);
        return 1;
    }
    /* final state group */
    strcpy(statename[1], finals);
    for (i=j=0; i < n; i++)
    {
        if (i == *finals-'A')
        {
            finals++;
        }
        else statename[0][j++] = i+'A';
    }
    statename[0][j] = '\0';
    return 2;
}

int Update_DFA(char stnt[][STATES+1], int n, int dfa[][SYMBOLS], int n_sym, int newdfa[][SYMBOLS])
{
    /* 'n' + <num. of state-division info> */
    int n2=n;
    int i, j;
    char nextstate[STATES+1];
    for (i = 0; i < n; i++)
    {
        /* for each pseudo-DFA state */
        for (j = 0; j < n_sym; j++)
        {
            /* for each input symbol */
            Next_State(nextstate, stnt[i], dfa, j);
            newdfa[i][j] = state_index(nextstate, stnt, n, &n2, i)+'A';
        }
    }
    return n2;
}

void append(char *s, char ch)
{
    int n=strlen(s);
    *(s+n) = ch;
    *(s+n+1) = '\0';
}

void sort(char stnt[][STATES+1], int n)
{
    int i, j;
    char temp[STATES+1];
    for (i = 0; i < n-1; i++)
    {
        for (j = i+1; j < n; j++)
        {
            if (stnt[i][0] > stnt[j][0])
            {
                strcpy(temp, stnt[i]);
                strcpy(stnt[i], stnt[j]);
                strcpy(stnt[j], temp);
            }
        }
    }
}

int Split_Class(char stnt[][STATES+1], int i1, int i2, int n,  int n_dfa)
{
    //ARGUMENTS
    /* int i1 --> index of 'i1'-th equiv. class */
    /* int i2 --> index of equiv. vector for 'i1'-th class */
    /* int n --> number of entries in 'stnt' */
    /* int n_dfa --> number of source DFA entries */
    char *old=stnt[i1], *vec=stnt[i2];
    int i, n2, flag=0;
    /* max. 'n' subclasses */
    char newstates[STATES][STATES+1];
    for (i=0; i < STATES; i++)
        newstates[i][0] = '\0';
    for (i=0; vec[i]; i++)
        append(newstates[vec[i]-'0'], old[i]);
    for (i=0, n2=n; i < n_dfa; i++)
    {
        if (newstates[i][0])
        {
            if (!flag)
            {
                /* stnt[i1] = s1 */
                strcpy(stnt[i1], newstates[i]);
                /* overwrite parent class */
                flag = 1;
            }
            else  /* newstate is appended in 'stnt' */
                strcpy(stnt[n2++], newstates[i]);
        }
    }
    /* sort equiv. classes */
    sort(stnt, n2);
    /* number of NEW states(equiv. classes) */
    return n2;
}

int combine_class(char stnt[][STATES+1], int n, int newdfa[][SYMBOLS], int n_sym, int n_dfa)
{
    int i, j, k;
    for (i = 0; i < n; i++)
    {
        for (j = 0; j < n_sym; j++)
        {
            /* index of equiv. vector */
            k = newdfa[i][j]-'A';
            /* equiv. class 'i' should be segmented */
            if (k >= n)
                return Split_Class(stnt, i, k, n, n_dfa);
        }
    }
    return n;
}

void print_equiv_classes(char stnt[][STATES+1], int n)
{
    int i;
    cout<<"\nEQUIV. CLASS ARE ---->";
    for (i = 0; i < n; i++)
        cout<<" "<<i<<":["<<stnt[i]<<"]";
    cout<<"\n";
}


int optimize_DFA(int dfa[][SYMBOLS], int n_dfa, int n_sym, char *finals, char stnt[][STATES+1],
    int newdfa[][SYMBOLS])
{
    //ARGUMENTS
    /* int dfa[][SYMBOLS] -->  DFA state-transition table */
    /* int n_dfa --> number of DFA states */
    /* int n_sym --> number of input symbols */
    /* char *finals --> final states of DFA */
    /* char stnt[][STATES+1] --> state name table */
    /* int newdfa[][SYMBOLS] --> reduced DFA table */

    char nextstate[STATES+1];
    /* number of new DFA states */
    int n;
    /* 'n' + <num. of state-dividing info> */
    int n2;
    n = Divide_Class(stnt, n_dfa, finals);
    while (1)
    {
        print_equiv_classes(stnt, n);
        n2 = Update_DFA(stnt, n, dfa, n_sym, newdfa);
        if (n != n2)
            n = combine_class(stnt, n, newdfa, n_sym, n_dfa);
        else break; /* equiv. class segmentation ended!!! */
    }
    /* number of DFA states */
    return n;
}

int is_subset(char *s, char *t)
{
    int i;
    for (i = 0; *t; i++)
        if (!strchr(s, *t++)) return 0;
    return 1;
}

void get_NEW_finals(char *newfinals,char *oldfinals,char stnt[][STATES+1],int n)
{
    //ARGUMENTS
    /* char *newfinals --> new DFA finals */
    /* char oldfinals --> source DFA finals */
    /* char stnt[][STATES+1] --> state name table */
    /* int n --> number of states in 'stnt' */
    int i;
    for (i = 0; i < n; i++)
        if (is_subset(oldfinals, stnt[i])) *newfinals++ = i+'A';
    *newfinals++ = '\0';
}

void DisplayDFA_Matrix(int tab[][SYMBOLS], int nstates,int nsymbols, char *finals)
{
    int i, j;
    cout<<"\nDFA: STATE TRANSITION MATRIX"<<endl;
    cout<<"     | ";
    for (i = 0; i < nsymbols; i++)
    {
      cout<<i<<"   ";
    }
    cout<<"\n-----+--";
    for (i = 0; i < nsymbols; i++)
    {
      cout<<"-----";
    }
    cout<<"\n";
    for (i = 0; i < nstates; i++)
    {
      //PRINT CURRENT STATE
      cout<<char('A'+i)<<"    | ";
      //PRINT NEXT STATE
      for (j = 0; j < nsymbols; j++)
      {
        cout<<char(tab[i][j])<<"   ";    /* next state */
      }
      cout<<"\n";
    }
    cout<<"FINAL STATES = "<<finals<<endl;
}
