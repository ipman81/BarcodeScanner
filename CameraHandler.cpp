//
//  CameraHandler.cpp
//  FirstStepsOpenCV
//
//  Created by Adrian Watzlawczyk on 20/01/14.
//  Copyright (c) 2014 Adrian Watzlawczyk. All rights reserved.
//

#include "CameraHandler.h"


using namespace cv;
using namespace std;

CameraHandler::CameraHandler()
{
    namedWindow("paintedFrame");
    namedWindow("maskChoice");
}

CameraHandler::~CameraHandler(void)
{
    destroyAllWindows();
}

void CameraHandler::readContoursOfFrame(cv::Mat &mask, int valueOfThresh)
{
     Mat maskCopy(mask.rows,mask.cols,CV_8SC3);
     vector<Vec2f> lines;
     HoughLines( mask, lines, 1, CV_PI/180, valueOfThresh );
     
     for( size_t i = 0; i < lines.size(); i++ )
     {
         float rho = lines[i][0];
         float theta = lines[i][1];
         double a = cos(theta), b = sin(theta);
         double x0 = a*rho, y0 = b*rho;
         Point pt1(cvRound(x0 + 1000*(-b)),
         cvRound(y0 + 1000*(a)));
         Point pt2(cvRound(x0 - 1000*(-b)),
         cvRound(y0 - 1000*(a)));
         line( maskCopy, pt1, pt2, Scalar(0,0,255), 1, 8 );
     }
     
     if (!maskCopy.empty())
     {
         cout << "contourImg" << endl;
         imshow("contourImage", maskCopy);
     }

}


void CameraHandler::repaintBarcode(cv::Mat& binaryImage)
{
    Mat toPaint(binaryImage.rows,binaryImage.cols,CV_8UC1);
    int black=0;
    int half=0;
    
    Mat toCheck;
    binaryImage.copyTo(toCheck);
    for (int i = 0; i < toCheck.cols; i++) {
        black = 0;
        for (int z = 0; z < toCheck.rows; z++) {
            Vec3b pixel2 = binaryImage.at<Vec3b>(z,i);
            int count = 0;
            for (int c = 0; c < 3; c++) {
                if ((pixel2[c]) == 0) {
                    count++;
                    if (count == 3) {
                        black++;
                    }//if
                }//if
            }//for c
        }//for z
        //col z to paint
        half = (toCheck.rows/4);
        
        if (black > half) {
            for (int k = 0; k < toCheck.rows; k++)
            {
                for (int c = 0; c < 3; c++)
                {
                    toPaint.at<Vec3b>(k,i)[c]=0;
                }
            }
        }
        else
        {
            for (int k = 0; k < toCheck.rows; k++)
            {
                for (int c = 0; c < 3; c++)
                {
                    toPaint.at<Vec3b>(k,i)[c]=255;
                }
            }
        }
    }
    imshow("painted mask", toPaint);
    repairBarcode(toPaint);
}

void CameraHandler::repairBarcode(cv::Mat &binaryMask)
{
    Mat toCheck;
    binaryMask.copyTo(toCheck);
    Mat repair(binaryMask.rows,binaryMask.cols, CV_8UC1);
    
    //int arrayOfValues[toCheck.cols];
    vector<int>values;
    
    
    for (int z = 0; z < toCheck.cols; z++) {
        Vec3b pixel = toCheck.at<Vec3b>(toCheck.rows/2,z);
        int count = 0;
        for (int c = 0; c < 3; c++) {
            //if black
            if ((pixel[c]) == 0)
            {
                count++;
                if (count == 3)
                {
                    values.push_back(1);
                }//if
            }//if
            else if (pixel[c] == 255)
            {
                count++;
                if (count == 3)
                {
                    values.push_back(0);
                }//if
                
            }
        }//for c
        cout << values[z];
        
    }//for x
    cout << endl;
    cout << "------------------------" << endl << endl;
    int black = 0;
    vector<int>sizes;
    
    for (int k = 0; k < values.size(); k++) {
        if (values[k] == 1) {
            black++;
        }
        else
        {
            if (black != 0) {
                sizes.push_back(black);
            }
            black=0;
        }
    }
    
    //get the smallest black value
    int min=toCheck.cols;
    
    for (int i = 0; i < sizes.size(); i++) {
        cout << sizes[i] << "<->";
        if (sizes[i]< min) {
            min=sizes[i];
        }
    }
    
    
    //get the biggest black value
    int max=0;
    
    for (int i = 0; i < sizes.size(); i++) {
        cout << sizes[i] << "<->";
        if (sizes[i]> max) {
            max=sizes[i];
        }
    }
    
    
    vector<int>correctBlack;
    int breite=0;
    //int rest=0;
    
    if (min % 2 != 0) {
        min +=1;
    }
    
    if (max % min == 0) {
        breite = min;
    }
    else
    {
        while (max % min !=0) {
            
            if (min * 4 < max)
            {
                min += 1;
            }
            else if(min * 4 > max)
            {
                min += 1;
            }
        }
        breite = min;
    }
    
    cout << "max " << max << endl;
    cout << "min " << min << endl;
    cout << "breite" <<breite << endl;
    
    for (int y = 0; y < sizes.size(); y++) {
        if (sizes[y] < min) {
            sizes[y]=min;
        }
        else if (sizes[y] <= (2*min) && sizes[y] > min)
        {
            sizes[y]=2*min;
        }
        else if (sizes[y] <= (3*min) && sizes[y] > 2*min)
        {
            sizes[y]=3*min;
        }
        else if (sizes[y] >= (4*min) && sizes[y] > 3*min)
        {
            sizes[y]=4*min;
        }
        cout << sizes[y] << "---";
    }
    
    
}


