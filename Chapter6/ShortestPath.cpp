#include <iostream>
#include <stdlib.h>
using namespace std;

const int nmax = 100;
const int MAX_WEIGHT = 9999999;

//6.4 shortest path
class Graph{
private:
	int length[nmax][nmax]; //connection matrix
	int a[nmax][nmax]; 
	int dist[nmax]; //distance from v to u.
	bool s[nmax]; //
	int newdist[nmax];

	int nodeSize;
public:
	void ShortestPath(const int, const int);
	void BellmanFord(const int, const int);
	void BellmanFord2(const int, const int);
	void AllLengths(const int);
	int choose(const int);
	void Out(int startNode, int n);
	void OutA(int);

	Graph(int nodeSize):nodeSize(nodeSize) {
		for (int i = 0; i < nmax; i++) {
			for (int k = 0; k < nmax; k++) {
				length[i][k] = MAX_WEIGHT;
			}
		}
	}
	
	void insertEdge(int start, int end, int weight);

	void displayConnectionMatrix();

	bool isNonNegativeEdgeCost() {
		for (int i = 0; i < nodeSize; i++) {
			for (int k = 0; k < nodeSize; k++) {
				if (length[i][k] < 0) return false;
			}
		}
		return true;
	}
};

void Graph::displayConnectionMatrix() {
	for (int i = 0; i < nodeSize; i++) {
		bool exists = false;
		for (int k = 0; k < nodeSize; k++) {
			if (length[i][k] == MAX_WEIGHT) continue;
			exists = true;
			cout << i << " -> " << k << "(weight=" << length[i][k] << "), ";
		}
		if (exists) cout << endl;
	}
}


void Graph::insertEdge(int start, int end, int weight) {
	if (start >= nodeSize || end >= nodeSize || start < 0 || end < 0) {
		cout << "the input node is out of bound, the biggest node is " << (nodeSize - 1) << endl;
		return;
	}

	length[start][end] = weight;
}

void Graph::Out(int startNode, int n) {
	//cout << endl;
	for (int i = 0; i < n; i++) {
		if (i == startNode) {
			cout << "0" << ", ";
		}
		else if (dist[i] == MAX_WEIGHT) 
			cout << "∞" << ", ";
		else 
			cout << dist[i] << ", ";
	}
		
	cout << endl;
}


void Graph::OutA(int n){
	//cout << endl;
	for (int i = 0; i < n; i++){
		cout << "startNode = " << i << ": ";
		for (int j = 0; j < n; j++) {
			if (i == j) {
				cout << "0" << "  ";
			}
			else if (a[i][j] > MAX_WEIGHT - 100000)//100000 is one a big enough number
				cout << "∞" << "  ";
			else cout << a[i][j] << "  ";
		}
		cout << endl;
	}
}


void Graph::ShortestPath(const int n, const int v)
{
	for (int i = 0; i < n; i++) {// initialize
		s[i] = false; 
		dist[i] = length[v][i]; 
	} 
	s[v] = true;
	dist[v] = 0;

	//Out(n);
	for (int i = 0; i < n - 2; i++) { // determine n-1 paths from vertex v
		Out(i, n);
		int u = choose(n);  // choose returns a value u:
							// dist[u] = minimum dist[w], where s[w] = false
		s[u] = true;
		for (int w = 0; w < n; w++)
			if (!s[w])
				if (dist[u] + length[u][w] < dist[w])
					dist[w] = dist[u] + length[u][w];
	} 

	Out(v, n);
}

int Graph::choose(const int n)
{
	int prevmax = -1; int index = -1;
	for (int i = 0; i < n; i++)
		if ((!s[i]) && ((prevmax == -1) || (dist[i] < prevmax)))
		{
			prevmax = dist[i]; index = i;
		}
	return index;
}

void Graph::BellmanFord(const int n, const int v){
	for (int i = 0; i < n; i++) 
		dist[i] = length[v][i];
	//
	cout << endl;
	int k = 0;
	for (k = 1; k <= n - 1; k++){
		cout << "k = " << k << ", ";
		Out(v, n);
		for (int u = 0; u < n; u++) {
			if (u != v) {
				for (int i = 0; i < n; i++) {
					if ((u != i) && (length[i][u] < MAX_WEIGHT)) {
						if (dist[u] > dist[i] + length[i][u]) {
							dist[u] = dist[i] + length[i][u];
						}
					}
				}
			}
		}
	}
	//cout << "k = " << k << ", ";
	//Out(n);
}

