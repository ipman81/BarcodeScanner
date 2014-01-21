#include "EAN_Scanner.h"

using namespace std;
using namespace cv;


EAN_Scanner::EAN_Scanner(void)
{
    
}
EAN_Scanner::~EAN_Scanner(void)
{
    
}

const bool CODINGOF13THEDIGIT[CODINGOF13THDIGITCOUNTER][NUMBERCOUNTER] =
{
    { UNEQUAL, UNEQUAL, UNEQUAL, UNEQUAL, UNEQUAL, UNEQUAL },   //0
    { UNEQUAL, UNEQUAL, EQUAL, UNEQUAL, EQUAL, EQUAL },         //1
    { UNEQUAL, UNEQUAL, EQUAL, EQUAL, UNEQUAL, EQUAL },         //2
    { UNEQUAL, UNEQUAL, EQUAL, EQUAL, EQUAL, UNEQUAL },         //3
    { UNEQUAL, EQUAL, UNEQUAL, UNEQUAL, EQUAL, EQUAL },         //4
    { UNEQUAL, EQUAL, EQUAL, UNEQUAL, UNEQUAL, EQUAL },         //5
    { UNEQUAL, EQUAL, EQUAL, EQUAL, UNEQUAL, UNEQUAL },         //6
    { UNEQUAL, EQUAL, UNEQUAL, EQUAL, UNEQUAL, EQUAL },         //7
    { UNEQUAL, EQUAL, UNEQUAL, EQUAL, EQUAL, UNEQUAL },         //8
    { UNEQUAL, EQUAL, EQUAL, UNEQUAL, EQUAL, UNEQUAL }          //9    
};


bool EAN_Scanner::checkEquality(bool equalOrUnequal[], int value)
{
    for (int i = 0; i < NUMBERCOUNTER; i++)
    {
        if (equalOrUnequal[i] != CODINGOF13THEDIGIT[value][i])
        {
            return false;
        }
    }
    return true;
    
}


deque<int> EAN_Scanner::scaningBarcode(cv::Mat& image)
{
    //left center of scanner begin
    Point currentPosition(0, image.size().height/ 2);
    
    int widthOfBar = scanFirstBar(image, currentPosition);
    deque<int> barcodeNumbers;
    bool unEqual[6];
    
    //left side to read
    for (int i = 0; i < NUMBERCOUNTER; i++)
    {
        int num = readBars(image, currentPosition, widthOfBar, LEFT);
        //cout << "num" << num << endl;
        
        if (num <= CODINGOF13THDIGITCOUNTER)
        {
            unEqual[i] = UNEQUAL;
        }
        else
        {
            unEqual[i] = EQUAL;
        }
        //modulo 10 to get 0-9 digits
        barcodeNumbers.push_back(num % 10);
        
        correctScannerPosition(image, currentPosition, WHITEBAR, BLACKBAR);
    }
    
    //left side to check
    for (int i = 0; i < CODINGOF13THDIGITCOUNTER; i++)
    {
        if (checkEquality(unEqual, i))
        {
            barcodeNumbers.push_front(i);
        }
    }
    //skip middle marker
    int middleOfMarker = sizeof(MIDDLE_MARKER)/sizeof(MIDDLE_MARKER[0]);
    
    for (int i = 0; i < middleOfMarker; ++i)
    {
        skipBarOrWhiteSpace(image, currentPosition);
    }
    
    //right side to read
    for (int i = 0; i < NUMBERCOUNTER; i++)
    {
        int num = readBars(image, currentPosition, widthOfBar, RIGHT);
        barcodeNumbers.push_back(num);
        correctScannerPosition(image, currentPosition, BLACKBAR, WHITEBAR);
        
    }
    return barcodeNumbers;
}