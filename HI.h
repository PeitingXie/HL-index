#include "graph.h"


struct Pair // pair in HL-Index
{
	int hID;
	int overlap;
	Pair(int _hID, int _overlap): hID(_hID), overlap(_overlap) {};
	bool operator< (const Pair &x) const
	{
		return hID < x.hID;
	}
};


struct QPair // triplet in TILL-Index
{
	int vID;
	int overlap;
	QPair(int _vID, int _overlap): vID(_vID), overlap(_overlap) {};
	// bool operator< (const QPair &x) const
	// {
	// 	return overlap > x.overlap;
	// }
};




struct Pair_in_queue {
	int hID;
	int overlap;
	int cover;
	Pair_in_queue(int _hID, int _overlap, int _cover): hID(_hID), overlap(_overlap), cover(_cover) {};
	bool operator< (const Pair_in_queue &x) const
	{
		return overlap < x.overlap;
	}
};


class SL
{
private:
	
	// the number of vertices and the number of edges
	int max_v, max_e; // set them in "graph.cpp"
	int n, m;

	bool directed; // directed graph or undirected graph

	// graph and reversed graph
	
	vector<Pair> *label, *tmpLabel, *ete_label;
	




	int *idx; // order --> ID
	int *order; // ID --> order
	priority_queue<Pair_in_queue> Q;
	vector<int> *E;
	HyperEdge *graph_edge;
	map<int, int> *vertex_map;
	long long neighbour[10000000];
	int *visited;
	int *visited_h;

	int *global_visited_h;


	int *temp, *rank, *penalty, *idx2;

	vector<int> *tmpE;

	long long total_memory;

	int total_count;
	
	vector<pair<int, int>> *nbr;
	vector<pair<int, int>> *allNbr;

	// TILL-Index


	map<int, int> * D;
	vector<QPair> * QPairs; 
	int * C;
	// set<int> * inverted;

	set<int> * W;

	// int *visited;
	// int *inVisited;
	// int *outVisited;
	// int *biVisited;
	
	// int *cycleNum;
	// int *branchVisited;


	//map<pair<int, int>, vector<int>> inAncestor[10000001];

	// most important node in negative cycle

	

public:
	SL(Graph *graph);
	~SL();


	void construct(string scalePath);
	void construct_for_a_vertex(HyperEdge * head, int u, bool update);
	void add_triplet(vector<Pair> *currLabel, int u, int h, int overlap, bool update);
	
	
	int span_reach(int u, int v, int k,  bool original_id = false);
	int reach(int src, int dst, bool original_id);
	int ete_reach(int src, int dst, bool original_id = false);
	int ete_edge_reach(int src, int dst);
	void insert(int sign);
	void erase(int ts);

	bool isCovered(int sign1, int sign2);
	bool veCover(int h, int u, int overlap);
	std::set<int> intersectAllSets(const std::vector<std::set<int>>& sets);
	// use for calculating the sign of u->v from u->w to w->v 
	int signCal(int sign1, int sign2);
	// update sign when go to the next vertex
	int signUpdate(int sign1, int sign2);
	bool isEqual(const std::pair<int, int>& element, const int val);

	int baseLine(int src, int dst,  bool original_id);
	
	void freeBaseline(int *visited);
	bool baseIsCovered(int sign1, int sign2);
	int baseLine_with_nbr(int src, int dst, bool original_id);
	int threshold_baseLine(int src, int dst, bool original_id, int k);
	int threshold_reach(int src, int dst, bool original_id, int k);
	int cover_check(int src, int dst, int k);
	int threshold_ete_reach(int src, int dst, bool original_id, int k);
	void estimate();
	//bool compareByFirstElement(const Pair_advanced& lhs, const Pair_advanced& rhs);
	//void SL::updateMustVisit(vector<mustVisit> *label_advanced_construct, int u, int v, int sign, vector<int> path, map<pair<int, int>, vector<int>> *visitedNode, vector<pair<int, set<levelRecord>>> *levelRecord);
	// int ete_reach(int src, int dst, bool original_id);

	long getMemoryUsage();
	int reach1(int src, int dst, bool original_id);

};