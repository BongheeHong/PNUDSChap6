// Adjacency Lists + BFS.


#include <iostream>
#include <string>

using namespace std;

template <class Type> class List;
template <class Type> class ListIterator;

template <class Type>
class ListNode {
	friend class List<Type>;
	friend class ListIterator<Type>;
private:
	Type data;
	ListNode *link;
	ListNode(Type);
};

template <class Type>
ListNode<Type>::ListNode(Type Default)
{
	data = Default;
	link = 0;
}

template <class Type>
class List {
	friend class ListIterator<Type>;
public:
	List() { first = 0; };
	void Insert(Type);
	void Delete(Type);
private:
	ListNode<Type> *first;
};

template <class Type>
void List<Type>::Insert(Type k)
{
	ListNode<Type> *newnode = new ListNode<Type>(k);
	newnode->link = first;
	first = newnode;
}

template <class Type>
void List<Type>::Delete(Type k)
{
	ListNode<Type> *previous = 0;
	for (ListNode<Type> *current = first; current && current->data != k;
		previous = current, current = current->link);
	if (current)
	{
		if (previous) previous->link = current->link;
		else first = first->link;
		delete current;
	}
}

template <class Type>
class ListIterator {
public:
	ListIterator(const List<Type>& l) : list(l) { current = l.first; }
	Type* First();
	Type* Next();
	bool NotNull();
	bool NextNotNull();
private:
	const List<Type> &list;
	ListNode<Type>* current;
};

template <class Type>
Type* ListIterator<Type>::First() {
	if (current) return &current->data;
	else return 0;
}

template <class Type>
Type* ListIterator<Type>::Next() {
	current = current->link;
	return &current->data;
}

template <class Type>
bool ListIterator<Type>::NotNull()
{
	if (current) return true;
	else return false;
}

template <class Type>
bool ListIterator<Type>::NextNotNull()
{
	if (current->link) return true;
	else return false;
}

//template <class Type>
ostream& operator<<(ostream& os, List<char>& l)
{
	ListIterator<char> li(l);
	if (!li.NotNull()) return os;
	os << *li.First() << endl;
	while (li.NextNotNull())
		os << *li.Next() << endl;
	return os;
}

class Queue;

class QueueNode {
	friend class Queue;
private:
	int data;
	QueueNode *link;
	QueueNode(int def = 0, QueueNode *l = 0)
	{
		data = def;
		link = l;
	};
};

class Queue {
private:
	QueueNode *front, *rear;
	void QueueEmpty() {};
public:
	Queue() { front = rear = 0; };
	void Insert(int);
	int* Delete(int&);
	bool IsEmpty() { if (front == 0) return true; else return false; };
};

void Queue::Insert(int y)
{
	if (front == 0) front = rear = new QueueNode(y, 0); // empty queue
	else rear = rear->link = new QueueNode(y, 0); // update \fIrear\fR
}

int* Queue::Delete(int& retvalue)
// delete the first node in queue and return a pointer to its data
{
	if (front == 0) { QueueEmpty(); return 0; };
	QueueNode *x = front;
	retvalue = front->data; // get data
	front = x->link;      // delete front node
	if (front == 0) rear = 0; // queue becomes empty after deletion
	delete x; // free the node
	return &retvalue;
}

class Graph
{
private:
	List<int> *HeadNodes;
	int n;
	bool *visited;

	void _DFS(const int v);
public:
	Graph(int vertices = 0) : n(vertices){
		HeadNodes = new List<int>[n];
	};
	void BFS(int);
	void InsertVertex(int startNode, int endNode);
	void Setup();

	void displayAdjacencyLists();

	
	void DFS(int v);
	
};

void Graph::displayAdjacencyLists() {
	for (int i = 0; i < n; i++) {
		//HeadNodes[i];
		ListIterator<int> iter(HeadNodes[i]);
		if (!iter.NotNull()) {
			cout << i << " -> null" << endl;
			continue;
		}
		cout << i;
		for (int *first = iter.First(); iter.NotNull(); first = iter.Next()) {
			cout << " -> " << (*first);
		}
		cout << endl;
	}
}

