#include <jni.h>
#include <string>
#include <opencv2/opencv.hpp>
#include <opencv2/photo.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core.hpp>
#include <opencv2/video/background_segm.hpp>
using namespace cv;
using namespace std;

void removeBg(Mat& src, Ptr<BackgroundSubtractor> pBackSub) {
    Mat fgmask, ret;
    pBackSub->apply(src, fgmask, 0);

    Mat element = getStructuringElement(MORPH_RECT, Size(3, 3));
    erode(fgmask, element, 1);

    bitwise_and(src, src, ret, fgmask);

    src = ret;
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
    static Ptr<BackgroundSubtractor> pBackSub;
    pBackSub = createBackgroundSubtractorMOG2();

    //removeBg(matInput, pBackSub);
    // asdfasdf
    //mask1 = matInput;
    pBackSub->apply(matInput, mask1);

    //cvtColor(matInput, rgb, COLOR_RGBA2RGB);
    //cvtColor(rgb, ycrcb, COLOR_RGB2YCrCb);

    ///핸드폰
    //inRange(ycrcb, Scalar(0, 142, 95), Scalar(255, 165, 135), mask1);


    matResult = mask1;

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
