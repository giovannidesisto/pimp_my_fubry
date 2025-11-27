/*
 * ButtonInput.h
 *
 *  Created on: 17 gen 2022
 *      Author: giovanni
 */
#include <Arduino.h>
#include "../FurbyConstants.h"
#include "Services/AudioManager.h"
#ifndef LIBRARIES_FURBY_BUTTONINPUT_H_
#define LIBRARIES_FURBY_BUTTONINPUT_H_

class InputManager {
public:
	InputManager(){};
	//	ButtonInput(AudioManager  &audioManager_): audioManager(audioManager_){};
	//void setAudioManager(AudioManager  &audioManager_){audioManager=&audioManager_;};
	//AudioManager  *audioManager;
	//INPUT VARIABLES
	enum inputs{BTN_FRONT,BTN_REAR,BTN_TONGUE,PHOTO_FRONT,AUDIO_PLAYNG_STATE};
	bool btn_front=false;
	bool btn_rear=false;
	bool btn_cam=false;
	bool btn_encoder=false;
	bool btn_tongue=false;
	int photoFront=0;
	//int enginePhotoresitorValue=0;
	//control cycle variables
	byte btn_front_click=NOT_CLICKING;
	byte btn_rear_click=NOT_CLICKING;
	unsigned long btn_front_cycle_counter=0;
	unsigned long btn_rear_cycle_counter=0;
	unsigned long prev_btn_front_cycle_counter=0;
	unsigned long prev_btn_rear_cycle_counter=0;

	//neopixel drive flag (avoid multiple call in same state)
	bool btn_set_cliking=true;
	bool btn_set_1_set_click=true;
	bool btn_set_2_set_click=true;
	bool btn_set_3_set_click=true;
	bool btn_set_off=true;
	//external logic method variables
	byte ext_btn_front_click=NOT_CLICKING;
	byte ext_btn_rear_click=NOT_CLICKING;
	int encoderLimit=0;
	int encoderAnalogValue=0;
	void readInputValues(){
		btn_front=digitalRead(BTN_FRONT_PIN)==1?true:false;
		btn_tongue=digitalRead(BTN_TONGUE_PIN)==1?true:false;


		btn_cam=digitalRead(BTN_CAM_PIN)==1?true:false;
		btn_rear=digitalRead(BTN_REAR_PIN)==1?true:false;
		//Serial.print("analogRead(BTN_ENCODER_PIN) ");
		//Serial.println(analogRead(BTN_ENCODER_PIN));
		encoderAnalogValue=analogRead(BTN_ENCODER_PIN);
		btn_encoder=encoderAnalogValue>encoderLimit?true:false;
		photoFront=analogRead(FRONT_PHOTORESISTOR_SIGNAL_PIN);
		//enginePhotoresitorValue=analogRead(ENGINE_PHOTO_RESISTOR_SIGNAL_PIN);
	};
	unsigned long delta_front=0;
	unsigned long delta_rear=0;
	unsigned long prev_front_click=0;
	boolean front_release=false;
	boolean front_double_click=false;

	unsigned long prev_rear_click=0;
	boolean rear_release=false;
	boolean rear_double_click=false;

