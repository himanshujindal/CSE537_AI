#include <iostream>
#include <vector>
#include <queue>

using namespace std;

int count = 0;

struct node
{
     int n;
     int poss_roots;
	 vector<int> poss;
};

class Compare 
{
public:
    bool operator()(node& a, node& b)
    {
		if(a.poss_roots<b.poss_roots)
            return true;
       else 
            return false;
    }
};

bool checkforpresence(int num, vector<int> distance)
{
	vector<int>::iterator it;
	for(it = distance.begin();it<distance.end();it++)
	{
		if(num==*it)
			return true;
	}
	return false;
}

bool checkconsistency(vector <int> ass, int n)
{
	(::count)++;
	vector <int> distance;
	int dist;
	ass.push_back(n);
	vector<int>::iterator it1, it2;
	for(it1 = ass.begin(); it1<ass.end();it1++)
	{
		for(it2 = it1+1; it2<ass.end();it2++)
		{
			dist = abs(*it1-*it2);
			if(checkforpresence(dist, distance) == true)
			{
				return false;
			}
			else
			{
				distance.push_back(dist);
			}
		}
	}
	return true;
}

void printsoln(vector <int> ass)
{
	int i = 1;
	cout<<"\n";
	vector<int>::iterator it;
	for(it = ass.begin(); it<ass.end();it++)
	{
		cout<<*it<<" ";
	}
}

priority_queue <node, vector<node>, Compare> create_queue (vector <int> base, vector <int> possibilities, int l, int m)
{
	 bool flag = false;
     priority_queue<node, vector<node>, Compare> pq, empty;
	 vector <int>::iterator it1, it2;
	 if(base.size() == m-1)
		flag = true;
	 for(it1 = possibilities.begin(); it1 < possibilities.end(); it1++)
     {
		  vector<int>poss_vec;
          if(checkconsistency(base, *it1)==true)
          {
			  if(flag == true)
			  {
				  node x = {*it1, m, poss_vec};
				  pq.push(x);
			  }
			  else
			  {
				   base.push_back(*it1);
				   int poss = 0;
				   for(it2 = possibilities.begin(); it2 < possibilities.end(); it2++)
				   {
						if(*it1!=*it2)
						{
								if(checkconsistency(base,*it2) == true)
								{
									poss++;
									poss_vec.push_back(*it2);
								}
						}
				   }
				   node x = {*it1, poss, poss_vec};
				   pq.push(x);
				   base.pop_back();
			  }
          }
     }
	 if(pq.size() < m - base.size())
	 {
		 return empty;
	 }
     return pq;
}

bool recursivebacktrack(vector <int> base, vector <int> poss, int l, int m)
{
     bool solnfound = false;
     if(base.size()== m)
	 {
         printsoln(base);
         return true;
	 }
     priority_queue<node, vector<node>, Compare> pq;
	 pq = create_queue(base, poss, l, m);
     while(!pq.empty())
     {
        node a = pq.top();
        pq.pop();
		base.push_back(a.n);
		if(true == recursivebacktrack(base, a.poss, l, m))
		{
			solnfound = true;
			break;
		}
		base.pop_back();
     }
     if(solnfound == true)
     {
          return true;
     }
     else
	     return false;
}

int main()
{
	char ch;
	do
	{
		::count = 0;
		int l,m;
		vector <int> base;
		vector<int> possibilities;
		cout<<"\nEnter the length of the Golomb ruler:";
		cin>>l;
		cout<<"\nEnter the number of parts the Golomb Ruler has to be split in:";
		cin>>m;
		base.push_back(0);
		base.push_back(l);
		for(int i = 1; i < l; i++)
		{
			possibilities.push_back(i);
		}
		if(false==recursivebacktrack(base, possibilities, l, m))
		{
			cout<<"\nNo solution was found for the problem";
		}
		cout<<"\n"<<::count<<" times consistency check has been performed";
		cout<<"\nDo you want to repeat?";
		cin>>ch;
	}while(ch == 'y');
	return 0;
}
