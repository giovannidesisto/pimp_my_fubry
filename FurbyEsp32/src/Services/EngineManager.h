/*
 * Engine.h
 *
 *  Created on: 17 gen 2022
 *      Author: giovanni
 */
#include <Arduino.h>
#include <analogWrite.h>
#include "../FurbyConstants.h"
#include "Services/InputManager.h"

#ifndef LIBRARIES_FURBY_ENGINE_H_
#define LIBRARIES_FURBY_ENGINE_H_
#define  motionDataRows 100
#define motionDataCols 4
class EngineManager {
public:

	EngineManager(){};//ButtonInput bi
	void setInputManager(InputManager &inputManager_){inputManager=&inputManager_;};


	void test(){
	//	Serial.print("em->im:");
		Serial.println((int) &(*inputManager), HEX);
	};

	/////vars controllo motore
	InputManager *inputManager;

	//int enginePhotoresistorBias=0;


	bool gotoSpecifiedPositionFlag=false;
	bool commandToParseAvailable=false;
	bool toggleGotoSpecifiedPosition=false;
	String command = "";

	int positionToReach=0;
	int pwmEngineValue=0;
	bool rotationFwdMode=false;
	int totalStep=0;
	int motionIndex=-1;
	int currentStepCounter=0;
	//int motionDataRows=100;
	//int motionDataCols=4;
	int currentCamPosition=0;
	boolean encoderToggle=false;
	/**
	 * FORMATO FILE
	 *
	 * ID_PASSO
	 * POSIZIONE
	 * SPEED
	 * DIREZIONE ROTAZIONE
	 */
	int motionData[motionDataRows][motionDataCols];
	void initEngineData(){
		for(int i=0;i<motionDataRows;i++)
			for(int y=0;y<motionDataCols;y++)
				motionData[i][y]=-1;
	};


	void print(){
		for(int i=0;i<motionDataRows;i++)
				{	for(int y=0;y<motionDataCols;y++)
						{
					Serial.print('\t');
							Serial.print(motionData[i][y]);
						}
				Serial.print('\n');
				}
	};


	void driveEngine(){

		//se deve raggiungere una spec pos,oppure, è in corso il movimento per raggiungerla
		if (gotoSpecifiedPositionFlag)
			gotoPos(positionToReach,pwmEngineValue,rotationFwdMode);
		//diversamente, verifica che non si tratti di una procedura composta da n passi
		else if (totalStep!=0)//se sto eseguendo una routine di passi
		{
			if (currentStepCounter==totalStep)//se l'ultimo passo è in corso
			{
				/*
				//se non ha finito di parlare, allora resetto la routine per consentire di mantenere animazione e audio
				if (!wavPlayingState) { //&& execRutine=="parla" mettere indice alla routine corrente
					execRutine=-1;
					//commandToParseAvailable=false;
				}
				 */
				currentStepCounter=0;
				totalStep=0;
				motionIndex=-1;
			}
			else//imposta il prossimo passo della routine
			{
				//engineCommandString=getValue(execRutine, ';', currentStepCounter);
				//imposta i valori di posizione, velocità e rotazione
				positionToReach=motionData[motionIndex][1];
				pwmEngineValue=motionData[motionIndex][2];
				rotationFwdMode=motionData[motionIndex][3]==0?false:true;

				gotoSpecifiedPositionFlag=true;//imposta il flag per andare ad una specifica posizione
				motionIndex++;//seleziona prossimo passo della routine
				currentStepCounter++;//aggiorna il contapassi
			}
		}

	}
;

