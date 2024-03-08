//
// Created by 周舒意 on 2019-08-14.
//

#ifndef TRY_COUNT_H
#define TRY_COUNT_H
#include <iostream>
#include <sstream>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/imgproc.hpp>

using namespace cv;
using namespace std;


void deleteStatic(vector<KeyPoint> &keypoints, const Mat &blur_frame);
//MODIFY: keypoints
//EFFECTS: delete the key points not belonging to chongzi.

int InitializeUTL(const KeyPoint &chong, int yLine, int& upperToDown);
//MODIFY: upperToDown
//EFFECTS: initialize upperThanLine. If the upper of keypoint is lower than line, return 0;
//          if the lower of keypoint is upper than line, return 1; otherwise return 2 and set
//          upperToDown to be 2.

void CountChong(int yLine, int &upperThanLine, int &count, int &upperToDown, const KeyPoint &chong);
//MODIFY: upperThanLine, count, upperToDown, chong
//EFFECTS: when the chong's lower bound pass the yLine in the direction from up to down, set upToDown to be 1;
//          then the upper bound pass the yLine, count plus one, upToDown go back to zero, and
//          upperThanLine become 0; Vice Versa.

void UpdateChong(KeyPoint &chong, const vector<KeyPoint> &keypoints, int x_begin,
        int x_end, int yUpperBound, int yLowerBound);
//MODIFY: chong
//EFFECTS: Update chong.

#endif //TRY_COUNT_H
