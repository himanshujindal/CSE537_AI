/*
Implementing Golomb ruler with plain backtracking
*/
#include <iostream>
#include <vector>
#include <time.h>

using namespace std;

int count = 0;

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
The recursive backtrack function which will calculate the marks on the golomb ruler
*/
bool recursivebacktrack(vector <int> base, int i, int l, int m)
{
     bool solnfound = false;
     if(base.size()== m)    //If m marks are made, that means solution has been found
	 {
         printsoln(base);
         return true;
	 }
     do
     {
         if(checkconsistency(base, i)==true) //Check for consistency for the solution with the new value i
         {
              base.push_back(i);   //If consistent push i into the base assignment
              if(false == recursivebacktrack(base,i+1,l,m))    //Recursively do this for the next variable
			  {
				  base.pop_back();                             //If fails, then pop back i and try i + 1
			  }
			  else
			  {
                  solnfound = true;                            //If true, that means solnfound return true
				  break;
			  }
         }
		 i++;
     }while(i<l);
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
		vector <int> base;  //Base assignment vector
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
		base.push_back(0); //Initialize the vector since 0 and l will always be there in the golomb ruler
		base.push_back(l);
		::count = 0;      //To mantain count of the number of times check consistency has run
		if(false==recursivebacktrack(base, 0, l, m))
		{
			cout<<"No solution was found for the problem";
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
