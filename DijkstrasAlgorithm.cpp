//////////////////////////////////////////////
//	Cai Gwatkin								//
//////////////////////////////////////////////
//	This application reads in a	graph text	//
//	file and finds the minimum cost from	//
//	each node to each other node using		//
//	Dijkstra's algorithm					//
//////////////////////////////////////////////

#include "DijkstrasAlgorithm.h"


int main(int argc, char **argv)
{
	Graph graph;

	// ensure correct number of arguments passed to program
	int error = ErrorArguments(argc);
	if (error) return error;

	// create the graph
	error = CreateGraph(argv[1], graph);
	if (error) return error;

	// find minimum cost
	cout << graph.DijkstraOnNodeA();

	return 0;
}

// adds a new Node to the front of a linked list
void AddNodeToFront(Node *&listPointer, char newKey, int distance)
{
	Node *temp = new Node;
	temp->key = newKey;
	temp->distance = distance;
	temp->next = listPointer;
	listPointer = temp;
}

// adds new Graph Node to the graph
void Graph::AddNewGraphNode(char nodeKey)
{
	GraphNode temp;
	temp.key = nodeKey;
	temp.listPointer = NULL;
	graph.push_back(temp);
}

// adds new Node to linked list of a Graph Node to store the edge distance between that Graph Node and another Graph Node
void Graph::NewEdgeBetweenGraphNodes(char fromNode, char toNode, int distance)
{
	// find Graph Node that the edge comes from
	int index, graphSize = graph.size();
	for (int i = 0; i < graphSize; i++)
	{
		if (graph[i].key == fromNode)
		{
			index = i;
			break;
		}
	}

	// add new node to linked list
	AddNodeToFront(graph[index].listPointer, toNode, distance);
}

// updates all edges with minimum costs
string Graph::DijkstraOnNodeA()
{
	// initialisations
	int graphSize = graph.size(), currentDistance = 0, currentIndex = 0;
	int *distances = new int[graphSize];
	char *states = new char[graphSize], *keys = new char[graphSize];
	DijkstraInitialise(graphSize, distances, states, keys, currentIndex);

	// while there are still temporary 
	bool stillTemporaries = true;
	while (stillTemporaries)
	{
		// update distances
		DijkstraCheckForDistanceUpdate(graphSize, states, currentIndex, keys, currentDistance, distances);

		// get next current index
		int nextCurrentIndex = DijkstraGetNextCurrentIndex(graphSize, states, distances);
		
		// get ready for next loop
		states[nextCurrentIndex] = 'p';
		currentIndex = nextCurrentIndex;
		currentDistance = distances[nextCurrentIndex];

		// check if there are still any temporary values in arrays
		stillTemporaries = DijkstraStillTemporaries(graphSize, states);
	}

	// create and return the output string
	return DijkstraOutput(graphSize, keys, distances);
}

// initialises arrays and current index for Dijkstra algorithm
void Graph::DijkstraInitialise(int graphSize, int *distances, char *states, char *keys, int &currentIndex)
{
	for (int i = 0; i < graphSize; i++)
	{
		// if Node A, initialise as permanent with distance 0 and make currentIndex
		if (graph[i].key == 'A')
		{
			distances[i] = 0;
			states[i] = 'p';
			keys[i] = graph[i].key;
			currentIndex == i;
		}
		// otherwise initialise with temporary state and infinite distance
		else
		{
			distances[i] = -1;
			states[i] = 't';
			keys[i] = graph[i].key;
		}
	}
}

// updates elements of array if there is a new minimum cost for that Graph Node
void Graph::DijkstraCheckForDistanceUpdate(int graphSize, char *states, int currentIndex, char *keys, int currentDistance, int *distances)
{
	// loop through arrays
	for (int i = 0; i < graphSize; i++)
	{
		// find temporary nodes
		if (states[i] == 't')
		{
			// go to node of linked list with key corresponding to current position in arrays
			Node *currentAdjacencyList = graph[currentIndex].listPointer;
			while (currentAdjacencyList != NULL)
			{
				if (currentAdjacencyList->key == keys[i]) break;
				else currentAdjacencyList = currentAdjacencyList->next;
			}
			if (currentAdjacencyList == NULL) continue;

			// update distance
			if (currentDistance + currentAdjacencyList->distance < distances[i] || distances[i] == -1)
			{
				distances[i] = distances[currentIndex] + currentAdjacencyList->distance;
			}
		}
	}
}

