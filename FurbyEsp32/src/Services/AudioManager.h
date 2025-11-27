/*
 * AudioManager.h
 *
 *  Created on: 17 gen 2022
 *      Author: giovanni
 */
#include <Arduino.h>
#include "../FurbyConstants.h"


#include <AudioOutputI2S.h>
#include "AudioFileSourceSD.h"
#include "AudioGeneratorMP3.h"

#ifndef LIBRARIES_FURBY_AUDIOMANAGER_H_
#define LIBRARIES_FURBY_AUDIOMANAGER_H_


AudioGeneratorMP3 *mp3;
AudioFileSourceSD *source;
AudioOutputI2S *out;



class AudioManager {


public:
	AudioManager(){

	};
	String audioFolder="";
	String audioFile="";
	float volume=0.5;
	float maxVolume=1.0;
	boolean wavPlayingState=false;




	void init(){

		source = new AudioFileSourceSD();
		out = new AudioOutputI2S();
		mp3 = new AudioGeneratorMP3();
		out->SetPinout(I2S_BCLK, I2S_LRC, I2S_DOUT);
		out->SetGain(volume);


	};
	void setVolume(){
		out->SetGain(volume);
	};

	void playAudio(){



		if(audioFile!=""){
			String currentFile ="";
			currentFile.concat(audioFolder);
			currentFile.concat(audioFile);
			Serial.print("audio file to play:");
			Serial.println(currentFile.c_str());
			out->SetGain(volume);
			source->open(currentFile.c_str());
			mp3->begin(source, out);
			wavPlayingState=true;
			audioFile="";
			audioFolder="";
		}


	};
	void setPlaiyngState(){

		if (mp3->isRunning())
		{
			if (!mp3->loop()){
				mp3->stop();
				wavPlayingState=false;
			}
		}
		else
			wavPlayingState=false;

	};

	void stop(){

		mp3->stop();
		wavPlayingState=false;

	};

};

#endif /* LIBRARIES_FURBY_AUDIOMANAGER_H_ */
