/*
 * TalesTeller.h
 *
 *  Created on: 17 gen 2022
 *      Author: giovanni
 */
#include <Arduino.h>
#include "Services/AudioManager.h"
#include "Services/InputManager.h"
#ifndef LIBRARIES_FURBY_TALESTELLER_H_
#define LIBRARIES_FURBY_TALESTELLER_H_
const byte TALSES_NOT_ANNOUNCED=0;
const byte TALSES_NOT_ACCEPTED=1;
const byte TALSES_ACCEPTED=2;
const byte TALSES_NOT_ENDED=3;
const byte TALSES_ENDED=4;
class TalesTeller {
public:
	TalesTeller(){};
	void setInputManager(InputManager &inputManager_){inputManager=&inputManager_;};
	void setAudioManager(AudioManager  &audioManager_){audioManager=&audioManager_;};

	void test(){

		Serial.print("tt->im");
		Serial.println((int)&(*inputManager),HEX);


		Serial.print("tt->am");
		Serial.println((int)&(*audioManager),HEX);

	}

	InputManager *inputManager;
	AudioManager *audioManager;
	int current_tale_index=0;
	int current_tale_file=0;
	byte talesMachineState=TALSES_NOT_ANNOUNCED;
	byte nr_tales_in_sd=0;
	void tellTale(){



		switch (talesMachineState) {
		case TALSES_NOT_ANNOUNCED:
			if (!audioManager->wavPlayingState){
				Serial.println("[tellTale] TALSES_NOT_ANNOUNCED");

				audioManager->audioFile="titolo.mp3";

				audioManager->audioFolder="/tales/";
				audioManager->audioFolder.concat(String(current_tale_index));
				audioManager->audioFolder.concat('/');

				talesMachineState=TALSES_NOT_ACCEPTED;
			}
			break;
		case TALSES_NOT_ACCEPTED:
			//if (!audioManager->wavPlayingState)
			{
				Serial.println("[tellTale] TALSES_NOT_ACCEPTED");

//				audioManager->audioFile="richiesta_consenso.mp3";
//				audioManager->audioFolder="/audio/";
				talesMachineState=TALSES_ACCEPTED;//attendere che finisca di parlare prima di accettare
			}
			break;
		case TALSES_ACCEPTED:
			//Serial.println("[tellTale] TALSES_ACCEPTED");
			if (!audioManager->wavPlayingState){

				if (inputManager->ext_btn_front_click==CLICK_1_SEC)//pancia verde
					talesMachineState=TALSES_NOT_ENDED;//attendere che finisca di parlare prima di accettare
				else
					if (inputManager->ext_btn_rear_click==CLICK_1_SEC)//schiena verde
					{
						talesMachineState=TALSES_NOT_ANNOUNCED;//attendere che finisca di parlare prima di accettare
						/*
						if (current_tale_index<nr_tales_in_sd-1)
							current_tale_index++;
						else
							current_tale_index=0;*/

						current_tale_index = random(0, nr_tales_in_sd);
					}
			}

			break;
		case TALSES_NOT_ENDED:
			if (!audioManager->wavPlayingState){
				audioManager->audioFile="favola.mp3";

				audioManager->audioFolder="/tales/";
				audioManager->audioFolder.concat(String(current_tale_index));
				audioManager->audioFolder.concat('/');

				talesMachineState=TALSES_ENDED;
			}
			break;
		case TALSES_ENDED:
			if (!audioManager->wavPlayingState || inputManager->ext_btn_front_click==CLICK_1_SEC){
				Serial.println("[tellTale] TALSES_END");
				audioManager->stop();
				/*
				audioManager->audioFile="finale.mp3";
				audioManager->audioFolder="/tales/";
				audioManager->audioFolder.concat(String(current_tale_index));
				audioManager->audioFolder.concat('/');
				 */
				talesMachineState=TALSES_NOT_ANNOUNCED;
				/*
				if (current_tale_index==nr_tales_in_sd-1)
					current_tale_index=0;
				else
					current_tale_index++;
				 */
				current_tale_index = random(0, nr_tales_in_sd);
			}

			break;
		}


	};

};

#endif /* LIBRARIES_FURBY_TALESTELLER_H_ */