// gets the index for the next current index for Dijkstra
int Graph::DijkstraGetNextCurrentIndex(int graphSize, char *states, int *distances)
{
	int nextCurrentIndex = -1;

	// find next current index
	for (int i = 1; i < graphSize; i++)
	{
		if (states[i] == 't')
		{
			if (distances[i] == -1) continue;
			else if (distances[i] < distances[nextCurrentIndex] || nextCurrentIndex == -1) nextCurrentIndex = i;
		}
	}

	return nextCurrentIndex;
}

// returns true if there are still temporary variable in states
bool Graph::DijkstraStillTemporaries(int graphSize, char *states)
{
	for (int i = 0; i < graphSize; i++)
	{
		if (states[i] == 't') return true;
	}

	return false;
}

// returns output string from Dijkstra algorithm
string Graph::DijkstraOutput(int graphSize, char *keys, int *distances)
{
	string output = "";
	for (int i = 1; i < graphSize; i++)
	{
		output += "From ";
		output += graph[0].key;
		output += " to ";
		output += keys[i];
		output += ":";
		output += to_string(distances[i]);
		output += "\n";
	}

	return output;
}

// returns error code if there are not enough arguments passed to the program
int ErrorArguments(int arguments)
{
	if (arguments == 2) return 0;
	else
	{
		cout << "Error: incorrect number of arguments (" << arguments << " arguments passed, need 2)" << endl;
		return 1;
	}
}

// returns error code if the file doesn't open
int ErrorFile(char *file)
{
	cout << "Error: file " << file << " does not open" << endl;
	return 2;
}

// used within Split function
vector<string> &Split(const string &s, char delimiter, vector<string> &elements)
{
	stringstream ss(s);
	string item;
	while (getline(ss, item, delimiter)) elements.push_back(item);
	return elements;
}

// splits string by delimiter
vector<string> Split(const string &inputString, char delimiter)
{
	vector<string> elements;
	Split(inputString, delimiter, elements);
	return elements;
}

// adds a new Graph Node to the graph using a string from a file
void NewGraphNode(string line, Graph &graph)
{
	vector<string> splitLine = Split(line, ' ');
	graph.AddNewGraphNode(splitLine[1][0]);
}

// adds a new edge Node to a Graph Node's linked list using a string from a file
void NewEdge(string line, Graph &graph)
{
	vector<string> splitLine = Split(line, ' ');
	graph.NewEdgeBetweenGraphNodes(splitLine[0][0], splitLine[1][0], stoi(splitLine[2]));
	return;
}

// creates the graph from the input file
int CreateGraph(char *file, Graph &graph)
{
	// check that file opens correctly
	ifstream inFile(file);
	if (!inFile) return ErrorFile(file);

	// itterate through file until edge information
	string firstEdgeLine = "";
	while (!inFile.eof())
	{
		// get line from file
		string line;
		getline(inFile, line);

		// check starting character of line and take appropriate action
		if (line[0] == '#')
		{
			continue;
		}
		else if (line[0] == 'N' && line[1] == 'o')
		{
			NewGraphNode(line, graph);
		}
		else
		{
			firstEdgeLine = line;
			break;
		}
	}

	// initialise all edges and deal with first edge line from file
	NewEdge(firstEdgeLine, graph);

	// deal with edge information
	while (!inFile.eof())
	{
		// get line from file
		string line;
		getline(inFile, line);
		if (inFile.eof()) break;

		NewEdge(line, graph);
	}

	inFile.close();
	return 0;
}
