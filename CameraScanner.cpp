#include "CameraScanner.h"
#include "BarcodeAnalyser.h"
#define MAXTHRESHOLD 255
#define CROSSVALUEMIN 75
#define CROSSVALUEMAX 100


using namespace cv;
using namespace std;

CameraScanner::CameraScanner(int kindOfScan)
:kindOfScanner(kindOfScan),valueThresh(128)
{
    namedWindow("VideoFrame");
    namedWindow("BinarizedImage");
    namedWindow("copyImageWithCross");
    namedWindow("resizedMask");
    createTrackbar("Select", "BinarizedImage", &valueThresh, MAXTHRESHOLD);
}

CameraScanner::~CameraScanner(void)
{
    destroyAllWindows();
}

bool CameraScanner::open(const std::string& path)
{
    stringstream ss (path);
    int s;
    ss >> s;
    videoCapture.open(s);
    videoCapture.set(CV_CAP_PROP_FRAME_WIDTH, CONSTANTFRAMEWIDTH);
    videoCapture.set(CV_CAP_PROP_FRAME_HEIGHT, CONSTANTFRAMEHEIGHT);
    if (videoCapture.isOpened()){
        frameNumber = 0;
        frameWidth = videoCapture.get(CV_CAP_PROP_FRAME_WIDTH);
        frameHeight = videoCapture.get(CV_CAP_PROP_FRAME_HEIGHT);
        cout << "camera loaded" << endl;
        return true;
    }
    else {
        printf("no camera founded!!!");
        return false;
    }
}

void CameraScanner::run()
{
    std::deque<int> numberOfBarcode;
    if (videoCapture.isOpened())
    {
        cout << "Focuswert" << videoCapture.get(CV_CAP_PROP_FOCUS) << endl;
        
        cout << "Please enter 'c' for capture frame" << endl;
    }
    while(true)
    {
        Mat videoFrame(frameWidth,frameHeight,CV_8UC3);
        
        if (videoCapture.read(videoFrame) == false)
        {
			break;
		}
        frameNumber++;
        showVideoFrame(videoFrame);
        processFrame(videoFrame, processedImage);
        
        //cout << "Please enter 'q' for quit" << endl;
        char key = waitKey(10);
    
        if (key == 'q')
        {
            cout << " best regards!" << endl;
            break;
        }

        
        if (key == 'c')
        {
            cout << "Please enter 'c' for capturing" << endl;
            if (!processedImage.empty())
            {
                showProcessedFrame(processedImage);
                camHandler.readContoursOfFrame(cannyFilteredImage, valueThresh);
                camHandler.repaintBarcode(processedImage);
                resizeMask(processedImage);
                keyPressed(true);
            }
        }
    
        if (keyWasPressed)
        {
            if (key == 's')
            {
                cout << "'s' key pressed: " << endl;
                
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
                
                numberOfBarcode = scanner -> scaningBarcode(processedImage);
                
                if (numberOfBarcode[numberOfBarcode.size()-1] != -1)
                {
                    showNumberOfBarcode(numberOfBarcode);
                }
                
            }
            else if (key == 'q')
            {
                cout << " best regards!" << endl;
                break;
            }
        }
    }
}

void CameraScanner::showVideoFrame(const cv::Mat& videoFrame)
{
    imshow("VideoFrame", videoFrame);
}
void CameraScanner::processFrame(const cv::Mat& videoFrame, cv::Mat& processedFrame)
{
    Scalar greenScalar(0,180, 0);
    videoFrame.copyTo(copyImage);
    linesHalfCrosses(copyImage, greenScalar, CROSSVALUEMIN, CROSSVALUEMAX);
    if (!copyImage.empty())
    {
        imshow("copyImageWithCross", copyImage);
    filterImage(copyImage);
    copyImage.copyTo(processedFrame);
    }
}

void CameraScanner::showProcessedFrame(const cv::Mat& processedFrame)
{
    if (!processedFrame.empty())
    {
        namedWindow("Processed");
        imshow("Processed", processedFrame);
    }
}

bool CameraScanner::keyPressed(bool key)
{
    keyWasPressed = key;
    return keyWasPressed;
}


void CameraScanner::filterImage(cv::Mat& toFilterImage)
{
    Mat binarizedFrame;
    Mat grayFrame;
    
    cvtColor(toFilterImage, grayFrame, CV_BGR2GRAY);
    
    //noise reduction 1 Channel
    medianBlur(grayFrame, grayFrame, 5);
    
    //contour detection
    
    binarizedFrame = binarizeMask(grayFrame);

    //include erode & dilate methods
    morphologyEx(binarizedFrame, toFilterImage, MORPH_CLOSE, 2);
    
    Mat canny;
    binarizedFrame.copyTo(canny);
    Canny(canny, canny, valueThresh, MAXTHRESHOLD,3, true);
    
    if (!canny.empty())
    {
        canny.copyTo(cannyFilteredImage);
        imshow("canny", cannyFilteredImage);
    }
    binarizedFrame.copyTo(toFilterImage);
}

