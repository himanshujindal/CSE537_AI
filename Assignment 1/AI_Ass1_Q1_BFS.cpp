/*
ASSIGNMENT 1 QUESTION 1
UNINFORMED SEARCH
****BFS SEARCH******
This is the solution to Q1 of Assigment 1 of AI
This program implements the solution of the 4 gallon and 3 gallon jugs using BFS
I explore the possibilities from the empty set where both jugs are empty
and the do BFS on it till a solution (2 in jug 4) is obtained.
*/

#include <iostream>
#include <queue>
#include <vector>
#include <time.h>

using namespace std;

/*
Node to store the state of the jugs
*/
struct node
{
    int jug_4;
    int jug_3;
};

/*
Checks if the visited node is already present in the queue
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
We generate nodes among these, if possible, and then add it to our current queue
*/
int Generate_Children(node , queue <node> &, vector <node> &);

/*
Compare two nodes return false if they are not equal
and true if they are equal
*/
bool CompareNodes(int, int, node);

int main()
{
    int iterations = 0; // To keep count of the number of times the iterations were run
    int memory = 0;  //To keep count of memory used
    int max_memory;  //Represents the maximum memory used
    clock_t start_time, end_time; //To keep track of time taken to run the BFS
    start_time = clock();
    queue <node> Q;      //A queue to store the nodes generated
    vector <node> Visited;   //To keep track of the visited nodes
    node root = {0,0};       //The starting node from where we will explore the space
    cout<<"\nUsing BFS to find the solution.";
    cout<<"\nInitialising the state space";
    cout<<"\nPushing the element: "<<root.jug_4<<" "<<root.jug_3;
    Q.push(root);  //Pushing the root into our queue and initialising
    AddVisited(root, Visited);
    memory++;
    while(!Q.empty())
    {
         iterations++;
         node cur_node = Q.front(); //Take the front from the queue
         cout<<"\nProcessing node "<<cur_node.jug_4<<" "<<cur_node.jug_3;   //Check if its the solution
         if(cur_node.jug_4 == 2)  //If we have found the solution
         {
              end_time = clock();
              cout<<"\n********************************"
                  <<"\nFound the solution in "<<iterations<<" iterations."
                  <<"\nMaximum memory used is "<<max_memory
                  <<"\nTime Taken is "<<((float)end_time-(float)start_time)*1000/CLOCKS_PER_SEC<<" microseconds."
                  <<"\n***********************************";
              break;
         }
         Q.pop();   //IF it is not the solution pop it and generate its children
         memory--;  //reduce memory since a node has been popped
         memory += Generate_Children(cur_node, Q, Visited);    //Increment memory by the number of nodes added    
         max_memory=max_memory<memory?memory:max_memory;       //If memory increased, store it as max_memory
    }
    getchar();
    return 0;
}

/*
Compare two nodes return false if they are not equal
and true if they are equal
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
Checks if the visited node is already present in the queue
Returns false if it is alredy prersent
Adds the node and returns true if it were not present
*/

bool AddVisited(node a, vector <node> & visited)
{
     vector <node> :: iterator it;
     bool isPresent = false;
     for( it = visited.begin(); it < visited.end(); it++)      //Check if the current element is present
     {
          if( true == CompareNodes(it->jug_4, it->jug_3, a))
          {
              isPresent = true;
              break;
          }
     }
     if(isPresent == false)    //If not present, push it into the visited nodes and return true
     {
          visited.push_back(a);
          return true;
     }
     else
          return false;        //If present, push it into the visited nodes and return false
}

/*
We take the node and the queueu as the input and generate child on the following possibilities
1. We add water to the 4 gallon jug    Only possible if jug is not full
2. We add water to the 3 gallon jug    Only possible if jug is not full
3. We empty 4 gallon jug               Only possible if jug is not empty
4. We empty 3 gallon jug               Only possible if jug is not empty
5. We transfer contetnts from 4 gallon to 3 gallon jug  Only possible if jug 4 not empty and jug 3 is not full
6. We transfer contents from 3 gallon to 4 gallon jug   Only possible if jug 3 not empty and jug 4 is not full
We generate nodes among these, if possible, and then add it to our current queue
*/

int Generate_Children(node a, queue <node> &Q, vector <node> &Visited)
{   
    int i = 0;
    if(a.jug_4 < 4)           //1. We add water to the 4 gallon jug    Only possible if jug is not full
    {
         node b = a;
         b.jug_4 = 4;
         if(AddVisited(b, Visited) == true)
         {
              cout<<"\nFilling Jug 4-->Pushing element "<<b.jug_4<<" "<<b.jug_3<<" into the queue";
              Q.push(b);
              i++;
         }
    }
    if(a.jug_3 < 3)            //2. We add water to the 3 gallon jug    Only possible if jug is not full
    {
         node b = a;
         b.jug_3 = 3;
         if(AddVisited(b, Visited) == true)
         {
              cout<<"\nFilling Jug 3-->Pushing element "<<b.jug_4<<" "<<b.jug_3<<" into the queue";
              Q.push(b);
              i++;
         }
    }
    if(a.jug_4 > 0)            //3. We empty 4 gallon jug               Only possible if jug is not empty
    {
         node b = a;
         b.jug_4 = 0;
         if(AddVisited(b, Visited) == true)
         {
              cout<<"\nEmptying Jug 4-->Pushing element "<<b.jug_4<<" "<<b.jug_3<<" into the queue";
              Q.push(b);
              i++;
         }
    }
    if(a.jug_3 > 0)             //4. We empty 3 gallon jug               Only possible if jug is not empty
    {
         node b = a;
         b.jug_3 = 0;
         if(AddVisited(b, Visited) == true)
         {
              cout<<"\nemptying Jug 3-->Pushing element "<<b.jug_4<<" "<<b.jug_3<<" into the queue";
              Q.push(b);
              i++;
         }
    }
    if(a.jug_4 > 0 && a.jug_3 < 3)              //5. We transfer contetnts from 4 gallon to 3 gallon jug  Only possible if jug 4 not empty and jug 3 is not full
    {
         node b = a;
         b.jug_3 += b.jug_4;
         b.jug_4 = 0;
         if(b.jug_3 > 3)
         {
              b.jug_4 = b.jug_3 - 3;
              b.jug_3 = 3;
         }
         if(AddVisited(b, Visited) == true)
         {
              cout<<"\nJug 4 to Jug 3-->Pushing element "<<b.jug_4<<" "<<b.jug_3<<" into the queue";
              Q.push(b);
              i++;
         }
    }
    if(a.jug_4 < 4 && a.jug_3 > 0)              //6. We transfer contents from 3 gallon to 4 gallon jug   Only possible if jug 3 not empty and jug 4 is not full
    {
         node b = a;
         b.jug_4 += b.jug_3;
         b.jug_3 = 0;
         if(b.jug_4 > 4)
         {
              b.jug_3 = b.jug_4 - 4;
              b.jug_4 = 4;
         }
         if(AddVisited(b, Visited) == true)
         {
              cout<<"\nJug 3 to Jug 4-->Pushing element "<<b.jug_4<<" "<<b.jug_3<<" into the queue";
              Q.push(b);
              i++;
         }
    }
    cout<<endl<<i<<" elements pushed into the queue"<<endl;
    return i;
}
