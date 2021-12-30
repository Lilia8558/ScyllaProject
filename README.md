# ScyllaProject

Summary:

Given class VectorProcessor which contains public numeric template function process with argument std::vector<numeric type> and this vector size is predefined.
Task is to apply some operation: in this case we just simply multiply by 5 on every single element of a vector in different threads and return the changed vector.
  
Implementation:
  
In this case we have 8 threads and we want to do some operation with our vector elements using that 8 different threads.
  
For that we have a helper function split_vector it's responsible for spliting vector to 8 pairs. 
Function returns vector of pairs where each pair represents start and end indexes for splited vector.
  
In function process we our creating threads, after spliting vector now we can give each thread it's own part of vector.
For thread work synchronization we are using std::condition_variable. 
