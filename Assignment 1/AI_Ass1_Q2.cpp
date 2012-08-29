/*
ASSIGNMENT 1 QUESTION 2
INFORMED SEARCH
****BFS SEARCH******
This is the solution to Q2 of Assigment 1 of AI
This program implements the solution to sliding-tile puzzle
There are 2 heuristics used for this
1. Using the number of white tiles to the right of black tiles
2. Second heuristic
   By careful analysis, i found that in the goal, the maximum distance between
   sum of pos of whites and blacks(B - W) will be 12 and minimum will be 9. 
   I used this a heuristic to see how far we are from the goal which is B-W to be made at least 9.
   However, hofn value is not allowed to be negative and is considered as zero if B-W is >9
*/

#include <iostream>
#include <vector>
#include <string>
#include <queue>

#define BLACK 'B'
#define WHITE 'W'
#define UNDERSCORE '_'

using namespace std;

/*
Datatype to store each set in the space
*/

struct Node
{
       string space;
       int HofN;
       int GofN;
};

/*
Function to store the priority queue
Takes fofn(H + G) as the key and uses hofn as tiebreaker
*/
class CompareHN
{
    public:
		bool operator()(Node& t1, Node& t2)
		{
			if((t2.HofN+t2.GofN)<(t1.HofN+t1.GofN)) 
			    return true;
			else if((t1.HofN+t1.GofN)==(t2.HofN+t2.GofN)) 
			{
				if(t2.HofN<t1.HofN)
				    return true;
			}
			else
				return false;
		}
};

/*
First Heurestic
Finds the hueristic of the current node
Calculates the number of whites to the right of black
*/
int FindHofN1(string node)   
{
    size_t  FirstBlack, FoundWhite;    //To keep the position of the blacs and white in the string
    int i = -1;
    FirstBlack = node.find_first_of(BLACK);
    FoundWhite = FirstBlack;
    do
    {
	    FoundWhite = node.find_first_of(WHITE, FoundWhite + 1);
        i++;
    }while (FoundWhite != string::npos);
    return i;
}
/*
Second heuristic
By careful analysis, i found that in the goal, the maximum distance between
sum of pos of whites and blacks(B - W) will be 12 and minimum will be 9. 
I used this a heuristic to see how far we are from the goal which is B-W to be made at least 9.
However, hofn value is not allowed to be negative and is considered as zero if B-W is >9
*/
int FindHofN2(string node)
{
    int Distance = 0;
    int pos = 0;
    int posW = 0;
    int posB = 0;
    
    pos = node.find_first_of(WHITE, 0);
    posW+= pos;
    pos = node.find_first_of(WHITE, pos+1);
    posW += pos;
    pos = node.find_first_of(WHITE, pos+1);
    posW += pos;
    pos = node.find_first_of(BLACK, 0);
    posB += pos;
    pos = node.find_first_of(BLACK, pos+1);
    posB += pos;
    pos = node.find_first_of(BLACK, pos+1);
    posB += pos;
    Distance = 9 - (posB - posW);
    if(Distance<=0)
         Distance = 0;
    return Distance;
}

/*
To compose the unique id of each string.
It is the sigma((asci value of char)*(position in string))
*/
int Uid(string str)
{
    int i = 0;
    int x = 0;
    while(str[i]!='\0')
    {
         x += str[i]*(i+1);
         i++;
    }
    return x;
}

/*
To test the uniqueness of each id
*/
bool Testuniqueness(int uid, vector <int> id)
{
     vector<int>::iterator it;
     for(it = id.begin() ; it < id.end(); it++ )
            if(uid == *it)
                   return false;
     return true;
}

