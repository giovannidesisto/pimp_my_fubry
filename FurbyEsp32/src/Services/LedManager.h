/*
 * FurbyLed.h
 *
 *  Created on: 17 gen 2022
 *      Author: giovanni
 */
#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include "Services/InputManager.h"
#ifndef LIBRARIES_FURBY_FURBYLED_H_
#define LIBRARIES_FURBY_FURBYLED_H_

class LedManager {
public:
	int ledB=50;
	Adafruit_NeoPixel pixels =Adafruit_NeoPixel(NUMPIXELS, NEOPIXEL_COMMAND_PIN, NEO_GRB + NEO_KHZ800);
	LedManager(){};
	void setInputManager(InputManager &inputManager_){inputManager=&inputManager_;};

	void test(){

		Serial.print("LedManager->InputManager ");
		Serial.println((int)&(*inputManager),HEX);
	}

	InputManager *inputManager;

	unsigned long blinking_counter=0;
	unsigned long prev_blinking_counter=0;
	unsigned long ref=250;
	boolean blinking_toggle=true;


	void init(){
		pixels.begin();
		pixels.clear();
		pixels.show();
		//pixels.setBrightness(10);
	};



	void setNexopielState(){

		if (inputManager->btn_rear && inputManager->btn_front)
		//if(true)
		{ //premendo entrambe i pulsanti, il led lampeggia [non verrà eseguito alcun comando]

			blinking_counter = millis();
			if(blinking_counter-prev_blinking_counter> ref){
				blinking_toggle=!blinking_toggle;
				prev_blinking_counter= blinking_counter;

			}


			if (prev_blinking_counter==blinking_counter)
			{



				if (blinking_toggle) {
					pixels.setPixelColor(0, ledB, 0, 0);//green
					pixels.show();
					Serial.printf("ac %d p %d t %d \r\n",blinking_counter,prev_blinking_counter,blinking_toggle);
				} else {
					pixels.setPixelColor(0, 0, 0, 0);//green
					pixels.show();
					Serial.printf("bc %d p %d t %d \r\n",blinking_counter,prev_blinking_counter,blinking_toggle);
				}
			}

		} else {
			if (inputManager->btn_front && !inputManager->btn_rear) {
				inputManager->btn_set_off=true;
				switch (inputManager->btn_front_click) {
				case CLICKING:
					if (inputManager->btn_set_cliking) {
						pixels.setPixelColor(0, ledB, 0, 0);//red
						pixels.show();
						inputManager->btn_set_cliking=false;
						inputManager->btn_set_3_set_click=true;
					}
					break;
				case CLICK_1_SEC:
					if (inputManager->btn_set_1_set_click) {
						pixels.setPixelColor(0, 0, ledB, 0);//green
						pixels.show();
						inputManager->btn_set_cliking=true;
						inputManager->btn_set_1_set_click=false;
					}
					break;
				case CLICK_2_SEC:
					if (inputManager->btn_set_2_set_click) {
						pixels.setPixelColor(0, 0, 0, ledB);//blue
						pixels.show();
						inputManager->btn_set_1_set_click=true;
						inputManager->btn_set_2_set_click=false;
					}
					break;
				case CLICK_3_SEC:
					if (inputManager->btn_set_3_set_click) {
						pixels.setPixelColor(0, ledB, ledB, 0);//yellow
						pixels.show();
						inputManager->btn_set_2_set_click=true;
						inputManager->btn_set_3_set_click=false;
					}
					break;
				}//end switch
			} else
				if (inputManager->btn_set_off && inputManager->ext_btn_front_click==NOT_CLICKING) {
					pixels.setPixelColor(0, 0, 0, 0);//off
					pixels.show();
					inputManager->btn_set_off=false;
					inputManager->btn_set_cliking=true;
					inputManager->btn_set_1_set_click=true;
					inputManager->btn_set_2_set_click=true;
					inputManager->btn_set_3_set_click=true;
				}

			/////REAR BUTTON
			if (inputManager->btn_rear && !inputManager->btn_front) {
				inputManager->btn_set_off=true;
				switch (inputManager->btn_rear_click) {
				case CLICKING:
					if (inputManager->btn_set_cliking) {
						pixels.setPixelColor(0, ledB, 0, 0);//red
						pixels.show();
						inputManager->btn_set_cliking=false;
						inputManager->btn_set_3_set_click=true;
					}
					break;
				case CLICK_1_SEC:
					if (inputManager->btn_set_1_set_click) {
						pixels.setPixelColor(0, 0, ledB, 0);//green
						pixels.show();
						inputManager->btn_set_cliking=true;
						inputManager->btn_set_1_set_click=false;
					}
					break;
				case CLICK_2_SEC:
					if (inputManager->btn_set_2_set_click) {
						pixels.setPixelColor(0, 0, 0, ledB);//blue
						pixels.show();
						inputManager->btn_set_1_set_click=true;
						inputManager->btn_set_2_set_click=false;
					}
					break;
				case CLICK_3_SEC:
					if (inputManager->btn_set_3_set_click) {
						pixels.setPixelColor(0, ledB, ledB, 0);//yellow
						pixels.show();
						inputManager->btn_set_2_set_click=true;
						inputManager->btn_set_3_set_click=false;
					}
					break;
				}
			} else
				if (inputManager->btn_set_off && inputManager->ext_btn_rear_click==NOT_CLICKING) {
					pixels.setPixelColor(0, 0, 0, 0);//off
					pixels.show();
					inputManager->btn_set_off=false;
					inputManager->btn_set_cliking=true;
					inputManager->btn_set_1_set_click=true;
					inputManager->btn_set_2_set_click=true;
					inputManager->btn_set_3_set_click=true;
				}

		}
	};





	// Input a value 0 to 255 to get a color value.
	// The colours are a transition r - g - b - back to r.
	uint32_t Wheel(byte WheelPos) {
	  if(WheelPos < 85) {
	    return pixels.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
	  }
	  else if(WheelPos < 170) {
	    WheelPos -= 85;
	    return pixels.Color(255 - WheelPos * 3, 0, WheelPos * 3);
	  }
	  else {
	    WheelPos -= 170;
	    return pixels.Color(0, WheelPos * 3, 255 - WheelPos * 3);
	  }
	};


};

#endif /* LIBRARIES_FURBY_FURBYLED_H_ */
