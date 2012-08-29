/*
 Author: Himanshu Jindal
 Description: This file plays the game of tic tac toe using Artificial Intelligence. It was part of my assignment in AI.
    It is impossible to beat the comp on this
*/
#include <iostream>
#include <queue>
#include <vector>

using namespace std;

int ncount = 0;

/*Structure to store the gameboard*/
struct node
{
	char a[3][3];
};

/*Function Declarations*/

/*Finds if the node is the end of the game*/
int isNodeEnd (node);
/*Initialises the nodes*/
node init();
/*Generates possible children of the node*/
vector <node> gen_child(int a, node cur);
/*Plays the turn for X*/
node Xturn(node);
/*Plays the turn for X*/
node Yturn(node);
/*Calculates the utility for the node*/
int utility(node);
/*Displays the node*/
void print(node);
/*The minimax function which uses the AI part*/
int minimax(int, node, int, int);

int main()
{
	int x;
	node cur = init();
	do
	{
		print(cur);
		//getchar();
		cout<<"\nX's turn"<<endl;
		cur = Xturn(cur);
		print(cur);
		if((x = isNodeEnd(cur))!=2)
		{
			break;
		}
		else
		{
			cur = Yturn(cur);
			print(cur);
			if((x = isNodeEnd(cur))!=2)
			{
				break;
			}
		}
	}while(true);
	cout<<endl<<x<<"\nNodes generated "<<::ncount<<" times";
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
		x = minimax(-1, children.back(), -10, 10);
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
	int x, y;
    bool cond = false;
    do
    {
        cout<<"\nThe values of row number and column number should be between 0 and 2.";
        cout<<"\nYour turn: Enter x(The row number):";
        cin>>x;
        cout<<"\nYour turn: Enter y(The row number):";
        cin>>y;
        if(x>=0 && x<=2 && y>=0 && y<=2)
        {
            if(cur.a[x][y]!='0'||cur.a[x][y]!='x')
            {
                cur.a[x][y] = '0';
                cond = true;
            }
        }
    }while(cond == false);
	return cur;
#if 0
	int min_value = 10;
	int x;
	node min;
	vector <node> children;
	children = gen_child(-1, cur);
	while(!children.empty())
	{
		x = minimax(1, children.back(), -10, 10);
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
	//getchar();
	return min;
#endif
}

int minimax(int player, node cur, int alpha, int beta)
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
			x = minimax(-1, children.back(), alpha, beta);
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
			x = minimax(1, children.back(), alpha, beta);
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
