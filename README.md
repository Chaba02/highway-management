# Highway Management System

This is my academic project for my Data Structures & Algorithms course.  
It is designed to have optimal time and space complexity.
Final grade: 30/30

## How it works
Suppose to be in a highway which can be run both ways. At some point there are stations which have parking lots with a maximum of 512 cars.   
The goal is to provide the best route for a person in order to go from a station to another one in the minimum number of possible stops.  
Each car has an autonomy and in case two paths has the exact same number of stops the software choose the nearest stop to the referring point, which is 0.

These are the commands: 
- Add car
- Add station
- Demolish car
- Demolish station
- Plan best route

### Plan best route

Suppose have the following highway: 0 - 5 - 20 - 40 - 50
Each number represents the station (0km, 2km, 4km and so on..)

Format: (Station #: car_autonomy)  
Station 0: 50km  
Station 5: 20km  
Station 20: 10km  
Station 40: 20km  
Station 50: 0km  
```
Input: 5 -> 40
Output: 5 20 40
```

And the same procedure is followed for the backwards

```
Input: 40 -> 0
Output: No route available
```


## Built With

* C - The programming language used
* Valgrind - Used to debug and memory leak
* CLion - IDE


## Authors

* **Omar Chaabani** - [Chaba02](https://github.com/Chaba02)


