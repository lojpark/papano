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
    /*
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

    return x;
}
