#include <iostream>
#include <vector>
#include <bits/stdc++.h>
#include <vector>
#include <map>
#include "producer.cpp"
#include "consumer.cpp"
using namespace std;

void getInput(int count);
void sortByArrival(vector<Process> p);
void scheduler(int n);
int qCheck(vector<Process> q, int time);

struct Process
{
    string ID;
    // arrival, burst, process, interrupt
    int AT, BT, PT, IT;

    bool operator< (const Process &other) const{
        return AT < other.AT;
    }
};

// make our queues
vector<Process> Q0;
vector<Process> Q1;
vector<Process> Q2;
vector<Process> done;

// time for scheduler
int time = 0;
// names for commentator function
const map<vector<Process>, string> Q_NAMES{
    {Q0, "Queue 0"},
    {Q1, "Queue 1"},
    {Q2, "Queue 2"}
};

int main()
{
    producer();

    // call consumer to get input from shared memory
    vector<string> input = consumer();

    // parse input and put into Q0
    parse(input);

    // sorts processes by arrival time
    sortByArrival(Q0);

    // run scheduler, should have commentator function
    scheduler();

    return 0;
}

// takes in consumer vector, insert the input into structs
// insert structs into the queues
// ex. [1, p1, 0, 10]
void parse(vector<string> input)
{
    for (int i = 0; i < count; i++)
    {
        Process temp;
        Q0.push_back(temp);
        cout << OUTPUT[0];
        cin >> Q0[i].ID;

        cout << OUTPUT[1];
        cin >> Q0[i].AT;

        cout << OUTPUT[2];
        cin >> Q0[i].BT;
    }
}

void sortByArrival(vector<Process> p)
{
    sort(p.begin(), p.end());
}

// n is number of processes
void scheduler()
{
    // while there is at least 1 process in some queue
    while (!Q0.empty() || !Q1.empty() || !Q2.empty())
    {
        if (qCheck(Q0) != -1)
        {
            RoundRobin(qCheck(Q0), 8, Q0);
        }
        else if (qCheck(Q1) != -1)
        {
            RoundRobin(qCheck(Q1), 16, Q1);
        }
        else
        {
            // only ran when other q's are empty or p's not arrived
            completeProcess(Q2, 0);
        }
    }
}

/* check if there is a process to be executed
returns -1 if no such process is found */
int qCheck(vector<Process> q)
{
    for (int i = 0; i < q.size(); i++)
    {
        if (q[i].AT <= time)
        {
            return i;
        }
    }

    return -1;
}

void RoundRobin(int idx, int q, vector<Process> queue)
{
    int BT = queue[idx].BT;
    if (BT - queue[idx].PT <= q)
    {
        completeProcess(queue, idx);
    }
    else
    {
        // increment time
        time += q;
        // add the quantum to the process time
        queue[idx].PT += q;
        // add to the interrupt count
        queue[idx].IT += 1;
        // output to user what's happening
        commentator(queue[idx], queue, q, false);
        // move the process to the next queue and delete from current
        if(queue == Q0)
        {
            Q1.push_back(queue[idx]);
            queue.erase(queue.begin() + idx);
        }
        else if(queue == Q1)
        {
            Q2.push_back(queue[idx]);
            queue.erase(queue.begin() + idx);
        }
        else
        {
            cerr << "Incorrect queue entered for Round Robin";
            exit(1);
        }
    }
}

// when the quantum of the queue is >= the burst time
void completeProcess(vector<Process> queue, int i)
{
    // increment time remaining to finish process
    time += queue[i].BT - queue[i].PT;
    // set the process time to the burst
    queue[i].PT = queue[i].BT;
    // let user know what's happening
    commentator(queue[i], queue, queue[i].BT - queue[i].PT, true);
    // the process is complete, so move it to done
    done.push_back(queue[idx]);
    // remove process from queue
    queue.erase(queue.begin() + idx);
}

void commentator(Process p, vector<string> q, int length, bool done)
{
    if (done)
    {
        cout << p.ID << " is interrupted " << p.IT <<\
        " time(s) and completes on " << Q_NAMES[q] <<\
        ", TAT for " << p.ID << " is " << time - p.BT;
    }
    else
    {
        cout << p.ID << " at " << Q_NAMES[q] << " it is executed for " << length << "\n";
    }
}
