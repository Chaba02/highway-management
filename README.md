# Highway System Management
This code provides a system of management of a highway  
> add a station  
> remove a station  
> add a car  
> remove a car  
> plan fastest trip (both directions)

To provide the map of the highway I used an array with a linked list to create a sort of a hashmap in order to access to elements in ~ O(1).  
The functions that plan the fastest trip are designed to pick, among all the fastest trip, the one with the stations closer to 0, here is an example:  
Between 10 -> 20 -> 40 -> 60 and 10 -> 30 -> 50 -> 60 , the program will pick the first one. Same with the backwards path.
All the functions are designed to perform well for large data set in order to provide an optimal level of time execution.
