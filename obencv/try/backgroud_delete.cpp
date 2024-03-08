
#include <iostream>
#include <sstream>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <math.h>
#include "count.h"


using namespace cv;
using namespace std;

const char* params
        = "{ help h         |                  | Print usage }"
          "{ input          | day~1.avi        | Path to a video or a sequence of image }"
          "{ algo           | KNN              | Background subtraction method (KNN, MOG2) }";

const int x_begin[] = {380, 580, 690, 800, 890, 1190, 1300, 1400, 1520, 1610};
const int x_end[] = {490, 680, 780, 880, 1000, 1300, 1400, 1500, 1600, 1700};
const int yLine = 790;
const int yLowerBound = 900;
const int yUpperBound = 500;
const int num = sizeof(x_begin)/ sizeof(int);


int main(int argc, char* argv[])
{
    CommandLineParser parser(argc, argv, params);
    parser.about( "This program shows how to use background subtraction methods provided by "
                  " OpenCV. You can process both videos and images.\n" );
    if (parser.has("help")) {
        //print help information
        parser.printMessage();
    }

    //! [create Background Subtractor objects]
    //create
    Ptr<BackgroundSubtractor> pBackSub;
    if (parser.get<String>("algo") == "MOG2")
        pBackSub = createBackgroundSubtractorMOG2();
    else
        pBackSub = createBackgroundSubtractorKNN();
    //! [create Background Subtractor objects]

    //! [create Blob Analysis objects]
    SimpleBlobDetector::Params params_BD;
    params_BD.filterByColor = false;
    params_BD.minThreshold = 100;
    params_BD.maxThreshold = 255;
    params_BD.filterByArea = true;
    params_BD.minArea = 200;
    params_BD.maxArea = 800;
    params_BD.filterByCircularity = false;
    params_BD.minCircularity = 0.1;
    params_BD.maxCircularity = 0.5;
    params_BD.filterByInertia = false;
    params_BD.maxInertiaRatio = 1;
    params_BD.maxInertiaRatio = 0.5;
    params_BD.filterByConvexity = false;
    params_BD.maxConvexity = 1;
    params_BD.minConvexity = 0.6;
    Ptr<SimpleBlobDetector> detector = SimpleBlobDetector::create(params_BD);
    vector<KeyPoint> keypoints;
    //! [create Blob Analysis objects]

    //! [Line}

    //! [Line}

    //! [capture]
    VideoCapture capture(parser.get<String>("input"));
    if (!capture.isOpened()){
        //error in opening the video input
        cerr << "Unable to open: " << parser.get<String>("input") << endl;
        return 0;
    }
    //! [capture]

    Mat frame, fgMask, blur_frame, grey_frame;
    KeyPoint *chong = new KeyPoint[num];
    for(int i = 0; i< num; i++) chong[i] = KeyPoint();
    int *upToDown = new int[num];
    for (int i = 0; i < num; i ++) upToDown[i] = 0;
    int *count =new int[num];
    for (int i = 0; i < num; i ++) count[i] = 0;
    int *upperThanLine = new int[num];
    for (int i = 0; i < num; i ++) upperThanLine[i] = InitializeUTL(chong[i], yLine, upToDown[i]);
    while (true) {
        capture >> frame;
        if (frame.empty())
            break;

        //! [subtract background]
        pBackSub->apply(frame, fgMask);
        //! [subtract background]

        //! [blur it]
        Size ksize(20, 20);
        blur(fgMask, blur_frame, ksize);
//        for(int i = 0; i<100000; i++)
//        {
//            int keyboard = waitKey(30);
//            if (keyboard == 'q' || keyboard == 27)
//                break;
//            uchar data = (blur_frame.ptr<uchar>(i)[0]);
//            cout<<(int)data<<endl;
//
//        }
        //! [blur it]

        //! [blob analysis]
        cvtColor(frame, grey_frame, COLOR_BGR2GRAY);
        detector->detect(blur_frame,keypoints);
        //deleteStatic(keypoints, blur_frame);
        Mat img_with_keypoints;
        drawKeypoints(grey_frame,keypoints,img_with_keypoints,Scalar(0,0,255),DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
        //! [blob analysis]

        //!!!
        for(int i = 0; i < num; i++){
            CountChong(yLine, upperThanLine[i], count[i], upToDown[i], chong[i]);
            UpdateChong(chong[i], keypoints, x_begin[i], x_end[i], yUpperBound, yLowerBound);
        }
        //!!!

        //! [display_frame_number]
        //get the frame number and write it on the current frame
        rectangle(frame, cv::Point(10, 2), cv::Point(100,20),
                  cv::Scalar(255,255,255), -1);
        stringstream ss;
        ss << capture.get(CAP_PROP_POS_FRAMES);
        string frameNumberString = ss.str();
        putText(frame, frameNumberString.c_str(), cv::Point(15, 15),
                FONT_HERSHEY_SIMPLEX, 0.5 , cv::Scalar(0,0,0));
        //! [display_frame_number]

        //![display count number]
        for(int i = 0; i < num; i++){
            rectangle(img_with_keypoints, Point(x_begin[i], yLine), Point(x_begin[i]+50,yLine+20),
                      cv::Scalar(255,255,255), -1);
            putText(img_with_keypoints, to_string(count[i]), Point(x_begin[i], yLine),
                    FONT_HERSHEY_SIMPLEX, 0.5 , cv::Scalar(0,0,0));
        }
//        for(int i = 0; i<keypoints.size(); i++){
//            rectangle(blur_frame, Point(keypoints[i].pt.x-10, keypoints[i].pt.y-10), Point(keypoints[i].pt.x+100, keypoints[i].pt.y+10),
//                      cv::Scalar(255,255,255), -1);
//            int flag = 0;
//            for(int j=0; j<keypoints[i].size/2;j++){
//                if(flag==1) break;
//                for(double k = 0; k< 2*3.1415926;k++){
//
//                    if(blur_frame.ptr<uchar>(keypoints[i].pt.x + cos(k)*j, keypoints[i].pt.y +sin(k)*j)[0]>100) {
//                        flag = 1;
//                        break;
//                    }
//                }
//            }
//            putText(blur_frame, to_string(flag), Point(keypoints[i].pt.x, keypoints[i].pt.y),
//                    FONT_HERSHEY_SIMPLEX, 0.5 , cv::Scalar(0,0,0));
//        }
        //![display count number]

        //! [show]
        //show the current frame and the fg masks
//        namedWindow("Frame", 0);
//        resizeWindow("Frame", 640, 480);
//        imshow("Frame", frame);
//        namedWindow("FG Mask", 0);
//        resizeWindow("FG Mask", 640, 480);
        imshow("FG Mask", blur_frame);
        namedWindow("keypoints", 0);
        resizeWindow("keypoints", 1000, 1200);
        imshow("keypoints",img_with_keypoints);
        //! [show]

        //get the input from the keyboard
        int keyboard = waitKey(10);
        if (keyboard == 'q' || keyboard == 27)
            break;

    }

    delete[] upToDown;
    delete[] upperThanLine;
    delete[] chong;
    return 0;
}