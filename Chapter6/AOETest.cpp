
#include <iostream>
#include <string>

using namespace std;

namespace topo {

	class Pair {
	public:
		int vertex, dur;
		Pair() {}

		Pair(int vertex, int dur):vertex(vertex), dur(dur) {}
	};

	enum Boolean { FALSE, TRUE };

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

	/*
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
	*/

	template <class Type>
	class ListIterator {
	public:
		ListIterator(const List<Type>& l) : list(l) { current = l.first; }
		Type* First();
		Type* Next();
		Boolean NotNull();
		Boolean NextNotNull();
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
	Boolean ListIterator<Type>::NotNull()
	{
		if (current) return TRUE;
		else return FALSE;
	}

	template <class Type>
	Boolean ListIterator<Type>::NextNotNull()
	{
		if (current->link) return TRUE;
		else return FALSE;
	}

	//template <class Type>
	ostream& operator<<(ostream& os, List<char>& l){
		ListIterator<char> li(l);
		if (!li.NotNull()) return os;
		os << *li.First() << endl;
		while (li.NextNotNull())
			os << *li.Next() << endl;
		return os;
	}

	class Graph{
	private:
		List<Pair> *HeadNodes; //adjacency Lists
		int *count; //incoming degree count
		int *ee; // earliest event occurence time
		int *le;//late event occurence time
		int n; //total event number, or node number
	public:
		Graph(const int vertices = 0) : n(vertices){
			HeadNodes = new List<Pair>[n];
			count = new int[n];
			ee = new int[n];
			le = new int[n];
			for (int i = 0; i < n; i++) {
				count[i] = 0;
				ee[i] = 0;
				le[i] = 0;
			}
		};

		~Graph() {
			delete[] count;
			delete[] ee;
			delete[] le;
			delete[] HeadNodes;
		}
		void Setup4();
		void EarlyActivity();
		void EarlyActivity_display(); 
		void LateActivity();
		void LateActivity_display();

		void display_early_late_criticalActivity();

		void TopologicalOrder();

		void addEdge(int start, int end, int duration) {
			if (start < 0 || end >= n) {
				cout << "the insert node is out of bound" << endl;
				return;
			}
			ListIterator<Pair> iter(HeadNodes[start]);//check duplication
			for (Pair *first = iter.First(); iter.NotNull(); first = iter.Next()) {
				if (first->vertex == end) {
					cout << "already exists" << endl;
					return;
				}
			}
			Pair pair(end, duration);
			HeadNodes[start].Insert(pair);
			count[end]++;
		}

		void printAdjacencyLists() {
			cout << "adjacency list: " << endl;
			for (int i = 0; i < n; i++) {
				List<Pair> list = HeadNodes[i];
				cout << "in-degree count=" << count[i] << ", " << i << " ";
				ListIterator<Pair> iter(list);

				if (!iter.NotNull()) {
					cout << "->null" << endl;
				}
				else {
					for (Pair *first = iter.First(); iter.NotNull(); first = iter.Next()) {
						cout << "->(vertex " << first->vertex << ",dur=" << first->dur << ")";
					}
					cout << endl;
				}
			}
			
		}

		Graph * inverseGraph() const{// the size is n
			Graph *newG = new Graph(n);;
			for (int i = 0; i < n; i++) {
				List<Pair> list = HeadNodes[i];
				ListIterator<Pair> iter(list);
				if (iter.NotNull()) {
					for (Pair *first = iter.First(); iter.NotNull(); first = iter.Next()) {
						newG->addEdge(first->vertex, i, first->dur);
					}
				}
			}
			return newG;
		}

	private:
		void displayArr(int * arr, int size) const{
			if (!arr) return;
			for (int i = 0; i < size; i++) {
				if (arr[i] < 10) cout << arr[i] << "  ";
				else cout << arr[i] << " ";
			}
			//cout << endl;
		}

		void displayStack(int top, int* countArr) const{
			cout << "stack=[ ";
			for (int i = 0; i < n; i++) {
				cout << top << ", ";
				top = countArr[top];
				if (top == -1) break;
			}
			cout << " ]" << endl;
		}

