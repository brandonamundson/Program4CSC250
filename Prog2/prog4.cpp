#include "queue.h"

int commandLineCheck(int argc, char *argv[], bool &dataLoc, int &avgInput, int &secondsPerPage);
bool openFiles(bool random, ifstream &arrival, ifstream &pages);
void getData(bool generateRandom, int avgInput, int secondsPerPage, int &numPages, int &arrivalTime, ifstream &arrivalFile, ifstream &pageFile);
void printStats(int avgInput, int secondsPerPage, int idleTime, int numOfDocs);

int main(int argc, char *argv[])
{
    bool generateRandom = false;
    ifstream arrivalFile;
    ifstream pageFile;
    int avgInput;
    int secondsPerPage;
    int numPages;
    int arrivalTime;
    int idleTime = 0;
    int numOfDocs = 0;
    int clock = 0;
    int lastArrivalTime = 0;
    int lastPageCount = 0;
    int timeBetweenPrintJobs;
    int timeToPrint;
    int error;
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
        getData(generateRandom, avgInput, secondsPerPage, numPages, arrivalTime, arrivalFile, pageFile);

        timeBetweenPrintJobs = (arrivalTime - (lastPageCount * secondsPerPage));
        timeToPrint = (numPages * secondsPerPage);

        // this works even when clock is zero
        if (timeBetweenPrintJobs >= 0)
        {
            idleTime += timeBetweenPrintJobs;
            clock += timeBetweenPrintJobs;
        }

        clock += timeToPrint;
        //arrivalTime;
        //numPages;
        lastPageCount = numPages;
        lastArrivalTime = arrivalTime;

        numOfDocs++;

    } while (clock < 28800);

    printStats(avgInput, secondsPerPage, idleTime, numOfDocs);

    if (generateRandom = false)
    {
        arrivalFile.close();
        pageFile.close();
    }
    return 0;

}

int commandLineCheck(int argc, char *argv[], bool &dataLoc, int &avgInput, int &secondsPerPage)
{
    if (argc == 4)
    {
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
    }
    else
    {
        cout << "Incorrect Number of arguments" << endl;
        return 1;//incorrect number of arguments
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

void getData(bool generateRandom, int avgInput, int secondsPerPage, int &numPages, int &arrivalTime, ifstream &arrivalFile, ifstream &pageFile)
{
    int min = avgInput - 30;
    int max = avgInput + 30;
    int maxPages = 20;
    int minPages = 10;

    if (generateRandom == true)
    {
        arrivalTime = rand();
        numPages = rand();
    }
    else
    {
        arrivalFile >> arrivalTime;
        pageFile >> numPages;
    }
    arrivalTime = (arrivalTime % (max - min + 1)) + (min);
    numPages = (numPages % (maxPages - minPages + 1)) + (minPages);
}

void printStats(int avgInput, int secondsPerPage, int idleTime, int numOfDocs)
{
    cout << "Number of seconds between printing documents (arrival): " << avgInput << endl;
    cout << "Number of seconds to print a page: " << secondsPerPage << endl;
    cout << "Documents printed: " << numOfDocs << endl;
    cout << "Number of seconds printer idle: " << idleTime << endl;
}


