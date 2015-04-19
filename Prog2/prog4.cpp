#include "queue.h"

int main(int argc, char *argv[])
{
    bool generateRandom = false;
    ifstream arrivalFile;
    ifstream pageFile;
    int avgInput;
    int secondsPerPage;
    int idleTime = 0;
    int numOfDocs = 0;
    int clock = 0;
    int lastArrivalTime = 0;
    int lastPageCount = 0;
    int timeBetweenPrintJobs;
    int timeToPrint;
    int error;
    document doc;
    myqueue<document> q1;
    srand((int)time(NULL));

    error = commandLineCheck(argc, argv, generateRandom, avgInput, secondsPerPage);

    if (error != 0)
    {
        return error;
    }

    if (generateRandom == false)
    {
        if (openFiles(generateRandom, arrivalFile, pageFile) == false)
        {
            return 3;//unable to open files
        }
    }

    do
    {
        getData(generateRandom, avgInput, secondsPerPage, doc, arrivalFile, pageFile);

        timeBetweenPrintJobs = (doc.time_arrived - (lastPageCount * secondsPerPage));
        timeToPrint = (doc.pages * secondsPerPage);

        // this works even when clock is zero
        if (timeBetweenPrintJobs >= 0)
        {
            idleTime += timeBetweenPrintJobs;
            clock += timeBetweenPrintJobs;
        }

        clock += timeToPrint;
        //arrivalTime;
        //numPages;
        lastPageCount = doc.pages;
        lastArrivalTime = doc.time_arrived;

        numOfDocs++;

    } while (clock < 28800);

    printStats(avgInput, secondsPerPage, idleTime, numOfDocs);

    if (generateRandom == false)
    {
        arrivalFile.close();
        pageFile.close();
    }
    return 0;

}

int commandLineCheck(int argc, char *argv[], bool &dataLoc, int &avgInput, int &secondsPerPage)
{
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

        if (strcmp(argv[3], "-r") == 0 || strcmp(argv[3], "-f") == 0)
        {
            (strcmp(argv[3], "-r") == 0 ? dataLoc = true : dataLoc = false);
        }
        else
        {
            cout << argv[3] << " is not a valid argument";
            return 2;//invalid command line arguments
        }
    return 0;
}

bool openFiles(bool random, ifstream &arrival, ifstream &pages)
{
    if (random == false)
    {
        arrival.open("arrival.rand");
        if (!arrival)
        {
            cout << "Failed to open arrival.rand" << endl;
            return false;
        }
        pages.open("pages.rand");
        if (!pages)
        {
            cout << "Failed to open the pages.rand" << endl;
            arrival.close();//close file that was open
            return false;
        }
    }

    return true;
}

void getData(bool generateRandom, int avgInput, int secondsPerPage, document &doc, ifstream &arrivalFile, ifstream &pageFile)
{
    int min = avgInput - 30;
    int max = avgInput + 30;
    int maxPages = 20;
    int minPages = 10;

    if (generateRandom == true)
    {
        doc.time_arrived = rand();
        doc.pages = rand();
    }
    else
    {
        arrivalFile >> doc.time_arrived;
        pageFile >> doc.pages;
    }
    doc.time_arrived = (doc.time_arrived % (max - min + 1)) + (min);
    doc.pages = (doc.pages % (maxPages - minPages + 1)) + (minPages);
}

void printStats(int avgInput, int secondsPerPage, int idleTime, int numOfDocs)
{
    cout << "Number of seconds between printing documents (arrival): " << avgInput << endl;
    cout << "Number of seconds to print a page: " << secondsPerPage << endl;
    cout << "Documents printed: " << numOfDocs << endl;
    cout << "Number of seconds printer idle: " << idleTime << endl;
}