	void gotoPos(int pos, int speedValue,boolean sxRotation){
//		Serial.println("gotopos ");
//					Serial.println(currentCamPosition);
		//se la posizione richiesta è zero, attende il contatto cam, diversamente la posizione encoder
		if ((pos!=0 && currentCamPosition!=pos)||(pos==0 && !inputManager->btn_cam))
		{
			if (!toggleGotoSpecifiedPosition) {
				digitalWrite(CMD_ENCODER_LED_PIN,HIGH);
				analogWrite(DC_ENGINE_FWD_COMMAND_PIN,sxRotation?speedValue:0);
				analogWrite(DC_ENGINE_BKW_COMMAND_PIN,sxRotation?0:speedValue);
				toggleGotoSpecifiedPosition=true;
			}



				//Serial.print("analogRead(BTN_CAM_PIN):" );
				//Serial.print(analogRead(BTN_CAM_PIN));
/*
				Serial.print("CAM:" );
				Serial.print(inputManager->btn_cam?150:0);
				Serial.print("\tcurrentCamPosition:" );
				Serial.print(currentCamPosition);
				Serial.print("\tencoderAnalogValue:");
				Serial.print(inputManager->encoderAnalogValue);
				Serial.print("\tencoderRef:");
				Serial.println(inputManager->encoderLimit);
*/
				/*
				Serial.print("\t cam " );
				Serial.print(inputManager->btn_cam);
				Serial.print("\t enc ");
				Serial.println(inputManager->btn_encoder);
				*/

		}

		else {
			//Serial.print("reached counter: ");
			//Serial.println(currentCamPosition);
			analogWrite(DC_ENGINE_FWD_COMMAND_PIN,0);
			analogWrite(DC_ENGINE_BKW_COMMAND_PIN,0);
			digitalWrite(CMD_ENCODER_LED_PIN,LOW);
			gotoSpecifiedPositionFlag=false;
			toggleGotoSpecifiedPosition=false;

		}

	};
	void cam(){
		//leggo il valore analogico della fotoresistenza
		//photoresitorVoltageValue = analogRead(sensorPin);
		// Serial.println(enginePhotoresitorValue);

		if (!inputManager->btn_cam) {
			//se il valore rilevato è inferiore a un determinato valore soglia, aumento il numero di giri effettuati
			if (encoderToggle && inputManager->btn_encoder) {
				if (rotationFwdMode) {
					if (currentCamPosition<208)
						currentCamPosition++;
					else
						currentCamPosition=0;
				} else {
					if (currentCamPosition > 0)
						currentCamPosition--;
					else
						currentCamPosition =208;
				}
				encoderToggle=false;



			} else if (!inputManager->btn_encoder)//altrimenti, superata la soglia, attendo che il valore torni nuovamente sotto soglia
					encoderToggle=true;

		//	Serial.print("cam ");
		//	Serial.println(inputManager->btn_encoder);

		} else
			//diversamente, la camma ha chisuo il contatto e, solo per la prima chiusura, stampo il contagiri e resetto le variabili
			//di ciclo
		{
/*
			Serial.print("cam r " );
				Serial.print(currentCamPosition);
				Serial.print(" enc ");
				Serial.println(inputManager->btn_encoder);
*/
			currentCamPosition=0;

		}
	}
;

	void getMotionRoutineStepCount(int selectedMotion){
		totalStep=0;
		motionIndex=-1;

		for(int i=0;i<motionDataRows;i++)
		{
			if(totalStep==0)//ottine l'indice del primo record per la routine selectedMotion in ingresso
				motionIndex++;

			if(motionData[i][0]==selectedMotion)
				totalStep++; //conta i passi per la selectedMotion in ingresso
			else
				if (totalStep > 0)break;
			/*
			 * se abbiamo già trovato il primo passo e il successivo appertiene a
			 * un altra motion, termina il ciclo di ricerca
			 */
		}

	};
	void parseCurrentCommand(){
		//parsifica il comando
		if (commandToParseAvailable)
		{
			getMotionRoutineStepCount(atoi(command.c_str()));
			//resetta le variabili per il parser
			commandToParseAvailable=false;
			command="";
		}


	};

};

#endif /* LIBRARIES_FURBY_ENGINE_H_ */
