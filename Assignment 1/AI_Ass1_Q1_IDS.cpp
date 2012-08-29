/*
ASSIGNMENT 1 QUESTION 1
UNINFORMED SEARCH
****IDS SEARCH******
This is the solution to Q1 of Assigment 1 of AI
This program implements the solution of the 4 gallon and 3 gallon jugs using IDS
I explore the possibilities from the empty set where both jugs are empty
and the do Iterative deeping search with cut-off 10 on it till a solution (2 in jug 4) is obtained.
*/
#include <iostream>
#include <stack>
#include <queue>
#include <vector>
#include <time.h>

#define CUT_OFF 10

using namespace std;

/*
Node to store the state of the jugs
*/
struct node
{
    int jug_4;
    int jug_3;
    int depth;
};

/*
Checks if the visited node is already present in the stack
Returns false if it is alredy prersent
Adds the node and returns true if it were not present
*/
bool AddVisited(node a, vector <node> & visited);

/*
We take the node and the queueu as the input and generate child on the following possibilities
1. We add water to the 4 gallon jug    Only possible if jug is not full
2. We add water to the 3 gallon jug    Only possible if jug is not full
3. We empty 4 gallon jug               Only possible if jug is not empty
4. We empty 3 gallon jug               Only possible if jug is not empty
5. We transfer contetnts from 4 gallon to 3 gallon jug  Only possible if jug 4 not empty and jug 3 is not full
6. We transfer contents from 3 gallon to 4 gallon jug   Only possible if jug 3 not empty and jug 4 is not full
We generate nodes among these, if possible, and then add it to our current stack
*/
int Generate_Children(node , stack <node> &, vector <node> &);

/*
To compare the nodes and return true if they are same, else false
*/
bool CompareNodes(int, int, node);

/*
Function to call IDS
*/

int IDS(stack <node> , int, vector <node>);

int main()
{
    int depth = 0;
    int memory = 0;
    clock_t start_time, end_time;
    start_time = clock();        //Initialise the clock
    cout<<"\n\t\tUsing IDS to find the solution.";
    do
    {
         stack <node> S;
         vector <node> Visited;
         node root = {0,0,0};  //Initialise the base node
         cout<<"\n\n\n----------------------------------------------";
         cout<<"\nBeginning IDS with depth = "<<depth;
         cout<<"\nInitialising the state space";
         cout<<"\nPushing the element: "<<root.jug_4<<" "<<root.jug_3;
         S.push(root);    //Push the root into the stack
         AddVisited(root, Visited);  //Add it to visited nodes
         if((memory = IDS(S, depth, Visited)) != -1)     //call the IDS with depth and the stack and visited nodes
         {
                 //If solution found
                 end_time = clock();          //end the clock
                 cout<<"\n********************************"
                     <<"\nMaximum memory used is "<<memory
                     <<"\nTime Taken is "<<((float)end_time-(float)start_time)*1000/CLOCKS_PER_SEC<<" microseconds."
                     <<"\n***********************************";
                 break;
         }
         else
         {
                 //solution not found
                 cout<<"\nIDS with depth = "<<depth<<" found no solution"
                     <<"\nIncreasing depth and starting iterations";
                 depth++;
                 continue;
         }
    }while(depth<CUT_OFF);
    getchar();
    return 0;
}

/*
Checks if the visited node is already present in the stack
Returns false if it is alredy prersent
Adds the node and returns true if it were not present
*/
bool AddVisited(node a, vector <node> & visited)
{
     vector <node> :: iterator it;
     bool isPresent = false;
     for( it = visited.begin(); it < visited.end(); it++)
     {
          if( true == CompareNodes(it->jug_4, it->jug_3, a))
          {
              isPresent = true;
              break;
          }
     }
     if(isPresent == false)
     {
          visited.push_back(a);
          return true;
     }
     else
          return false;
}

