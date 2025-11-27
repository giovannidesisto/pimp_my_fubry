/*
 * GameManager.h
 *
 *  Created on: 17 gen 2022
 *      Author: giovanni
 */
#include <Arduino.h>
#include "../FurbyConstants.h"
#include "FunctionEvaluator.h"
#include "Objects/Function.h"
#include "Objects/Game.h"
#include "Services/AudioManager.h"
#include "Services/EngineManager.h"
#include "Services/TalesTeller.h"

#ifndef LIBRARIES_FURBY_GAMEMANAGER_H_
#define LIBRARIES_FURBY_GAMEMANAGER_H_

class GameManager {
public:
	GameManager(){};
	void setAudioManager(AudioManager  &audioManager_){audioManager=&audioManager_;};
	void setEngineManager(EngineManager &engineManager_){engineManager=&engineManager_;};
	void setTalesTeller(TalesTeller &talesTeller_){talesTeller=&talesTeller_;};
	void setFunctionEvaluator(FunctionEvaluator &functionEvaluator_){functionEvaluator=&functionEvaluator_;};
	void test(){

	//	Serial.print("gm->am");
		Serial.println((int) &(*audioManager), HEX);



	//	Serial.print("gm->em");
		Serial.println((int) &(*engineManager), HEX);


	//	Serial.print("gm->tt");
		Serial.println((int) &(*talesTeller), HEX);

	};


	AudioManager *audioManager=NULL;
	EngineManager *engineManager=NULL;
	TalesTeller *talesTeller=NULL;
	FunctionEvaluator *functionEvaluator=NULL;



	/**
	 * FORMATO FILE
	 *
	 * ID_PASSO
	 * COMANDO
	 * PARAM 1 / AUDIO_FILE
	 * PARAM 2 / ENGINE_ROUTINE
	 * NEXT_STEP
	 * ID_FUNZIONE
	 *
	 */
	Game gameData[gameRows];//[gameCols];
	/**
	 * un gioco è un insieme di passi, tra questi
	 * 1 esecuzione di un file audio
	 * 2 esecuzione di una routine motore
	 * 3 esecuzione contemporanea di un file audio + routine motore
	 * 4 valutazione condizionale degli ingressi
	 *
	 * i passi vengono memorizzati in un array bidimensionale come da definizione
	 * sopra riportata
	 *
	 *
	 * per quanto riguarda il punto 4, il sistema memorizza le funzioni disponibili per il gioco corrente in una array
	 * le funzioni afferenti un determinato blocco funzione, vengono valutate tutte sequenzialmente, se una di queste
	 * viene soddisfatta, viene impostato come prossimo passo il valore associato alla funzione di blocco
	 *
	 *
	 *
	 */
	Function gameFunctions[functionRows];

	boolean game_functionEvaluating=false;
	int  game_functionIndexToEvaluate=0;
	int game_nextStep=0;
	int game_currentGame=0;
	boolean game_currentStepDone=false;
	int nr_games_in_sd=0;


	void initGameData(){
		for(int i=0;i<gameRows;i++)
		{
			gameData[i].id=-1;
			gameData[i].command=-1;
			gameData[i].audioFile=-1;
			gameData[i].engineRoutine=-1;
			gameData[i].nextStep=-1;
		}
	};

	void initFunctionData(){
		for(int i=0;i<functionRows;i++)
		{

			gameFunctions[i].functionId=-1;
			gameFunctions[i].functionToEvaluate="";
			gameFunctions[i].nextStep=-1;

		}
	};



