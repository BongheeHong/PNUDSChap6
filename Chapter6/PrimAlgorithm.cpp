// 6.4 minimal spanning tree
// min heap, set 사용하여 MST 구현

#include <string>
#include <iostream>
#include "list"

using namespace std;

class WeightAndDestination {
public:
	int dest;
	int weight;

	WeightAndDestination() {}
	WeightAndDestination(int dest, int weight):dest(dest), weight(weight) {}
};

class Graph
{
private:
	list<WeightAndDestination*> *HeadNodes;//two-dimensional list
	int n;//total node number
	bool *visited = nullptr;

public:
	Graph(int vertices = 0) : n(vertices) {
		HeadNodes = new list<WeightAndDestination*>[n];
	};

	~Graph();
	void InsertVertex(int startNode, int endNode, int weight);
	void displayAdjacencyLists();

	Graph * spanningTree_PrimAlgorithm();
	bool getNearestEdge(int &start, int & destination, int& weight);

	//check whether all nodes connect to the same graph.
	bool checkCurrGraph();
};

/*void PrimMST(struct Graph* graph){
	TV = { 0 };
	For(T = 0; T contains fewer than n - 1 edges; add(u, v) to T){
		Let(u, v) be a least - cost edge such that u ∈≠TV and v ≠∈ TV;
		If(there is no such change) break;
		Add v to TV;
	}
	IF(T contains fewer than n - 1 edges) cout << ”no spanning tree” << 두이;
}*/
Graph * Graph::spanningTree_PrimAlgorithm() {
	if (!checkCurrGraph()) { return nullptr; }

	Graph * T = new Graph(this->n);
	int edgeSize = 0;

	visited = new bool[n]; //TV
	for (int i = 0; i < n; i++)
		visited[i] = false;

	int start = 0, destination = 0, weight = 0;
	visited[0] = 1;
	for (; edgeSize < n - 1; T->InsertVertex(start, destination, weight)) {
		if (!getNearestEdge(start, destination, weight))
			break;
		visited[destination] = 1;//Add v to TV;
		edgeSize++;
	}
	if (edgeSize < n - 1) {
		cout << "no spanning tree" << endl;
		return nullptr;
	}

	delete[]visited;
	visited = nullptr;

	return T;
}

bool Graph::getNearestEdge(int &start, int & destination, int& weight) {
	bool isHaveNearestEdge = false;//
	int minWeight = 9999999;//store the smallest weight
	for (int i = 0; i < n; i++) {//find U.
		if (visited[i] == true) {
			list<WeightAndDestination*> connectedNodes = this->HeadNodes[i];
			for (WeightAndDestination * edge : connectedNodes) {//find V.
				if (visited[edge->dest] == false) {
					if (minWeight > edge->weight) {
						minWeight = edge->weight;
						start = i;
						destination = edge->dest;
						weight = minWeight;

						isHaveNearestEdge = true;
					}
				}
			}
		}
	}
	return isHaveNearestEdge;
}

bool Graph::checkCurrGraph() {
	for (int i = 0; i < n; i++) {
		if (this->HeadNodes[i].size() == 0) {
			cout << "The node " << i << "does not connect to the graph." << endl;
			return false;
		}
	}
	return true;
}

void Graph::displayAdjacencyLists() {
	for (int i = 0; i < n; i++) {
		if (HeadNodes[i].size() == 0) {
			cout << i << " -> null" << endl;
			continue;
		}
		cout << i;
		for (WeightAndDestination* edge: HeadNodes[i]) {
			cout << " -> " << edge->dest << " (weight=" << edge->weight << ") ";
		}
		cout << endl;
	}
}

void Graph::InsertVertex(int start, int end, int weight) {
	if (start < 0 || start >= n || end < 0 || end >= n) {
		cout << "the start node number is out of bound.";
		throw "";
	}
	//check if already existed.
	for (WeightAndDestination* edge : this->HeadNodes[start]) {
		if (edge->dest == end) return;
	}

	HeadNodes[start].push_back(new WeightAndDestination(end, weight));
	HeadNodes[end].push_back(new WeightAndDestination(start, weight));
}

Graph::~Graph() {
	for (int i = 0; i < n; i++) {
		list<WeightAndDestination*> edgeList = this->HeadNodes[i];
		for (WeightAndDestination* edge : edgeList) {
			delete edge;
			edge = nullptr;
		}
	}
	delete [] this->HeadNodes;
}


int main(void)
{

	int select = 0, n, start = -1, end = -1, weight = -1;
	cout << "Input the total node number: ";
	cin >> n;
	Graph g(n);
	Graph *spanningTree = nullptr;

	while (select != '0')
	{
		cout << "\nSelect command 1: Add edges and Weight, 2: Display Adjacency Lists, 3: spanningTree, 4: Quit => ";
		cin >> select;
		switch (select) {
		case 1:
			cout << "Add an edge: " << endl;
			cout << "--------Input start node: ";
			cin >> start;
			cout << "--------Input  destination  node: ";
			cin >> end;
			if (start < 0 || start >= n || end < 0 || end >= n) {
				cout << "the input node is out of bound." << endl;
				break;
			}
			cout << "--------Input  weight: ";
			cin >> weight;

			g.InsertVertex(start, end, weight);
			break;
		case 2:
			//display
			g.displayAdjacencyLists();
			break;
		case 3:
			cout << "\nSpanningTree - Prim's algorithm: " << endl;
			spanningTree =  g.spanningTree_PrimAlgorithm();
			if (spanningTree) {
				spanningTree->displayAdjacencyLists();
			}
			delete spanningTree;
			spanningTree = nullptr;
			break;
		case 4:
			exit(0);
			break;
		default:
			cout << "WRONG INPUT  " << endl;
			cout << "Re-Enter" << endl;
			break;
		}
	}

	system("pause");
	return 0;
}