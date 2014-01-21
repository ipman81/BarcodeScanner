#include "ScannerMenu.h"

using namespace std;

ScannerMenu::ScannerMenu()
{

}

ScannerMenu::~ScannerMenu()
{
    
}

void ScannerMenu::menuEAN()
{
    cout << "Press Make your choice to Barcode-Format: " << endl;
    cout << "Press'1' for EAN13 Part 1" << endl;
    cout << "Press'2' for EAN13 Part 2" << endl;
    cout << "Press'3' VideoCapture" << endl;
    cout << endl;
}

void ScannerMenu::menuUPC()
{
    cout << "Make your choice to Barcode-Format: " << endl;
    cout << "Press'1' UPC" << endl;
    cout << "Press'2' UPC rotated" << endl;
    cout << "Press'3' VideoCapture" << endl;
    cout << endl;
}



void ScannerMenu::scannerStart()
{
    pathName = "/Users/adrianwatzlawczyk/Documents/AVPRG Homework/FirstStepsOpenCV/FirstStepsOpenCV/images/";
    concatinateString = "";
    
    
    int inputScannerType = -1;
    char input = '0';
    std::string str = " ";
    
    
    while (input < '1' || input > '3')
    {
        cout << "Please enter '0' for EAN13 or '1' for UPC :-)" << endl;
        cin >> inputScannerType;
        cout<<endl;
        
        if (inputScannerType == 0)
        {
            
            menuEAN();
            
            cin >> input;
            cout << endl;
            switch (input)
            {
                case '1':
                    cout << "EAN 13 Part 1" << endl;
                    concatinateString = "ean0.png";
                    str =pathName + concatinateString;
                    break;
                case '2':
                    cout << "EAN 13 Part 2" << endl;
                    concatinateString = "ean1.png";
                    str =pathName + concatinateString;
                    break;

                case '3':
                    
                    cout << "Videocamera" << endl;
                    str = "video";
                    break;
                    

                default:
                    cout << "Falsche Eingabe." << endl;
                    cout << endl;
                    menuEAN();
                    break;
            }
        }
        else if (inputScannerType == 1)
        {
            menuUPC();
            
            cin >> input;
            cout << endl;
            switch (input)
            {
                case '1':
                    cout << "UPC" << endl;
                    concatinateString = "UPC1.png";
                    str =pathName + concatinateString;
                    break;
                case '2':
                    cout << "UPC rotated" << endl;
                    concatinateString = "UPCr.png";
                    str =pathName + concatinateString;
                    break;
                case '3':
                    
                    cout << "Videocamera" << endl;
                    str = "video";
                    break;
                    
                default:
                    cout << "Wrong input! " << endl;
                    cout << endl;
                    menuUPC();
                    break;
            }
        }
            
        if ( str == "video")
        {
            cout << "test Video" << endl;
            CameraScanner scannerCamApp(inputScannerType);
            scannerCamApp.open("1");
            scannerCamApp.run();
            break;
            
        }
        else if (str != " ")
        {
            cout << "test" << endl;
            cout << "please enter 'q' for quit" << endl;
            ImageScanner scannerImageApp(inputScannerType);
            cout << "open" << endl;
            scannerImageApp.open(str);
            cout << "run" << endl;
            scannerImageApp.run();
            cout << "break" << endl;
            break;
        }
    }
}



