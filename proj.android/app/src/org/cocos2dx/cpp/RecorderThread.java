package org.cocos2dx.cpp;

import android.media.AudioFormat;
import android.media.AudioRecord;
import android.media.MediaRecorder;

public class RecorderThread extends Thread {
	private volatile boolean running = true;
	private volatile boolean suspended = false;
	
	private AudioRecord audioRecorder;
	public int RECORDER_CHANNELS = AudioFormat.CHANNEL_IN_MONO; // 16
	public int RECORDER_AUDIO_ENCODING = AudioFormat.ENCODING_PCM_16BIT; // 2
	public int RECORDER_SAMPLERATE = 44100;
	public int bufferSizeInBytes;
	private byte[] audioBuffer;
	public int audioRecorderState = 11; // soem dummy random value for now
	
	private float lastVolume = 0.0f;
	private OnVolumeChangedListener onVolumeChangedListener;
	
	public RecorderThread() {
		bufferSizeInBytes = AudioRecord.getMinBufferSize(RECORDER_SAMPLERATE, RECORDER_CHANNELS, 
														 RECORDER_AUDIO_ENCODING);
		audioRecorder = new AudioRecord(MediaRecorder.AudioSource.MIC , RECORDER_SAMPLERATE, RECORDER_CHANNELS, 
									  RECORDER_AUDIO_ENCODING, bufferSizeInBytes);
		audioRecorderState = audioRecorder.getState();
		audioBuffer = new byte[bufferSizeInBytes];
	}
	
	public boolean isRecording() {
		return this.isAlive() && audioRecorder.getRecordingState() == AudioRecord.RECORDSTATE_RECORDING;
	}
	
	public float getVolume() {
		if(isRecording()) {
			int numOfReadBytes = audioRecorder.read(audioBuffer, 0, bufferSizeInBytes);
			
			int totalAbsValue = 0;
			short sample = 0;
			
			for(int i = 0; i < bufferSizeInBytes; i += 2) {
				sample = (short) ((audioBuffer[i]) | audioBuffer[i+1] << 8);
				totalAbsValue += Math.abs(sample);
			}
			return totalAbsValue / (numOfReadBytes / 2);
		}
		else {
			return 0.0f;
		}
	}
	
	public void run() {
		while(running) {
			try {
				if(suspended) {
					synchronized (this) {
						wait();
					}
				}
			} catch(InterruptedException e) {
			}
			float volume = getVolume();
			if(volume != lastVolume) {
				lastVolume = volume;
				onVolumeChange();
			}
		}
	}
	
	public void startRecording() {
		audioRecorder.startRecording();
		suspended = false;
	}
	
	public void stopRecording() {
		audioRecorder.stop();
		suspended = true;
	}
	
	public void releaseResources() {
		audioRecorder.release();
		running = false;
	}
	
	private void onVolumeChange() {
		if(onVolumeChangedListener != null) {
			onVolumeChangedListener.onVolumeChange(lastVolume);
		}
	}
	
	public void setOnVolumeChangedListener(OnVolumeChangedListener listener) {
		onVolumeChangedListener = listener;
	}
}