		List<Pair> * getAdjacencyLists() {
			return this->HeadNodes;
		}

		int * getCountArr() {
			return this->count;
		}
		
	};

	//void Graph::TopologicalOrder()
	//	// The n vertices of a network are listed in topological order
	//{
	//	int top = -1;
	//	for (int i = 0; i < n; i++)   // create a linked stack of verices with
	//		if (count[i] == 0) { count[i] = top; top = i; } // no predecessors
	//	for (int i = 0; i < n; i++)
	//		if (top == -1) { cout << " network has a cycle" << endl; return; }
	//		else {
	//			int j = top; top = count[top]; // unstack a vertex
	//			cout << j << endl;
	//			ListIterator<Pair> li(HeadNodes[j]);
	//			if (!li.NotNull()) continue;
	//			int k = *li.First();
	//			while (1) {
	//				count[k]--;
	//				if (count[k] == 0) { count[k] = top; top = k; }
	//				if (li.NextNotNull()) k = *li.Next();
	//				else break;
	//			}
	//		}
	//}


	/*void Graph::Setup4() {
		for (int i = 0; i < n; i++)
			ee[i] = 0;

		Pair p26, p34, p45, p51, p62, p79, p87, p84, p92, p94;
		p26.vertex = 1; p26.dur = 6;
		p34.vertex = 2; p34.dur = 4;
		p45.vertex = 3; p45.dur = 5;
		p51.vertex = 4; p51.dur = 1;
		p62.vertex = 5; p62.dur = 2;
		p79.vertex = 6; p79.dur = 9;
		p87.vertex = 7; p87.dur = 7;
		p84.vertex = 7; p84.dur = 4;
		p92.vertex = 8; p92.dur = 2;
		p94.vertex = 8; p94.dur = 4;

		HeadNodes[0].Insert(p45); HeadNodes[0].Insert(p34); HeadNodes[0].Insert(p26);
		count[0] = 0;

		HeadNodes[1].Insert(p51);
		count[1] = 1;

		HeadNodes[2].Insert(p51);
		count[2] = 1;

		HeadNodes[3].Insert(p62);
		count[3] = 1;

		HeadNodes[4].Insert(p87); HeadNodes[4].Insert(p79);
		count[4] = 2;

		HeadNodes[5].Insert(p84);
		count[5] = 1;

		HeadNodes[6].Insert(p92);
		count[6] = 1;

		HeadNodes[7].Insert(p94);
		count[7] = 2;

		count[8] = 2;

	}*/

	void Graph::Setup4() {
		
		addEdge(0, 3, 5);
		addEdge(0, 2, 4);
		addEdge(0, 1, 6);
		
		addEdge(1, 4, 1);
		
		addEdge(2, 4, 1);
	
		addEdge(3, 5, 2);
		addEdge(4, 7, 7);
		addEdge(4, 6, 9);
	
		addEdge(5, 7, 4);
		addEdge(6, 8, 2);
		addEdge(7, 8, 4);
	}

	void Graph::display_early_late_criticalActivity() {
		EarlyActivity();
		LateActivity();

		cout << endl << endl;
		for (int i = 0; i < n; i++) {
			List<Pair> list = HeadNodes[i];
			
			ListIterator<Pair> iter(list);
		
			/*if (count[i] == 0) {
				cout << "edge(" << i << "->" << i << "), early time=0, late time=0" << endl;
			}*/
			if (iter.NotNull()) {
				for (Pair *first = iter.First(); iter.NotNull(); first = iter.Next()) {
					int earlyTime = ee[i];
					int lateTime = le[first->vertex] - first->dur;
					int slack = lateTime - earlyTime;

					string critical;
					if (slack == 0) critical = "Yes";
					else critical = "No";

					cout << "edge(" << i << "->" 
						<< first->vertex << "), early time=" 
						<< earlyTime <<", late time=" 
						<<  lateTime 
						<< ", slack=" << slack
						<< ", critical=" << critical<< endl;
				}
			}
		}
	}