cv::Mat CameraScanner::binarizeMask(cv::Mat &grayFrame)
{
    Mat imageCopy;
    threshold(grayFrame, imageCopy, valueThresh, MAXTHRESHOLD, CV_THRESH_BINARY);
    if (!imageCopy.empty())
    {
        imageCopy.copyTo(grayFrame);
        imshow("BinarizedImage", grayFrame);
    }
    return imageCopy;
}

void CameraScanner::linesHalfCrosses(cv::Mat &img, const Scalar &color, int value1, int value2)
{
    int height = frameHeight;
    int width = frameWidth;
    
    int valueOne = value1;
    valueOfDistance=valueOne;
    int valueTwo = value2;
    
    //links oben halbes Kreuz
    Point leftAboveHorizontalX1((width+valueOne )- width, (height+valueOne ) - height);
    Point leftAboveHorizontalX2((width+valueOne )- width, (height+valueTwo ) - height);
    Point leftAboveVerticalY1((width+valueOne )- width, (height+valueOne ) - height);
    Point leftAboveVerticalY2((width+valueTwo )- width, (height+valueOne ) - height);
    
    //rechts oben halbes Kreuz
    Point rightAboveHorizontalX1(width-valueOne , (height+valueOne ) - height);
    Point rightAboveHorizontalX2(width-valueOne , (height+valueTwo ) - height);
    Point rightAboveVerticalY1(width-valueOne , (height+valueOne ) - height);
    Point rightAboveVerticalY2(width-valueTwo , (height+valueOne ) - height);
    
    //links unten halbes Kreuz
    Point leftBottomHorizontalX1((width+valueOne) - width, height-valueOne );
    Point leftBottomHorizontalX2((width+valueOne) - width, height-valueTwo );
    Point leftBottomVerticalY1((width+valueOne)- width, height-valueOne );
    Point leftBottomVerticalY2((width+valueTwo )- width, height-valueOne );
    
    //rechts unten halbes Kreuz
    Point rightBottomHorizontalX1(width-valueOne , height-valueOne  );
    Point rightBottomHorizontalX2(width-valueOne , height-valueTwo  );
    Point rightBottomVerticalY1(width-valueOne , height-valueOne  );
    Point rightBottomVerticalY2(width-valueTwo , height-valueOne  );
    
    //Scalar greenScalar(130,20,80);
    
    line(img, leftAboveHorizontalX1, leftAboveHorizontalX2, color );
    line(img, leftAboveVerticalY1, leftAboveVerticalY2, color);
    
    line(img, rightAboveHorizontalX1, rightAboveHorizontalX2, color);
    line(img, rightAboveVerticalY1, rightAboveVerticalY2, color);
    
    line(img, leftBottomHorizontalX1, leftBottomHorizontalX2, color);
    line(img, leftBottomVerticalY1, leftBottomVerticalY2, color);
    
    line(img, rightBottomHorizontalX1, rightBottomHorizontalX2, color);
    line(img, rightBottomVerticalY1, rightBottomVerticalY2, color);
    
}

void CameraScanner::resizeMask(cv::Mat &mask)
{
    Mat regionMask(mask.rows, mask.cols, CV_8UC1);
    
    rectangle.x = valueOfDistance;
    rectangle.y = valueOfDistance;
    rectangle.width = regionMask.cols-(2*valueOfDistance);
    rectangle.height = regionMask.rows-(2*valueOfDistance);
    
    Mat resizeM(mask.rows,mask.cols,CV_8UC1);
    threshold(mask, resizeM, valueThresh, MAXTHRESHOLD, CV_THRESH_BINARY_INV);
    Point center(mask.cols/2,mask.rows/2);
    Size size(rectangle.width,rectangle.height);
    getRectSubPix(mask, size, center, mask);
    
    imshow("resizedMask", mask);
}


void CameraScanner::showNumberOfBarcode(deque<int> numberOfBarcode)
{
    cout << "numberOf" << numberOfBarcode[numberOfBarcode.size() - 1] << endl << endl;
    if (numberOfBarcode[numberOfBarcode.size() - 1] != -1)
    {
        cout << " " << endl;
        cout << "Barcode Number: " << endl;
        for (unsigned i = 0; i < numberOfBarcode.size(); i++)
        {
            if (numberOfBarcode[i]!= -1)
            {
                cout << numberOfBarcode[i];
            
            }
        }
        cout << " " << endl;
    }
    else
    {
        cout << "Barcode recognizing Failure" << endl;
    }
}
