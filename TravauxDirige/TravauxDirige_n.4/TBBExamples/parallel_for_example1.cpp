#include <iostream>
#include <tbb/parallel_for.h>
#include <tbb/tbb.h>
#include <mutex>

void print_hello( int i )
{
    std::cout << i << " : Hello world !" << std::endl;
}

int main()
{
	//auto partitioner = tbb::simple_partitioner();
	//auto partitioner = tbb::auto_partitioner();
	auto partitioner = tbb::static_partitioner();
	//auto partitioner = NULL;
    tbb::parallel_for(tbb::blocked_range<int>(0, 100, 2), 
    	[](tbb::blocked_range<int> &r) { 
    		for (auto i=r.begin(); i != r.end(); i++){
    			print_hello(i); 
    		}}, 
    	partitioner);
    return 0;
}