	void Graph::LateActivity_display() {
		for (int i = 0; i < n; i++) {
			ee[i] = 0;
		}

		EarlyActivity();

		Graph * inversedG = this->inverseGraph();
		List<Pair> * inversedHeadNodes = inversedG->getAdjacencyLists();
		int * inversedCount = inversedG->getCountArr();

		cout << endl;
		inversedG->printAdjacencyLists();//print

		//get and set max value in ee
		int max = -1;
		for (int i = 0; i < n; i++) {
			if (ee[n - 1] > max) max = ee[n - 1];
		}
		for (int i = 0; i < n; i++) {
			le[i] = max;
		}


		int top = -1;//store the first vertice with no predecessors
		for (int i = 0; i < n; i++) {   // create a linked stack of verices with no predecessors
			if (inversedCount[i] == 0) {
				inversedCount[i] = top;
				top = i;
			}
		}

		cout << endl << "Display le (late event occurence time) " << endl;
		cout << "initial   ";
		displayArr(le, n);
		displayStack(top, inversedCount);


		for (int i = 0; i < n; i++)
			if (top == -1) {
				cout << " network has a cycle" << endl;
				return;
			}
			else {
				int j = top;
				top = inversedCount[top]; // unstack a vertex

				cout << "output " << j << "; ";
				ListIterator<Pair> li(inversedHeadNodes[j]);
				if (!li.NotNull()) continue;
				Pair p = *li.First();
				while (1) {
					int k = p.vertex;
					if (le[k] > le[j] - p.dur)
						le[k] = le[j] - p.dur;
					inversedCount[k]--;
					if (inversedCount[k] == 0) {//update the vertice with no predecessors
						inversedCount[k] = top;
						top = k;
					}
					if (li.NextNotNull())
						p = *li.Next();
					else
						break;
				}

				displayArr(le, n);
				displayStack(top, inversedCount);

			}

		displayArr(le, n);

		delete inversedG;
	}

	// The n vertices of a network are listed in topological order
	void Graph::LateActivity() {
		for (int i = 0; i < n; i++) {
			ee[i] = 0;
		}

		EarlyActivity();

		Graph * inversedG = this->inverseGraph();
		List<Pair> * inversedHeadNodes = inversedG->getAdjacencyLists();
		int * inversedCount = inversedG->getCountArr();
		//get and set max value in ee
		int max = -1;
		for (int i = 0; i < n; i++) {
			if (ee[n - 1] > max) max = ee[n-1];
		}
		for (int i = 0; i < n; i++) {
			le[i] = max;
		}
		int top = -1;//store the first vertice with no predecessors
		for (int i = 0; i < n; i++) {   // create a linked stack of verices with no predecessors
			if (inversedCount[i] == 0) {
				inversedCount[i] = top;
				top = i;
			}
		}
		for (int i = 0; i < n; i++)
			if (top == -1) { 
				cout << " network has a cycle" << endl; 
				return; 
			}
			else {
				int j = top;
				top = inversedCount[top]; // unstack a vertex
				ListIterator<Pair> li(inversedHeadNodes[j]);
				if (!li.NotNull()) continue;
				Pair p = *li.First();
				while (1) {
					int k = p.vertex;
					if (le[k] > le[j] - p.dur)
						le[k] = le[j] - p.dur;
					inversedCount[k]--;
					if (inversedCount[k] == 0) {//update the vertice with no predecessors
						inversedCount[k] = top;
						top = k;
					}
					if (li.NextNotNull())
						p = *li.Next();
					else
						break;
				}
			}
		delete inversedG;
	}

