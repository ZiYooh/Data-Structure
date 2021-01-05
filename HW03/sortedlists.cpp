// 201624453 HW03
#include <iostream>
#include <fstream>
#include <queue>
#include <vector>
#include <algorithm>

using namespace std;

vector<int> heap;

void makeHeap(int n, int count){
	int node = n*2;

	if(node < count && heap[node] < heap[node+1]){
		node++;
	}

	if(heap[n] < heap[node]){
		swap(heap[n],heap[node]);

		if(node <= count/2){
			makeHeap(node, count);
		}
	}
}

void heapSort(int n){
	int root_node = 1;
	int node = 2;

	swap(heap[1],heap[n]);

	while(node/2 < n){
		node = 2*root_node;

		if(node < n-1 && heap[node] < heap[node+1]){
			node++;
		}

		if(node < n && heap[root_node] < heap[node]){
			swap(heap[root_node], heap[node]);
		}

		root_node = node;
	}
}

int main() {
	ifstream fin;
	ofstream fout;
	fin.open("lists.in");
	fout.open("list.out");

	int temp;
	queue<int> input;

	fin >> temp;
	while(!fin.eof()){
		input.push(temp);
		fin >> temp;
	}

	int count = input.size();

	heap.push_back(-1);

	for(int i = 1; i <= count; i++){
		heap.push_back(input.front());
		input.pop();
	}

	for(int j = count/2; j > 0; j--){
		makeHeap(j, count);
	}

	/*
	for(int n = 1; n <= count; n++){
		cout << heap[n] << " ";

	}
	cout << endl;
	*/

	for(int k = count; k > 0; k--){
		heapSort(k);
	}

	for(int n = 1; n <= count; n++){
		//cout << heap[n] << " ";
		fout << heap[n];
		if(n != count){
            fout << " ";
		}
	}
	cout << endl;
	cout << "File Write Success..." << endl;

	fin.close();
}
