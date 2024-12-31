#include "HI.h"


SL::SL(Graph *graph)
{
	max_v = graph -> max_v;
	max_e = graph -> max_e;
	n = graph -> n;
	m = graph -> m;
	directed = graph -> directed;

	E = graph->E;
	graph_edge = graph->graph_edge;


	vertex_map = &(graph -> vertex_map);

	//theta = 2000000000;

	total_memory = 0;


	label = new vector<Pair> [n + 1];
	ete_label = new vector<Pair> [m + 1];
	tmpLabel = new vector<Pair> [n + 1];
	visited = new int [n + 1];
	visited_h = new int [m + 1];
	fill(visited_h, visited_h + m + 1, 0);
	
	global_visited_h = new int [m+1];
	fill(global_visited_h, global_visited_h + m + 1, 0);

	for (auto i = 1; i <= m; i++) {
		for (auto v: graph_edge[i].node) {
			// neighbour[i] += pow(E[v].size(), 2);
			neighbour[i] += E[v].size();
		}
	}


	idx = new int [m + 1];
	
	iota(idx + 1, idx + m + 1, 1);
	sort(idx + 1, idx + m + 1, [&](int i, int j){return (long long)(neighbour[i] + 1) > (long long)(neighbour[j] + 1);});

	order = new int [m + 1];
	iota(order + 1, order + m + 1, 1);
	for (int i = 1; i <= m; i++)
		order[idx[i]] = i;

	allNbr = new vector<pair<int, int>> [m + 1];


	D = new map<int, int>[n + 1];
	QPairs = new vector<QPair>[m + 1];
	C = new int [m + 1];
	// inverted = new set<int> [m + 1];
	W = new set<int> [n + 1];
	total_count = 0;



	for (auto i = 1; i <= m; i++) {
		graph_edge[i].node.push_back(n + i);
	}

	
	

}


SL::~SL()
{
	
	if (idx) delete[] idx;
	if (order) delete[] order;
	if (nbr) delete[] nbr;
	if (visited_h) delete[] visited_h;
	if (global_visited_h) delete[] global_visited_h;
	if (visited) delete[] visited;
	// for baseline
	if (allNbr) delete[] allNbr;
	if (D) delete[] D;
	if (QPairs) delete[] QPairs;
	if (tmpLabel) delete[] tmpLabel;
	if (C) delete[] C;
	if (W) delete[] W;
	// for baseline
}



void SL::add_triplet(vector<Pair> *currLabel, int u, int h, int overlap, bool update)
{
	
	

	auto it = std::upper_bound(
		QPairs[h].begin(),
		QPairs[h].end(),
		QPair(u, overlap),
		[](const QPair& a, const QPair& b) {
			return a.overlap > b.overlap; // 自定义从大到小的比较规则
		}
	);
	QPairs[h].insert(it, QPair(u, overlap));
	// QPairs[h].push_back(QPair(u, overlap));
	
	if (!currLabel[u].size()) {
		currLabel[u].push_back(Pair(h, overlap));
	} else {
		

		Pair temp(h, overlap);

		auto it = std::lower_bound(currLabel[u].begin(), currLabel[u].end(), temp,
        [](const Pair a, const Pair& b) {
            return a.hID < b.hID;
        });



		if (it->hID != h || it == currLabel[u].end()) {

			currLabel[u].insert(it, temp);
		} 

	}
}


void SL::construct_for_a_vertex(HyperEdge * head,  int u, bool update) {
	int count = 0;
	int num = 0;
	// ofstream myfile;
  	// myfile.open ("out.txt");
	int max_h = 0;
	long long total = 0;
	int execute = 0;


	while (!Q.empty()) {
	
		Pair_in_queue pair = Q.top();
		Q.pop();
		int h = pair.hID;
		int overlap = pair.overlap;
		int c_val = pair.cover;
		if (visited_h[h] == -1) continue;
		if (visited_h[h] > 0) {
			visited_h[h] = -1;
		}
		execute ++;

		if (u != h) {
			global_visited_h[h] = max(global_visited_h[h], overlap);
		} else {
			if (overlap <= global_visited_h[u]) continue;
		}
		
		for (auto v : graph_edge[idx[h]].node) {
			if (v > n) {
				continue;

			} else {
				
				if (visited[v] == 0) {
					visited[v] = overlap;
					add_triplet(tmpLabel, v, u, overlap, update);
					total_count += 1;
				}
			}
			
		}
	
		map<int, int> m;

		for (auto v : graph_edge[idx[h]].node) {
			if (v > n) continue;
			for (auto nextH : E[v]) {
				if (order[nextH] <= u || order[nextH] == h) continue;
				// if (visited_h[order[nextH]] >= overlap) {
				// 	continue;
				// }
				if (m.find(order[nextH]) != m.end()) {
					m[order[nextH]]++;
				} else {
					m.insert(make_pair(order[nextH], 1));
				}
			}
		}
			
		for (auto it = m.begin(); it != m.end(); it++) {
			
			
			if (min(overlap, it->second) <= global_visited_h[u]) {
				// total_memory++;
				continue;
			} 
			if (visited_h[it->first] >= min(overlap, it->second) || visited_h[it->first] == -1) continue;
			visited_h[it->first] = min(overlap, it->second);
			
			Q.push(Pair_in_queue(it->first, min(overlap, it->second), c_val));
		}

		m.clear();
		

	}

	// myfile.close();
}

