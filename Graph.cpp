//Pavan Kumar Mathamsetty U03273631
//Vyomesh Chowdary Tatineni U11237877
//Sri Sai Koushik Yaganti U65258662
//Karthik Yechuri U51358914


#include "Graph.hpp"
using namespace std;

// declaring unordered maps
unordered_map<std::string, Node *> adjlist;//adjacency list
unordered_map<std::string, unsigned long> m;

void Graph::addVertex(string label)
{ // adding the label as vertex to the graph
    Node *edges = new Node();
    edges = NULL;
    adjlist[label] = edges;
}

void Graph::removeVertex(string label)
{ // function for vertex removal in graph based on the input label
    unordered_map<string, Node *>::iterator x;
    for (x = adjlist.begin(); x != adjlist.end(); x++)
    {

        if (x->first == label)//checking if the vertex has no edges
            continue;

        removeEdge(x->first, label);//removing all the edges that are related to the vertex that is being removed by calling the fucntion remove edge recursively
    }
    adjlist.erase(label);//removing the label from the underordered map adjlist
}

void Graph::addEdge(string label1, string label2, unsigned long weight)
{  
    if (adjlist.find(label1) == adjlist.end() || adjlist.find(label2) == adjlist.end())//checking if the label1 and label2 are existing in the adjlist
    {
        return;
    }
    if (label1 == label2)//checking if the label1 and label2 are same
        return;
    Node *p = new Node();
    p->neigb = label2;
    p->weight = weight;
    p->next = adjlist[label1];
    adjlist[label1] = p;//creating the edge from label 1 to label 2
    Node *l = new Node();
    l->neigb = label1;
    l->weight = weight;
    l->next = adjlist[label2];
    adjlist[label2] = l;//creating the edge from label 2 to label 1 because its an undirected graph
}

void Graph::removeEdge(string label1, string label2)
{

    Node *p = adjlist[label1];
    if (p == NULL)//checking if the adjlist does not contain label 1
        return;

    while (p->next != NULL)
    {

        if (p->neigb == label2)//checking if label 1 and label 2 are neighbors
        {

            p = p->next;
            adjlist[label1] = p;

            break;
        }
        else if (p->next->neigb == label2)//checking if label 1 and label 2 are not immediate neighbors
        {
            Node *temp = p->next;

            p->next = temp->next;
            delete temp;

            break;
        }

        p = p->next;
    }
}

string minStr(unordered_map<string, unsigned long> shortest, unordered_map<string, bool> visited)
{//function to return the shortest distance between vertices
    unsigned long min = ULONG_MAX;
    string min_str;
    for (auto x : adjlist)
    {
        if (visited[x.first] == false && shortest[x.first] <= min)//using visited to check if the vertex has been visited already and at the same if the vertex weight is <= the minimum
        {
            min = shortest[x.first];//updating the minimum variable
            min_str = x.first;//assigning the key for x to output
        }
    }
    return min_str;
}

vector<string> emp(string startLabel, string endLabel, vector<string> &path, unordered_map<string, string> pathmap)//function to find the vector path
{
    if (startLabel == endLabel)//if both the labels are same return the path
    {
        return path;
    }

    string needed = pathmap[endLabel];
    path.push_back(needed);//adds vertices into path
    path = emp(startLabel, needed, path, pathmap);//recursive function to find the vector path
    return path;
}
unsigned long Graph::shortestPath(string startLabel, string endLabel, vector<string> &path)//function to find the shortest distance
{
    unordered_map<string, unsigned long> shortest;//shortest map saves the shortest distance between all vertices
    unordered_map<string, bool> visited;//visited map saves the bool value for the condition if we already visited the vertex or not
    unordered_map<string, string> pathmap;//pathmap saves all the previous vertices for the shortest path of each vertex

    for (auto x : adjlist)
    {
        if (startLabel == x.first)//initializes the shortest and visited maps with initial values
        {
            visited[x.first] = false;
            shortest[x.first] = 0;
            continue;
        }
        visited[x.first] = false;
        shortest[x.first] = ULONG_MAX;//initializes with infinity
    }

    for (unsigned int i = 0; i < adjlist.size() - 1; i++)
    {
        string min_str = minStr(shortest, visited);//gets the shortest distance between the required vertices
        visited[min_str] = true;
        Node *z = adjlist[min_str];
        while (z != NULL)
        {
            if (!visited[z->neigb] && shortest[min_str] != ULONG_MAX && ((shortest[min_str] + z->weight) < shortest[z->neigb]))
            {
                shortest[z->neigb] = shortest[min_str] + z->weight;//saves the shortest distance in the shortest map
                pathmap[z->neigb] = min_str;//always updates the previous vertex
            }
            z = z->next;//iteration
        }
    }
    path.push_back(endLabel);//adds the endlabel to the path map

    path = emp(startLabel, endLabel, path, pathmap);
    reverse(path.begin(), path.end());//reverses the vector because we are adding them from the other end

    return shortest[endLabel];
}
