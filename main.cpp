#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>
#include <thread>

#include "CSRGraph.h"
#include "BSPRunner.h"
#include "BFS.h"
#include "SSSP.h"
#include "CC.h"

using namespace std;

void WriteDistances(const string& filename, const vector<int>& values) {
    ofstream fout(filename);

    for (int v = 0; v < values.size(); v++) {
        fout << v << " " << values[v] << endl;
    }
}

void WriteLabels(const string& filename, const vector<int>& values) {
    ofstream fout(filename);

    for (int v = 0; v < values.size(); v++) {
        fout << v << " " << values[v] << endl;
    }
}

bool SameVector(const vector<int>& a, const vector<int>& b) {
    return a == b;
}

int main() {
    // string graph_file = "soc-LiveJournal1-weighted.txt";
    string graph_file = "test.txt";

    CSRGraph graph = LoadGraph(graph_file.c_str());

    cout << "Nodes: " << graph.num_vertices << endl;
    cout << "Edges: " << graph.edges.size() << endl;
    cout << endl;

    int source = 0;
    int nthreads = 1;

    chrono::high_resolution_clock::time_point t1 = chrono::high_resolution_clock::now();

    BFS bfs_s(graph.num_vertices, source, 1);
    BspSerial(graph, bfs_s);

    chrono::high_resolution_clock::time_point t2 = chrono::high_resolution_clock::now();

    BFS bfs_p(graph.num_vertices, source, nthreads);
    BspParallel(graph, bfs_p, nthreads);

    chrono::high_resolution_clock::time_point t3 = chrono::high_resolution_clock::now();

    double bfs_serial_time = chrono::duration<double>(t2 - t1).count();
    double bfs_parallel_time = chrono::duration<double>(t3 - t2).count();

    cout << "BFS serial time: " << bfs_serial_time << " seconds" << endl;
    cout << "BFS parallel time: " << bfs_parallel_time << " seconds" << endl;
    cout << endl;

    WriteDistances("BFS.txt", bfs_p.dist);

    chrono::high_resolution_clock::time_point t4 = chrono::high_resolution_clock::now();

    SSSP sssp_s(graph.num_vertices, source, 1);
    BspSerial(graph, sssp_s);

    chrono::high_resolution_clock::time_point t5 = chrono::high_resolution_clock::now();

    SSSP sssp_p(graph.num_vertices, source, nthreads);
    BspParallel(graph, sssp_p, nthreads);

    chrono::high_resolution_clock::time_point t6 = chrono::high_resolution_clock::now();

    double sssp_serial_time = chrono::duration<double>(t5 - t4).count();
    double sssp_parallel_time = chrono::duration<double>(t6 - t5).count();

    cout << "SSSP serial time: " << sssp_serial_time << " seconds" << endl;
    cout << "SSSP parallel time: " << sssp_parallel_time << " seconds" << endl;
    cout << endl;

    WriteDistances("SSSP.txt", sssp_p.dist);

    chrono::high_resolution_clock::time_point t7 = chrono::high_resolution_clock::now();

    CC cc_s(graph.num_vertices, 1);
    BspSerial(graph, cc_s);

    chrono::high_resolution_clock::time_point t8 = chrono::high_resolution_clock::now();

    CC cc_p(graph.num_vertices, nthreads);
    BspParallel(graph, cc_p, nthreads);

    chrono::high_resolution_clock::time_point t9 = chrono::high_resolution_clock::now();

    double cc_serial_time = chrono::duration<double>(t8 - t7).count();
    double cc_parallel_time = chrono::duration<double>(t9 - t8).count();

    cout << "CC serial time: " << cc_serial_time << " seconds" << endl;
    cout << "CC parallel time: " << cc_parallel_time << " seconds" << endl;

    WriteLabels("CC.txt", cc_p.label);

    return 0;
}