	void print(){
		Serial.println("game data");
		for(int i=0;i<gameRows;i++)
		{
			Serial.print('\t');
			Serial.print(gameData[i].id);
			Serial.print('\t');
			Serial.print(gameData[i].command);
			Serial.print('\t');
			Serial.print(gameData[i].audioFile);
			Serial.print('\t');
			Serial.print(gameData[i].engineRoutine);
			Serial.print('\t');
			Serial.print(gameData[i].nextStep);
			Serial.print('\n');
		}
		Serial.println("function data");
		for(int i=0;i<functionRows;i++)
		{
			Serial.print('\t');
			Serial.print(gameFunctions[i].functionId);
			Serial.print('\t');
			Serial.print(gameFunctions[i].functionToEvaluate);
			Serial.print('\t');
			Serial.print(gameFunctions[i].nextStep);
			Serial.print('\n');
		}

	};
	void evalGameFunction(){


		for(int i =0;i<functionRows;i++){
			//valuto il blocco funzioni indicati da gameData[game_nextStep].function
			//in maniera sequenziale
			//la prima funzione soddisfatta, imposta il passo associato interrompe il ciclo
			//di valutazione funzioni
			if(
					gameData[game_nextStep].function == gameFunctions[i].functionId
					&&
					functionEvaluator->evalFunction(gameFunctions[i].functionToEvaluate)
			)
			{
					game_nextStep=gameFunctions[i].nextStep;
					game_functionEvaluating=false;
					return;
			}


		}


	};
	void execGameRoutine () {
		/*
		Serial.print(audioManager->wavPlayingState );
		Serial.print('_');
		Serial.print(engineManager->motionIndex);
		Serial.print('_');
		Serial.println(game_functionEvaluating);
		 */
		if(!audioManager->wavPlayingState && engineManager->motionIndex==-1 && !game_functionEvaluating)
		{
			switch (game_nextStep){
			case GAME_TYPE_AUDIO:
				setGameAudio();
				game_nextStep=gameData[game_nextStep].nextStep;// [game_next_step_column];
				Serial.print("ns ");
				Serial.println(game_nextStep);
				//routineDone=true;
				break;
			case GAME_TYPE_AUDIO_ENGINE:
				setGameAudio();
				engineManager->getMotionRoutineStepCount(gameData[game_nextStep].engineRoutine);// [game_engine_column]);
				//engine->command=""+(gameData[game_nextStep][game_engine_column]);
				//engine->commandToParseAvailable=true;
				game_nextStep=gameData[game_nextStep].nextStep;//[game_next_step_column];
				//printf("GAME_AUDIO_ENGINE %s %d %d\n",audioFile,getMotionRoutineStepCount,game_nextStep);
				//			Serial.print("gea [");
				//			Serial.print(audioFile);
				//			Serial.print("] - [");
				//			Serial.print(furby.engine.motionIndex);
				//			Serial.print("] - ");
				Serial.println(game_nextStep);
				break;
			case GAME_TYPE_ENGINE:
				engineManager->getMotionRoutineStepCount(gameData[game_nextStep].engineRoutine);//[game_engine_column]);
				game_nextStep=gameData[game_nextStep].nextStep;//[GAME_NEXT_STEP_COLUMN];
				//printf("GAME_ENGINE %s %d\n",getMotionRoutineStepCount,game_nextStep);
				//			Serial.print("ge [");
				//			Serial.print(furby.engine.motionIndex);
				//			Serial.print("] - ");
				//			Serial.println(game_nextStep);
				break;
			case GAME_TYPE_EVAL_INPUT:
				//il prossimo passo lo imposta il risultato della valutazione funzioni
				game_functionEvaluating=true;
				break;

			case GAME_TYPE_EVAL_INPUT_AUDIO:
				//il prossimo passo lo imposta il risultato della valutazione funzioni
				setGameAudio();
				game_functionEvaluating=true;
				break;
			}


			if(game_nextStep==-1){

				game_nextStep=0;
				game_currentGame++;
				if(game_currentGame==nr_games_in_sd)
					game_currentGame=0;


			}
		}//diversamente, attende input utente e/o eventi
		else if(game_functionEvaluating)evalGameFunction();

	};
	void setGameAudio(){
		audioManager->audioFolder="3";//1+talesTeller->nr_tales_in_sd+game_currentGame;
		audioManager->audioFile=(gameData[game_nextStep].audioFile);// [game_audio_file_column]);
		//Serial.print("aa ");
		Serial.println(audioManager->audioFolder);
		//Serial.print("af ");
		Serial.println(audioManager->audioFile);
	};
};

#endif /* LIBRARIES_FURBY_GAMEMANAGER_H_ */
