#include "queue.h"

int main(int argc, char *argv[])
{
    ifstream arrivalFile;
    ifstream pageFile;
    bool generateRandom = false;
    int avgInput;
    int secondsPerPage;
    int error;
    int idleTime = 0;
    int numOfDocs = 0;
    int clock = 0;
    int timeDequeued = 0;
    int endPrint = 0;
    document currDoc;
    myqueue<document> q1;
    srand((int)time(NULL));

    //sets error to value depending on whether invalid arguments were given in command line or not
    error = commandLineCheck(argc, argv, generateRandom, avgInput, secondsPerPage);
    // if an error was found
    if (error != 0)
    {
        cout << "Please run as prog4.exe, time that documents arrive on "
             << "average, number of seconds to print document, and whether to "
             << "use randomly generated times and pages or to use provided "
             << "files from arrivals.rand and pages.rand" << endl;
        return error;
    }
    if (!generateRandom && !openFiles(generateRandom, arrivalFile, pageFile))
        return 3;//unable to open files

    while (clock < 28800)
    {
        getData(generateRandom, avgInput, secondsPerPage, currDoc, arrivalFile, pageFile, clock);
        if (q1.isEmpty())
            idleTime += clock - endPrint;
        q1.enqueue(currDoc);
        q1.top(currDoc);
        if (q1.size() == 1)
            currDoc.time_started_print = currDoc.time_arrived;
        else
            currDoc.time_started_print = timeDequeued;
        endPrint = currDoc.time_started_print + (currDoc.pages * secondsPerPage);
        while (endPrint < clock)
        {
            q1.dequeue(currDoc);
            timeDequeued = endPrint;
            numOfDocs++;
            q1.top(currDoc);
            currDoc.time_started_print = endPrint;
            endPrint = currDoc.time_started_print + (currDoc.pages * secondsPerPage);
        }
    }

    printStats(avgInput, secondsPerPage, idleTime, numOfDocs, q1.size());

    if (!generateRandom)
        closeFiles(arrivalFile, pageFile);
    //program completed
    return 0;
}

//error check for command line
int commandLineCheck(int argc, char *argv[], bool &dataLoc, int &avgInput, int &secondsPerPage)
{
    //if there weren't 3 arguments added to the program
    if (argc != 4)
    {
        cout << "Incorrect Number of arguments" << endl;
        return 1;//incorrect number of arguments
    }

    try
    {
        avgInput = stol(argv[1]);
    }
    catch (exception)
    {
        cout << argv[1] << " is not a valid command line argument" << endl;
        return 2;//invalid command line arguments
    }
    try
    {
        secondsPerPage = stol(argv[2]);
    }
    catch (exception)
    {
        cout << argv[2] << " is not a valid command line argument" << endl;
        return 2;//invalid command line arguments
    }

    //if -r or -f are used
    if (strcmp(argv[3], "-r") == 0 || strcmp(argv[3], "-f") == 0)
    {
        //set dataLoc to true or false to determine which option was used
        (strcmp(argv[3], "-r") == 0 ? dataLoc = true : dataLoc = false);
    }
    //if neither were used
    else
    {
        cout << argv[3] << " is not a valid argument";
        return 2;//invalid command line arguments
    }
    //completion of function. No errors found.
    return 0;
}

bool openFiles(bool random, ifstream &arrival, ifstream &pages)
{
    if (random == false)
    {
        arrival.open("arrival.rand");
        //file error check
        if (!arrival)
        {
            cout << "Failed to open arrival.rand" << endl;
            return false;
        }
        pages.open("pages.rand");
        //file error check
        if (!pages)
        {
            cout << "Failed to open the pages.rand" << endl;
            arrival.close();//close file that was open
            return false;
        }
    }
    return true;
}

void getData(bool generateRandom, int avgInput, int secondsPerPage, document &doc, ifstream &arrivalFile, ifstream &pageFile, int &clock)
{
    //upper and lower bounds
    int min = avgInput - 30;
    int max = avgInput + 30;
    int maxPages = 20;
    int minPages = 10;
    //if -r
    if (generateRandom == true)
    {
        doc.time_arrived = rand();
        doc.pages = rand();
    }
    //if -f
    else
    {
        arrivalFile >> doc.time_arrived;
        pageFile >> doc.pages;
    }
    //setting value within bounds
    doc.time_arrived = ((doc.time_arrived % (max - min + 1)) + (min)) + clock;
    clock = doc.time_arrived;
    doc.pages = (doc.pages % (maxPages - minPages + 1)) + (minPages);
}

void printStats(int avgInput, int secondsPerPage, int idleTime, int numOfDocs, int size)
{
    cout << "Number of seconds between printing documents (arrival): " << avgInput << endl;
    cout << "Number of seconds to print a page: " << secondsPerPage << endl;
    cout << "Documents printed: " << numOfDocs << endl;
    cout << "Number of seconds printer idle: " << idleTime << endl;
    cout << "Number of documents still in queue: " << size << endl;
}

void closeFiles(ifstream &arrival, ifstream &pages)
{
    arrival.close();
    pages.close();
}
