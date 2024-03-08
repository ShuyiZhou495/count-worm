//
// Created by 周舒意 on 2019-08-14.
//
#include "count.h"

using namespace cv;
using namespace std;


void deleteStatic(vector<KeyPoint> &keypoints, const Mat &m){
    vector<KeyPoint>::iterator it = keypoints.begin();
    while(it!=keypoints.end()){
        Scalar color = m.at<uchar>(it->pt.x, it->pt.y);//读取原图像(150, 150)的BGR颜色值，如果是灰度图像，将Vec3b改为uchar
        uchar data = m.ptr<uchar>(it->pt.x, it->pt.y)[0];
        if((int)data>100) {
            it = keypoints.erase(it);
        }
        else it++;
    }
}

int InitializeUTL(const KeyPoint &chong, int yLine, int& upperToDown){
    if(chong.size==0) return -1;
    else if(chong.pt.y - chong.size/2 >yLine) return 0;
    else if(chong.pt.y + chong.size/2 <yLine) return 1;
    else {
        upperToDown = 2;
        return 2;
    }
}

void CountChong(int yLine, int &upperThanLine, int &count, int &upperToDown, const KeyPoint &chong){
    if(upperThanLine == -1 && chong.size > 0)
        upperThanLine = InitializeUTL(chong, yLine, upperToDown);
    else if (upperThanLine == 1 && chong.pt.y + chong.size/2 > yLine) {
        upperThanLine = 2;
        upperToDown = 1;
    }
    else if (upperThanLine == 2 && chong.pt.y - chong.size/2 > yLine &&
    (upperToDown == 1 || upperToDown == 2)){
        count++;
        upperThanLine = 0;
        upperToDown = 0;
    }
    else if(upperThanLine == 0 && chong.pt.y - chong.size/2 < yLine){
        upperThanLine = 2;
        upperToDown = -1;
    }
    else if(upperThanLine == 2 && chong.pt.y + chong.size/2 < yLine &&
    (upperToDown == -1 || upperToDown == 2)){
        count++;
        upperThanLine = 1;
        upperToDown = 0;
    }
}

void UpdateChong(KeyPoint &chong, const vector<KeyPoint> &keypoints, int x_begin,
        int x_end, int yUpperBound, int yLowerBound){
    for(int i = 0; i < keypoints.size(); i++){
        if(keypoints[i].pt.x > x_begin && keypoints[i].pt.x < x_end &&
        keypoints[i].pt.y > yUpperBound && keypoints[i].pt.y < yLowerBound)
            chong = keypoints[i];
    }
}
