# ScyllaProject

Summary:

Given class VectorProcessor which contains public numeric template function process with argument std::vector<numeric type> and this vector size is predefined.
Task is to apply some operation: in this case we just simply multiply by 5 on every single element of a vector in different threads and return the changed vector.
  
Generic approach:

Vector was processed via many threads, via passing independent ranges to the threads, which are synced via condition variable.

In this case we have 8 threads and we want to do some operation with our vector elements using that 8 different threads.
  
For that we have a helper function split_vector which is responsible for spliting vector to 8 pairs. 
Function returns vector of pairs where each pair represents start and end indexes for splited vector.
  
Build:

Necessary environment to run the demo: Visual Studio 2019 or higher.
As it's a Visual Studio project you just need to download the repo and build via MSVC.