void SL::construct(string scalePath) {


	cout << "ready to construct\n";
	

	double avg = 0;
	for (auto i = 1; i <= n; i++) {
		avg += E[i].size();
	}
	avg /= n;
	cout << "average neighbour = " << avg << "\n";


	vector<vector<int>> hot;
	vector<int> centrality;
	for (auto i = 1; i <= n; i++) {
		centrality.push_back(E[i].size());
	}
	sort(centrality.begin(), centrality.end());
	double level = 0.85;
	int count = 0;
	double val = level + count * 0.03;
	while (val < 1) {
		cout << "at level " << val << " " << centrality[n * val] << "\n";
		count += 1;
		val = level + count * 0.03;
	}


	int singleNode = 0;



	// while (true) {

	// }
	
	cout << "total n = " << n << ", single node = " << singleNode << "\n";




	// ofstream myConstructionfile;
	// myConstructionfile.open ("construction.txt");
	cout << "start construction\n";
	// int c;
	// cin >> c;


	
	auto start_time = std::chrono::high_resolution_clock::now();
	for (auto i = 1; i <= m; i++) {
		// cout << "\n------------------------------construct for hID = " << i << " with overlap = " << graph_edge[idx[i]].node.size() - 1 << "-----------------------------\n";
		
	
		Q.push(Pair_in_queue(i, graph_edge[idx[i]].length, 0));
		// cout << "construct for hID = " << i << " is finished\n";
		
		fill(visited, visited + n + 1, 0);
		fill(visited_h, visited_h + m + 1, 0);
		construct_for_a_vertex(graph_edge, i, false);



		if (i % 1000 == 1) {
		cout << "Edge  " << i << " has ben constructed " << "\n";
		}
		std::priority_queue<Pair_in_queue>().swap(Q);
		
	}
	
	auto end_time = std::chrono::high_resolution_clock::now();
	auto elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
	// myConstructionfile.close();
	

	
	for (auto i = 1; i<=m; i++) {
		graph_edge[i].node.pop_back();
		graph_edge[i].node.shrink_to_fit();
	}
	cout << "construction finished\n";

	cout << "construct time is " << elapsed_time.count() << "\n";

	double vte = 0;

	
	cout << "here\n";
	double ete = 0;
	
	for (auto i = 1; i <=n; i++) {
		vte += tmpLabel[i].size();
	}
	cout << "tmp total vte = " << vte << "\n";
	// file << "tmp total vte = " << vte << "\n";


	long nonMinimaltotalBytes = 0;
	for (int i = 1; i < n + 1; ++i) {
		nonMinimaltotalBytes += sizeof(Pair) * tmpLabel[i].size();
	}

	int sumNum = 0;
	int* vis = new int[n + 1]();
	// fill(vis, vis + n + 1, 0);
	start_time = std::chrono::high_resolution_clock::now();
	for (auto i = 1; i <= m; ++i) {
		// cout << "1\n";

		// vector<pair<int, int>> inverted[m + 1];
		if (QPairs[i].size() == 0) continue;
		if (QPairs[i].size() == 1) {
			bool re = false;
			for (auto it = tmpLabel[QPairs[i][0].vID].begin(); it != tmpLabel[QPairs[i][0].vID].end(); ++it) {
				if (it->hID != i && it->overlap >= QPairs[i][0].overlap) {
					re = true;
					break;
				}
				if (re) {
					auto it2 = std::lower_bound(tmpLabel[QPairs[i][0].vID].begin(), tmpLabel[QPairs[i][0].vID].end(), Pair(i, 1),
					[](const Pair a, const Pair& b) {
						return a.hID < b.hID;
					});
					if (it2->hID == i) {
						tmpLabel[QPairs[i][0].vID].erase(it2);
					}
				} else {
					auto temp = Pair(i, QPairs[i][0].overlap);
					auto it_1 = std::lower_bound(label[QPairs[i][0].vID].begin(), label[QPairs[i][0].vID].end(), temp,
					[](const Pair a, const Pair& b) {
						return a.hID < b.hID;
					});
					label[QPairs[i][0].vID].insert(it_1, temp);
				}
			}
			continue;
		}
		unordered_set<int> nonDup;
		// auto* inverted = new vector<pair<int, int>>[m + 1];
		std::map<int, std::vector<std::pair<int, int>>> inverted;

		for (auto it = QPairs[i].begin(); it != QPairs[i].end(); ++it) {	
			for (auto p = tmpLabel[it->vID].begin(); p != tmpLabel[it->vID].end(); ++p) {
				if (p->overlap >= it->overlap  && p->hID != i) {
					inverted[p->hID].push_back(make_pair(it->vID, it->overlap));
				}
			}
		}

		for (auto it = QPairs[i].begin(); it != QPairs[i].end();) {

			unordered_set<int> candidates; // S
			bool redundant = false;
			for (auto p = tmpLabel[it->vID].begin(); p != tmpLabel[it->vID].end(); ++p) {
				if (redundant) break;
				if (p->hID == i) continue;
	
				for (auto p_1 = inverted[p->hID].begin(); p_1 != inverted[p->hID].end(); ++p_1) {
					if (redundant) break;

					if (p->overlap >= p_1->second && vis[p_1->first] != i) {
						candidates.insert(p_1->first);
					}

					if (candidates.size() == QPairs[i].size()) redundant = true;
				}
			}
		
			
			auto found = nonDup.find(it->vID);

			if (!redundant || found != nonDup.end()) {
			
				auto temp = Pair(i, it->overlap);
				auto it_1 = std::lower_bound(label[it->vID].begin(), label[it->vID].end(), temp,
				[](const Pair a, const Pair& b) {
				    return a.hID < b.hID;
				});
	
				label[it->vID].insert(it_1, temp);
				for (auto it2 = QPairs[i].begin(); it2 != QPairs[i].end(); ++it2) {
					if (candidates.find(it2->vID) == candidates.end()) {
						nonDup.insert(it2->vID);
					}
				}
				
			} else {
			

				auto it2 = std::lower_bound(tmpLabel[it->vID].begin(), tmpLabel[it->vID].end(), Pair(i, 1),
				[](const Pair a, const Pair& b) {
				    return a.hID < b.hID;
				});
				if (it2->hID == i) {
					tmpLabel[it->vID].erase(it2);
				}

		
			}

			nonDup.erase(it->vID);

			vis[it->vID] = i;
			it = QPairs[i].erase(it);
			if (nonDup.size() == QPairs[i].size()) {

				for(; it != QPairs[i].end(); ++it) {
					auto temp = Pair(i, it->overlap);
					auto it_1 = std::lower_bound(label[it->vID].begin(), label[it->vID].end(), temp,
					[](const Pair a, const Pair& b) {
						return a.hID < b.hID;
					});
					label[it->vID].insert(it_1, temp);
				}
				break;
			}
		}


	}

	
	end_time = std::chrono::high_resolution_clock::now();
	elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
	cout << "minimal time is " << elapsed_time.count() << "\n";
	// file << "minimal time is " << elapsed_time.count() << "\n";
	cout << "sumNum = " << sumNum << "\n";

	delete[] vis;
	


	vte = 0;
	ete = 0;

	for (auto i = 1; i <= m; i++) {
		cout << "eID = " << idx[i] << " map to " << i << "\n";  
	}


	cout << "total minimal vte = " << vte << "\n";

	vte /= n;

	for (auto i = 1; i <= m; i++) {
		ete += ete_label[i].size();
	}
	ete /= m;
	cout << "average vte = " << vte << "\n";


	long totalBytes = 0;
	for (int i = 1; i < n + 1; ++i) {
		totalBytes += sizeof(Pair) * label[i].size();
	}

	
	

	cout << "Minimal/non Minimal VTE index size is " << totalBytes / 1024 << ", "<< nonMinimaltotalBytes / 1024 << "kb\n";

	long eteBytes = 0;
	for (int i = 1; i < m + 1; ++i) {
		eteBytes += sizeof(Pair) * ete_label[i].size();
	}
	cout << "ETE index size is " << eteBytes / 1024 << "kb\n";
	cout << "ETE Container size is " << sizeof(vector<Pair>) * m /1024 << "kb\n"; 
	// long memoryBefore = getMemoryUsage();

	long graphSize = 0;

	for (auto i = 1; i <= n; i++) {
		graphSize += E[i].size() * sizeof(int);
	}
	cout << "Graph size is " << graphSize /1024 << "kb\n"; 

	cout << "construction finished\n";
	






}