	// The n vertices of a network are listed in topological order
	void Graph::EarlyActivity(){
		int * countArr = new int[n];
		for (int i = 0; i < n; i++) {
			countArr[i] = count[i];
		}

		int top = -1;//store the first vertice with no predecessors
		for (int i = 0; i < n; i++) {   // create a linked stack of verices with no predecessors
			if (countArr[i] == 0) {
				countArr[i] = top;
				top = i;
			}
		}
		for (int i = 0; i < n; i++)//initialize ee
			ee[i] = 0;

		for (int i = 0; i < n; i++)
			if (top == -1) { 
				cout << " network has a cycle" << endl;
				return; 
			}
			else {
				int j = top; 
				top = countArr[top]; // unstack a vertex

				//cout << j << endl;
				ListIterator<Pair> li(HeadNodes[j]);
				if (!li.NotNull()) continue;
				Pair p = *li.First();
				while (1) {
					int k = p.vertex;
					if (ee[k] < ee[j] + p.dur)
						ee[k] = ee[j] + p.dur;
					countArr[k]--;
					if (countArr[k] == 0) {//update the vertice with no predecessors
						countArr[k] = top;
						top = k;
					}
					if (li.NextNotNull())
						p = *li.Next();
					else 
						break;
				}
			}
		for (int l = 0; l < n; l++) {
			//cout << ee[l] << " ";
		}


		delete [] countArr;
	}


	// The n vertices of a network are listed in topological order
	void Graph::EarlyActivity_display() {
		int * countArr = new int[n];
		for (int i = 0; i < n; i++) {
			countArr[i] = count[i];
		}

		int top = -1;//store the first vertice with no predecessors
		for (int i = 0; i < n; i++) {   // create a linked stack of verices with no predecessors
			if (countArr[i] == 0) {
				countArr[i] = top;
				top = i;
			}
		}
		cout << "Display ee (early event occurence time) " << endl;
		cout << "initial   ";
		for (int i = 0; i < n; i++) ee[i] = 0; //initialize ee

		displayArr(ee, n);
		displayStack(top, countArr);

		for (int i = 0; i < n; i++){
			if (top == -1) { cout << " network has a cycle" << endl; return; }
			else {
				int j = top;
				top = countArr[top]; // unstack a vertex

				cout <<"output " << j << "; ";
				ListIterator<Pair> li(HeadNodes[j]);
				if (!li.NotNull()) continue;
				Pair p = *li.First();
				while (1) {
					int k = p.vertex;
					if (ee[k] < ee[j] + p.dur)
						ee[k] = ee[j] + p.dur;
					countArr[k]--;
					if (countArr[k] == 0) {//update the vertice with no predecessors
						countArr[k] = top;
						top = k;
					}
					if (li.NextNotNull())
						p = *li.Next();
					else
						break;
				}

				displayArr(ee, n);
				displayStack(top, countArr);
			}
		}
		displayArr(ee, n);
		delete []countArr;
	}

}
//void main()
//{
//	topo::Graph g(9);
//	g.Setup4();
//	g.printAdjacencyLists();
//	cout << endl;
//	g.EarlyActivity_display();
//	cout << endl;
//	//g.TopologicalOrder();
//
//	g.LateActivity_display();
//
//	g.display_early_late_criticalActivity();
//
//	system("pause");
//}



int main(void)
{
	topo::Graph *g = nullptr;
	int select = 0, n, start = -1, end = -1, weight = -1;
	topo::Graph * inversedG = nullptr;;
	cout << "1: custom setup, 2: default setup ";
	cin >> select;
	if (select == 1) {
		cout << "Input the total node number: ";
		cin >> n;
		g = new topo::Graph(n);
	}
	else if (select == 2) {
		g = new topo::Graph(9);
		g->Setup4();
		
	}

	while (select != '0')
	{
		cout << "\nSelect command 1: Add edge, 2: AdjacencyLists, 3: Inversed AdjacencyList"<< 
			"4: Early Activity, 5: Late Activity, 6:Critical activity 7. Quit => ";
		cin >> select;
		switch (select) {
		case 1:
			cout << "Add an edge: " << endl;
			cout << "--------Input start node: ";
			cin >> start;
			cout << "--------Input destination node: ";
			cin >> end;
			cout << "--------Input weight: ";
			cin >> weight;

			g->addEdge(start, end, weight);
			break;
		case 2:
			//display
			g->printAdjacencyLists();
			break;
		case 3: 
			inversedG = g->inverseGraph();
			inversedG->printAdjacencyLists();
			delete inversedG;
			break;
		case 4:
			g->EarlyActivity_display();
			break;
		case 5:
			g->LateActivity_display();
			break;
		case 6:
			g->display_early_late_criticalActivity();
			break;
		case 7:
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


