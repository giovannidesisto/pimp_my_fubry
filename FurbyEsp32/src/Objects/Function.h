/*
 * Function.h
 *
 *  Created on: 24 gen 2022
 *      Author: giovanni
 */
//#include <Arduino.h>
#ifndef LIBRARIES_FURBY_FUNCTION_H_
#define LIBRARIES_FURBY_FUNCTION_H_


class Function {
public:

	Function(){};
	int functionId=0;
	String functionToEvaluate;
	int nextStep;
};

#endif /* LIBRARIES_FURBY_FUNCTION_H_ */