int SL::span_reach(int u, int v, int k, bool original_id) {
	// cout << "span reach checking " << u << " and " << v << " with o = " << overlap << "\n";
	int init_u = u;
	int init_v = v;
	if (original_id)
	{
		u = (*vertex_map)[u];
		v = (*vertex_map)[v];		
	}
	
	vector<Pair> *label_u, *label_v;

	int tmp_u = u;
	int tmp_v = v;

	if (u > n) {
		label_u = tmpLabel;
		tmp_u -= n;
	} else {
		label_u =  label;
	}

	if (v > n) {
		label_v = tmpLabel;
		tmp_v -= n;
	} else {
		label_v = label;
	}
	
	
	int overlap = k;


	auto srcIt = label_u[tmp_u].begin();
	auto dstIt = label_v[tmp_v].begin();
	while (srcIt != label_u[tmp_u].end() && dstIt != label_v[tmp_v].end()) {
		if (srcIt->hID > dstIt->hID ) {
			dstIt++;
		} else if (srcIt->hID < dstIt->hID) {
			srcIt++;
		} else {
			if (srcIt->overlap > overlap && dstIt->overlap > overlap) {
				overlap = min(srcIt->overlap, dstIt->overlap);
			}
			srcIt++;
			dstIt++;
		}

	}
	
	return overlap;
}