void Graph::BellmanFord2(const int n, const int v)
// Single source all destination shortest paths with negative edge lengths
{
	for (int i = 0; i < n; i++) 
		dist[i] = length[v][i]; // initialize dist
	for (int k = 2; k <= n - 1; k++){
		for (int l = 0; l < n; l++) 
			newdist[l] = dist[l];
		for (int u = 0; u < n; u++)
			if (u != v) {
				for (int i = 0; i < n; i++)
					if ((u != i) && (length[i][u] < MAX_WEIGHT))
						if (newdist[u] > dist[i] + length[i][u]) 
							newdist[u] = dist[i] + length[i][u];
			}
		for (int i = 0; i < n; i++)
			dist[i] = newdist[i];
	}
}

void Graph::AllLengths(const int n)
// length[n][n] is the adjacency matrix of a graph with n vertices.
// a[i][j] is the length of the shortest path between i and j
{
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			a[i][j] = length[i][j];   // copy length into a
	
	int k = 0;
	for (; k < n; k++) {  // for a path with highest vertex index k
		cout << "\n k = " << k << endl;
		OutA(n);
		for (int i = 0; i < n; i++)  // for all possible pairs of vertices
			for (int j = 0; j < n; j++)
				if ((a[i][k] + a[k][j]) < a[i][j]) 
					a[i][j] = a[i][k] + a[k][j];
	}
	cout << "\n k = " << k << endl;
	OutA(n);
}

Graph * defaultSetup1() {
	Graph *g = new Graph(7);

	g->insertEdge(0, 1, 6);
	g->insertEdge(0, 2, 5);
	g->insertEdge(0, 3, 5);
	g->insertEdge(1, 4, -1);
	g->insertEdge(2, 1, -2);
	g->insertEdge(2, 4, 1);
	g->insertEdge(3, 2, -2);
	g->insertEdge(3, 5, -1);
	g->insertEdge(4, 6, 3);
	g->insertEdge(5, 6, 3);

	return g;
}


Graph *defaultSetup2() {
	Graph *g = new Graph(8);

	g->insertEdge(1, 0, 300);
	g->insertEdge(2, 1, 800);
	g->insertEdge(2, 0, 1000);
	g->insertEdge(3, 2, 1200);
	g->insertEdge(4, 3, 1500);
	g->insertEdge(4, 5, 250);
	g->insertEdge(5, 3, 1000);
	g->insertEdge(5, 7, 1400);
	g->insertEdge(5, 6, 900);
	g->insertEdge(6, 7, 1000);
	g->insertEdge(7, 0, 1700);
	
	return g;
}

int main(void)
{
	Graph *g = nullptr;
	int select = 0, n, start = -1, end = -1, weight = -1;
	cout << "1: input total node number, 2: use setup1, 3: use setup2 ";
	cin >> select;
	if (select == 1) {
		cout << "Input the total node number: ";
		cin >> n;
		g = new Graph(n);
	}
	else if (select == 2) {
		g = defaultSetup1();
		n = 7;
	}
	else if (select == 3) {
		g = defaultSetup2();
		n = 8;
	}
	
	
	Graph *spanningTree = nullptr;

	while (select != '0')
	{
		cout << "\nSelect command 1: Add edges and Weight, 2: Display Adjacency Lists, 3: single source/all destinations: non-negative edge costs, "
			<< "4:  single source/all destinations: negative edge costs, 5. All-pairs shortest paths, 6. Quit => ";
		cin >> select;
		switch (select) {
		case 1:
			cout << "Add an edge: " << endl;
			cout << "--------Input start node: ";
			cin >> start;
			cout << "--------Input  destination  node: ";
			cin >> end;
			cout << "--------Input  weight: ";
			cin >> weight;

			g->insertEdge(start, end, weight);
			break;
		case 2:
			//display
			g->displayConnectionMatrix();
			break;
		case 3:
			cout << "\nsingle source/all destinations: non-negative edge costs: " << endl;
			if (!g->isNonNegativeEdgeCost()) {
				cout << "Negative edge cost exists!!" << endl;
				cout << "Please re-build the graph with non-negative edge costs." << endl;
				break;
			}

			cout << "\n ----------> Input start node: ";
			cin >> start;

			g->ShortestPath(n, start);
			break;
		case 4:
			cout << "\nsingle source/all destinations: negative edge costs: " << endl;
			cout << "\n ----------> Input start node: ";
			cin >> start;

			g->BellmanFord(n, start);
			break;
		case 5:
			cout << "\nAll-pairs shortest paths:" << endl;
			g->AllLengths(n);
			break;
		case 6: 
			exit(0);
		default:
			cout << "WRONG INPUT  " << endl;
			cout << "Re-Enter" << endl;
			break;
		}
	}
	delete g;
	return 0;
}

