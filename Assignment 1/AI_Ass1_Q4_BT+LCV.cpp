#include <iostream>
#include <vector>
#include <queue>
#include <time.h>
using namespace std;

int count = 0;

/*
structure to store the heap for the priority queue
to implement LCV
*/
struct node
{
     int n;
     int poss_roots;
	 vector<int> poss;
};

/*
Class to store the structure in the priority queue
*/

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

/*
Checks for the presence of the distance in the current distance vector
*/

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

/*
checks for consistenct of the golomb ruler with the new point n
*/
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

/*
print the solution of the golomb ruler
*/
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

/*
function returns priority queue of the possible nodes
sorted according to the number of possible children
in descending order
*/

priority_queue <node, vector<node>, Compare> create_queue (vector <int> base, vector <int> possibilities, int l, int m)
{
	 bool flag = false;
     priority_queue<node, vector<node>, Compare> pq;
	 vector <int>::iterator it1, it2;	//Iterators
	 if(base.size() == m-1)	//If this variable is last variable, then no point of doing LCV or forward checking
		flag = true;
	 for(it1 = possibilities.begin(); it1 < possibilities.end(); it1++)
     {
		 vector<int>poss_vec;
          if(checkconsistency(base, *it1)==true)// If this possibility is consistent
          {
			  if(flag == true)
			  {
				  node x = {*it1, 0, poss_vec};//If this is the last variable to be decided
				  pq.push(x);	//Push the possibility into the heap
			  }
			  else
			  {//Generate possible children
				   base.push_back(*it1);
				   int poss = 0;
				   for(it2 = possibilities.begin(); it2 < possibilities.end(); it2++)//Find consistency for each possibilities
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
				   pq.push(x);	//Push possibilities into the heap
				   base.pop_back();
			  }
          }
     }
     return pq;
}

/*
The recursive backtrack function which will calculate the marks on the golomb ruler
*/

bool recursivebacktrack(vector <int> base, vector <int> poss, int l, int m)
{
     bool solnfound = false;
     if(base.size()== m)    //If m marks are made, that means solution has been found
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
//    freopen("output.txt", "w+", stdout);
	char ch;
	do
	{
		bool bflag = false;
        clock_t start_time, end_time, t1;
		int l,m, time_taken;
		vector <int> base, possibilities;  //Base assignment vector
		do
		{    
		   cout<<"\nEnter the length of the Golomb ruler:";
		   cin>>l;
		   cout<<"Enter the number of parts the Golomb Ruler has to be split in:";
		   cin>>m;
		   if(l>1&&m>0&&m<=l)
		   {
              bflag = true;
           }
           else
             cout<<"Enter correct values for m and l.";
        }while(bflag == false);
		start_time = clock();
		base.push_back(0);
		base.push_back(l);
		::count = 0;      //To mantain count of the number of times check consistency has run
		for(int i = 1; i < l; i++)
		{
			possibilities.push_back(i);
		}
		if(false==recursivebacktrack(base, possibilities, l, m))
		{
			cout<<"\nNo solution was found for the problem";
		}
		end_time = clock();
		t1 = end_time - start_time;
		time_taken = ((double)end_time - (double)start_time)*1000/CLOCKS_PER_SEC;
		cout<<endl<<::count<<" times consistency check have been performed"
		    <<"\nin time "<<time_taken<<" microseconds";
		cout<<"\nDo you want to repeat?";
		cin>>ch;
	}while(ch == 'y');
	return 0;
}