int SL::ete_reach(int src, int dst, bool original_id) {
	if (original_id)
	{
		src = (*vertex_map)[src];
		dst = (*vertex_map)[dst];
	}
	int overlap = 0;
	if (src == dst) {
		overlap = 1;
	}

	vector<Pair> srcSet;
	vector<Pair> dstSet;


	for (auto h_u : E[src]) {
		auto srcIt1 = srcSet.begin();
		h_u = order[h_u];
		auto srcIt2 = ete_label[h_u].begin();
		// cout << "1\n";
		while (srcIt1 != srcSet.end() && srcIt2 != ete_label[h_u].end()) {
			// cout << "2\n";
			if (srcIt1->hID < srcIt2->hID) {
				srcIt1++;
			} else {
				// cout << "?\n";
				auto dist = std::distance(srcSet.begin(), srcIt1);
				if (srcIt1->hID == srcIt2->hID) {
					srcIt1->overlap = max(srcIt1->overlap, srcIt2->overlap);
				} else {
					srcSet.insert(srcIt1, Pair(srcIt2->hID, srcIt2->overlap));
					srcIt1 = srcSet.begin() + dist + 1;
				}
				srcIt2++;
			}
		}	
		

		if (srcIt2 != ete_label[h_u].end()) {
			while (srcIt2 != ete_label[h_u].end()) {
				auto dist = std::distance(srcSet.begin(), srcIt1);
				if (srcIt1 == srcSet.end()) {
					srcSet.insert(srcIt1, Pair(srcIt2->hID, srcIt2->overlap));
					srcIt1 = srcSet.begin() + dist + 1;
				} else {
					if (srcIt1->hID == srcIt2->hID) {
						srcIt1->overlap = max(srcIt1->overlap, srcIt2->overlap);
					} else {
						srcSet.insert(srcIt1 + 1, Pair(srcIt2->hID, srcIt2->overlap));
						srcIt1 = srcSet.begin() + dist + 1;
					}
				}
				srcIt2++;

			}
		}


		
	}


	for (auto h_v : E[dst]) {
		auto dstIt1 = dstSet.begin();
		h_v = order[h_v];
		auto dstIt2 = ete_label[h_v].begin();
		
		while (dstIt1 != dstSet.end() && dstIt2 != ete_label[h_v].end()) {
			
			if (dstIt1->hID <= dstIt2->hID) {
				dstIt1++;
			} else {
				auto dist = std::distance(dstSet.begin(), dstIt1);
				dstSet.insert(dstIt1, Pair(dstIt2->hID, dstIt2->overlap));
				dstIt1 = dstSet.begin() + dist + 1;
				dstIt2++;
			}
				
		}
		if (dstIt2 != ete_label[h_v].end()) {
			while (dstIt2 != ete_label[h_v].end()) {
				auto dist = std::distance(dstSet.begin(), dstIt1);
				if (dstIt1 == dstSet.end()) {
					dstSet.insert(dstIt1, Pair(dstIt2->hID, dstIt2->overlap));
				} else {
					dstSet.insert(dstIt1 + 1, Pair(dstIt2->hID, dstIt2->overlap));
				}
				dstIt1 = dstSet.begin() + dist + 1;
				dstIt2++;
			}
		}

	}

	auto srcIt = srcSet.begin();
	auto dstIt = dstSet.begin();
	while (srcIt != srcSet.end() && dstIt != dstSet.end()) {
		if (srcIt->hID > dstIt->hID ) {
			dstIt++;
		} else if (srcIt->hID < dstIt->hID) {
			srcIt++;
		} else {
			if (srcIt->overlap > overlap && dstIt->overlap > overlap) {
				overlap = min(srcIt->overlap, dstIt->overlap);
			}
			if ((srcIt + 1) != srcSet.end() && srcIt->hID == (srcIt + 1)->hID) {
				srcIt++;
			} else {
				dstIt++;
			}
		}

	}

	return overlap;
}






