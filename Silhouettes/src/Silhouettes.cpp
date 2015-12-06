/*
 * File: Silhouettes.cpp
 * ---------------------------
 * This program finds silhouettes in the image.
 */

#include<iostream>
#include "gwindow.h"
#include "gobjects.h"
#include "gbufferedimage.h"

using namespace std;

/* Сonstants */
const int THRESHOLD_COLOR = 13882323; // max value of color for background (in decimal, HEX #d3d3d3)

/* Function prototypes */
void introduction();
void imgToBitMap(int **bitMap, int imgHeight, int imgWidth, GBufferedImage* imgBuffer);
void putMask(int y, int x, int newMask, int baseMask, int **bitMap, int imgHeight, int imgWidth);
int getNumberOfSilhouettes(string nameImage);
void cleanHeap();

/* Main program */
int main() {

    introduction();

    string inputStr = "";

    while(true){
        cout << "Enter name of the source image: ";
        if(cin >> inputStr){
            if(inputStr == "Q"){
                break;
            }
            else{
                cout << "Result: " << getNumberOfSilhouettes(inputStr) << endl << endl;
            }
        }
    }

    return 0;
}

/*
* Function: introduction
* Usage: introduction();
* ---------------------------------------
* The function displays the Introduction.
*/
void introduction(){
    cout << "This program finds silhouettes in the image." << endl;
    cout << "For testing we have 8 files with silhouettes." << endl;
    cout << "Enter the name of the file in format: 1.jpg or 2.jpg and so on to 8.jpg" << endl;
    cout << "Or add your file to the folder 'res' and enter his name." << endl;
    cout << "Enter 'Q' to exit the program." << endl << endl;
}

/*
* Function: imgToBitMap
* Usage: imgToBitMap(bitMap, imgHeight, imgWidth, imgBuffer);
* ---------------------------------------
* The function separate silhouettes from the background color of the threshold color.
*/
void imgToBitMap(int **bitMap, int imgHeight, int imgWidth, GBufferedImage* imgBuffer){
    int pixelRGB = 0;
    for(int y = 0; y < imgHeight; y++){
        for(int x = 0; x < imgWidth; x++){
            pixelRGB = imgBuffer->getRGB(x, y);
            if(pixelRGB > THRESHOLD_COLOR){
                bitMap[y][x] = 0;
            }
            else {
                bitMap[y][x] = 1;
            }
        }
    }
}

/*
* Function: putMask
* Usage: putMask(y, x, newMask, baseMask, bitMap, imgHeight, imgWidth);
* ---------------------------------------
* The function put unique numeric mask on each found Silhouette.
*/
void putMask(int y,int x,int newMask,int baseMask,int **bitMap,int imgHeight,int imgWidth){
    if(x >= 0 && x < imgWidth && y >= 0 && y < imgHeight && bitMap[y][x] == baseMask && bitMap[y][x] != newMask){
        bitMap[y][x] = newMask;
        putMask(y, x+1, newMask, baseMask, bitMap, imgHeight, imgWidth);
        putMask(y, x-1, newMask, baseMask, bitMap, imgHeight, imgWidth);
        putMask(y-1, x, newMask, baseMask, bitMap, imgHeight, imgWidth);
        putMask(y+1, x, newMask, baseMask, bitMap, imgHeight, imgWidth);
    }
}

/*
* Function: getNumberOfSilhouettes
* Usage: getNumberOfSilhouettes(inputStr);
* ---------------------------------------
* The function counting the number of of silhouettes.
*/
int getNumberOfSilhouettes(string nameImage){

    // imgObj graphical object represents an image from a file
    GImage* imgObj;
    imgObj = new GImage(nameImage);

    // gw represents a graphics window that supports simple graphics
    GWindow gw(imgObj->getWidth(),imgObj->getHeight());
    gw.add(imgObj);

    // imgBuffer implements a 2D region of colored pixels that can be read/set individually
    GBufferedImage* imgBuffer = new GBufferedImage(0, 0, imgObj->getWidth(), imgObj->getHeight());
    imgBuffer->load(nameImage);
    int imgHeight = (int)imgBuffer->getHeight();
    int imgWidth = (int)imgBuffer->getWidth();

    // create array for bitMap
    int **bitMap = new int *[imgHeight];
    for(int y = 0; y < imgHeight; y++){
        bitMap[y] = new int[imgWidth];
    }

    // create numeric map for image
    imgToBitMap(bitMap, imgHeight, imgWidth, imgBuffer);

    int result = 0; // for counting silhouettes
    int newMask = 1; // numeric marker for masks
    int baseMask = 1;

    // search silhouettes on bitMap and put a unique mask.
    for(int y = 1; y < imgHeight; y++){
        for(int x = 1; x < imgWidth; x++){
            if(bitMap[y][x] == 1){
                newMask++;
                result++;
                putMask(y, x, newMask, baseMask, bitMap, imgHeight, imgWidth);
            }
        }
    }

    for(int y = 0; y < imgHeight; y++){
        delete[]bitMap[y];
    }
    delete []bitMap;
    delete imgObj;
    delete imgBuffer;

    return result;
}
