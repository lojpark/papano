#include <jni.h>
#include <string>
#include <opencv2/opencv.hpp>
#include <opencv2/photo.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core.hpp>

using namespace cv;
using namespace std;


extern "C" JNIEXPORT int JNICALL
Java_com_example_wherever_1piano_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject  instance,
        jlong matAddrInput,
        jlong matAddrResult,
        jbooleanArray keyboard_press,
        jbooleanArray keyboard_avail,
        jintArray check_array){
    Mat &matInput = *(Mat *)matAddrInput;
    Mat &matResult = *(Mat *)matAddrResult;
    int rows = matInput.rows;
    int cols = matInput.cols;
    Mat rgb, hsv, mask1, mask2;
    int main_rect_num = 0;
    double max_rect_size = 0;
    int keyborad_check = 1;
    int key_num = 14;
    //jintArray returns[3] = {0, 0, 0};
    //jintArray result;
    //result = env->NewIntArray(3);
    //result[0] = 1;
    jboolean press_fill[14];
    jboolean avail_fill[14];
    jint check_fill[3];
    int fill_num = 0;

    //for (int i = 0; i < 3; i++) {
    //    check_fill[i] = 1;
    //}
    //env->SetIntArrayRegion(check_array, 0, 3, check_fill);

    //press_fill array 초기화
    for (int i = 0; i < key_num; i++) {
        press_fill[i] = false;
    }

    //env->SetBooleanArrayRegion(keyboard_press, 0, 14, press_fill);
    //env->SetBooleanArrayRegion(keyboard_press, 0, 14, press_fill);
    // keyboard_avail array를 avail_fill에 복사
    env->GetBooleanArrayRegion(keyboard_avail, 0, 14, avail_fill);

    //matResult = matInput;
    //cvtColor(matInput, matResult, COLOR_RGBA2GRAY);
    // 입력으로 들어온  matInput의 타입을 BGR에서 RGB로 변환하여 rgb에 저장
    cvtColor(matInput, rgb, COLOR_BGR2RGB);
    // rgb의 타입을 HSV로 변환하여 hsv에 저장
    cvtColor(rgb, hsv, COLOR_RGB2HSV);

    // hsv에서 특정 색을 분리하여 mask1, mask2에 저장하고 mask1에 합친다
    inRange(hsv, Scalar(0, 75, 100), Scalar(10, 255, 255), mask1);
    inRange(hsv, Scalar(170, 75, 100), Scalar(180, 255, 255), mask2);

    mask1 = mask1 + mask2;
    //matResult = mask1;
    matResult = mask1;
    //matResult = hsv;
    //matResult = rgb;

    //임의의 사각형 생성
    //현재 가로세로 픽셀 = 1280, 960 하지만 카메라에 나타나는 영역픽셀 = 1280, 720
    Rect rt1(900, 600, 300, 300);
    Rect rt2(cols - 300, rows - 300, 300, 300);
    Rect rt3(10, rows - 270, cols - 10, 150);

    //flip(mask1, mask1, 1);

    //vector<vector<Point>> contours;
    //vector<vector<Point>> contours2;
    //vector<Point> mycontour1;
    //mycontour1.push_back(Point(300, 300));
    //mycontour1.push_back(Point(400, 300));
    //mycontour1.push_back(Point(300, 400));
    //contours2.push_back(mycontour1);
    //findContours(mask1.clone(), contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point(0, 0));
    //vector<RotatedRect> rects;
    //for(int i = 0 ; i < (int)contours.size() ; i++){
    //    if(contours[i].size() > 100) {
//
    //        RotatedRect mr = minAreaRect(contours[i]);
//
    //        rects.push_back(mr);
    //    }
//
    //}
    //matResult = mask1;
    //rectangle(mask1, rt3, Scalar(255, 0, 0), 2);

    // 빨간색을 사용하기 위해서 mask1의 자료형을 RGB로 변환
    cvtColor(mask1, mask1, COLOR_BGR2RGB);
    rectangle(mask1, rt3, Scalar(255, 0, 0), 2);
    //drawContours(mask1, contours, -1, Scalar(255, 0, 0), 2);
    //if(contours.size() > 0){
    //    rectangle(mask1, rt3, Scalar(255, 0, 0), 2);
    //    for(int i = 0 ; i < contours.size() ; i ++){
    //        if(i == 0) {
    //            main_rect_num = i;
    //            max_rect_size = contourArea(contours.at(i));
    //        }
    //        else{
    //            if(contourArea(contours.at(i)) > max_rect_size){
    //                main_rect_num = i;
    //                max_rect_size = contourArea(contours.at(i));
    //            }
    //        }
    //    }
    //    Rect main_rect = boundingRect(contours.at(main_rect_num));
    //    //rectangle(mask1, main_rect, Scalar(255, 0, 0), 2);
    //}
    //Rect main_rect = boundingRect(contours.at(main_rect_num));
    //rectangle(mask1, main_rect, Scalar(255, 0, 0), 2);
    //for(int i = 0 ; i < contours.size() ; i ++){
    //    if(i = 0) {
    //        main_rect_num = i;
    //    }
    //}

    //matResult = mask1;
    //rectangle(matResult, rt3, Scalar(255, 255, 255), 2);


    //Vec3b color = matResult.at<Vec3b>(Point(1, 1));


    //cvtColor(matResult, matResult, COLOR_BGR2RGB);
    matResult = mask1;
    flip(matResult, matResult, 1);


    Vec3b color;
    //Vec3i color2;
    int x = 0;
    int y = matResult.rows - 130;
    //line(matResult, Point(10, y), Point(1270, y), Scalar(0, 255, 0), 2);
    //for(x = 10 ; x < matResult.cols - 10 ; x++){
    //    color = matResult.at<Vec3b>(Point(x, y));
    //    //color2 = matResult.at<Vec3i>(Point(x, y));
    //    if(color.val[0] == 0){
    //        keyborad_check = 0;
    //        break;
    //    }
    //    else{
    //        matResult.at<Vec3b>(y - 40, x).val[0] = 255;
    //        matResult.at<Vec3b>(y - 40, x).val[1] = 0;
    //        matResult.at<Vec3b>(y - 40, x).val[2] = 0;
    //        matResult.at<Vec3b>(x, y).val[0] = 0;
    //        matResult.at<Vec3b>(x, y).val[1] = 0;
    //        matResult.at<Vec3b>(x, y).val[2] = 255;
    //    }
    //}
    //fill_num = 0;
    for(int i = 0 ; i < key_num ; i++) {
        for(int j = 10 + i * 90 + 30 ; j < 10 + i * 90 + 60 ; j++){
            color = matResult.at<Vec3b>(Point(j, y));
            if(color.val[0] == 0 && fill_num < 3){
                press_fill[i] = true;
                fill_num++;
                break;
            }
            else if(j == (10 + i * 90 + 60 - 1)){
                avail_fill[i] = true;
            }
        }
        //if(fill_num == 3){
        //    fill_num = 0;
        //    break;
        //}
    }
    env->SetBooleanArrayRegion(keyboard_press, 0, 14, press_fill);
    env->SetBooleanArrayRegion(keyboard_avail, 0, 14, avail_fill);
    //check_fill[0] = fill_num + 4;
    //env->SetIntArrayRegion(check_array, 0, 3, check_fill);
    //env->Get
    //std::string hello = "Hello from C++";
    //return env->NewStringUTF(hello.c_str());
    //return *returns;
    return x;
}