int SL::reach(int src, int dst, bool original_id) {
	if (original_id)
	{
		src = (*vertex_map)[src];
		dst = (*vertex_map)[dst];		
	}

	int overlap = 0;
	if (src == dst) {
		overlap = 1;
	}
	auto srcIt = label[src].begin();
	auto dstIt = label[dst].begin();
	while (srcIt != label[src].end() && dstIt != label[dst].end()) {
		if (srcIt->hID > dstIt->hID ) {
			dstIt++;
		} else if (srcIt->hID < dstIt->hID) {
			srcIt++;
		} else {
			if (srcIt->overlap > overlap && dstIt->overlap > overlap) {
				overlap = min(srcIt->overlap, dstIt->overlap);
			}
			srcIt++;
			dstIt++;
		}

	}
	return overlap;
}





int SL::threshold_reach(int src, int dst, bool original_id, int k) {
	if (original_id)
	{
		src = (*vertex_map)[src];
		dst = (*vertex_map)[dst];		
	}

	// int overlap = 0;
	// if (src == dst) {
	// 	overlap = 1;
	// }
	auto srcIt = label[src].begin();
	auto dstIt = label[dst].begin();
	while (srcIt != label[src].end() && dstIt != label[dst].end()) {
		if (srcIt->hID > dstIt->hID || dstIt->overlap < k) {
			dstIt++;
		} else if (srcIt->hID < dstIt->hID || srcIt->overlap < k) {
			srcIt++;
		} else {
			return 1;
		}

	}
	return 0;
}





