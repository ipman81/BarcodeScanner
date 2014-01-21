#include "ImageScanner.h"
#include "BarcodeAnalyser.h"
#define MAXTHRESHOLD 255

using namespace cv;
using namespace std;

ImageScanner::ImageScanner(int kindOfScan)
:kindOfScanner(kindOfScan), valueThresh(128)
{
    cout << "ImageScanner erfolgreich geladen!" << endl;
    namedWindow("Image");
    namedWindow("BinarizedImage");
    createTrackbar("Select", "BinarizedImage", &valueThresh, MAXTHRESHOLD);
}

ImageScanner::~ImageScanner()
{
    destroyAllWindows();
}

bool ImageScanner::open(const std::string& path)
{
    Mat copyFrameOfImage;
    copyFrameOfImage = imread(path);
    if (!copyFrameOfImage.empty())
    {
        frameWidth = copyFrameOfImage.rows;
        frameHeight = copyFrameOfImage.cols;
        copyFrameOfImage.copyTo(image);
        return true;
    }
    else
    {
        printf("no picture!!!");
        return false;
    }
}

void ImageScanner::run()
{
    std::deque<int> numberOfBarcode;
    char key;
    bool keyWasPressed=false;
    cout << "Please enter 's' for scanning" << endl;
    while(!image.empty())
    {
        Mat frame(frameWidth,frameHeight,CV_8UC3);
        image.copyTo(frame);
        showVideoFrame(frame);
        processFrame(frame, frame);
        showProcessedFrame(frame);
        //cout << "Please enter 'q' for quit" << endl;
        //click 's' to scan an image with barcode
        key = waitKey(10);
        
        if (key == 's') {
            cout << "'s' key pressed: " << endl;
            
            cout << keyWasPressed << endl;
            if (kindOfScanner == 0)
            {
                scanner = &scannerEAN;
            }
            else if (kindOfScanner == 1)
            {
                scanner = &scannerUPC;
            }
            else
            {
                cout << "Please enter '0' for EAN or '1' for UPC!" << endl;
            }
            
            numberOfBarcode = scanner -> scaningBarcode(frame);
     
            if (numberOfBarcode[numberOfBarcode.size()-1] != -1)
            {
                showNumberOfBarcode(numberOfBarcode);
            }
            keyWasPressed = false;
        }// key s was pressed
        else if (key == 'q')
        {
            cout << " 'q' was typed see you ;-)" << endl;
            break;
        }//key q was pressed
    }
        
}



void ImageScanner::showVideoFrame(const cv::Mat& videoFrame)
{
    videoFrame.copyTo(image);
    imshow("Image", image);
}
void ImageScanner::processFrame(const cv::Mat& videoFrame, cv::Mat& processedFrame)
{
    Mat copyImg;
    videoFrame.copyTo(copyImg);
    filterImage(copyImg);
    copyImg.copyTo(processedFrame);
    
}
void ImageScanner::showProcessedFrame(const cv::Mat& processedFrame)
{
    if (!processedFrame.empty()) {
        namedWindow("Processed");
        imshow("Processed", processedFrame);
        
    }
    
}


void ImageScanner::filterImage(cv::Mat& toFilterImage)
{
    Mat binarizedFrame;
    Mat grayFrame;
    
    cvtColor(toFilterImage, grayFrame, CV_BGR2GRAY);

    //noise reduction 1 Channel
    medianBlur(grayFrame, grayFrame, 1);
    
    binarizedFrame = binarizeMask(grayFrame);
    
    //erode(mask, mask, KERNEL_GENERAL );
    //dilate(mask, mask, KERNEL_GENERAL );
    
    //morpholgyEx include both erode and dilate
    morphologyEx(binarizedFrame, toFilterImage, MORPH_OPEN, KERNEL_GENERAL);
    
}

cv::Mat ImageScanner::binarizeMask(cv::Mat &grayFrame)
{
    Mat imageCopy;
    threshold(grayFrame, imageCopy, valueThresh, MAXTHRESHOLD, CV_THRESH_BINARY);
    imshow("BinarizedImage", imageCopy);
    return imageCopy;
}

void ImageScanner::showNumberOfBarcode(deque<int> numberOfBarcode)
{
    //check if container is empty
    if (numberOfBarcode[numberOfBarcode.size() - 1] != -1)
    {
        cout << " " << endl;
        cout << "Barcode Number: " << endl;
        
        for (unsigned i = 0; i < numberOfBarcode.size(); i++)
        {
            if (numberOfBarcode[i]!= -1)
            {
                if ((i == 0) || (i == (numberOfBarcode.size())))
                {
                    
                    cout << numberOfBarcode[i] << " " ;
                }
                else
                {
                    cout << numberOfBarcode[i];
                }
            }
            else
            {
                cout << "Readerfailure["<< i << "=-1]";
            }
        }
        cout << " " << endl;
    }
    else
    {
        cout << "Barcode recognizing Failure" << endl;
    }

}
