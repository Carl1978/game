/****************************************************************************
Copyright (c) 2015-2016 Chukong Technologies Inc.
Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.
 
http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/
package org.cocos2dx.cpp;

import org.cocos2dx.lib.Cocos2dxActivity;
import android.Manifest;
//import android.app.Activity;

import android.content.pm.PackageManager;
import android.os.Bundle;

import com.memirsoftware.game.R;
import android.widget.TextView;

public class AppActivity extends Cocos2dxActivity implements OnVolumeChangedListener {

    private RecorderThread recThread;

    private static final int MY_PERMISSIONS_REQUEST_RECORD_AUDIO = 1;

    public static native void sendVolume(float volume);

    // Requesting permission to RECORD_AUDIO
    @Override
    public void onRequestPermissionsResult(int requestCode,
                                           String permissions[], int[] grantResults) {
        switch (requestCode) {
            case MY_PERMISSIONS_REQUEST_RECORD_AUDIO: {
                // If request is cancelled, the result arrays are empty.
                if (grantResults.length > 0
                        && grantResults[0] == PackageManager.PERMISSION_GRANTED) {
                    // permission was granted, yay! Do the
                    // contacts-related task you need to do.
//                    TextView tv2 = (TextView) findViewById(R.id.IDLabel2);
//                    tv2.setText("Permission was granted!");
                } else {
                    // permission denied, boo! Disable the
                    // functionality that depends on this permission.
//                    TextView tv2 = (TextView) findViewById(R.id.IDLabel2);
//                    tv2.setText("Permission was denied  again!");
                }
                return;
            }

            // other 'case' lines to check for other
            // permissions this app might request.
        }
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        //super.setEnableVirtualButton(false);
        super.onCreate(savedInstanceState);
        //setContentView(R.layout.layout);

        if (this.checkSelfPermission(Manifest.permission.RECORD_AUDIO)
                != PackageManager.PERMISSION_GRANTED) {
            // Permission is not granted
//            TextView tv = (TextView) findViewById(R.id.IDLabel);
//            tv.setText("Permission is not granted");

            // Permission is not granted
            // Should we show an explanation?
            if (this.shouldShowRequestPermissionRationale(Manifest.permission.RECORD_AUDIO)) {
                // Show an explanation to the user *asynchronously* -- don't block
                // this thread waiting for the user's response! After the user
                // sees the explanation, try again to request the permission.
            } else {
                // No explanation needed; request the permission
                this.requestPermissions(new String[]{Manifest.permission.RECORD_AUDIO},
                        MY_PERMISSIONS_REQUEST_RECORD_AUDIO);

                // MY_PERMISSIONS_REQUEST_READ_CONTACTS is an
                // app-defined int constant. The callback method gets the
                // result of the request.
            }
        }
        else {
//            TextView tv = (TextView) findViewById(R.id.IDLabel);
//            tv.setText("Permission is granted");
////            recThread = new RecorderThread();
////            recThread.start();
////            recThread.setOnVolumeChangedListener(this);
        }

        //ActivityCompat.requestPermissions(this, permissions, REQUEST_RECORD_AUDIO_PERMISSION);

//        TextView tv = (TextView) findViewById(R.id.IDLabel);
//        tv.setText("Carl Phillips");

//        recThread = new RecorderThread();
//        recThread.start();
//        recThread.setOnVolumeChangedListener(this);
        // maybe we can call  in here!

        // Workaround in https://stackoverflow.com/questions/16283079/re-launch-of-activity-on-home-button-but-only-the-first-time/16447508
        if (!isTaskRoot()) {
            // Android launched another instance of the root activity into an existing task
            //  so just quietly finish and go away, dropping the user back into the activity
            //  at the top of the stack (ie: the last state of this task)
            // Don't need to finish it again since it's finished in super.onCreate .
            return;
        }
        // DO OTHER INITIALIZATION BELOW
        recThread = new RecorderThread();
        recThread.start();
        recThread.setOnVolumeChangedListener(this);
    }

    @Override
    protected void onResume() {
        super.onResume();

        // crashes with this code here!
//        TextView tv = (TextView) findViewById(R.id.IDLabel);
//        //tv.setText("recThread.RECORDER_CHANNELS : " + recThread.RECORDER_CHANNELS);
//        tv.setText("recThread.audioRecorderState : " + recThread.audioRecorderState);
//
//        TextView tv2 = (TextView) findViewById(R.id.IDLabel2);
//        tv2.setText("recThread.RECORDER_AUDIO_ENCODING : " + recThread.RECORDER_AUDIO_ENCODING);
//
//        TextView tv3 = (TextView) findViewById(R.id.IDLabel3);
//        tv3.setText("recThread.RECORDER_SAMPLERATE : " + recThread.RECORDER_SAMPLERATE);
//
//        TextView tv4 = (TextView) findViewById(R.id.IDLabel4);
//        tv4.setText("recThread.bufferSizeInBytes : " + recThread.bufferSizeInBytes);

        //TextView tv5 = (TextView) findViewById(R.id.IDLabel5);
        //tv5.setText("recThread.audioRecorderState : " + recThread.audioRecorderState);

        //tv.setText("bufferSizeInBytes : " + recThread.bufferSizeInBytes); // small ~4k
        //tv.setText("audioRecorderState : " + recThread.audioRecorderState);

        if (this.checkSelfPermission(Manifest.permission.RECORD_AUDIO)
                == PackageManager.PERMISSION_GRANTED) {
            // Permission is granted
            recThread.startRecording();
            synchronized (recThread) {
                recThread.notify();
            }
        }
    }

    @Override
    protected void onPause() {
        super.onPause();

        recThread.stopRecording();
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();

        recThread.releaseResources();
    }

    @Override
     public void onVolumeChange(final float newVolume) {
         runOnUiThread(new Runnable() {
             public void run() {
                 sendVolume(newVolume);
//                 TextView tv3 = (TextView) findViewById(R.id.IDLabel3);
//                 tv3.setText("newVolume : " + newVolume);
             }
         });
     }

}