int SL::baseLine(int src, int dst, bool original_id) {
	auto start_time = std::chrono::high_resolution_clock::now();
	if (original_id)
	{
		src = (*vertex_map)[src];
		dst = (*vertex_map)[dst];
	}


	vector<int> in_visit, out_visit;
	for (auto i = 0; i <= m; i++) {
		in_visit.push_back(0);
		out_visit.push_back(0);
	}
	priority_queue<Pair_in_queue> Q_in, Q_out;

	for (auto h : E[dst]) {
		// cout << "insert " << h << " with o = " << graph_edge[h].length << "\n";
		Q_in.push(Pair_in_queue(h, graph_edge[h].length, 0));
	}

	// cout << "for src:\n";
	for (auto h : E[src]) {
		// cout << "insert " << h << " with o = " << graph_edge[h].length << "\n";
		Q_out.push(Pair_in_queue(h, graph_edge[h].length, 0));
	}
	int result = 0;
	int token = 0;

	if (src == dst) {
		result = 1;
	}

	while (!Q_in.empty() || !Q_out.empty()) {
		
		if (!token) {
			int size = Q_in.size();
			for (auto i = 0; i < size; i++) {
				auto end = std::chrono::high_resolution_clock::now();
				auto time_spent = std::chrono::duration_cast<std::chrono::seconds>(end - start_time);
				if (time_spent.count() > 100) return result;
				auto pair = Q_in.top();
				auto h = pair.hID;
				auto o = pair.overlap;
				// cout << "from in: get h = " << h << ", o = " << o << "\n";
				Q_in.pop();

				if (out_visit[h]) {
					result = max(result, min(o, out_visit[h]));
				}
				if (in_visit[h] >= o) continue;
				in_visit[h] = o;
				
				map<int, int> m;
	
				for (auto v : graph_edge[h].node) {
					if (v > n) continue;
					for (auto nextH : E[v]) {
						// if (h <= i) continue;
						// if (in_visit[nextH]) continue;
						if (m.find(nextH) != m.end()) {
							m[nextH]++;
						} else {
							m.insert(make_pair(nextH, 1));
						}
					}
				}

				for (auto it = m.begin(); it != m.end(); it++) {
					

					if (it->second <= result) continue;
					if (in_visit[it->first] >= min(it->second, o)) continue;
					Q_in.push(Pair_in_queue(it->first, min(it->second, o), 0));
				}
				m.clear();
			}
			token = 1;
		} else {
			int size = Q_out.size();

			// cout << "now switch to out neighbour\n";
			// cout << "size = " << size << "\n";
			for (auto i = 0; i < size; i++) {
				auto end = std::chrono::high_resolution_clock::now();
				auto time_spent = std::chrono::duration_cast<std::chrono::seconds>(end - start_time);
				if (time_spent.count() > 100) return result;
				auto pair = Q_out.top();
				auto h = pair.hID;
				auto o = pair.overlap;
				// cout << "from out: get h = " << h << ", o = " << o << "\n";
				// cout << "get edge h = " << h << " with overlap = " << o << "\n";
				// if (h == 2578) cout << "2 detect 2578 with" << o << "\n";
				Q_out.pop();
				
				if (in_visit[h]) {
					result = max(result, min(o, in_visit[h]));
				}
				
				if (out_visit[h] >= o) continue;
				out_visit[h] = o;
				// for (auto nextPair : neighbour[h]) {
				// 	if (nextPair.second < overlap) continue;
				// 	if (out_visit[nextPair.first]) continue;
				// 	// cout << "push " << nextPair.first << " " << nextPair.second << "\n";
				// 	Q_out.push(nextPair);
				// }


				map<int, int> m;
	
				for (auto v : graph_edge[h].node) {
					if (v > n) continue;
					for (auto nextH : E[v]) {
						// if (h <= i) continue;
						// if (out_visit[nextH]) continue;
						if (m.find(nextH) != m.end()) {
							m[nextH]++;
						} else {
							m.insert(make_pair(nextH, 1));
						}
					}
				}

				for (auto it = m.begin(); it != m.end(); it++) {

				
					if (it->second <= result) continue;
					if (out_visit[it->first] >= min(it->second, o)) continue;
					Q_out.push(Pair_in_queue(it->first, min(it->second, o), 0));
				}
				m.clear();	
			}
			token = 0;
		}

	}
	return result;
}	


