#include<vector>
#include<future>
#include<iostream>

int main(int argc,char* argv[]){
	std::vector<std::future<int>> futures;
	for (int i = 0; i < 5; i++) {
		int v = i;
		futures.emplace_back(std::async(std::launch::async, [&v] {return v; }));
	}
	for (int i = 0; i < 5; i++) {
		futures[i].wait();
		std::cout << futures[i].get() << std::endl;
		system("pause");
	}

}