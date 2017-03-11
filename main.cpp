in the original video)

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;
using namespace std;

int main( int argc, char** argv ) {
    VideoCapture cap(0); //capture the video from web cam

    if (!cap.isOpened())  // if not success, exit program
    {
        cout << "Cannot open the web cam" << endl;
        return -1;
    }

    namedWindow("Control", CV_WINDOW_AUTOSIZE); //create a window called "Control"

    int HSV_MIN_HUE = 0;
    int HSV_MAX_HUE = 179;

    int HSV_MIN_SAT = 0;
    int HSV_MAX_SAT = 255;

    int HSV_MIN_VAL = 0;
    int HSV_MAX_VAL = 255;

    //Create trackbars in "Control" window
    cvCreateTrackbar("LowH", "Control", &HSV_MIN_HUE, 179); //Hue (0 - 179)
    cvCreateTrackbar("HighH", "Control", &HSV_MAX_HUE, 179);

    cvCreateTrackbar("LowS", "Control", &HSV_MIN_SAT, 255); //Saturation (0 - 255)
    cvCreateTrackbar("HighS", "Control", &HSV_MAX_SAT, 255);

    cvCreateTrackbar("LowV", "Control", &HSV_MIN_VAL, 255); //Value (0 - 255)
    cvCreateTrackbar("HighV", "Control", &HSV_MAX_VAL, 255);

    while (true) {
        Mat imgOriginal;

        bool bSuccess = cap.read(imgOriginal); // read a new frame from video

        if (!bSuccess) //if not success, break loop
        {
            cout << "Cannot read a frame from video stream" << endl;
            break;
        }

        Mat imgHSV;

        cvtColor(imgOriginal, imgHSV, COLOR_BGR2HSV); //Convert the captured frame from BGR to HSV

        Mat imgThresholded;

        inRange(imgHSV, Scalar(HSV_MIN_HUE, HSV_MIN_SAT, HSV_MIN_VAL), Scalar(HSV_MAX_HUE, HSV_MAX_SAT, HSV_MAX_VAL),
                imgThresholded); //Threshold the image

        //morphological opening (remove small objects from the foreground)
        erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
        dilate(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

        //morphological closing (fill small holes in the foreground)
        dilate(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
        erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

        imshow("Thresholded Image", imgThresholded); //show the thresholded image
        imshow("Original", imgOriginal); //show the original image

        if (waitKey(30) == 27) //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
        {
            cout << "esc key is pressed by user" << endl;
            break;
        }
    }

    return 0;

}