int SL::threshold_baseLine(int src, int dst, bool original_id, int k) {
	auto start_time = std::chrono::high_resolution_clock::now();
	if (original_id)
	{
		src = (*vertex_map)[src];
		dst = (*vertex_map)[dst];
	}


	vector<int> in_visit, out_visit;
	for (auto i = 0; i <= m; i++) {
		in_visit.push_back(0);
		out_visit.push_back(0);
	}
	priority_queue<Pair_in_queue> Q_in, Q_out;

	for (auto h : E[dst]) {
		// cout << "insert " << h << " with o = " << graph_edge[h].length << "\n";
		Q_in.push(Pair_in_queue(h, graph_edge[h].length, 0));
	}

	// cout << "for src:\n";
	for (auto h : E[src]) {
		// cout << "insert " << h << " with o = " << graph_edge[h].length << "\n";
		Q_out.push(Pair_in_queue(h, graph_edge[h].length, 0));
	}
	int result = 0;
	int token = 0;

	if (src == dst) {
		result = 1;
	}

	while (!Q_in.empty() || !Q_out.empty()) {
		
		if (!token) {
			int size = Q_in.size();
			for (auto i = 0; i < size; i++) {
				auto end = std::chrono::high_resolution_clock::now();
				auto time_spent = std::chrono::duration_cast<std::chrono::seconds>(end - start_time);
				if (time_spent.count() > 100) return -1;
				auto pair = Q_in.top();
				auto h = pair.hID;
				auto o = pair.overlap;
				
				// cout << "from in: get h = " << h << ", o = " << o << "\n";
				Q_in.pop();
				if (o < k) continue;
				if (out_visit[h]) {
					return 1;
				}
				if (in_visit[h] >= o) continue;
				in_visit[h] = o;
				
				map<int, int> m;
	
				for (auto v : graph_edge[h].node) {
					if (v > n) continue;
					for (auto nextH : E[v]) {
						// if (h <= i) continue;
						// if (in_visit[nextH]) continue;
						if (m.find(nextH) != m.end()) {
							m[nextH]++;
						} else {
							m.insert(make_pair(nextH, 1));
						}
					}
				}

				for (auto it = m.begin(); it != m.end(); it++) {
					

					if (it->second <= result) continue;
					if (in_visit[it->first] >= min(it->second, o)) continue;
					Q_in.push(Pair_in_queue(it->first, min(it->second, o), 0));
				}
				m.clear();
			}
			token = 1;
		} else {
			int size = Q_out.size();

;
			for (auto i = 0; i < size; i++) {
				auto end = std::chrono::high_resolution_clock::now();
				auto time_spent = std::chrono::duration_cast<std::chrono::seconds>(end - start_time);
				if (time_spent.count() > 100) return result;
				auto pair = Q_out.top();
				auto h = pair.hID;
				auto o = pair.overlap;

				Q_out.pop();
				if (o < k) continue;
				
				if (in_visit[h]) {
					return 1;
				}
				
				if (out_visit[h] >= o) continue;
				out_visit[h] = o;

				map<int, int> m;
	
				for (auto v : graph_edge[h].node) {
					if (v > n) continue;
					for (auto nextH : E[v]) {
						// if (h <= i) continue;
						// if (out_visit[nextH]) continue;
						if (m.find(nextH) != m.end()) {
							m[nextH]++;
						} else {
							m.insert(make_pair(nextH, 1));
						}
					}
				}

				for (auto it = m.begin(); it != m.end(); it++) {

				
					if (it->second <= result) continue;
					if (out_visit[it->first] >= min(it->second, o)) continue;
					Q_out.push(Pair_in_queue(it->first, min(it->second, o), 0));
				}
				m.clear();	
			}
			token = 0;
		}

	}
	return 0;
}	



int SL::cover_check(int src, int dst, int k) {
	vector<int> in_visit, out_visit;
	for (auto i = 0; i <= m; i++) {
		in_visit.push_back(0);
		out_visit.push_back(0);
	}
	priority_queue<Pair_in_queue> Q_in, Q_out;
	Q_in.push(Pair_in_queue(src, graph_edge[idx[src]].length, 0));
	Q_out.push(Pair_in_queue(dst, graph_edge[idx[dst]].length, 0));
	int token = 0;
	int result = 0;
	while (!Q_in.empty() || !Q_out.empty()) {
		
		if (!token) {
			int size = Q_in.size();
			for (auto i = 0; i < size; i++) {
				auto pair = Q_in.top();
				auto h = pair.hID;
				auto o = pair.overlap;
				

				Q_in.pop();
				if (out_visit[h] && h < src) {
					result = max(result, min(out_visit[h], o));
				}
				if (in_visit[h] >= o) continue;
				in_visit[h] = o;
				
				map<int, int> m;
	
				for (auto v : graph_edge[idx[h]].node) {
					if (v > n) continue;
					for (auto nextH : E[v]) {

						if (m.find(order[nextH]) != m.end()) {
							m[order[nextH]]++;
						} else {
							m.insert(make_pair(order[nextH], 1));
						}
					}
				}

				for (auto it = m.begin(); it != m.end(); it++) {
					

					if (it->second <= result) continue;
					if (in_visit[it->first] >= min(it->second, o)) continue;
					Q_in.push(Pair_in_queue(it->first, min(it->second, o), 0));
				}
				m.clear();
			}
			token = 1;
		} else {
			int size = Q_out.size();

			for (auto i = 0; i < size; i++) {
				auto pair = Q_out.top();
				auto h = pair.hID;
				auto o = pair.overlap;

				Q_out.pop();
				
				if (in_visit[h] && h < src) {
					result = max(result, min(o, in_visit[h]));
				}
				
				if (out_visit[h] >= o) continue;
				out_visit[h] = o;

				map<int, int> m;
	
				for (auto v : graph_edge[idx[h]].node) {
					if (v > n) continue;
					for (auto nextH : E[v]) {
						// if (h <= i) continue;
						// if (out_visit[nextH]) continue;
						if (m.find(order[nextH]) != m.end()) {
							m[order[nextH]]++;
						} else {
							m.insert(make_pair(order[nextH], 1));
						}
					}
				}

				for (auto it = m.begin(); it != m.end(); it++) {

				
					if (it->second <= result) continue;
					if (out_visit[it->first] >= min(it->second, o)) continue;
					Q_out.push(Pair_in_queue(it->first, min(it->second, o), 0));
				}
				m.clear();	
			}
			token = 0;
		}
	}
	return result;
}



