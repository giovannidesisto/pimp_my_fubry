/*
 * FunctionEvaluator.h
 *
 *  Created on: 17 gen 2022
 *      Author: giovanni
 */
#include <StringUtils.h>
#include "../FurbyConstants.h"
#include "Services/InputManager.h"
#include "Services/AudioManager.h"
#ifndef LIBRARIES_FUNCTIONEVALUATOR_FUNCTIONEVALUATOR_H_
#define LIBRARIES_FUNCTIONEVALUATOR_FUNCTIONEVALUATOR_H_

class FunctionEvaluator {
public:
	FunctionEvaluator(){};
	InputManager *inputManager;
	void setInputManager(InputManager &inputManager_){inputManager=&inputManager_;};

	AudioManager *audioManager;
	void setAudioManager(AudioManager &audioManager_){audioManager=&audioManager_;};


	int evaluateBoolExpr(String &s){
		int n = s.length();
		// Traverse all operands by jumping
		// a character after every iteration.
		for (int i = 0; i < n; i += 2) {
			// If operator next to current operand
			// is AND.
			if (s[i + 1] == '&') {
				if (s[i + 2] == '0'|| s[i] == '0')
					s[i + 2] = '0';
				else
					s[i + 2] = '1';
			}
			// If operator next to current operand
			// is OR.
			else if (s[i + 1] == '|') {
				if (s[i + 2] == '1'|| s[i] == '1')
					s[i + 2] = '1';
				else
					s[i + 2] = '0';
			}
		}
		return s[n - 1] -'0';
	};

	int getInputValue(int inputCode){
		switch(inputCode){
		case InputManager::inputs::BTN_FRONT:
			return inputManager->ext_btn_front_click;
		case InputManager::inputs::BTN_REAR:
			return inputManager->ext_btn_rear_click;

		case InputManager::inputs::BTN_TONGUE:
			return int(inputManager->btn_tongue);

		case InputManager::inputs::PHOTO_FRONT:
			return inputManager->photoFront;

		case InputManager::inputs::AUDIO_PLAYNG_STATE:
			return int(audioManager->wavPlayingState);
		default:
			return 0;
		}

	}


	int evalFunction(String &f){
		/**
		 * FUNZIONE DI ESEMPIO
		 * 1_=_1_-1_&;2_=_-1_3_|;1_=_100_-1
		 *
		 * return var1==1 && var2==var3 || var1==100;
		 */

		//ottengo i blocchi di valutazione
		char blockSeparator=';';
		char functionSeparator='_';
		int nrBlock = StringUtils::countOccurrences(blockSeparator,f)+1;
		String *blocks = new String[nrBlock];
		bool *partialResults = new bool[nrBlock];

		for(int i=0;i<nrBlock;i++)
			blocks[i]=StringUtils::getElementAtPos(f,blockSeparator,i);//f.substr(start, end - start);

		for(int i=0;i<nrBlock;i++)
		{
			int var=getInputValue(atoi(StringUtils::getElementAtPos(blocks[i],functionSeparator,0).c_str()));
			char op=StringUtils::getElementAtPos(blocks[i],functionSeparator,1).charAt(0);
			int val=atoi(StringUtils::getElementAtPos(blocks[i],functionSeparator,2).c_str());
			int otherVar=atoi(StringUtils::getElementAtPos(blocks[i],functionSeparator,3).c_str());
			if(otherVar!=-1)otherVar=getInputValue(otherVar);

			switch(op){
			case '=':
				partialResults[i]=var==val;
				break;
			case '>':
				partialResults[i]=var>val;
				break;
			case '<':
				partialResults[i]=var<val;
				break;
			case 'M':
				partialResults[i]=var>=val;
				break;
			case 'm':
				partialResults[i]=var<=val;
				break;
			}
		}

		String toEval="";

		for(int i=0;i<nrBlock;i++){

			//printf("block [%d] %s result[%d]\n",i,blocks[i].c_str(),partialResults[i]);
			toEval=toEval+ (partialResults[i]?'1':'0');
			if(i<nrBlock-1)//l'ultimo elem
				toEval=toEval+StringUtils::getElementAtPos(blocks[i],functionSeparator,4).charAt(0);
		}
		//printf("toEval [%s]\n",toEval.c_str());

		return evaluateBoolExpr(toEval);
	};

};

#endif /* LIBRARIES_FUNCTIONEVALUATOR_FUNCTIONEVALUATOR_H_ */
