# HL-index
---
This project implements the HL-index construction algorithms and its corresponding VTE-reach to answer the max/threshold-reachability problem in hypergraphs for the following paper:
* Peiting Xie, Xiangjun Zai, Yanping Wu, Xiaoyang Wang, Wenjie Zhang, Lu Qin, "Efficient Reachability Query in Hypergraphs", 2025.

### Dataset
---
All the datasets in this paper can be downloaded in [__cornell__](https://www.cs.cornell.edu/~arb/data/).

### Compile
---
  
```
g++ -O3 graph.cpp HI.cpp main.cpp -o hl -std=c++17
```

### Preliminary
---
* Each line in the input hypergraph is a list of vertex ID, for example:
```
1 2 3 4
1 4 5
1 5 6
```
* The test code is in main.cpp
* The hypergraph reading procedures is implemented in graph.cpp

### Run/Test
To execute our Algorithm with hypergraph input file called __test.txt__, such test.txt should under src/data/
After that, in <strong>src/run.sh</strong>, add
```
./hl data/test.txt D query.txt output1.txt 1 5
```
and run the code 
```
cd src
./run.sh
```

The result of query time will be reported under __src/test_result/test/*.txt__