void Graph::InsertVertex(int start, int end) {
	if (start < 0 || start >= n || end < 0 || end >= n) {
		cout << "the start node number is out of bound.";
		throw "";
	}
	//check if already existed.
	ListIterator<int> iter(HeadNodes[start]);
	for (int *first = iter.First(); iter.NotNull(); first = iter.Next()) {
		if (*first == end) return;
	}

	HeadNodes[start].Insert(end);
	HeadNodes[end].Insert(start);
}

//void Graph::Setup(){
//	HeadNodes[0].Insert(2); HeadNodes[0].Insert(1);
//	HeadNodes[1].Insert(4); HeadNodes[1].Insert(3); HeadNodes[1].Insert(0);
//	HeadNodes[2].Insert(6); HeadNodes[2].Insert(5); HeadNodes[2].Insert(0);
//	HeadNodes[3].Insert(7); HeadNodes[3].Insert(1);
//	HeadNodes[4].Insert(7); HeadNodes[4].Insert(1);
//	HeadNodes[5].Insert(7); HeadNodes[5].Insert(2);
//	HeadNodes[6].Insert(7); HeadNodes[6].Insert(2);
//	HeadNodes[7].Insert(6); HeadNodes[7].Insert(5); HeadNodes[7].Insert(4);
//	HeadNodes[7].Insert(3);
//}

void Graph::BFS(int v)
{
	visited = new bool[n]; // @visited@ is declared as a @Boolean@\(** data member of @Graph@.
	for (int i = 0; i < n; i++) visited[i] = false; // initially, no vertices have been visited

	visited[v] = true; 
	cout << v << ",";
	Queue q;
	q.Insert(v);

	while (!q.IsEmpty()) {
		v = *q.Delete(v);
		ListIterator<int> li(HeadNodes[v]);
		if (!li.NotNull()) continue;
		int w = *li.First();
		while (1) {
			if (!visited[w]) { 
				q.Insert(w); 
				visited[w] = true; 
				cout << w << ","; 
			};
			if (li.NextNotNull()) 
				w = *li.Next();
			else break;
		}
	}
	delete[] visited;
}

// Driver
void Graph::DFS(int v)
{
	visited = new bool[n]; // @visited@ is declared as a @bool@\(** data member of @Graph@.
	for (int i = 0; i < n; i++)
		visited[i] = false; // initially, no vertices have been visited

	_DFS(v); // start search at vertex 0
	delete[] visited;

}

// Workhorse
void Graph::_DFS(const int v)
//  visit all previously unvisited vertices that are reachable from vertex v
{
	visited[v] = true;
	cout << v << ", ";
	ListIterator<int> li(HeadNodes[v]);
	if (!li.NotNull()) return;
	int w = *li.First();
	while (1) {
		if (!visited[w]) _DFS(w);
		if (li.NextNotNull()) w = *li.Next();
		else return;
	}
}

int main(void)
{

	int select = 0, n, startEdge = -1, endEdge = -1;
	int startBFSNode = 100;//the start node to BFS

	cout << "Input the total node number: ";
	cin >> n;
	Graph g(n);
	
	while (select != '0')
	{
		cout << "\nSelect command 1: Add edges, 2: Display Adjacency Lists, 3: BFS, 4: DFS, 5: Quit => ";
		cin >> select;
		switch (select){
		case 1:
			cout << "Add an edge: " << endl;
			cout << "--------Input start node: ";
			cin >> startEdge;
			
			cout << "--------Input  end  node: ";
			cin >> endEdge;
			if (startEdge < 0 || startEdge >= n || endEdge < 0 || endEdge >= n) {
				cout << "the input node is out of bound." << endl;
				break;
			}
			//get smallest start node.
			if (startEdge < startBFSNode)startBFSNode = startEdge;
			if (endEdge < startBFSNode) startBFSNode = endEdge;

			g.InsertVertex(startEdge, endEdge);
			break;

		case 2:
			//display
			g.displayAdjacencyLists();
			break;
			
		case 3:
			cout << "Start BFS from node: " << startBFSNode << endl;
			g.BFS(startBFSNode);
			break;
		case 4:
			cout << "Start DFS from node: " << startBFSNode << endl;
			g.DFS(startBFSNode);
			break;
		case 5:
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