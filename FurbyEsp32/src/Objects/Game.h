/*
 * Game.h
 *
 *  Created on: 24 gen 2022
 *      Author: giovanni
 */

#ifndef LIBRARIES_FURBY_OBJECTS_GAME_H_
#define LIBRARIES_FURBY_OBJECTS_GAME_H_

	/**
	 * FORMATO FILE
	 *
	 * ID_PASSO
	 * COMANDO
	 * PARAM 1 / AUDIO_FILE
	 * PARAM 2 / ENGINE_ROUTINE
	 * NEXT_STEP
	 *
	 */
class Game {
public:

	Game(){};
	int id=-1;
	int command=0;
	int audioFile=0;
	int engineRoutine=0;
	int nextStep=0;
	int function=0;

};



#endif /* LIBRARIES_FURBY_OBJECTS_GAME_H_ */
