#include "UPC_Scanner.h"


using namespace std;
using namespace cv;

UPC_Scanner::UPC_Scanner()
{
    
}


UPC_Scanner::~UPC_Scanner()
{
    
}

deque<int> UPC_Scanner::scaningBarcode(cv::Mat& image)
{
    Point currentPosition(0, image.size().height / 2);
    skipBarOrWhiteSpace(image, currentPosition);
    
    int widthOfBar = scanFirstBar(image, currentPosition);
    
    //Zahl des Barcodes
    deque<int> barcodeNumbers;
    
    //Den linken Bereich auslesen
    for (int i = 0; i < NUMBERCOUNTER; ++i)
    {
        int num = readBars(image, currentPosition, widthOfBar, LEFT);
        barcodeNumbers.push_back(num);
        correctScannerPosition(image, currentPosition, WHITEBAR, BLACKBAR);
    }
    
    int middleOfMarker = sizeof(MIDDLE_MARKER)/sizeof(MIDDLE_MARKER[0]);
    for (int i = 0; i < middleOfMarker; ++i)
    {
        skipBarOrWhiteSpace(image, currentPosition);
    }
    
    //Den rechten Bereich auslesen
    for (int i = 0; i < NUMBERCOUNTER; ++i)
    {
        int num = readBars(image, currentPosition, widthOfBar, RIGHT);
        barcodeNumbers.push_back(num);
        correctScannerPosition(image, currentPosition, BLACKBAR, WHITEBAR);
    }
    
    return barcodeNumbers;
}