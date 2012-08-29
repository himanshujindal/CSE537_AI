#include <iostream>
#include <queue>
#include <vector>

#define PRUNING

using namespace std;

int ncount = 0;

struct node
{
	char a[3][3];
};

int isNodeEnd (node);
node init();
vector <node> gen_child(int a, node cur);
node Xturn(node);
node Yturn(node);
int utility(node);
void print(node);
int minimax(int, node);
int alphabeta(int, node, int, int);

int main()
{
    freopen("output.txt", "w+", stdout);
	int x;
	node cur = init();
	cout<<"\nStarting game of TIC-TAC-TOE";
	print(cur);
	do
	{
		cout<<"\nX's turn"<<endl;
		cur = Xturn(cur);
		print(cur);
		if((x = isNodeEnd(cur))!=2)
		{
			break;
		}
		else
		{
            cout<<"\nY's turn";
			cur = Yturn(cur);
			print(cur);
			if((x = isNodeEnd(cur))!=2)
			{
				break;
			}
		}
	}while(true);
	if(x==1)
	     cout<<"\nx won!!!";
	else if(x==-1)
	     cout<<"\n0 won!!!";
    else
         cout<<"\nIt was a draw";
	cout<<"\nNodes generated is :"<<ncount<<" times";
	getchar();
	getchar();
	return 0;
}

node Xturn(node cur)
{
	int max_value = -10;
	int x;
	node max;
	vector <node> children;
	children = gen_child(1, cur);
	while(!children.empty())
	{
#ifdef PRUNING
        x = alphabeta(-1, children.back(), -10, 10);
#else
		x = minimax(-1, children.back());
#endif
		cout<<"\n The below node has minimax value of "<<x;
		print(children.back());
		if(max_value<x)
		{
			max = children.back();
			max_value = x;
		}
		children.pop_back();
	}
	cout<<"\nReturning the node with maximum value "<<max_value;
	print(max);
	cout<<"\nReturning above node as next choice";
	//getchar();
	return max;
}

node Yturn(node cur)
{
#if 1
	int x, y;
	bool flag = false;
	do
	{
	cout<<"\nYour turn: Enter the row number(1,2,3):";
	cin>>x;
	x--;
	cout<<"\nYour turn: Enter column number(1,2,3):";
	cin>>y;
	y--;
	if(x<3 && y<3 && cur.a[x][y] == '_')
	{
		cur.a[x][y] = '0';
		flag = true;
    }
	else
	{
	    cout<<"\nEnter value of empty box.";
        flag = false;
    }   
    }while(flag == false);
	return cur;
#endif
#if 0
{
	int min_value = 10;
	int x;
	node min;
	vector <node> children;
	children = gen_child(-1, cur);
/*	cout<<"\nGenerated children for Y's turn";
	vector<node>::iterator it;
	for(it = children.begin(); it< children.end(); it++)
	{
		print(*it);
	}
	getchar();
*/
	while(!children.empty())
	{
#ifdef PRUNING
        x = alphabeta(1, children.back(), -10, 10);
#else
		x = minimax(1, children.back());
#endif
    	cout<<"\n The below node has minimax value"<<x;
		print(children.back());
		if(x<min_value)
		{
			min = children.back();
			min_value = x;
		}
		children.pop_back();
	}
	cout<<"\nReturning the node with minimum value "<<min_value;
	print(min);
	cout<<"\nReturning above node as next choice";
	getchar();
	return min;
}
#endif
}

