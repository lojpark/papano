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
        jlong matAddrResult){
    Mat &matInput = *(Mat *)matAddrInput;
    Mat &matResult = *(Mat *)matAddrResult;

    cvtColor(matInput, matResult, COLOR_RGBA2GRAY);
    //std::string hello = "Hello from C++";
    //return env->NewStringUTF(hello.c_str());
    return 1;
}
