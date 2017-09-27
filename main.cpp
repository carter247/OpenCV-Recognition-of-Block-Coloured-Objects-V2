
#include <sstream>
#include <string>
#include <iostream>
#include <string>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;

vector<vector<Point> > contours;    // Stores the X, Y co-ordinates of contours in a vector within a vector
vector<Vec4i> hierarchy;            // Stores image topology in a 4D vector within a vector

int objectNumber = 0;

// Object No's
// No object = 0
// Blue = 1
// Red = 2
// Green = 3
// Yellow = 4

char colour[50];            // Storage of user input
char blue[] = "blue";       // Comparison string - blue
char red[] = "red";         // Comparison string - red
char green[] = "green";     // Comparison string - green
char yellow[] = "yellow";   // Comparison string - yellow

int objectColour(char* str1, char* str2, char* str3, char* str4, char* str5){
    // Function to compare user input to possible options of colour
    
    int r1 = strncmp(str1, str2, 4);
    int r2 = strncmp(str1, str3, 3);
    int r3 = strncmp(str1, str4, 5);    // Comparison functions
    int r4 = strncmp(str1, str5, 6);
    
    if(r1 == 0){
        
        objectNumber = 1;
        
    }   // Comparison of user input agaianst blue, if it matches then variable 'objectNumber' = 1
    
    if(r2 == 0){
        
        objectNumber = 2;
        
    }   // Comparison of user input agaianst red, if it matches then variable 'objectNumber' = 2
    
    if(r3 == 0){
        
        objectNumber = 3;
    }   // Comparison of user input agaianst green, if it matches then variable 'objectNumber' = 3
    
    if(r4 == 0){
        
        objectNumber = 4;
        
    }   // Comparison of user input agaianst yellow, if it matches then variable 'objectNumber' = 4
    
    if((r1 !=0) && (r2 != 0) && (r3 != 0) && (r4 != 0)){
        
        objectNumber = 0;
    }
    
    return objectNumber;
    
}   // Function returns the value of 'objectNumber'