/*
Generate successor of each node
It considers six cases
1. Moving the hole to the left
2. Moving the hole to the right
3. Jumping the second element from hole on the right to the hole
4. Jumping the second element from hole on the left to the hole
5. Jumping the third element from hole on the right to the hole
6. Jumping the third element from hole on the left to the hole 
All these cases are pushed to the queue and then the  one with the best fofn value is fetched
*/
bool GenSuc(priority_queue<Node, vector<Node>, CompareHN> &node, vector <int> &id, Node top, int heuristic)
{
    int _Pos;
    int gofn = top.GofN + 1;
    string str = top.space;
    _Pos = str.find_first_of(UNDERSCORE);
    {
         /*
         Generate first successor
         by shifting the underscore to the right
         */
         if(_Pos<6)
         {
              string SucStr = str;
              SucStr[_Pos] = SucStr[_Pos + 1];
              SucStr[_Pos + 1] = UNDERSCORE;
              if(Testuniqueness(Uid(SucStr), id))
              {
                   if(heuristic == 1)
                   {
                        Node SucNode = {SucStr, FindHofN1(SucStr), gofn};
                        node.push(SucNode);
                        id.push_back(Uid(SucStr));
                        cout<<"\nGenSuc: String pushed: "<<SucNode.space<<" with HofN:"<<SucNode.HofN<< " and GofN"<<SucNode.GofN;
                   }
                   if(heuristic == 2)
                   {
                        Node SucNode = {SucStr, FindHofN2(SucStr), gofn};
                        node.push(SucNode);
                        id.push_back(Uid(SucStr));
                        cout<<"\nGenSuc: String pushed: "<<SucNode.space<<" with HofN:"<<SucNode.HofN<< " and GofN"<<SucNode.GofN;
                   }
              }
         }
    }
    {
         if(_Pos>0)
         {
              /*
              Generate second successor
              by shifting the underscore to the left
              */
              string SucStr = str;
              SucStr[_Pos] = SucStr[_Pos - 1];
              SucStr[_Pos - 1] = UNDERSCORE;
              if(Testuniqueness(Uid(SucStr), id))
              {
                   if(heuristic == 1)
                   {
                        Node SucNode = {SucStr, FindHofN1(SucStr), gofn};
                        node.push(SucNode);
                        id.push_back(Uid(SucStr));
                        cout<<"\nGenSuc: String pushed: "<<SucNode.space<<" with HofN:"<<SucNode.HofN<< " and GofN"<<SucNode.GofN;
                   }
                   if(heuristic == 2)
                   {
                        Node SucNode = {SucStr, FindHofN2(SucStr), gofn};
                        node.push(SucNode);
                        id.push_back(Uid(SucStr));
                        cout<<"\nGenSuc: String pushed: "<<SucNode.space<<" with HofN:"<<SucNode.HofN<< " and GofN"<<SucNode.GofN;
                   }
              }
         }
    }
    {
         /*
         Generate third successor
         by jumping the right character over the underscore
         */
         if(_Pos<5)
         {
               string SucStr = str;
               SucStr[_Pos] = SucStr[_Pos + 2];
               SucStr[_Pos + 2] = UNDERSCORE;
               if(Testuniqueness(Uid(SucStr), id))
               {
			       if(heuristic == 1)
                   {
                        Node SucNode = {SucStr, FindHofN1(SucStr), gofn};
                        node.push(SucNode);
                        id.push_back(Uid(SucStr));
                        cout<<"\nGenSuc: String pushed: "<<SucNode.space<<" with HofN:"<<SucNode.HofN<< " and GofN"<<SucNode.GofN;
                   }
                   if(heuristic == 2)
                   {
                        Node SucNode = {SucStr, FindHofN2(SucStr), gofn};
                        node.push(SucNode);
                        id.push_back(Uid(SucStr));
                        cout<<"\nGenSuc: String pushed: "<<SucNode.space<<" with HofN:"<<SucNode.HofN<< " and GofN"<<SucNode.GofN;
                   }
               }
         }
    }
    {
         /*
         Generate fourth successor
         by jumping the left character over the underscore
         */
         if(_Pos>1)
         {
              string SucStr = str;
              SucStr[_Pos] = SucStr[_Pos - 2];
              SucStr[_Pos - 2] = UNDERSCORE;
              if(Testuniqueness(Uid(SucStr), id))
              {
			       if(heuristic == 1)
                   {
                        Node SucNode = {SucStr, FindHofN1(SucStr), gofn};
                        node.push(SucNode);
                        id.push_back(Uid(SucStr));
                        cout<<"\nGenSuc: String pushed: "<<SucNode.space<<" with HofN:"<<SucNode.HofN<< " and GofN"<<SucNode.GofN;
                   }
                   if(heuristic == 2)
                   {
                        Node SucNode = {SucStr, FindHofN2(SucStr), gofn};
                        node.push(SucNode);
                        id.push_back(Uid(SucStr));
                        cout<<"\nGenSuc: String pushed: "<<SucNode.space<<" with HofN:"<<SucNode.HofN<< " and GofN"<<SucNode.GofN;
                   }
              }
         }
    }
    {
         /*
         Generate fifth successor
         by jumping the second left character over the underscore
         */
         if(_Pos<4)
         {
              string SucStr = str;
              SucStr[_Pos] = SucStr[_Pos + 3];
              SucStr[_Pos + 3] = UNDERSCORE;
              if(Testuniqueness(Uid(SucStr), id))
              {
			       if(heuristic == 1)
                   {
                        Node SucNode = {SucStr, FindHofN1(SucStr), gofn};
                        node.push(SucNode);
                        id.push_back(Uid(SucStr));
                        cout<<"\nGenSuc: String pushed: "<<SucNode.space<<" with HofN:"<<SucNode.HofN<< " and GofN"<<SucNode.GofN;
                   }
                   if(heuristic == 2)
                   {
                        Node SucNode = {SucStr, FindHofN2(SucStr), gofn};
                        node.push(SucNode);
                        id.push_back(Uid(SucStr));
                        cout<<"\nGenSuc: String pushed: "<<SucNode.space<<" with HofN:"<<SucNode.HofN<< " and GofN"<<SucNode.GofN;
                   }
              }
         }
    }
    {
         /*
         Generate sixth successor
         by jumping the second right character over the underscore
         */
         if(_Pos>2)
         {
              string SucStr = str;
              SucStr[_Pos] = SucStr[_Pos - 3];
              SucStr[_Pos - 3] = UNDERSCORE;
              if(Testuniqueness(Uid(SucStr), id))
              {
			       if(heuristic == 1)
                   {
                        Node SucNode = {SucStr, FindHofN1(SucStr), gofn};
                        node.push(SucNode);
                        id.push_back(Uid(SucStr));
                        cout<<"\nGenSuc: String pushed: "<<SucNode.space<<" with HofN:"<<SucNode.HofN<< " and GofN"<<SucNode.GofN;
                   }
                   if(heuristic == 2)
                   {
                        Node SucNode = {SucStr, FindHofN2(SucStr), gofn};
                        node.push(SucNode);
                        id.push_back(Uid(SucStr));
                        cout<<"\nGenSuc: String pushed: "<<SucNode.space<<" with HofN:"<<SucNode.HofN<< " and GofN"<<SucNode.GofN;
                   }
              }
         }
    }
    return true;
}

