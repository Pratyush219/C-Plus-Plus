/**
 * @file fcfs_scheduling.cpp
 * @brief Implementation of FCFS CPU scheduling algorithm
 * @details
 * FCFS is a non-preemptive CPU scheduling algorithm in which whichever process arrives first, gets executed first. 
 * If two or more processes arrive simultaneously, the process with smaller process ID gets executed first.
 * @link https://bit.ly/3ABNXOC
 * @author Pratyush Vatsa(https://github.com/Pratyush219)
*/

#include <iostream> // for IO operations
#include <vector> // for using vector
#include <unordered_set> // for using unordered_set
#include <queue> // for priority_queue
#include <iomanip> // for formatting the output
#include <cstdlib> // random number generation
#include <algorithm> // for sorting
#include <cassert> //for assert
#include <ctime>

using std::cin;
using std::cout;
using std::get;
using std::priority_queue;
using std::unordered_set;
using std::make_tuple;
using std::vector;
using std::tuple;
using std::endl;
using std::left;
using std::rand;
using std::srand;
/** 
 * @brief Comparator function for sorting of vector
 * @tparam S Data type of Process ID
 * @tparam T Data type of Arrival time
 * @tparam E Data type of Burst time 
 * @param t1 first tuple
 * @param t2 second tuple
 * @returns true if t1 and t2 are in CORRECT ORDER
 * @returns false if t1 and t2 are in INCORRECT ORDER
*/ 
template<typename S, typename T, typename E>
bool sortcol(tuple<S, T, E>& t1, tuple<S, T, E>& t2){
    if(get<1>(t1) < get<1>(t2)){
        return true;
    }
    else if(get<1>(t1) == get<1>(t2) && get<0>(t1) < get<0>(t2)){
        return true;
    }
    return false;
}

/** 
 * @class Compare
 * @brief Comparator class for priority queue
 * @tparam S Data type of Process ID
 * @tparam T Data type of Arrival time
 * @tparam E Data type of Burst time 
*/ 
template<typename S, typename T, typename E>
class Compare{
    public:
    /**
     * @param t1 first tuple
     * @param t2 second tuple 
     * @brief A comparator function that checks whether to swap the two tuples or not.
     * @link Refer to https://www.geeksforgeeks.org/comparator-class-in-c-with-examples/ for detailed description of comparator
     * @returns true if the tuples SHOULD be swapped
     * @returns false if the tuples SHOULDN'T be swapped
    */
    bool operator () (tuple<S, T, E, double, double, double>& t1, tuple<S, T, E, double, double, double>& t2){
        // Compare arrival times
        if(get<1>(t2) < get<1>(t1)){
            return true;
        }
        // If arrival times are same, then compare Process IDs
        else if(get<1>(t2) == get<1>(t1)){
            return get<0>(t2) < get<0>(t1);
        }
        return false;
    }
};

/**
 * @class FCFS
 * @brief Class which implements the FCFS scheduling algorithm
 * @tparam S Data type of Process ID
 * @tparam T Data type of Arrival time
 * @tparam E Data type of Burst time 
*/ 
template<typename S, typename T, typename E>
class FCFS{
    /**
     * Priority queue of schedules(stored as tuples) of processes.
     * In each tuple
     * 1st element: Process ID
     * 2nd element: Arrival Time
     * 3rd element: Burst time
     * 4th element: Completion time
     * 5th element: Turnaround time
     * 6th element: Waiting time
    */
    priority_queue<tuple<S, T, E, double, double, double>, vector<tuple<S, T, E, double, double, double>>, Compare<S, T, E>> schedule;

    // Stores final status of all the processes after completing the execution.
    vector<tuple<S, T, E, double, double, double>> result; 
    
    // Stores process IDs. Used for confirming absence of a process while adding it.
    unordered_set<S> idList;
    public:
    /**
     * @brief adds the process to the ready queue if it isn't already there
     * @param ID Process ID
     * @param arrival Arrival time of the process
     * @param burst Burst time of the process
     * @returns void
     * 
    */
    void addProcess(S id, T arrival, E burst){
        // Add if a process with process ID as id is not found in idList.
        if(idList.find(id) == idList.end()) {
            tuple<S, T, E, double, double, double> t = make_tuple(id, arrival, burst, 0, 0, 0);
            schedule.push(t);
            idList.insert(id);
        }
        
    }

