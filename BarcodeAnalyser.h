#ifndef __FirstStepsOpenCV__BarcodeAnalyser__
#define __FirstStepsOpenCV__BarcodeAnalyser__

#include <opencv2/opencv.hpp>
#define WHITEBAR 255
#define BLACKBAR 0
#define EQUALTOKENCOUNTER 10
#define UNEQUALEQUALTOKENCOUNTER 20
#define BLOCKOFFOUR 4
#define NUMBERIS7BITS 7
#define NUMBERCOUNTER 6

class BarcodeAnalyser
{
    
public:

    BarcodeAnalyser(void);
    enum sides { LEFT, RIGHT };
    virtual ~BarcodeAnalyser(void);
    virtual std::deque<int> scaningBarcode(cv::Mat& image) = 0;
    
protected:

    const int START_AND_END_MARKERS [3]= {BLACKBAR, WHITEBAR, BLACKBAR};
    const int MIDDLE_MARKER [5] = {WHITEBAR, BLACKBAR, WHITEBAR, BLACKBAR, WHITEBAR};
    void correctScannerPosition(const cv::Mat& image, cv::Point &currPos, int begin, int end);
    int readBars(cv::Mat& image, cv::Point &currPos, int unitWidth, int side);
    unsigned scanFirstBar(cv::Mat& image, cv::Point &currPos);
    void skipBarOrWhiteSpace(cv::Mat& image, cv::Point &currPos);
    int calculateBarsInToken(int foundToken[], int unitWidth, int side);
    int getColorOfBar(cv::Mat& image, cv::Point &currPos);
    int switchingEqualColors(int colorToCheck, int colorToSet);

};

#endif /* defined(__FirstStepsOpenCV__BarcodeAnalyser__) */
