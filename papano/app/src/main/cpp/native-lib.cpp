#include <jni.h>
#include <string>
#include <algorithm>
#include <opencv2/opencv.hpp>
#include <opencv2/photo.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core.hpp>
#include <opencv2/video/background_segm.hpp>
#include <android/log.h>


using namespace cv;
using namespace std;

bool cmp(Point t, Point u) {
    return t.y > u.y;
}

void removeBg(Mat& src, Ptr<BackgroundSubtractor> pBackSub) {
    Mat fgmask, ret;
    pBackSub->apply(src, fgmask, 0);

    Mat element = getStructuringElement(MORPH_RECT, Size(3, 3), Point(1, 1));
    erode(fgmask, fgmask, element);

    bitwise_and(src, src, ret, fgmask);

    src = ret;
}

Point getCentroid(vector<Point> contour) {


    Moments moment = moments(contour);
    Point ret(0, 0);

    if (moment.m00 != 0) {
        ret.x = (int)(moment.m10 / moment.m00);
        ret.y = (int)(moment.m01 / moment.m00);
    }
    return ret;
}

vector<Point> getFarthestPoints(vector<Point> contour, Point centroid) {
    int n = contour.size();
    vector<Point> ret;

    if (n == 0 || (centroid.x == 0 && centroid.y == 0)) {
        return ret;
    }

    sort(contour.begin(), contour.end(), cmp);
    for (int i = 0; i < n; i++) {
        if (contour[i].y < centroid.y) {
            continue;
        }

        int minDistance = 2147483647;
        int m = ret.size();
        for (int j = 0; j < m; j++) {
            if (minDistance > abs(ret[j].x - contour[i].x)) {
                minDistance = abs(ret[j].x - contour[i].x);
            }
        }

        if (minDistance >= 100) {
            ret.push_back(contour[i]);
        }

        if (ret.size() >= 5) {
            break;
        }
    }

    return ret;
}

void cacluateFingers(vector<Point> contour, Mat& src) {
    //Mat ret;
    Point centroid = getCentroid(contour);

    //__android_log_print(ANDROID_LOG_INFO,"OPENCVTEST - JNI", "%d", n);
    circle(src, centroid, 12, Scalar(255, 0, 255), 10);

    vector<Point> farthestPoints = getFarthestPoints(contour, centroid);
    for (int i = 0; i < (int)farthestPoints.size(); i++) {
        circle(src, farthestPoints[i], 12, Scalar(255, 0, 0), 10);
    }

    //src = ret;
}

extern "C" JNIEXPORT int JNICALL
Java_com_example_wherever_1piano_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject  instance,
        jlong matAddrInput,
        jlong matAddrResult,
        jbooleanArray keyboard_press,
        jbooleanArray keyboard_avail,
        jintArray window_resolution){
    Mat &matInput = *(Mat *)matAddrInput;
    Mat &matResult = *(Mat *)matAddrResult;
    int rows = matInput.rows;
    int cols = matInput.cols;
    Mat rgb, ycrcb, mask1, mask2;
    int main_rect_num = 0;
    double max_rect_size = 0;
    int keyborad_check = 1;
    int key_num = 14;

    jboolean press_fill[14];
    jboolean avail_fill[14];
    jint check_fill[3];
    jint win_resol[2];
    int fill_num = 0;

    env->SetIntArrayRegion(window_resolution, 0, 2, win_resol);


    //press_fill array 초기화
    for (int i = 0; i < key_num; i++) {
        press_fill[i] = false;
    }

    // keyboard_avail array를 avail_fill에 복사
    env->GetBooleanArrayRegion(keyboard_avail, 0, 14, avail_fill);

    // Set background
    //static Mat background = matInput;
    static Ptr<BackgroundSubtractor> pBackSub = createBackgroundSubtractorMOG2();

    removeBg(matInput, pBackSub);

    cvtColor(matInput, rgb, COLOR_RGBA2RGB);
    cvtColor(rgb, ycrcb, COLOR_RGB2YCrCb);

    ///핸드폰
    inRange(ycrcb, Scalar(0, 132, 85), Scalar(255, 175, 145), mask1);

    vector<vector<Point>> contours;
    double largest_area = 0.0;
    int largest_contour_index = 0;

    findContours(mask1.clone(), contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point(0, 0));

    if (!contours.empty())
    {
        matResult = Scalar(0,0,0);

        // Find largest contour
        for (size_t i = 0; i < contours.size(); i++)
        {
            double a = contourArea(contours[i], false);
            if (a > largest_area)
            {
                largest_area = a;
                largest_contour_index = i;
            }
        }

        // Draw largest contors
        drawContours(matResult, contours, largest_contour_index, Scalar(255, 255, 255), 1);

        // Find convex hull of largest contour
        vector<Point>hull;
        convexHull(contours[largest_contour_index], hull, true, true);

        // Draw the convex hull
        vector<vector<Point>> tmp;
        tmp.push_back(hull);
        drawContours(matResult, tmp, 0, Scalar(0, 0, 255), 3);

        cacluateFingers(contours[largest_contour_index], matResult);
    }

    //flip(mask1, mask1, 1);
/*
    vector<vector<Point>> contours;

    findContours(mask1.clone(), contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point(0, 0));
    vector<Rect> rects;
    vector<RotatedRect> r_rects;
    for(int i = 0 ; i < (int)contours.size() ; i++){
        if(contours[i].size() > 100) {
            Rect rec = boundingRect(contours[i]);
            rects.push_back(rec);
            RotatedRect r_rec = minAreaRect(contours[i]);
            r_rects.push_back(r_rec);
        }
    }

    /// RGB를 사용하기 위해서 mask1의 자료형을 RGB로 변환
    cvtColor(mask1, mask1, COLOR_BGR2RGB);

    for(int i = 0 ; i < (int)rects.size() ; i++){
        rectangle(mask1, rects.at(i), Scalar(0, 255, 0), 2);
    }

    ///추출한 결과를 matResult에 저장
    matResult = mask1;
    ///웹캠(AVD) 사용시 좌우반전
    //flip(matResult, matResult, 1);

    Vec3b color;
    int x = 0;
    int y = matResult.rows - 130;
    ///건반이 눌렸는지, avail 혹은 unavail인지 확인

    int interval = cols / 14;
    int sub_interval = interval / 9;
    for(int i = 0 ; i < key_num ; i++) {
        for(int j = i * interval + 4 * sub_interval ; j < i * interval + 5 * sub_interval ; j++){
            color = matResult.at<Vec3b>(Point(j, y));
            if(color.val[0] == 0 && fill_num < 3){
                press_fill[i] = true;
                fill_num++;
                break;
            }
            else if(j == (i * interval + 5 * sub_interval - 1)){
                avail_fill[i] = true;
            }
        }
    }
    */
    ///최종정보를 자바배열에 전달
    env->SetBooleanArrayRegion(keyboard_press, 0, 14, press_fill);
    env->SetBooleanArrayRegion(keyboard_avail, 0, 14, avail_fill);

    return 0;
}