int SL::baseLine_with_nbr(int src, int dst, bool original_id) {
	auto start_time = std::chrono::high_resolution_clock::now();
	if (original_id)
	{
		src = (*vertex_map)[src];
		dst = (*vertex_map)[dst];
	}


	vector<int> in_visit, out_visit;
	for (auto i = 0; i <= m; i++) {
		in_visit.emplace_back(0);
		out_visit.emplace_back(0);
	}
	priority_queue<Pair_in_queue> Q_in, Q_out;

	for (auto h : E[dst]) {
		// cout << "insert " << h << " with o = " << graph_edge[h].length << "\n";
		Q_in.push(Pair_in_queue(h, graph_edge[h].length, 0));
	}

	// cout << "for src:\n";
	for (auto h : E[src]) {
		// cout << "insert " << h << " with o = " << graph_edge[h].length << "\n";
		Q_out.push(Pair_in_queue(h, graph_edge[h].length, 0));
	}
	int result = 0;
	int token = 0;

	if (src == dst) {
		result = 1;
	}

	while (!Q_in.empty() || !Q_out.empty()) {
		
		if (!token) {
			int size = Q_in.size();
			for (auto i = 0; i < size; i++) {
				auto end = std::chrono::high_resolution_clock::now();
				auto time_spent = std::chrono::duration_cast<std::chrono::seconds>(end - start_time);
				if (time_spent.count() > 100) return result;
				auto pair = Q_in.top();
				auto h = pair.hID;
				auto o = pair.overlap;
				// cout << "from in: get h = " << h << ", o = " << o << "\n";
				Q_in.pop();

				if (out_visit[h]) {
					result = max(result, min(o, out_visit[h]));
				}
				if (in_visit[h] >= o) continue;
				in_visit[h] = o;
			
				for (auto it = allNbr[h].begin(); it != allNbr[h].end(); ++it) {
					if (it->second <= result) continue;
					if (in_visit[it->first] >= min(it->second, o)) continue;
					Q_in.push(Pair_in_queue(it->first, min(it->second, o), 0));
				}
			}
			token = 1;
		} else {
			int size = Q_out.size();

			// cout << "now switch to out neighbour\n";
			// cout << "size = " << size << "\n";
			for (auto i = 0; i < size; i++) {
				auto end = std::chrono::high_resolution_clock::now();
				auto time_spent = std::chrono::duration_cast<std::chrono::seconds>(end - start_time);
				if (time_spent.count() > 100) return result;
				auto pair = Q_out.top();
				auto h = pair.hID;
				auto o = pair.overlap;

				Q_out.pop();
				
				if (in_visit[h]) {
					result = max(result, min(o, in_visit[h]));
				}
				
				if (out_visit[h] >= o) continue;
				out_visit[h] = o;
			
				for (auto it = allNbr[h].begin(); it != allNbr[h].end(); ++it) {
					if (it->second <= result) continue;
					if (out_visit[it->first] >= min(it->second, o)) continue;
					Q_out.push(Pair_in_queue(it->first, min(it->second, o), 0));
				}
			}
			token = 0;
		}

	}
	return result;
}	






long SL::getMemoryUsage() {
    std::string line;
    std::ifstream stat_stream("/proc/self/status", std::ios_base::in);

    while (std::getline(stat_stream, line)) {
        if (line.substr(0, 6) == "VmRSS:") {
            std::string rss = line.substr(7);
            rss = rss.substr(0, rss.find(" kB"));
            return std::stol(rss);
        }
    }
    return -1; // 如果没有找到相关信息
}