int minimax(int player, node cur)
{
	int x;
	bool flag = false;
	if((x=isNodeEnd(cur)) !=2)
	{
//        cout<<"\nReached the end of the node with x = "<<x;
//        print(cur);
		return x;
	}
	vector<node> children;
	if(player == 1)
	{
//        cout<<"\nIn minimax fn, player is X. Generating children and calling minimax again.";
		int max = -10;
		children = gen_child(1, cur);
/*		cout<<"\nGenerated children for X's turn";
		getchar();
	    vector<node>::iterator it;
	    for(it = children.begin(); it< children.end(); it++)
	    {
		    print(*it);
	    }
*/  
		while(!children.empty())
		{
/*
            cout<<"\nFinding the maximum of all the children";
            cout<<"\nCalling minmax with the following child node";
            print(children.back());
            getchar();
*/
			x = minimax(-1, children.back());
			if(x>max)
			{
//                cout<<"\nthis node";
//                print(children.back());
//                cout<<"\nhas higher value than max = "<<max<<" which is "<<x;
				max = x;
//				getchar();
			}
			children.pop_back();
		}
		return max;
	}
	if(player == -1)
	{
		int min = 10;
//		cout<<"\nIn minimax fn, player is Y. Generating children and calling minimax again.";
		children = gen_child(-1, cur);
/*		cout<<"\nGenerated children for Y's turn";
		getchar();
	    vector<node>::iterator it;
	    for(it = children.begin(); it< children.end(); it++)
	    {
		    print(*it);
	    }
*/
		while(!children.empty())
		{
			x = minimax(1, children.back());
			if(x<min)
			{
//                cout<<"\nthis node";
//                print(children.back());
//                cout<<"\nhas lower value than min = "<<min<<" which is "<<x;
				min = x;
//				getchar();
			}
			children.pop_back();
		}
//		cout<<"\nReturning min = "<<min;
//		getchar();
		return min;
	}
	cout<<"\nERROR";
	//getchar();
	return x;
}

vector <node> gen_child(int a, node cur)
{
    ::ncount++;
	vector <node> children;
	for(int i = 0; i<3; i++)
	{
		for(int j = 0; j<3; j++)
		{
			if(cur.a[i][j]=='_')
			{
				if(a==1)
					cur.a[i][j] = 'x';
				else if(a==-1)
					cur.a[i][j] = '0';
				children.push_back(cur);
				cur.a[i][j] = '_';
			}
		}
	}
	return children;
}

node init()
{
	node a;
	for(int i = 0; i < 3; i++)
	{
		for(int j = 0; j < 3; j++)
		{
			a.a[i][j] = '_';
		}
	}
	return a;
}

int isNodeEnd (node cur)
{
	for(int i = 0; i < 3; i++)
	{
		if((cur.a[i][0]==cur.a[i][1]) && (cur.a[i][1]==cur.a[i][2]))
		{
			if(cur.a[i][0]=='x')
				return 1;
			else if(cur.a[i][0]=='0')
				return -1;
		}
		else if((cur.a[0][i]==cur.a[1][i])&&(cur.a[1][i]==cur.a[2][i]))
		{
			if(cur.a[0][i]=='x')
				return 1;
			else if(cur.a[0][i]=='0')
				return -1;
		}
	}
	if((cur.a[0][0] == cur.a[1][1]) && (cur.a[1][1]== cur.a[2][2]))
	{
		if(cur.a[0][0]=='x')
				return 1;
		else if(cur.a[0][0]=='0')
				return -1;
	}
	if((cur.a[0][2] == cur.a[1][1] )&& (cur.a[1][1] == cur.a[2][0]))
	{
		if(cur.a[0][2]=='x')
				return 1;
		else if(cur.a[0][2]=='0')
				return -1;
	}
	for(int i = 0; i < 3; i++)
	{
		for(int j = 0; j < 3; j++)
		{
			if(cur.a[i][j] == '_')
				return 2;
		}
	}
	return 0;
}

void print(node x)
{
    cout<<endl;
	for(int i =0; i<3; i++)
	{
		for(int j =0; j<3; j++)
		{
			cout<<x.a[i][j]<<" ";
		}
		cout<<endl;
	}
	cout<<endl;
}
int alphabeta(int player, node cur, int alpha, int beta)
{
	int x;
	bool flag = false;
	if((x=isNodeEnd(cur)) !=2)
	{
		return x;
	}
	vector<node> children;
	if(player == 1)
	{
		int max = -10;
		children = gen_child(1, cur);
		while(!children.empty())
		{
			x = alphabeta(-1, children.back(), alpha, beta);
			if(x>alpha)
			{
				alpha = x;;
			}
			children.pop_back();
			if(alpha>=beta)
			{
				return alpha;
			}
		}
		return alpha;
	}
	if(player == -1)
	{
		int min = 10;
		children = gen_child(-1, cur);
		while(!children.empty())
		{
			x = alphabeta(1, children.back(), alpha, beta);
			if(x<beta)
			{
				beta = x;
			}
			children.pop_back();
			if(alpha>=beta)
			{
				return beta;
			}	
		}
		return beta;
	}
	cout<<"\nERROR";
	return x;
}
