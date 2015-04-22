#pragma once

#define _CRT_SECURE_NO_DEPRECATE

//libraries
#include <time.h>
#include <iostream>
#include <fstream>
#include <string>
#include <cctype>
#include <cstring>
#include <iomanip>
#include <cstdlib>

using namespace std;


//template class
template <class _TY>
class myqueue
{
public:
    myqueue();
    ~myqueue();
    bool isEmpty();
    bool enqueue(_TY item);
    bool dequeue(_TY &item);
    bool top(_TY &item);
    int size();
private:
    int count;
    struct node
    {
        node *next;
        _TY item;
    };
    node *headptr;
    node *tailptr;
};

//document info
struct document
{
    int pages;
    int time_arrived;
    int time_dequeued;
    int time_started_print;
    int time_end_printing;
};

int commandLineCheck(int argc, char *argv[], bool &dataLoc, int &avgInput, int &secondsPerPage);
bool openFiles(bool random, ifstream &arrival, ifstream &pages);
void getData(bool generateRandom, int avgInput, int secondsPerPage, document &doc, ifstream &arrivalFile, ifstream &pageFile);
void printStats(int avgInput, int secondsPerPage, int idleTime, int numOfDocs);

//constructor
template <class _TY>
myqueue<_TY>::myqueue()
{
    //assigning initial values to headptr, tailptr, and count
    headptr = new (nothrow)node;
    if (headptr == nullptr)
        return;
    headptr = nullptr;
    tailptr = new (nothrow)node;
    if (tailptr == nullptr)
        return;
    tailptr = nullptr;
    count = 0;

}

//destructor
template <class _TY>
myqueue<_TY>::~myqueue()
{
    node *temp = headptr;
    node *next = headptr;

    //walk through nodes freeing up memory
    while (temp != nullptr)
        {
        next = temp->next;
        delete temp;
        temp = next;
        }
    delete headptr;
    delete tailptr;
}

//Empty function
template <class _TY>
bool myqueue<_TY>::isEmpty()
{
    return (headptr == nullptr ? true : false)
}

//push
template <class _TY>
bool myqueue<_TY>::enqueue(_TY item)
{
    //nodes created
    node *temp;
    //error check and initial assignment for temp
    temp = new (nothrow)node;
    if (temp == nullptr)
        return false;
    temp->next = nullptr;
    temp->item = item;
    //if empty
    if (headptr == nullptr)
    {
        headptr = temp;
        tailptr = temp;
        count += 1;
        return true;
    }
    //if not empty
    tailptr->next = temp;
    tailptr = temp;
    count += 1;
    return true;
}

//pop
template <class _TY>
bool myqueue<_TY>::dequeue(_TY &item)
{
    //nodes created
    node *temp;
    //if empty
    if (headptr == nullptr)
        return false;
    //if not empty
    temp = headptr;
    headptr = headptr->next;
    item = temp->item;
    delete[] temp;
    count -= 1;
    return true;
    /*shouldn't there be an error check and/or nothrow for temp?
    No because we are deleting and just need temp to hold the original headptr
    we are not creating a new node as much as we are temporarily holding it while
    we shift headptr to remove the original headptr.*/
}

//top
template <class _TY>
bool myqueue<_TY>::top(_TY &item)
{
    //if empty
    if (headptr == nullptr)
        return false;
    //if not empty
    item = headptr->item;
    return true;
}

//size
template <class _TY>
int myqueue<_TY>::size()
{
    //gives count
    return count;
}