int main()
{
        freopen ("output.txt","w",stdout);
        int ncount1 =0, ncount2 = 0;
        clock_t start_time, end_time, t1, t2;
        cout<<"\n\n\nWe will use 2 heurestics:";
        cout<<"\n1. Heuristic 1:"
            <<"\n\tUses the number of white tiles to the right of black tiles as h(n)"
            <<"\n2. Heuristic 2:"
            <<"\n\tIn the goal, the maximum distance between sum of position of "
            <<"\n\twhites and blacks(B - W) will be 12 and \n\tminimum will be 9."
            <<"\n\tThis is used to see how far we are from the goal \n\twhich is B-W to be made at least 9."
            <<"\n\tHowever, hofn value is not allowed to be negative \n\tand is considered as zero if B-W is >9"
            <<"\n\tHence heuristic becomes \n\tsum of black - sum of whites - 9.";
        cout<<"\nPress enter to continue";
        getchar();
        int size1, size2;
		    /*
		    Heurestic 1
		    */
            {
                start_time = clock();
                vector <int> UidStore;           //Storage for all the unique combinations of the sliding tiles
                vector <Node> set;      //Our vector to store the set
                priority_queue<Node, vector<Node>, CompareHN> pq; //Our priority queue to store the vector and then fetch one by one                int ncount = 0;
                string init = "BBB_WWW"; //initial state
                Node Start = {init, FindHofN1(init), 0}; //Generate the initial node
                pq.push(Start);     //Push this into priority queue
                cout<<"\n\n\n*********USING HEURISTIC 1 TO SOLVE THE PUZZLE*********";
                cout<<"\nMAIN: Initialising the start position\n"<<init<<" pushed\n";
                UidStore.push_back(Uid(init)); //Push the current tile into the unique id store
                while(!pq.empty())
                {
//                   getchar();
                   Node x = pq.top();     //Pick the node which has least f(n) = g(n) + h(n)
                   cout<<"\n\nMAIN: Loop "<<++ncount1<<endl;
                   cout<<"\nMAIN: Top of the node is "<<x.space<<" with HofN:"<<x.HofN<< " and GofN"<<x.GofN;
                   if(x.HofN<=0)
                   {
						/*
						If solution found
						*/
						 end_time = clock();
						 size1 = pq.size();
						 t1 = ((double)end_time - (double)start_time)*1000/CLOCKS_PER_SEC;
                         cout<<"\nMAIN: Solution Found in "<<ncount1<<"runs";
                         cout<<"\nSOULTION IS "<<x.space<<" with HofN:"<<x.HofN<< " and GofN"<<x.GofN;
                         cout<<"\nComputed in "<<t1<<" microseconds with "<<size1<<" memory";
                         cout<<"\nPress Enter to continue!!";
                         getchar();
                         break;
                   }
                cout<<"\nMAIN: Popping Node and generating successors";
                pq.pop();
                GenSuc(pq, UidStore, x, 1);	//Generate successors
                }
            }
			/*
			Second heuristic
			*/
            {
                start_time = clock();
                vector <int> UidStore;           //Storage for all the unique combinations of the sliding tiles
                vector <Node> set;      //Our vector to store the set
                priority_queue<Node, vector<Node>, CompareHN> pq; //Our priority queue to store the vector and then fetch one by one
                string init = "BBB_WWW"; //initial state
                Node Start = {init, FindHofN2(init), 0}; //Generate the initial node
                pq.push(Start);     //Push this into priority queue
                cout<<"\n\n\n*********USING HEURISTIC 2 TO SOLVE THE PUZZLE*********";
                cout<<"\nMAIN: Initialising the start position\n"<<init<<" pushed\n";
                UidStore.push_back(Uid(init)); //Push the current tile into the unique id store
                while(!pq.empty())
                {
                    Node x = pq.top();     //Pick the node which has least f(n) = g(n) + h(n)
                    size2 = pq.size();
                    end_time = clock();
                    t2 = ((double)end_time - (double)start_time)*1000/CLOCKS_PER_SEC;
                    cout<<"\n\nMAIN: Loop "<<++ncount2;
                    cout<<"\nMAIN: Top of the node is "<<x.space<<" with HofN:"<<x.HofN<< " and GofN"<<x.GofN;
                    if(x.HofN<=0)
                    {
                         cout<<"\nMAIN: Solution Found in "<<ncount2<<"runs";
                         cout<<"\nSOULTION IS "<<x.space<<" with HofN:"<<x.HofN<< " and GofN"<<x.GofN;
                         cout<<"\nComputed in "<<t2<<" microseconds with "<<size2<<" memory required";
                         cout<<"\nPress Enter to continue!!";
                         getchar();
                         break;
                    }
				    cout<<"\nMAIN: Popping Node and generating successors";
					pq.pop();
					GenSuc(pq, UidStore, x, 2);
                }
            }
            cout<<"\nHeurestic 1: \nMaximum Heap Size:"<<size1<<"\nTimetaken:"<<t1<<" microseconds\nNumber of loops"<<ncount1;
            cout<<"\n\nHeuristic 2: \nMaximum heap size:"<<size2<<"\nTimetaken:"<<t2<<"microseconds\nNumber of loops"<<ncount2;
            getchar();
    return 0;
}