/*
We take the node and the queueu as the input and generate child on the following possibilities
1. We add water to the 4 gallon jug    Only possible if jug is not full
2. We add water to the 3 gallon jug    Only possible if jug is not full
3. We empty 4 gallon jug               Only possible if jug is not empty
4. We empty 3 gallon jug               Only possible if jug is not empty
5. We transfer contetnts from 4 gallon to 3 gallon jug  Only possible if jug 4 not empty and jug 3 is not full
6. We transfer contents from 3 gallon to 4 gallon jug   Only possible if jug 3 not empty and jug 4 is not full
We generate nodes among these, if possible, and then add it to our current stack
*/
int Generate_Children(node a, stack <node> &S, vector <node> &Visited)
{
    int i = 0;
    cout<<"\n\nProcessing node "<<a.jug_4<<" "<<a.jug_3<<" "<<a.depth;
    if(a.jug_4 < 4)
    {
         node b = a;
         b.depth++;
         b.jug_4 = 4;
         if(AddVisited(b, Visited) == true)
         {
              cout<<"\nFilling Jug 4-->Pushing element "<<b.jug_4<<" "<<b.jug_3<<" "<<b.depth<<" into the stack";
              S.push(b);
              i++;
         }
    }
    if(a.jug_3 < 3)
    {
         node b = a;
         b.depth++;
         b.jug_3 = 3;
         if(AddVisited(b, Visited) == true)
         {
              cout<<"\nFilling Jug 3-->Pushing element "<<b.jug_4<<" "<<b.jug_3<<" "<<b.depth<<" into the stack";
              S.push(b);
              i++;
         }
    }
    if(a.jug_4 > 0)
    {
         node b = a;
         b.depth++;
         b.jug_4 = 0;
         if(AddVisited(b, Visited) == true)
         {
              cout<<"\nEmptying Jug 4-->Pushing element "<<b.jug_4<<" "<<b.jug_3<<" "<<b.depth<<" into the stack";
              S.push(b);
              i++;
         }
    }
    if(a.jug_3 > 0)
    {
         node b = a;
         b.depth++;
         b.jug_3 = 0;
         if(AddVisited(b, Visited) == true)
         {
              cout<<"\nEmptying Jug 3-->Pushing element "<<b.jug_4<<" "<<b.jug_3<<" "<<b.depth<<" into the stack";
              S.push(b);
              i++;
         }
    }
    if(a.jug_4 > 0 && a.jug_3 < 3)
    {
         node b = a;
         b.depth++;
         b.jug_3 += b.jug_4;
         b.jug_4 = 0;
         if(b.jug_3 > 3)
         {
              b.jug_4 = b.jug_3 - 3;
              b.jug_3 = 3;
         }
         if(AddVisited(b, Visited) == true)
         {
              cout<<"\nJug 4 to Jug 3-->Pushing element "<<b.jug_4<<" "<<b.jug_3<<" "<<b.depth<<" into the stack";
              S.push(b);
              i++;
         }
    }
    if(a.jug_4 < 4 && a.jug_3 > 0)
    {
         node b = a;
         b.depth++;
         b.jug_4 += b.jug_3;
         b.jug_3 = 0;
         if(b.jug_4 > 4)
         {
              b.jug_3 = b.jug_4 - 4;
              b.jug_4 = 4;
         }
         if(AddVisited(b, Visited) == true)
         {
              cout<<"\nJug 3 to Jug 4-->Pushing element "<<b.jug_4<<" "<<b.jug_3<<" "<<b.depth<<" into the stack";
              S.push(b);
              i++;
         }
    }
    cout<<endl<<i<<" elements pushed into the stack";
    return i;
}
/*
To compare the nodes and return true if they are same, else false
*/
bool CompareNodes(int a4, int a3, node b)
{
     if(a4 == b.jug_4 && a3 == b.jug_3)
     {
          return true;
     }
     else
          return false;
}

/*

Function to call Iterative Deepening Search

*/

int IDS(stack <node> S, int depth, vector <node> Visited)
{
     int max_memory = 1;
     int memory = 1;
     static int iterations_total = 0;
     int iterations = 0;
     bool depth_reached = false;
     while(!S.empty())
     {
         iterations_total++;
         iterations++;
         node cur_node = S.top();
         if(cur_node.jug_4 == 2)
         {
               cout<<"\nTotal iterations so far :"<<iterations_total;
               cout<<"\nIterations in this IDS :"<<iterations;
               return max_memory;
               break;
         }
         S.pop();
         memory--;
         if(cur_node.depth < depth)
         {
              memory+=Generate_Children(cur_node, S, Visited);
         }
         max_memory = max_memory<memory?memory:max_memory;
     }
     cout<<"\nTotal iterations so far :"<<iterations_total;
     cout<<"\nIterations in this IDS :"<<iterations;
     return -1;
}
