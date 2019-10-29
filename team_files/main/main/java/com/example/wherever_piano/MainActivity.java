package com.example.wherever_piano;

import androidx.appcompat.app.AlertDialog;
import androidx.appcompat.app.AppCompatActivity;

import android.content.DialogInterface;
import android.os.Bundle;
import android.annotation.TargetApi;
import android.content.pm.PackageManager;
import android.os.Build;
import android.util.Log;
import android.view.SurfaceView;
import android.view.WindowManager;
import android.media.MediaPlayer;
import android.media.AudioAttributes;
import org.opencv.android.BaseLoaderCallback;
import org.opencv.android.CameraBridgeViewBase;
import org.opencv.android.LoaderCallbackInterface;
import org.opencv.android.OpenCVLoader;
import org.opencv.core.Mat;

import java.util.Collections;
import java.util.List;
import java.util.concurrent.ExecutionException;

import static android.Manifest.permission.CAMERA;



public class MainActivity extends AppCompatActivity
        implements CameraBridgeViewBase.CvCameraViewListener2 {

    private static final String TAG = "opencv";
    private static final String TAG2 = "MainActivity";

    private Mat matInput;
    private Mat matResult;
    public  int count = 0;
    public  int entered_key = 0;
    public  int rows = 0;
    public  int cols = 0;
    int[] pianos = {R.raw.mf_a4, R.raw.mf_b4, R.raw.mf_c4, R.raw.mf_d4, R.raw.mf_e4, R.raw.mf_f4, R.raw.mf_g4};

    MediaPlayer mediaplayer = new MediaPlayer();
    MediaPlayer mediaplayer2 = new MediaPlayer();
    MediaPlayer mediaplayer3 = new MediaPlayer();
    MediaPlayer[] mediaplayers = {mediaplayer, mediaplayer2, mediaplayer3};
    private MediaPlayer[] mediaFiles;
    AudioAttributes audio_att = new AudioAttributes.Builder()
            .setUsage(AudioAttributes.USAGE_MEDIA)
            .setContentType(AudioAttributes.CONTENT_TYPE_MUSIC)
            .build();

    private CameraBridgeViewBase mOpenCvCameraView;

    public native int stringFromJNI(long matAddrInput, long matAddrResult, int[] myarr);


    static {
        System.loadLibrary("opencv_java4");
        System.loadLibrary("native-lib");
    }



    private BaseLoaderCallback mLoaderCallback = new BaseLoaderCallback(this) {
        @Override
        public void onManagerConnected(int status) {
            switch (status) {
                case LoaderCallbackInterface.SUCCESS:
                {
                    mOpenCvCameraView.enableView();
                } break;
                default:
                {
                    super.onManagerConnected(status);
                } break;
            }
        }
    };


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN,
                WindowManager.LayoutParams.FLAG_FULLSCREEN);
        getWindow().setFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON,
                WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);

        setContentView(R.layout.activity_main);

        //mediaplayer.setAudioAttributes(audio_att);
        mediaplayer = MediaPlayer.create(this, R.raw.mf_a4);
        mediaplayer2 = MediaPlayer.create(this, R.raw.mf_b4);
        mediaplayer3 = MediaPlayer.create(this, R.raw.mf_c4);
        //mediaplayer.setAudioAttributes(audio_att);
        mediaplayers[0] = mediaplayer;
        mediaplayers[1] = mediaplayer2;
        mediaplayers[2] = mediaplayer3;

        mOpenCvCameraView = (CameraBridgeViewBase)findViewById(R.id.activity_surface_view);
        mOpenCvCameraView.setVisibility(SurfaceView.VISIBLE);
        mOpenCvCameraView.setCvCameraViewListener(this);
        mOpenCvCameraView.setCameraIndex(0); // front-camera(1),  back-camera(0)
    }

    @Override
    public void onPause()
    {
        super.onPause();
        if (mOpenCvCameraView != null)
            mOpenCvCameraView.disableView();
    }

    @Override
    public void onResume()
    {
        super.onResume();

        if (!OpenCVLoader.initDebug()) {
            Log.d(TAG, "onResume :: Internal OpenCV library not found.");
            OpenCVLoader.initAsync(OpenCVLoader.OPENCV_VERSION_3_2_0, this, mLoaderCallback);
        } else {
            Log.d(TAG, "onResum :: OpenCV library found inside package. Using it!");
            mLoaderCallback.onManagerConnected(LoaderCallbackInterface.SUCCESS);
        }
    }


    public void onDestroy() {
        super.onDestroy();

        if (mOpenCvCameraView != null)
            mOpenCvCameraView.disableView();
    }

    @Override
    public void onCameraViewStarted(int width, int height) {

    }

    @Override
    public void onCameraViewStopped() {

    }

    @Override
    public Mat onCameraFrame(CameraBridgeViewBase.CvCameraViewFrame inputFrame) {

        matInput = inputFrame.rgba();
        int[] myarray = new int[3];
        myarray[0] = 1;
        myarray[1] = 1;
        myarray[2] = 1;
        //int myarray;
        //rows = matInput.rows();
        //cols = matInput.cols();
        //Log.v(TAG, "rows = " + rows + "cols = " + cols);
        if ( matResult == null ) {
            matResult = new Mat(matInput.rows(), matInput.cols(), matInput.type());
        }

        int b = stringFromJNI(matInput.getNativeObjAddr(), matResult.getNativeObjAddr(), myarray);
        Log.v(TAG, "arr[0] = " + myarray[0] + "arr[1] = " + myarray[1] + "arr[2] = " + myarray[2]);

        //Log.v(TAG, "mycount = " + count);
        //if(count == 6){
        //    mediaplayer.release();
        //}
        if(count == 0 || count == 30 /*&& entered_key == 1*/) {
            //try {
            //mediaplayer.setDataSource("D:/AndroidStudioProjects/wherever_piano/app/src/main/res/raw/mf_f4.wav");
            //mediaplayer.setDataSource(this, "res/raw/");
            //if(mediaplayer.isPlaying()){
            //    mediaplayer.stop();
            //}
            //mediaplayer = MediaPlayer.create(this, R.raw.mf_f4);
            //if(mediaplayer.isPlaying()){
            //    mediaplayer.stop();
            //}
            //mediaplayer.prepare();
            if (!mediaplayers[0].isPlaying())
                mediaplayers[0].start();
            else{
                //mediaplayer.setAudioAttributes(audio_att);
                mediaplayers[0].stop();
                //mediaplayer = MediaPlayer.create(this, R.raw.mf_f4);
                try {
                    mediaplayers[0].setAudioAttributes(audio_att);
                    mediaplayers[0].prepare();
                }
                catch (Exception e){

                }
                mediaplayers[0].start();
            }
            //else{
            //    mediaplayer.stop();
            //    try {
            //        mediaplayer.prepare();
            //    }
            //    catch (Exception e){
//
            //    }
            //    mediaplayer.start();
            //}
            //mediaplayer.release();
            count++;
            //} catch (Exception e) {

            //}
            //count = 0;
        //} else if (count == 6 && mediaplayer.isPlaying()) {
        //    mediaplayer.stop();
        //    try {
        //        mediaplayer.prepare();
        //    }
        //    catch (Exception e){
//
        //    }

        } else if(count == 10 || count == 40) {
            if (!mediaplayers[1].isPlaying())
                mediaplayers[1].start();
            else{
                mediaplayers[1].stop();
                try {
                    mediaplayers[1].setAudioAttributes(audio_att);
                    mediaplayers[1].prepare();
                }
                catch (Exception e){

                }
                mediaplayers[1].start();
            }
            count++;
        } else if(count == 20 || count == 50) {
            if (!mediaplayers[2].isPlaying())
                mediaplayers[2].start();
            else{
                mediaplayers[2].stop();
                try {
                    mediaplayers[2].setAudioAttributes(audio_att);
                    mediaplayers[2].prepare();
                }
                catch (Exception e){

                }
                mediaplayers[2].start();
            }
            if(count == 50) {
                count = -10;
            }
            else if(count == 20){
                count++;
            }
        } else {
            count++;
        }


        return matResult;
    }


    protected List<? extends CameraBridgeViewBase> getCameraViewList() {
        return Collections.singletonList(mOpenCvCameraView);
    }


    //여기서부턴 퍼미션 관련 메소드
    private static final int CAMERA_PERMISSION_REQUEST_CODE = 200;


    protected void onCameraPermissionGranted() {
        List<? extends CameraBridgeViewBase> cameraViews = getCameraViewList();
        if (cameraViews == null) {
            return;
        }
        for (CameraBridgeViewBase cameraBridgeViewBase: cameraViews) {
            if (cameraBridgeViewBase != null) {
                cameraBridgeViewBase.setCameraPermissionGranted();
            }
        }
    }

    @Override
    protected void onStart() {
        super.onStart();
        boolean havePermission = true;
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {
            if (checkSelfPermission(CAMERA) != PackageManager.PERMISSION_GRANTED) {
                requestPermissions(new String[]{CAMERA}, CAMERA_PERMISSION_REQUEST_CODE);
                havePermission = false;
            }
        }
        if (havePermission) {
            onCameraPermissionGranted();
        }
    }

    @Override
    @TargetApi(Build.VERSION_CODES.M)
    public void onRequestPermissionsResult(int requestCode, String[] permissions, int[] grantResults) {
        if (requestCode == CAMERA_PERMISSION_REQUEST_CODE && grantResults.length > 0
                && grantResults[0] == PackageManager.PERMISSION_GRANTED) {
            onCameraPermissionGranted();
        }else{
            showDialogForPermission("앱을 실행하려면 퍼미션을 허가하셔야합니다.");
        }
        super.onRequestPermissionsResult(requestCode, permissions, grantResults);
    }


    @TargetApi(Build.VERSION_CODES.M)
    private void showDialogForPermission(String msg) {

        AlertDialog.Builder builder = new AlertDialog.Builder( MainActivity.this);
        builder.setTitle("알림");
        builder.setMessage(msg);
        builder.setCancelable(false);
        builder.setPositiveButton("예", new DialogInterface.OnClickListener() {
            public void onClick(DialogInterface dialog, int id){
                requestPermissions(new String[]{CAMERA}, CAMERA_PERMISSION_REQUEST_CODE);
            }
        });
        builder.setNegativeButton("아니오", new DialogInterface.OnClickListener() {
            public void onClick(DialogInterface arg0, int arg1) {
                finish();
            }
        });
        builder.create().show();
    }


}