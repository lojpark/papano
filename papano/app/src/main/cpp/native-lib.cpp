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
        jintArray window_resolution){
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
    jint win_resol[2];
    int fill_num = 0;

    env->SetIntArrayRegion(window_resolution, 0, 2, win_resol);

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
    /// 웹캠(AVD)를 사용할 경우 입력으로 들어온  matInput의 타입을 BGR에서 RGB로 변환하여 rgb에 저장
    //cvtColor(matInput, rgb, COLOR_BGR2RGB);
    /// 웹캠(AVD)를 사용할 경우 rgb의 타입을 HSV로 변환하여 hsv에 저장
    //cvtColor(rgb, hsv, COLOR_RGB2HSV);
    ///핸드폰 카메라를 사용할 경우
    cvtColor(matInput, rgb, COLOR_RGBA2RGB);
    cvtColor(rgb, hsv, COLOR_RGB2HSV);

    /// hsv에서 특정 색을 분리하여 mask1, mask2에 저장하고 mask1에 합친다
    ///빨강추출
    //inRange(hsv, Scalar(0, 75, 100), Scalar(10, 255, 255), mask1);
    //inRange(hsv, Scalar(170, 75, 100), Scalar(180, 255, 255), mask2);

    ///녹색추출
    //inRange(hsv, Scalar(55, 25, 25), Scalar(85, 255, 255), mask1);

    ///파랑추출
    ///웹캠
    //inRange(hsv, Scalar(110, 75, 100), Scalar(130, 255, 255), mask1);
    ///핸드폰
    inRange(hsv, Scalar(100, 30, 120), Scalar(120, 255, 255), mask1);

    ///빨간색 추출시 사용
    //mask1 = mask1 + mask2;

    matResult = mask1;
    //matResult = hsv;
    //matResult = rgb;

    ///임의의 사각형 생성
    ///현재 가로세로 픽셀 = 1280, 960 하지만 카메라에 나타나는 영역픽셀 = 1280, 720
    Rect rt1(900, 600, 300, 300);
    Rect rt2(cols - 300, rows - 300, 300, 300);
    ///AVD사용시 사각형
    //Rect rt3(0, rows - 270, cols, 150);
    ///핸드폰 카메라 사용시 사각형
    Rect rt3(0, rows - 270, cols, 150);

    //flip(mask1, mask1, 1);

    vector<vector<Point>> contours;
    //vector<vector<Point>> contours2;
    //vector<Point> mycontour1;
    //mycontour1.push_back(Point(300, 300));
    //mycontour1.push_back(Point(400, 300));
    //mycontour1.push_back(Point(300, 400));
    //contours2.push_back(mycontour1);
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
    //matResult = mask1;
    //rectangle(mask1, rt3, Scalar(255, 0, 0), 2);

    /// RGB를 사용하기 위해서 mask1의 자료형을 RGB로 변환
    cvtColor(mask1, mask1, COLOR_BGR2RGB);
    ///인식 기준선 그리기
    rectangle(mask1, rt3, Scalar(255, 0, 0), 2);


    for(int i = 0 ; i < (int)rects.size() ; i++){
        rectangle(mask1, rects.at(i), Scalar(0, 255, 0), 2);

        //Point2f vertices2f[4];
        //RotatedRect temp_ro_rect = r_rects.at(0);
        //temp_ro_rect.points(vertices2f);
        //Point vertices[4];
        //for(int i = 0; i < 4; ++i){
        //    vertices[i] = vertices2f[i];
        //}
        //Scalar temp_color = Scalar(0, 255, 0);
        //fillConvexPoly(mask1, vertices, 4, temp_color);
    }


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
    ///추출한 결과를 matResult에 저장
    matResult = mask1;
    ///웹캠(AVD) 사용시 좌우반전
    //flip(matResult, matResult, 1);

    //matResult = hsv;

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
    ///건반이 눌렸는지, avail 혹은 unavail인지 확인
    int interval = cols / 14;
    int sub_interval = interval / 9;
    for(int i = 0 ; i < key_num ; i++) {
        for(int j = i * interval + 4 * sub_interval ; j < i * interval + 5 * sub_interval ; j++){
            color = matResult.at<Vec3b>(Point(j, y));
            if(/*j == i * interval + interval / 2 && */color.val[0] == 0 && fill_num < 3){
                press_fill[i] = true;
                fill_num++;
                break;
            }
            else if(j == (i * interval + 5 * sub_interval - 1)){
                avail_fill[i] = true;
            }
        }
        //if(fill_num == 3){
        //    fill_num = 0;
        //    break;
        //}
    }
    ///최종정보를 자바배열에 전달
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
