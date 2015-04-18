#pragma once

#define _CRT_SECURE_NO_DEPRECATE

#include <time.h>
#include <iostream>
#include <fstream>
#include <string>
#include <cctype>
#include <cstring>
#include <iomanip>
#include <cstdlib>

using namespace std;

int commandLineCheck(int argc, char *argv[], bool &dataLoc, int &avgInput, int &secondsPerPage);
bool openFiles(bool random, ifstream &arrival, ifstream &pages);
void getData(bool generateRandom, int avgInput, int secondsPerPage, int &numPages, int &arrivalTime, ifstream &arrivalFile, ifstream &pageFile);
void printStats(int avgInput, int secondsPerPage, int idleTime, int numOfDocs);


template <class _TY>
class myqueue
{
public:
    myqueue();
    ~myqueue();
    bool isEmpty();
    bool push(_TY item);
    bool pop(_TY &item);
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
template <class _TY>
myqueue<_TY>::myqueue()
{
    headptr = new (nothrow)node;
    if (headptr == nullptr)
        return;
    headptr = nullptr;
    headptr->next = nullptr;
    headptr->item = 0;
    tailptr = new (nothrow)node;
    if (tailptr == nullptr)
        return;
    tailptr = nullptr;
    tailptr->next = nullptr;
    tailptr->item = 0;
    count = 0;

}

template <class _TY>
myqueue<_TY>::~myqueue()
{
    delete[] headptr;
    headptr = nullptr;
    headptr->next = nullptr;
    tailptr = nullptr;
    tailptr->next = nullptr;
}

template <class _TY>
bool myqueue<_TY>::isEmpty()
{
    return (headptr == nullptr ? true : false)
}

template <class _TY>
bool myqueue<_TY>::push(_TY item)
{
    node *temp;
    node *curr;
    node *prev;
    temp = new (nothrow)node;
    if (temp == nullptr)
        return false;
    temp->next = nullptr;
    temp->item = item;
    if (headptr == nullptr)
    {
        headptr = temp;
        tailptr = temp;
        count += 1;
        return true;
    }
    tailptr->next = temp;
    tailptr = temp;
    count += 1;
    return true;
}

template <class _TY>
bool myqueue<_TY>::pop(_TY &item)
{
    node *temp;
    node *prev;
    node *curr;
    if (headptr == nullptr)
        return false;
    temp = headptr;
    headptr = headptr->next;
    item = temp->item;
    delete[] temp;
    count -= 1;
    return true;
}

template <class _TY>
bool myqueue<_TY>::top(_TY &item)
{
    if (headptr == nullptr)
        return false;
    item = headptr->item;
    return true;
}

template <class _TY>
int myqueue<_TY>::size()
{
    return count;
}
