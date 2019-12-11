#include <jni.h>
#include <string>
#include <opencv2/opencv.hpp>
#include <opencv2/photo.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core.hpp>
#include <android/log.h>

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
    Mat rgb, hsv, mask1, mask2, mask3;
    int key_num = 21;
    Point left_dot, right_dot;
    jboolean press_fill[21];
    jboolean avail_fill[21];
    jint check_fill[3];
    jint win_resol[2];
    int fill_num = 0;

    env->GetIntArrayRegion(window_resolution, 0, 2, win_resol);

    //press_fill array 초기화
    for (int i = 0; i < key_num; i++) {
        press_fill[i] = false;
    }

    //env->SetBooleanArrayRegion(keyboard_press, 0, 14, press_fill);
    //env->SetBooleanArrayRegion(keyboard_press, 0, 14, press_fill);
    // keyboard_avail array를 avail_fill에 복사
    env->GetBooleanArrayRegion(keyboard_avail, 0, key_num, avail_fill);

    //matResult = matInput;
    //cvtColor(matInput, matResult, COLOR_RGBA2GRAY);
    /// 웹캠(AVD)를 사용할 경우 입력으로 들어온  matInput의 타입을 BGR에서 RGB로 변환하여 rgb에 저장
    //cvtColor(matInput, rgb, COLOR_BGR2RGB);
    /// 웹캠(AVD)를 사용할 경우 rgb의 타입을 HSV로 변환하여 hsv에 저장
    //cvtColor(rgb, hsv, COLOR_RGB2HSV);
    ///핸드폰 카메라를 사용할 경우
    //cvtColor(matInput.clone(), rgb, COLOR_BGR2RGB);
    cvtColor(matInput.clone(), hsv, COLOR_RGB2HSV);

    /// hsv에서 특정 색을 분리하여 mask1, mask2에 저장하고 mask1에 합친다
    ///빨강추출
    //inRange(hsv, Scalar(0, 75, 100), Scalar(10, 255, 255), mask1);
    //inRange(hsv, Scalar(170, 75, 100), Scalar(180, 255, 255), mask2);

    ///녹색추출
    //inRange(hsv, Scalar(55, 25, 25), Scalar(85, 255, 255), mask1);
    //inRange(hsv.clone(), Scalar(55, 25, 20), Scalar(85, 255, 255), mask3);

    ///보라색추출
    //inRange(hsv, Scalar(55, 25, 25), Scalar(85, 255, 255), mask1);
    inRange(hsv.clone(), Scalar(120, 80, 80), Scalar(130, 255, 255), mask3);

    ///파랑추출
    ///웹캠
    //inRange(hsv.clone(), Scalar(110, 75, 100), Scalar(130, 255, 255), mask1);
    ///핸드폰
    inRange(hsv.clone(), Scalar(95, 25, 30), Scalar(115, 255, 255), mask1);

    ///빨간색 추출시 사용
    //mask1 = mask1 + mask2;

    //matResult = mask1 + mask3;
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
    vector<vector<Point>> contours2;
    findContours(mask1.clone(), contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point(0, 0));
    vector<Rect> rects;
    vector<RotatedRect> r_rects;
    for(int i = 0 ; i < (int)contours.size() ; i++){
        if(contourArea(contours[i]) > 200) {
            Rect rec = boundingRect(contours[i]);
            rects.push_back(rec);
            RotatedRect r_rec = minAreaRect(contours[i]);
            r_rects.push_back(r_rec);
        }
    }
    findContours(mask3.clone(), contours2, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point(0, 0));
    vector<Rect> rects2;
    vector<RotatedRect> r_rects2;
    for(int i = 0 ; i < (int)contours2.size() ; i++){
        if(contourArea(contours2[i]) > 100) {
            Rect rec = boundingRect(contours2[i]);
            rects2.push_back(rec);
            RotatedRect r_rec = minAreaRect(contours2[i]);
            Point2f vertices2f[4];
            r_rec.points(vertices2f);
            Point vertices[4];
            for(int i = 0; i < 4; ++i){
                vertices[i] = vertices2f[i];
            }
            Point center = (vertices[0] + vertices[1] + vertices[2] + vertices[3]) / 4;
            //if(center.y < ((win_resol[0] * 2) / 3)/* && center.y > win_resol[0] / 3*/) {
                r_rects2.push_back(r_rec);
            //}
        }
    }

    /// RGB를 사용하기 위해서 mask의 자료형을 RGB로 변환
    cvtColor(mask1, mask1, COLOR_BGR2RGB);
    cvtColor(mask3, mask3, COLOR_BGR2RGB);
    //matResult = mask1 + mask3;
    ///인식 기준선 그리기
    //rectangle(matResult, rt3, Scalar(255, 0, 0), 2);


    for(int i = 0 ; i < (int)rects.size() ; i++){
        ///Rect 녹색으로 그리기
        //rectangle(mask1, rects.at(i), Scalar(0, 255, 0), 2);

        Point2f vertices2f[4];
        RotatedRect temp_ro_rect = r_rects.at(i);
        temp_ro_rect.points(vertices2f);
        Point vertices[4];
        for(int i = 0; i < 4; ++i){
            vertices[i] = vertices2f[i];
        }
        Scalar temp_color = Scalar(0, 0, 255);
        Point *polygon[1] = { vertices };
        int npts[1] = {4};
        //fillConvexPoly(mask1, vertices, 4, temp_color);

        ///Rotated rect 파란색으로 그리기
        //polylines(mask1, polygon, npts, 1, true, temp_color, 2);
    }
    for(int i = 0 ; i < (int)rects2.size() ; i++){

        ///Rect 녹색으로 그리기
        //rectangle(mask3, rects2.at(i), Scalar(0, 255, 0), 2);

        Point2f vertices2f[4];
        RotatedRect temp_ro_rect = r_rects2.at(i);
        temp_ro_rect.points(vertices2f);
        Point vertices[4];
        for(int i = 0; i < 4; ++i){
            vertices[i] = vertices2f[i];
        }
        if(r_rects2.size() == 2){
            if(i == 0){
                left_dot = (vertices[0] + vertices[1] + vertices[2] + vertices[3]) / 4;
            }
            else if(i == 1){
                right_dot = (vertices[0] + vertices[1] + vertices[2] + vertices[3]) / 4;
                if(left_dot.x > right_dot.x){
                    Point temp_p = right_dot;
                    right_dot = left_dot;
                    left_dot = temp_p;
                }
            }
        }
        Scalar temp_color = Scalar(0, 0, 255);
        Point *polygon[1] = { vertices };
        int npts[1] = {4};
        //fillConvexPoly(mask1, vertices, 4, temp_color);

        ///Rotated rect 파란색으로 그리기
        polylines(mask3, polygon, npts, 1, true, temp_color, 2);
    }
    //const char *c1 = (to_string(left_dot.x)).c_str();
    //const char *c2 = (to_string(left_dot.y)).c_str();
    //const char *c3 = (to_string(right_dot.x)).c_str();
    //const char *c4 = (to_string(right_dot.y)).c_str();
    //__android_log_print(4, "leftdot = ", "%s %s", c1, c2);
    //__android_log_print(4, "rightdot = ", "%s %s", c3, c4);

    //cvtColor(matResult, matResult, COLOR_BGR2RGB);
    ///추출한 결과를 matResult에 저장
    //matResult = mask1 + mask3;
    //matResult = mask1;
    //matResult = mask3;
    //matResult = rgb;
    matResult = matInput;
    ///웹캠(AVD) 사용시 좌우반전
    //flip(matResult, matResult, 1);
    rectangle(matResult, Point(0, win_resol[0] / 3), Point(win_resol[1], win_resol[0] * 2 / 3), Scalar(255, 0, 0), 2);
    //rectangle(matResult, Point(0, 0), Point(100, 100), Scalar(255, 0, 0), 2);
    //matResult = hsv;

    Vec3b color;
    //Vec3i color2;
    //int x;
    //int z = abs(left_dot.x - right_dot.x) * 27 / 562;
    //x = left_dot.x + z;
    //int y = (left_dot.y + right_dot.y) / 2;
    //const char *c5 = (to_string(win_resol[0])).c_str();
    //const char *c6 = (to_string(win_resol[1])).c_str();
    //__android_log_print(4, "myxy = ", "%s %s", c5, c6);


    ///건반이 눌렸는지, avail 혹은 unavail인지 확인
    //int interval = cols / 14;
    //int sub_interval = interval / 9;
    //for(int i = 0 ; i < key_num ; i++) {
    //    for(int j = i * interval + 4 * sub_interval ; j < i * interval + 5 * sub_interval ; j++){
    //        color = matResult.at<Vec3b>(Point(j, y));
    //        if(/*j == i * interval + interval / 2 && */color.val[0] == 0 && fill_num < 3){
    //            press_fill[i] = true;
    //            fill_num++;
    //            break;
    //        }
    //        else if(j == (i * interval + 5 * sub_interval - 1)){
    //            avail_fill[i] = true;
    //        }
    //    }
    //    //if(fill_num == 3){
    //    //    fill_num = 0;
    //    //    break;
    //    //}
    //}

    //int interval = abs(left_dot.x - right_dot.x) * 254 / (281 * 14);
    //int sub_interval = interval / 3;
    //const char *c7 = (to_string(interval)).c_str();
    //const char *c8 = (to_string(sub_interval)).c_str();
    //__android_log_print(4, "myinterval = ", "%s %s", c7, c8);
    //line(matResult, Point(x, y), Point(right_dot.x - z, y), Scalar(255, 0, 0), 2);
    //line(matResult, left_dot, right_dot, Scalar(255, 0, 0), 2);
    //
    //for(int i = 0 ; i < key_num ; i++) {
    //    for(int j = x + i * interval + 1 * sub_interval ; j < x + i * interval + 2 * sub_interval ; j++){
    //        color = mask1.at<Vec3b>(Point(j, y));
    //        if(/*j == i * interval + interval / 2 && */color.val[0] == 0 && fill_num < 3){
    //            press_fill[i] = true;
    //            fill_num++;
    //            break;
    //        }
    //        else if(j == (x + i * interval + 2 * sub_interval - 1)){
    //            avail_fill[i] = true;
    //        }
    //    }
    //}

    int interval = 463 / key_num;
    int sub_interval = interval / 3;
    //const char *c7 = (to_string(interval)).c_str();
    //const char *c8 = (to_string(sub_interval)).c_str();
    //__android_log_print(4, "myinterval = ", "%s %s", c7, c8);
    //line(matResult, Point(x, y), Point(right_dot.x - z, y), Scalar(255, 0, 0), 2);
    //line(matResult, left_dot, right_dot, Scalar(255, 0, 0), 2);
    Point a1;
    Point a2;


    for(int i = 0 ; i < key_num ; i++) {
        //int recogzation_start = i * interval + (i * (interval - sub_interval) / (key_num - 1));
        //if(i == 0){
        //    recogzation_start+=3;
        //}
        int recogzation_start = i * interval + 1 * sub_interval;
        // for(Point p = left_dot * (254 - (27 + i * interval + 1 * sub_interval)) + right_dot *  ; j < x + i * interval + 2 * sub_interval ; j++){
        for(int j = 0 ; j < sub_interval ; j++){
            if(j == 0){
                a1 = (left_dot * (1070 - (71 + (recogzation_start + j) * 2)) + right_dot * (71 + (recogzation_start + j) * 2)) / 1070;
            }
            else if(j == sub_interval - 1){
                a2 = (left_dot * (1070 - (71 + (recogzation_start + j) * 2)) + right_dot * (71 + (recogzation_start + j) * 2)) / 1070;
                line(matResult, a1, a2, Scalar(255, 0, 0), 2);
            }
            //if(i == 0 && j == 0){
            //    j = 3;
            //}
            //color = mask1.at<Vec3b>((left_dot * (562 - (27 + (recogzation_start + j) * 2)) + right_dot * (27 + (recogzation_start + j) * 2)) / 562);
            if(j == 0){
                color = mask1.at<Vec3b>(a1);
            }
            else if(j == sub_interval - 1){
                color = mask1.at<Vec3b>(a2);
            }
            //color = mask1.at<Vec3b>((left_dot * (562 - (27 + (i * interval + (i * (interval - sub_interval) / 13) + j) * 2)) + right_dot * (27 + (i * interval + (i * (interval - sub_interval) / 13) + j) * 2)) / 562);
            if(color.val[0] == 0 && fill_num < 3){
                press_fill[i] = true;
                fill_num++;
                break;
            }
            else if(j == sub_interval - 1){
                avail_fill[i] = true;
            }
        }
    }

    ///최종정보를 자바배열에 전달
    env->SetBooleanArrayRegion(keyboard_press, 0, 21, press_fill);
    env->SetBooleanArrayRegion(keyboard_avail, 0, 21, avail_fill);
    //check_fill[0] = fill_num + 4;
    //env->SetIntArrayRegion(check_array, 0, 3, check_fill);
    //env->Get
    //std::string hello = "Hello from C++";
    //return env->NewStringUTF(hello.c_str());
    //return *returns;
    
    return key_num;
}
