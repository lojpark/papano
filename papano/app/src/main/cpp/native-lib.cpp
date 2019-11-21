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
        jintArray window_resolution,
        jboolean isTracking){
    Mat &matInput = *(Mat *)matAddrInput;
    Mat &matResult = *(Mat *)matAddrResult;
    int rows = matInput.rows;
    int cols = matInput.cols;
    Mat rgb, hsv, mask1, mask2;
    int key_num = 14;
    jboolean press_fill[14];
    jboolean avail_fill[14];
    jint win_resol[2];
    int fill_num = 0;

    env->SetIntArrayRegion(window_resolution, 0, 2, win_resol);

    //press_fill array 초기화
    for (int i = 0; i < key_num; i++) {
        press_fill[i] = false;
    }

    // keyboard_avail array를 avail_fill에 복사
    env->GetBooleanArrayRegion(keyboard_avail, 0, 14, avail_fill);

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
    ///핸드폰
    inRange(hsv, Scalar(100, 30, 120), Scalar(120, 255, 255), mask1);

    matResult = mask1;

    ///임의의 사각형 생성
    ///현재 가로세로 픽셀 = 1280, 960 하지만 카메라에 나타나는 영역픽셀 = 1280, 720
    ///AVD사용시 사각형
    //Rect rt3(0, rows - 270, cols, 150);
    ///핸드폰 카메라 사용시 사각형
    Rect rt3(0, rows - 270, cols, 150);


    vector<vector<Point>> contours;
    findContours(mask1.clone(), contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point(0, 0));

    /// RGB를 사용하기 위해서 mask1의 자료형을 RGB로 변환
    cvtColor(mask1, mask1, COLOR_BGR2RGB);
    ///인식 기준선 그리기
    //rectangle(mask1, rt3, Scalar(255, 0, 0), 2);

    unsigned int maxSize = 0;
    static unsigned int prevSize = 0;
    static unsigned int trackCount = 0;
    static Rect trackedRect(0, 0, 0, 0);

    if (isTracking) {
        Rect maxRect(0, 0, 0, 0);
        for (auto &contour : contours) {
            if (contour.size() > maxSize) {
                maxRect = boundingRect(contour);
                maxSize = contour.size();
            }
        }

        unsigned int deltaSize = (prevSize > maxSize) ? prevSize - maxSize : maxSize - prevSize;
        __android_log_print(ANDROID_LOG_INFO,"OPENCVTEST - JNI", "%d, %d", maxRect.height, maxRect.width);
        if ((prevSize == 0 || deltaSize < 200) && maxRect.height > 100 && maxRect.width > 1000) {
            prevSize = maxSize;
            trackCount++;
            if (trackCount >= 100) {
                isTracking = (jboolean) false;
                trackedRect = maxRect;
            }
        }
        else {
            prevSize = 0;
            trackCount = 0;
        }

        rectangle(mask1, maxRect, Scalar(0, 255, 0), 2);
    }

    if (!isTracking) {
        rectangle(mask1, trackedRect, Scalar(255, 0, 0), 2);
    }

    //cvtColor(matResult, matResult, COLOR_BGR2RGB);
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
    ///최종정보를 자바배열에 전달
    env->SetBooleanArrayRegion(keyboard_press, 0, 14, press_fill);
    env->SetBooleanArrayRegion(keyboard_avail, 0, 14, avail_fill);

    return isTracking;
}
