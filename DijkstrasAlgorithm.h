#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>

using namespace std;


struct Node                                                                     // Node structure for linked list of keys and distances for nodes of a graph
{
	char key;
	int distance;
	Node *next;
};
void AddNodeToFront(Node *&listPointer, char newKey, int distance);             // adds a new Node to the front of a linked list

struct GraphNode                                                                // a node of a graph: stores the node's key and a linked list of distances to other nodes
{
	char	key;
	Node	*listPointer;
};

class Graph                                                                     // class to create and store graphs as a vector of Graph Nodes
{
private:
	vector<GraphNode> graph;
	void DijkstraInitialise(int graphSize, int *distances, char *states, char *keys, int &currentIndex);
	void DijkstraCheckForDistanceUpdate(int graphSize, char *states, int currentIndex, char *keys, int currentDistance, int *distances);
	int DijkstraGetNextCurrentIndex(int graphSize, char *states, int *distances);
	bool DijkstraStillTemporaries(int graphSize, char *states);
	string DijkstraOutput(int graphSize, char *keys, int *distances);
public:
	Graph() { };
	~Graph() { };
	void AddNewGraphNode(char nodeKey);
	void NewEdgeBetweenGraphNodes(char fromNode, char toNode, int distance);
	string DijkstraOnNodeA();
};

int ErrorArguments(int arguments);                                              // returns error code if there are not enough arguments passed to the program
int ErrorFile(char *file);                                                      // returns error code if the file doesn't open
vector<string> &Split(const string &s, char delimiter, vector<string> &elements);   // used within Split function
vector<string> Split(const string &inputString, char delimiter);                // splits string by delimiter
void NewGraphNode(string line, Graph &graph);                                   // adds a new Graph Node to the graph using a string from a file
void NewEdge(string line, Graph &graph);                                        // adds a new edge Node to a Graph Node's linked list using a string from a file
int CreateGraph(char *file, Graph &graph);                                      // creates the graph from the input file
