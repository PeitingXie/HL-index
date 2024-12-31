#include "HI.h"


int main(int argc, char *argv[])
{
	bool directed = false;
	if (string(argv[2]) == "D")
		directed = true;
	else if (string(argv[2]) != "U")
	{
		cout << "Parameter Error" << endl;
		return 0;
	}
	double scale = stod(argv[5]);

	int t_value = stoi(argv[6]);

	
	string folder = "test_case";

	if (!std::filesystem::exists(folder)) {
        try {
            std::filesystem::create_directory(folder);
        } catch (const std::filesystem::filesystem_error& e) {
            std::cerr << "1" << e.what() << std::endl;
        }
    }


	string dictName = argv[1];
	dictName.erase(dictName.size() - 4);
	dictName.erase(0, 5);
	cout << "dict path is " << dictName << "\n";

	string dictPath = folder + "/" + dictName;

	
	
	string filePath = dictPath + "/" + "scale.txt";


	if (!std::filesystem::exists(dictPath)) {
        try {
            std::filesystem::create_directory(dictPath);
        } catch (const std::filesystem::filesystem_error& e) {
            std::cerr << "1" << e.what() << std::endl;
        }
    }



	// if (!std::filesystem::exists(dictPath + "/query_time")) {
    //     try {
    //         std::filesystem::create_directory(dictPath + "/query_time");
    //     } catch (const std::filesystem::filesystem_error& e) {
    //         std::cerr << "创建文件夹时出现错误1: " << e.what() << std::endl;
    //     }
    // }

	// if (!std::filesystem::exists(dictPath + "/query_time/" + to_string(static_cast<int>(scale * 100)))) {
    //     try {
    //         std::filesystem::create_directory(dictPath + "/query_time/" + to_string(static_cast<int>(scale * 100)));
    //     } catch (const std::filesystem::filesystem_error& e) {
    //         std::cerr << "创建文件夹时出现错误1: " << e.what() << std::endl;
    //     }
    // }

	// if (scale == 0.2) {
	// 	std::ofstream file(filePath, std::ofstream::trunc);
	// }
	
	// std::ofstream file(filePath, std::ofstream::app);

	// if (!file.is_open()) {
	// 	return -1;
	// }

	// file.close();

	srand (time(NULL));

	Graph * graph = new Graph(argv[1], directed, scale);
	
	if (!std::filesystem::exists(dictPath + "/query.txt")) {
        std::ofstream query_gen(dictPath + "/query.txt", std::ofstream::trunc);
		for (auto i = 0; i < 100000; i++) {
			int u = rand() % graph->n + 1;
			int v = rand() % graph->n + 1;
			query_gen << u << " " << v << "\n";
		}
    }
	// exit(1);

	SL * alg = new SL(graph);

	
	cout << "graph init complete\n";
	


	
	alg -> construct(filePath);

	cout << "construct complete\n";

	char* query_file = argv[3];
	char* output_file = argv[4];

	
	ifstream fin(query_file);
	// ofstream fout(output_file);

	int v, u, sign;

	
	std::string fullPath = argv[1];
	size_t lastSlashPos = fullPath.find_last_of('/');
    size_t dotPos = fullPath.find_last_of('.');
    std::string folderName;
    if (lastSlashPos != std::string::npos) {
        if (dotPos != std::string::npos && dotPos > lastSlashPos) {
            folderName = fullPath.substr(lastSlashPos + 1, dotPos - lastSlashPos - 1);
		}   
    }

	cout << folderName << "\n";



	if (!std::filesystem::exists("test_result/")) {
        try {
            std::filesystem::create_directory("test_result/");
        } catch (const std::filesystem::filesystem_error& e) {
            std::cerr << "创建文件夹时出现错误: " << e.what() << std::endl;
        }
    }

	if (!std::filesystem::exists("test_result/" + folderName)) {
        try {
            std::filesystem::create_directory("test_result/" + folderName);
        } catch (const std::filesystem::filesystem_error& e) {
            std::cerr << "创建文件夹时出现错误: " << e.what() << std::endl;
        }
    }


	// string baseOutput = "test_result/" + folderName + "/basetime";
	// string base1Output = "test_result/" + folderName + "/base1time";
	// string twoHopOutput = "test_result/" + folderName + "/spanReachTime";
	// string eteOutput = "test_result/" + folderName + "/eteTime";
	string minOutput = "test_result/" + folderName + "/minTime";

	std::ofstream testmr("record/" + folderName+"min.txt", std::ios::trunc);




	int reach = 0;
	int total = 0;
	int k;
	string currBaseOutFile, currBaseNbrFile, minFile;
	string currTwoHopOutFile; 
	string currEteOutFile; 

	
	minFile = minOutput + "0.txt";
		// currBaseOutFile = baseOutput + to_string(overlap) + ".txt";
		// currBaseNbrFile = base1Output + to_string(overlap) + ".txt";
		// currTwoHopOutFile = twoHopOutput + to_string(overlap) + ".txt";
		// currEteOutFile = eteOutput + to_string(overlap) + ".txt";

		// std::ofstream outputFile_nbr(currBaseNbrFile, std::ios::trunc);
		// outputFile_nbr.close();

		// std::ofstream outputFile(currBaseOutFile, std::ios::trunc);
		// outputFile.close();

		// std::ofstream outputFile2(currTwoHopOutFile, std::ios::trunc);
		// outputFile2.close();

		// std::ofstream outputFile3(currEteOutFile, std::ios::trunc);
		// outputFile3.close();
	std::ofstream outputFile(minFile, std::ios::trunc);
	outputFile.close();

	int count = 0;


	std::ofstream Minimal(minFile, std::ios::app);
	std::ifstream input_query(dictPath + "/query.txt");
	string src, dst;

	while (input_query >> src >> dst) {
	// while (total < 10000) {
		total++;
		int i = stoi(src);
		int j = stoi(dst);
		
		if (total == 10001) {
			// count ++;
			break;
			// continue;
		}
		cout << "test " << count << " with " << i << ", " << j << "\n";


		auto start_time = std::chrono::high_resolution_clock::now();
		
		auto res1 = alg->reach(i,j, 1);

		auto end_time = std::chrono::high_resolution_clock::now();

		auto elapsed_time_min = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time);
		Minimal << elapsed_time_min.count() << "\n";

		// cout << "result is " << res1 << "\n";



		start_time = std::chrono::high_resolution_clock::now();
		

		auto res2 = alg->baseLine(i,j, 1);
		end_time = std::chrono::high_resolution_clock::now();
		// auto elapsed_time_base = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
		auto elapsed_time_base = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);


		std::ofstream myfile(currBaseOutFile, std::ios::app);
		
		myfile << elapsed_time_base.count() << "\n";
		myfile.close();
		

		

		
		if (res1 != res2) {
			cout << "failed, baseline is " << res2 << ", vte_reach is " << res1 << "\n" ;
			return 0;
		}
		if (res1 >= 1) reach++;
		count++;
		
	}
	

	cout << "test for " << argv[1] << " is finished\n";	
	cout << "total test case = " << total << ", true case = " << reach << "\n"; 

	
	



	




    // cout << "baseline query result is " << alg->baseLine(i,j,k, 1) << "\n";
	// cout << "label query result is " << alg->span_reach(i,j,k, 1) << "\n";
	fin.close();

	


	// for (auto i = 1; i <= 8; i++) {
	// 	for (auto j = 1; j <= 8; j++) {
	// 		for (auto k = 1; k <= graph->max_size; k++) {
	// 			cout << "check " << i << ", " << j << ", " << k << "\n";
	// 			auto res1 = alg->baseLine(i,j,k, 1);
	// 			auto res2 = alg->span_reach(i,j,k, 1);
	// 			if (res1 != res2) {
	// 				cout << "failed, baseline is " << res1 << ", span reach is " << res2 << "\n" ;
	// 				return 0;
	// 			}
				
	// 		}
	// 	}
	// }

	// auto res1 = alg->baseLine(1,4,1, 1);
	// auto res2 = alg->span_reach(1,4,1, 1);
	
	// if (res1 != res2) {
	// 	cout << "failed, baseline is " << res1 << ", span reach is " << res2 << "with k = " << 1 <<"\n" ;
	// 	return 0;
	// }

	cout << "================================PASS================================\n";

	//
		//fout << "Hello world\n";



	// for (int i = 1; i <= 10; i++) {
		
	// 	auto res1 = alg->baseLine(1134,1135,i, 1);
	// 	auto res2 = alg->span_reach(1134,1135,i, 1);
		
	// 	if (res1 != res2) {
	// 		cout << "failed, baseline is " << res1 << ", span reach is " << res2 << "with k = " << i <<"\n" ;
	// 		return 0;
	// 	}
	
	// }



	return 0;
}