	void checkButtonClikingState(){
		////Serial.printf("[checkButtonClikingState] front %d  rear \n",btn_front_cycle_counter,btn_rear_cycle_counter);
		front_double_click=false;
		rear_double_click=false;
		if(!btn_front){
			front_release=true;
		}
		if(!btn_rear){
			rear_release=true;
		}

		if (btn_front && btn_rear) {
			ext_btn_rear_click=NOT_CLICKING;
			btn_rear_click=NOT_CLICKING;
			ext_btn_front_click=NOT_CLICKING;
			btn_front_click=NOT_CLICKING;
			btn_front_cycle_counter=0;
			btn_rear_cycle_counter=0;
			prev_btn_front_cycle_counter=0;
			prev_btn_rear_cycle_counter=0;
		} else

		{

			btn_front_cycle_counter=millis();
			btn_rear_cycle_counter=millis();
			//			prev_btn_front_cycle_counter=0;

			/*

			prev_btn_rear_cycle_counter=0;
			 */

			if (btn_front && !btn_rear)
			{
				if(prev_btn_front_cycle_counter > 0)
				{
					delta_front = btn_front_cycle_counter - prev_btn_front_cycle_counter;
					if(delta_front > 0 &&  delta_front < NUM_CYCLE_1_SEC_CLICK)
					{
						btn_front_click =CLICKING;
						if(prev_front_click==0)prev_front_click=btn_front_cycle_counter;
						else if(front_release){
							if(btn_front_cycle_counter-prev_front_click<500)
							{
								Serial.println("f double click");
								front_double_click=true;
							}
							prev_front_click=0;
							front_release=false;
						}

					}
					else if (delta_front > NUM_CYCLE_1_SEC_CLICK &&  delta_front < NUM_CYCLE_2_SEC_CLICK)
						btn_front_click=CLICK_1_SEC;
					else if (delta_front > NUM_CYCLE_2_SEC_CLICK &&  delta_front < NUM_CYCLE_3_SEC_CLICK)
						btn_front_click=CLICK_2_SEC;
					else if (delta_front > NUM_CYCLE_3_SEC_CLICK &&  delta_front < NUM_CYCLE_4_SEC_CLICK)
						btn_front_click=CLICK_3_SEC;
					else if (delta_front > NUM_CYCLE_4_SEC_CLICK)
					{
						prev_btn_front_cycle_counter=0;
						btn_front_click=NOT_CLICKING;
					}
				}
				else prev_btn_front_cycle_counter=btn_front_cycle_counter;
			}
			else{
				ext_btn_front_click=btn_front_click;
				btn_front_click=NOT_CLICKING;
				prev_btn_front_cycle_counter=0;
			}

			if (btn_rear && !btn_front)
			{
				if(prev_btn_rear_cycle_counter > 0)
				{
					delta_rear = btn_rear_cycle_counter - prev_btn_rear_cycle_counter;
					if(delta_rear > 0 &&  delta_rear < NUM_CYCLE_1_SEC_CLICK)
					{
						btn_rear_click =CLICKING;
						if(prev_rear_click==0)prev_rear_click=btn_rear_cycle_counter;
						else if(rear_release){
							if(btn_rear_cycle_counter-prev_rear_click<500)
							{
								Serial.println("r double click");
								rear_double_click=true;
							}
							prev_rear_click=0;
							rear_release=false;
						}
					}
					else if (delta_rear > NUM_CYCLE_1_SEC_CLICK &&  delta_rear < NUM_CYCLE_2_SEC_CLICK)
						btn_rear_click=CLICK_1_SEC;
					else if (delta_rear > NUM_CYCLE_2_SEC_CLICK &&  delta_rear < NUM_CYCLE_3_SEC_CLICK)
						btn_rear_click=CLICK_2_SEC;
					else if (delta_rear > NUM_CYCLE_3_SEC_CLICK &&  delta_rear < NUM_CYCLE_4_SEC_CLICK)
						btn_rear_click=CLICK_3_SEC;
					else if (delta_rear > NUM_CYCLE_4_SEC_CLICK)
					{
						prev_btn_rear_cycle_counter=0;
						btn_rear_click=NOT_CLICKING;
					}
				}
				else prev_btn_rear_cycle_counter=btn_rear_cycle_counter;
			}
			else{
				ext_btn_rear_click=btn_rear_click;
				btn_rear_click=NOT_CLICKING;
				prev_btn_rear_cycle_counter=0;
			}



		}


	}
	;/*
	void test(){
		int memAudio=&audioManager;
		Serial.print("ButtonInput memAudio address: ");
		Serial.println(memAudio,HEX);
	};
	 */
};

#endif /* LIBRARIES_FURBY_BUTTONINPUT_H_ */
