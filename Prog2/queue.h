/*************************************************************************//**
 * @file
 *****************************************************************************/
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


/**
* @brief This is the template queue class that is able to hold any type of item
         needed.  This queue is implemented with a node structure that holds the
         item and a pointer to the next node in the queue.  In this program, the
         queue will be used to hold a document structure to simulate a printing
         queue.
*/
template <class _TY>
class myqueue
{
public:
    myqueue();/*!< The constructor of the queue*/
    ~myqueue();/*!< The destructor of the queue*/
    bool isEmpty();/*!< The isEmpty function of the queue used to see if the queue isEmpty
                        before inserting items.
                        @returns true - if the queue is empty
                        @returns false - If the queue is not empty*/
    bool enqueue(_TY item);/*!< The enqueue function of the queue used to insert
                                items as needed.
                                @param item - this is the item that is to be
                                              inserted into the queue
                                @returns true - if it inserted the item
                                @return  false - if it could not insert the item*/
    bool dequeue(_TY &item);/*!< The remove function of the queue used to remove
                                 items as needed.
                                 @param item this is the item that is to be
                                        removed from the queue
                                 @returns true - successfully deleted an item.
                                 @returns false - item could not be deleted.*/
    bool top(_TY &item);/*!< The function that allows users access to the first
                             item in the queue without it being removed from the
                             queue.
                             @param - item this is the item that is in the front
                                      of the queue.
                             @returns - true if headptr->item was copied to item
                             @returns - false if the list is empty*/
    int size();/*!< The size function of the queue used to count the number of items
                    in the queue.
                    @returns count - the number of items in the queue.*/
private:
    int count;/*!< Count Variable to keep track of number of items in the queue*/
    /*!
    * @brief Holds item and pointer to next node.
    */
    struct node
    {
        node *next;/*!< Next word in the queue*/
        _TY item;/*!< Item of the node */
    };
    node *headptr;/*!< First node in the queue.  This allows access to other nodes in queue*/
    node *tailptr;/*!< Last node in the queue.  This allows access to the last node in queue*/
};

/*!
* @brief This is the document structure that simulates everything needed to print
         a document.
*/
struct document
{
    int pages;/*!< number of pages in the document*/
    int time_arrived;/*!< The time the document arrived*/
    int time_dequeued;/*!< The time the document will be dequeued*/
    int time_started_print;/*!< The time the document will start printing*/
    int time_end_printing;/*!< The time the document will finish printing*/
};

int commandLineCheck(int argc, char *argv[], bool &dataLoc, int &avgInput, int &secondsPerPage);
bool openFiles(bool random, ifstream &arrival, ifstream &pages);
void simulatePrint(int &idleTime, int secondsPerPage, int avgInput,
	int &numOfDocs, bool generateRandom, document &currDoc, document &deqDoc,
	myqueue<document> &q1, ifstream &arrivalFile, ifstream &pageFile);
void getData(bool generateRandom, int avgInput, int secondsPerPage, document &doc, ifstream &arrivalFile, ifstream &pageFile);
void printStats(int avgInput, int secondsPerPage, int idleTime, int numOfDocs, int size);
void closeFiles(ifstream &arrival, ifstream &pages);

/**************************************************************************//**
 * @author Brandon Amundson
 *
 * @par Description:
 * This function creates/starts the queue by setting the queue to nullptr
 *****************************************************************************/
template <class _TY>
myqueue<_TY>::myqueue()
{
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

/**************************************************************************//**
 * @author Brandon Amundson, Jake Davidson
 *
 * @par Description:
 * This function deletes every member of the queue.
 *****************************************************************************/
template <class _TY>
myqueue<_TY>::~myqueue()
{
    node *temp = headptr;
    node *next = headptr;

    while (temp != nullptr)
    {
        next = temp->next;
        delete temp;
        temp = next;
    }
}


/**************************************************************************//** 
 * @author Brandon Amundson
 * 
 * @par Description: 
 *
 * Checks to see if the queue is empty by checking to see if headptr is set to
 * nullptr.
 * 
 * @returns true - if the queue is empty
 * @returns false - If the queue is not empty
 *****************************************************************************/
template <class _TY>
bool myqueue<_TY>::isEmpty()
{
    return (headptr == nullptr ? true : false);
}

/**************************************************************************//** 
 * @author Brandon Amundson
 * 
 * @par Description: 
 * Inserts item into the queue at the end of the queue and returns true or false
 * based on whether the insert was successful or not.
 * 
 * @param[in]      item - item that will be inserted into the lsit
 * 
 * @returns true - if it inserted the item
 * @return  false - if it could not insert the item
 *****************************************************************************/
template <class _TY>
bool myqueue<_TY>::enqueue(_TY item)
{
    node *temp;
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

/***************************************************************************//**
 * @author Brandon Amundson
 *
 * @par Description:
 * This is the remove function of the queue.  It is used when needing to
 * delete an item from the queue.
 *
 * @param[in,out]        item - the item that is being removed from the queue,
 *                          passed back to user for use.
 *
 * @returns true - successfully deleted item from queue.
 * @returns false - could not delete item from queue because queue is empty
 ******************************************************************************/
template <class _TY>
bool myqueue<_TY>::dequeue(_TY &item)
{
    node *temp;
    if (headptr == nullptr)
        return false;
    temp = headptr;
    headptr = headptr->next;
    item = temp->item;
    delete[] temp;
    count -= 1;
    return true;
}

/**************************************************************************//** 
 * @author Brandon Amundson
 * 
 * @par Description: 
 * Returns the first item in the queue to the user for later use.
 * 
 * @param[in,out] item - this is the first item in the queue that is passed back
 *                       to the user to allow them access to the front of the
 *                       queue without having to dequeue.
 *
 * @returns true - if we were able to set item to headptr->item
 * @returns false - If the queue is empty and there is nothing to return to user.
 *****************************************************************************/
template <class _TY>
bool myqueue<_TY>::top(_TY &item)
{
    if (headptr == nullptr)
        return false;
    item = headptr->item;
    return true;
}

 /**************************************************************************//**
* @author Brandon Amundson
*
* @par Description:
* This function returns the count variable of the private class data.  The count
* keeps track of the number of items in the queue.
* 
* @returns count - the number of words in the queue.  
*****************************************************************************/
template <class _TY>
int myqueue<_TY>::size()
{
    return count;
}