int main(){
    
    Mat cameraFeed;     // Matrix to store each frame of the webcam feed
    Mat HSV;            // Matrix storage for HSV image
    Mat threshold1;     // Matrix storage for binary threshold image 1
    Mat threshold2;     // Matrix storage for binary threshold image 2
    Mat thresholdComb;  // Matrix storage for binary threshold image combination
    Mat erodeElement;   // Matrix storage for eroded bianry threshold image combination
    Mat dilateElement;  // Matrix storage for dilated bianry threshold image combination
    
    VideoCapture capture;   // Video capture object to acquire webcam feed
    
    capture.open(0);   // Open capture object at location zero (default location for webcam)
    
    cout << "What is the block colour of the object being detected?\n";
    cout << "blue/red/green/yellow \n";
    cin >> colour;
    // User defines what coloured object to detect
    
    while(objectColour(colour, blue, red, green, yellow) == 0){
        
        cout << "Wrong colour was chosen, please choose one of the following: \n";
        cout << "blue/red/green/yellow \n";
        cin >> colour;
    }   // If wrong colour was chosen, ask user again
    
    while(1){
        
        capture.read(cameraFeed);   // Store image to matrix "cameraFeed"
        
        cvtColor(cameraFeed,HSV,COLOR_BGR2HSV);   // Convert image from BGR to HSV colorspace
        
        if(objectColour(colour, blue, red, green, yellow) == 1){
            // If blue object was chosen
            
            inRange(HSV, Scalar(101, 124, 128), Scalar(203, 205, 255),threshold1);
            // Filter HSV image between values and store filtered image to threshold matrix
            
            inRange(HSV, Scalar(101, 124, 128), Scalar(203, 205, 255),threshold2);
            // Filter HSV image between values and store filtered image to threshold matrix 2
            
            add(threshold1, threshold2, thresholdComb);
            // adds value of elements from threshold 1 and threshold 2 and stored in thresholdComb
            
            
        }
        
        if(objectColour(colour, blue, red, green, yellow) == 2){
            // If red object was chosen
            
            inRange(HSV, Scalar(0, 160, 160), Scalar(14, 255, 255),threshold1);
            // Filter HSV image between values and store filtered image to threshold matrix 1
            
            inRange(HSV, Scalar(207, 160, 160), Scalar(255, 255, 255),threshold2);
            // Filter HSV image between values and store filtered image to threshold matrix
            
            add(threshold1, threshold2, thresholdComb);
            // adds value of elements from threshold 1 and threshold 2 and stored in thresholdComb
            
            
        }
        
        if(objectColour(colour, blue, red, green, yellow) == 3){
            
            inRange(HSV, Scalar(43, 214, 28), Scalar(122, 255, 255),threshold1);
            // Filter HSV image between values and store filtered image to threshold matrix
            
            inRange(HSV, Scalar(43, 214, 28), Scalar(122, 255, 255),threshold2);
            // Filter HSV image between values and store filtered image to threshold matrix 2
            
            add(threshold1, threshold2, thresholdComb);
            // adds value of elements from threshold 1 and threshold 2 and stored in thresholdComb


        }
        
        if(objectColour(colour, blue, red, green, yellow) == 4){
            
            inRange(HSV, Scalar(19, 135, 135), Scalar(56, 255, 255),threshold1);
            // Filter HSV image between values and store filtered image to threshold matrix
            
            inRange(HSV, Scalar(19, 135, 135), Scalar(56, 255, 255),threshold2);
            // Filter HSV image between values and store filtered image to threshold matrix 2
            
            add(threshold1, threshold2, thresholdComb);
            // adds value of elements from threshold 1 and threshold 2 and stored in thresholdComb
            

        }
        
        erodeElement = getStructuringElement(MORPH_RECT,Size(3,3));
        dilateElement = getStructuringElement(MORPH_RECT,Size(3,3));
        // Returns a structuring element of the size denoted
        
        
        erode(thresholdComb,thresholdComb,erodeElement);
        erode(thresholdComb,thresholdComb,erodeElement);
        // Erodes the specified structuring element
        
        dilate(thresholdComb,thresholdComb,dilateElement);
        dilate(thresholdComb,thresholdComb,dilateElement);
        // Dilates the specified structuring element
        
        GaussianBlur(thresholdComb, thresholdComb, Size(9, 9), 2, 2);
        // Blurs an image using Gaussian Blur
        
        findContours(thresholdComb, contours, hierarchy, RETR_CCOMP, CHAIN_APPROX_SIMPLE);
        // Finds the contours of the binary image
        
        if(hierarchy.empty()){
            
            imshow("Camera Feed", cameraFeed);
            
        } // If no contours are found then just show camera feed
        
        if(!hierarchy.empty()){
            
            vector<Moments> mu(contours.size());
            // Creates a vector using as many points as there are contour points
            
            for(size_t i = 0; i < contours.size(); i++){
                
                mu[i] = moments(contours[i], false );
                
            } // Calculates all the moments and stores in mu[i]
            
            vector<Point2f> mc(contours.size());
            // Creates a vector of 2D float using as many points as there are contour points
            
            for(size_t i = 0; i < contours.size(); i++){
                
                mc[i] = Point2f( static_cast<float>(mu[i].m10/mu[i].m00) , static_cast<float>(mu[i].m01/mu[i].m00));
            }
            
            for(size_t i = 0; i< contours.size(); i++){
                    
                Scalar color(50, 255, 255);     // Defines 'color' as scalar using the following values
                drawContours(cameraFeed, contours, (int)i, color, 2, 8, hierarchy, 0, Point());   // Draws contours around object
                circle(cameraFeed, mc[i], 4, color, -2, 4, 0 );     // Draws a circle at the centre of the object
             }
        }
        
        imshow("Camera Feed", cameraFeed);      // Show camera feed
        
        waitKey(100);   // Delay 100ms so that screen can refresh. Image will not appear without this waitKey() command
        
    }
    
    return 0;

}

