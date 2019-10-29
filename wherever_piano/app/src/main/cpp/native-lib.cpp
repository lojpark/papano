#include <jni.h>
#include <string>
#include <opencv2/opencv.hpp>
#include <opencv2/photo.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core.hpp>

using namespace cv;

extern "C" JNIEXPORT int JNICALL
Java_com_example_wherever_1piano_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject  instance,
        jlong matAddrInput,
        jlong matAddrResult,
        jintArray myarray){
    Mat &matInput = *(Mat *)matAddrInput;
    Mat &matResult = *(Mat *)matAddrResult;
    int rows = matInput.rows;
    int cols = matInput.cols;
    Mat bgr, hsv, mask1, mask2;
    //jintArray returns[3] = {0, 0, 0};
    //jintArray result;
    //result = env->NewIntArray(3);
    //result[0] = 1;
    jint fill[3];
    for (int i = 0; i < 3; i++) {
        fill[i] = 0; // put whatever logic you want to populate the values here.
    }

    env->SetIntArrayRegion(myarray, 0, 3, fill);


    //matResult = matInput;
    //cvtColor(matInput, matResult, COLOR_RGBA2GRAY);
    cvtColor(matInput, bgr, COLOR_RGBA2BGR);
    cvtColor(bgr, hsv, COLOR_BGR2HSV);
    inRange(hsv, Scalar(0, 60, 70), Scalar(30, 255, 255), mask1);
    inRange(hsv, Scalar(150, 60, 70), Scalar(180, 255, 255), mask2);
    //mask1 = mask1 + mask2;
    //matResult = mask1;
    matResult = hsv;

    Rect rt1(900, 600, 300, 300);
    Rect rt2(cols - 300, rows - 300, 300, 300);
    Rect rt3(0, rows - 420, cols, 300);

    flip(matResult, matResult, 1);

    rectangle(matResult, rt3, Scalar(255, 255, 255), 2);

    //env->Get
    //std::string hello = "Hello from C++";
    //return env->NewStringUTF(hello.c_str());
    //return *returns;
    return 1;
}