    /**
     * @brief Algorithm for scheduling CPU processes according to the First Come First Serve(FCFS) scheduling algorithm.
     * 
     * @description FCFS is a non-preemptive algorithm in which the process which arrives first gets executed first. If two or 
     * more processes arrive together then the process with smaller process ID runs first (each process has a unique proces ID).
     * 
     * I used a min priority queue of tuples to accomplish this task. The processes are ordered by their arrival times. If arrival
     * times of some processes are equal, then they are ordered by their process ID.
     * 
     * @returns void
    */ 
    vector<tuple<S, T, E, double, double, double>> scheduleForFcfs(){
        // Variable to keep track of time elapsed so far
        double timeElapsed = 0;

        while(!schedule.empty()){
            tuple<S, T, E, double, double, double> cur = schedule.top();
            
            // If the current process arrived at time t2, the last process completed its execution at time t1, and t2 > t1.
            if(get<1>(cur) > timeElapsed){
                timeElapsed += get<1>(cur) - timeElapsed;
            }

            // Add Burst time to time elapsed
            timeElapsed += get<2>(cur);
            
            // Completion time of the current process will be same as time elapsed so far
            get<3>(cur) = timeElapsed;
            
            // Turnaround time = Completion time - Arrival time
            get<4>(cur) = get<3>(cur) - get<1>(cur);

            // Waiting time = Turnaround time - Burst time
            get<5>(cur) = get<4>(cur) - get<2>(cur);

            result.push_back(cur);
            schedule.pop();
        }
        return result;
    }

    /**
     * @brief Utility function for printing the status of each process after execution
     * @returns void
    */ 
    void printResult(){
        cout << "Status of all the proceses post completion is as follows:" << endl;

        cout << std::setw(17) << left << "Process ID"
             << std::setw(17) << left << "Arrival Time"
             << std::setw(17) << left << "Burst Time"
             << std::setw(17) << left << "Completion Time"
             << std::setw(17) << left << "Turnaround Time"
             << std::setw(17) << left << "Waiting Time" << endl;
             
        for(size_t i{}; i < result.size(); i++){
            cout << std::setprecision(2) << std::fixed 
                 << std::setw(17) << left << get<0>(result[i])
                 << std::setw(17) << left << get<1>(result[i])
                 << std::setw(17) << left << get<2>(result[i])
                 << std::setw(17) << left << get<3>(result[i])
                 << std::setw(17) << left << get<4>(result[i])
                 << std::setw(17) << left << get<5>(result[i]) << endl;
        }
    }
    

};

template<typename S, typename T, typename E>
vector<tuple<S, T, E, double, double, double>> get_final_status(vector<tuple<uint32_t, uint32_t, uint32_t>>& input){
    sort(input.begin(), input.end(), sortcol<S, T, E>);
    vector<tuple<S, T, E, double, double, double>> result(input.size());
    double timeElapsed = 0;
    for(size_t i{}; i < input.size(); i++){
        T arrival = get<1>(input[i]);
        E burst = get<2>(input[i]);

        if(arrival > timeElapsed){
            timeElapsed += arrival - timeElapsed;
        }
        timeElapsed += burst;
        double completion = timeElapsed;
        double turnaround = completion - arrival;
        double waiting = turnaround - burst;

        get<0>(result[i]) = get<0>(input[i]);
        get<1>(result[i]) = arrival;
        get<2>(result[i]) = burst;
        get<3>(result[i]) = completion;
        get<4>(result[i]) = turnaround;
        get<5>(result[i]) = waiting;

    }
    return result;
}

void test(){
    for(int i{}; i < 1000; i++){
        srand(time(0));
        int n = 1 + rand()%1000;
        FCFS<uint32_t ,uint32_t, uint32_t> readyQueue;
        vector<tuple<uint32_t, uint32_t, uint32_t>> input(n);

        for(int i{}; i < n; i++){
            get<0>(input[i]) = i;
            srand(time(0));
            get<1>(input[i]) = 1 + rand()%10000;
            srand(time(0));
            get<2>(input[i]) = 1 + rand()%10000;
        }

        for(uint32_t i{}; i < n; i++){
            readyQueue.addProcess(get<0>(input[i]), get<1>(input[i]), get<2>(input[i]));
        }
        vector<tuple<uint32_t ,uint32_t, uint32_t, double, double, double>> res = get_final_status<uint32_t ,uint32_t, uint32_t>(input);
        assert(res == readyQueue.scheduleForFcfs());
        // readyQueue.printResult();
    }
    cout << "All tests passed" << endl;

}

/**
 * @brief Entry point of the program
 * @returns 0 on exit
*/
int main(){
    // FCFS<uint32_t ,uint32_t, uint32_t> readyQueue;

    // //Sample test case
    // int n = 3;
    // vector<tuple<uint32_t, uint32_t, uint32_t>> input = {
    //     make_tuple(1, 0, 30),
    //     make_tuple(2, 0, 5),
    //     make_tuple(3, 0, 5)
    // };

    // for(uint32_t i{}; i < n; i++){
    //     readyQueue.addProcess(get<0>(input[i]), get<1>(input[i]), get<2>(input[i]));
    // }

    // readyQueue.scheduleForFcfs();
    test();
    return 0;
}
