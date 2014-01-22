#include "BarcodeAnalyser.h"

using namespace cv;
using namespace std;

BarcodeAnalyser::BarcodeAnalyser(void)
{
}

BarcodeAnalyser::~BarcodeAnalyser(void)
{
}

//Codingtable
const int EQUALTOKEN[EQUALTOKENCOUNTER][BLOCKOFFOUR] =
{
    //Right
    {3, 2, 1, 1}, // 0
    {2, 2, 2, 1}, // 1
    {2, 1, 2, 2}, // 2
    {1, 4, 1, 1}, // 3
    {1, 1, 3, 2}, // 4
    {1, 2, 3, 1}, // 5
    {1, 1, 1, 4}, // 6
    {1, 3, 1, 2}, // 7
    {1, 2, 1, 3}, // 8
    {3, 1, 1, 2}  // 9
};

const int UNEQUALEQUALTOKEN[UNEQUALEQUALTOKENCOUNTER][BLOCKOFFOUR] =
{
    //left unequal
    {3, 2, 1, 1}, // 0
    {2, 2, 2, 1}, // 1
    {2, 1, 2, 2}, // 2
    {1, 4, 1, 1}, // 3
    {1, 1, 3, 2}, // 4
    {1, 2, 3, 1}, // 5
    {1, 1, 1, 4}, // 6
    {1, 3, 1, 2}, // 7
    {1, 2, 1, 3}, // 8
    {3, 1, 1, 2}, // 9
    
    //left equal
    {1, 1, 2, 3}, // 0 == 10
    {1, 2, 2, 2}, // 1 == 11
    {2, 2, 1, 2}, // 2 == 12
    {1, 1, 4, 1}, // 3 == 13
    {2, 3, 1, 1}, // 4 == 14
    {1, 3, 2, 1}, // 5 == 15
    {4, 1, 1, 1}, // 6 == 16
    {2, 1, 3, 1}, // 7 == 17
    {3, 1, 2, 1}, // 8 == 18
    {2, 1, 1, 3}  // 9 == 19
};

void BarcodeAnalyser::correctScannerPosition(const Mat& image, Point &currentPosition, int begin, int end)
{
    if (image.at<uchar>(currentPosition.y,currentPosition.x) == end)
    {
        while (image.at<uchar>(currentPosition.y,currentPosition.x) == end)
            currentPosition.x++;
    }
    else
    {
        while (image.at<uchar>(currentPosition.y,currentPosition.x-1) == begin)
            currentPosition.x--;
    }
}

//this method read bars and put it in an array of size 4 to calculate this array in a number of barcode
int BarcodeAnalyser::readBars(Mat& image, Point &currentPosition, int widthOfBar, int leftOrRight)
{
    
    int tokens[BLOCKOFFOUR]={0,0,0,0};
    
    //one bar (white or black) is one digit, and 7 digits are one number
    int endPosition = currentPosition.x + (NUMBERIS7BITS * widthOfBar) -1;
    int counterPositionOfTokenArray = 0;
    
    // color switching (bar switching)
    unsigned int otherColor=image.at<uchar>(currentPosition.y,currentPosition.x);
    otherColor = switchingEqualColors(image.at<uchar>(currentPosition.y,currentPosition.x), otherColor);
    
    //Default failure return -1
    int calculatedNumberOfBarcode = -1;
    
    while (currentPosition.x <= endPosition)
    {
        if (getColorOfBar(image, currentPosition) != otherColor)
        {
            tokens[counterPositionOfTokenArray]++;
        }
        else
        {
            counterPositionOfTokenArray++;
            if (counterPositionOfTokenArray == BLOCKOFFOUR)
            {
                break;
            }
            else
            {
                otherColor = switchingEqualColors(image.at<uchar>(currentPosition.y,currentPosition.x), otherColor);
            }
        }
        currentPosition.x++;
    }
    
    calculatedNumberOfBarcode = calculateBarsInToken(tokens, widthOfBar, leftOrRight);
    
    return calculatedNumberOfBarcode;
}

int BarcodeAnalyser::getColorOfBar(Mat& image, cv::Point &currentPosition)
{
    int colorBlackOrWhite = image.at<uchar>(currentPosition.y,currentPosition.x);
    return colorBlackOrWhite;
}

int BarcodeAnalyser::switchingEqualColors(int colorToCheck, int colorToSet)
{
    if (colorToCheck == colorToSet) {
        if (colorToSet == BLACKBAR) {
            colorToSet = WHITEBAR;
        }
        else
        {
            colorToSet = BLACKBAR;
        }
        return colorToSet;
    }
    else return colorToSet;
}


int BarcodeAnalyser::calculateBarsInToken(int readedBars[], int widthOfBar, int leftOrRight)
{
    
    //correct values of bars
    for (int i = 0; i < BLOCKOFFOUR; i++)
    {
        int rest = readedBars[i] % widthOfBar;
        while (rest != 0)
        {
            if (rest > widthOfBar / 2)
            {
                readedBars[i]++;
            }
            else if (rest <= widthOfBar / 2)
            {
                readedBars[i]--;
            }
            rest = readedBars[i] % widthOfBar;
        }
    }
    
    //take the corresponding codingtable 
    int counter =  ((leftOrRight == LEFT) ? UNEQUALEQUALTOKENCOUNTER : EQUALTOKENCOUNTER);
    
    //calculate values
    for (int i = 0; i < counter; i++)
    {
        int tokens[BLOCKOFFOUR] = {0, 0, 0, 0};
        int goal = 0;
        for (int z = 0; z < BLOCKOFFOUR; z++)
        {
            tokens[z] = (leftOrRight == LEFT ? UNEQUALEQUALTOKEN[i][z] : EQUALTOKEN[i][z]);
            int founded = readedBars[z];
            int needed = tokens[z] * widthOfBar;
            if (founded == needed)
            {
                goal++;
            }
            else
            {
                break;
            }
        }
        if (goal == BLOCKOFFOUR)
        {
            //value of arrayindex of LEFT 0-9 or of RIGHT 0-19
            return i;
        }
    }
    //Failure return
    cout << "failure -1!" <<endl;
    return -1;
}



void BarcodeAnalyser::skipBarOrWhiteSpace(Mat& image, Point &currentPosition)
{
    //0 or 255 color
    int valueOfX = getColorOfBar(image, currentPosition);
    
    while (image.at<uchar>(currentPosition.y,currentPosition.x) == valueOfX)
    {
        currentPosition.x++ ;
    }
}


unsigned int BarcodeAnalyser::scanFirstBar(Mat& image, Point &currentPosition)
{
    unsigned int widthOfBar = 0;
    
    //skip field of white
    while (image.at<uchar>(currentPosition.y,currentPosition.x) == WHITEBAR)
    {
        currentPosition.x++;
    }
    
    //first Bar of Marker
    while (image.at<uchar>(currentPosition.y,currentPosition.x) == BLACKBAR)
    {
        currentPosition.x++;
        widthOfBar++;
    }
    
    //First Start characters are 0 255 0, the first black Bar has to be substract, also persist 0 255
    int sizeOfFirstMarker = sizeof(START_AND_END_MARKERS)/sizeof(START_AND_END_MARKERS[0])-1;
    
    for (int i = 0; i < sizeOfFirstMarker; i++)
    {
        skipBarOrWhiteSpace(image, currentPosition);
    }
    return widthOfBar;
}


