package com.example.wherever_piano;

import androidx.appcompat.app.AlertDialog;
import androidx.appcompat.app.AppCompatActivity;

import android.content.DialogInterface;
import android.media.SoundPool;
import android.os.Bundle;
import android.annotation.TargetApi;
import android.content.pm.PackageManager;
import android.os.Build;
import android.util.Log;
import android.view.SurfaceView;
import android.view.WindowManager;
import android.media.MediaPlayer;
import android.media.SoundPool.Builder;
//import android.media.SoundPool;
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
    boolean[] keyboard_press = new boolean[14];
    boolean[] keyboard_available = new boolean[14];
    int[] check_array = new int[3];

    //int[] pianos = {R.raw.mf_a4, R.raw.mf_b4, R.raw.mf_c4, R.raw.mf_d4, R.raw.mf_e4, R.raw.mf_f4, R.raw.mf_g4};
    SoundPool sound_pool;
    int sound[] = new int[14];
    int sound_stream[] = new int[14];
    MediaPlayer[] mediaplayers = {new MediaPlayer(), new MediaPlayer(), new MediaPlayer(), new MediaPlayer(), new MediaPlayer(), new MediaPlayer(), new MediaPlayer(), new MediaPlayer(), new MediaPlayer()};
    private MediaPlayer[] mediaFiles;
    AudioAttributes audio_att = new AudioAttributes.Builder()
            .setUsage(AudioAttributes.USAGE_MEDIA)
            .setContentType(AudioAttributes.CONTENT_TYPE_MUSIC)
            .build();

    SoundPool sp = new SoundPool.Builder()
            .setAudioAttributes(audio_att)
            .setMaxStreams(7)
            .build();
    int[] soundIDs = {0, 0, 0, 0, 0, 0, 0};

    private CameraBridgeViewBase mOpenCvCameraView;

    public native int stringFromJNI(long matAddrInput, long matAddrResult, boolean[] key_press, boolean[] key_avail, int[] check_array);

    public void play(int num){
        if (!mediaplayers[num].isPlaying())
            mediaplayers[num].start();
        else{
            mediaplayers[num].stop();
            try {
                mediaplayers[num].setAudioAttributes(audio_att);
                mediaplayers[num].prepare();
            }
            catch (Exception e){

            }
            mediaplayers[num].start();
        }
    }

    public void play2(int num, int num2){
        if (!mediaplayers[num].isPlaying() && !mediaplayers[num2].isPlaying()) {
            mediaplayers[num].start();
            mediaplayers[num2].start();
        }
        else{
            mediaplayers[num].stop();
            mediaplayers[num2].stop();
            try {
                mediaplayers[num].setAudioAttributes(audio_att);
                mediaplayers[num].prepare();
                mediaplayers[num2].setAudioAttributes(audio_att);
                mediaplayers[num2].prepare();
            }
            catch (Exception e){

            }
            mediaplayers[num].start();
            mediaplayers[num2].start();
        }
    }


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

        for(int i = 0 ; i < 14 ; i++){
            keyboard_press[i] = false;
            keyboard_available[i] = false;
        }
        for(int i = 0 ; i < 3 ; i++){
            check_array[i] = 0;
        }

        sound_pool = new SoundPool.Builder().setMaxStreams(14).build();
        // 미리 로드.
        sound[0] = sound_pool.load(this, R.raw.mf_a4,0);
        sound[1] = sound_pool.load(this, R.raw.mf_b4,0);
        sound[2] = sound_pool.load(this, R.raw.mf_c4,0);
        sound[3] = sound_pool.load(this, R.raw.mf_d4,0);
        sound[4] = sound_pool.load(this, R.raw.mf_e4,0);
        sound[5] = sound_pool.load(this, R.raw.mf_f4,0);
        sound[6] = sound_pool.load(this, R.raw.mf_g4,0);
        sound[7] = sound_pool.load(this, R.raw.mf_a4,0);
        sound[8] = sound_pool.load(this, R.raw.mf_b4,0);
        sound[9] = sound_pool.load(this, R.raw.mf_c4,0);
        sound[10] = sound_pool.load(this, R.raw.mf_d4,0);
        sound[11] = sound_pool.load(this, R.raw.mf_e4,0);
        sound[12] = sound_pool.load(this, R.raw.mf_f4,0);
        sound[13] = sound_pool.load(this, R.raw.mf_g4,0);

        //mediaplayer.setAudioAttributes(audio_att);
        //mediaplayer = MediaPlayer.create(this, R.raw.mf_a4);
        //mediaplayer2 = MediaPlayer.create(this, R.raw.mf_b4);
        //mediaplayer3 = MediaPlayer.create(this, R.raw.mf_c4);
        //mediaplayer.setAudioAttributes(audio_att);
        //mediaplayers[0] = MediaPlayer.create(this, R.raw.mf_a4);
        //mediaplayers[1] = MediaPlayer.create(this, R.raw.mf_b4);
        //mediaplayers[2] = MediaPlayer.create(this, R.raw.mf_c4);
        //mediaplayers[3] = MediaPlayer.create(this, R.raw.mf_d4);
        //mediaplayers[4] = MediaPlayer.create(this, R.raw.mf_e4);
        //mediaplayers[5] = MediaPlayer.create(this, R.raw.mf_f4);
        //mediaplayers[6] = MediaPlayer.create(this, R.raw.mf_g4);
        //mediaplayers[7] = MediaPlayer.create(this, R.raw.mf_e4);
        //mediaplayers[8] = MediaPlayer.create(this, R.raw.mf_e4);


        //soundIDs[0] = sp.load(this, R.raw.mf_a4, 0);
        //soundIDs[1] = sp.load(this, R.raw.mf_b4, 0);
        //soundIDs[2] = sp.load(this, R.raw.mf_c4, 0);
        //soundIDs[3] = sp.load(this, R.raw.mf_d4, 0);
        //soundIDs[4] = sp.load(this, R.raw.mf_e4, 0);
        //soundIDs[5] = sp.load(this, R.raw.mf_f4, 0);
        //soundIDs[6] = sp.load(this, R.raw.mf_g4, 0);

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
            Log.d(TAG, "onResume :: OpenCV library found inside package. Using it!");
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

        //myarray[0] = 1;
        //myarray[1] = 1;
        //myarray[2] = 1;
        //int myarray;
        //rows = matInput.rows();
        //cols = matInput.cols();
        //Log.v(TAG, "rows = " + rows + "cols = " + cols);
        if ( matResult == null ) {
            matResult = new Mat(matInput.rows(), matInput.cols(), matInput.type());
        }

        stringFromJNI(matInput.getNativeObjAddr(), matResult.getNativeObjAddr(), keyboard_press, keyboard_available, check_array);
        //Log.v(TAG, "arr[0] = " + myarray[0] + "arr[1] = " + myarray[1] + "arr[2] = " + myarray[2]);
        //Log.v(TAG, "keyboard_check = " + keyboard_check);

        //if (count == 20) {
        //    sound_pool.stop(sound_stream[0]);
        //    //sound_stream[0] = sound_pool.play(sound[0], 1, 1, 0, 0, 1);
        //    sound_pool.play(sound[0], 1, 1, 0, 0, 1);
        //}
        //if (count == 40) {
        //    sound_pool.stop(sound_stream[1]);
        //    //sound_stream[1] = sound_pool.play(sound[1],1,1,1,0,1);
        //    sound_pool.play(sound[1],1,1,1,0,1);
        //}
        //if (count == 60) {
        //    sound_pool.stop(sound_stream[0]);
        //    //sound_stream[0] = sound_pool.play(sound[0], 1, 1, 0, 0, 1);
        //    sound_pool.play(sound[0], 1, 1, 0, 0, 1);
        //    sound_pool.stop(sound_stream[1]);
        //    //sound_stream[1] = sound_pool.play(sound[1],1,1,1,0,1);
        //    sound_pool.play(sound[1],1,1,1,0,1);
        //    count = 0;
        //}
        //count++;

        //int true_num = 0;
        //int true_num2 = 0;
        //for(int i = 0 ; i < 14 ; i++){
        //    if(keyboard_press[i] == true){
        //        true_num++;
        //    }
        //    if(keyboard_available[i] == true) {
        //        true_num2++;
        //    }
        //}
        //Log.v(TAG, "true num1 = " + true_num + " true num2 = " + true_num2 + " check_num = " + check_array[0]);
        for(int i = 0 ; i < 14 ; i++){
            if(keyboard_press[i] && keyboard_available[i]) {
                //true인 것 소리내기 코드
                sound_pool.stop(sound[i]);
                sound_pool.play(sound[i],1,1,1,0,1);
                //소리난것 avail 을 false로 변경
                keyboard_available[i] = false;
            }
        }
        //for(int i = 0 ; i < 14 ; i++){
        //    keyboard_press[i] = false;
            //keyboard_available[i] = false;
        //}

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