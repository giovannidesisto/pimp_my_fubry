/*
 * StringUtils.h
 *
 *  Created on: 17 gen 2022
 *      Author: giovanni
 */

#ifndef STRINGUTILS_H_
#define STRINGUTILS_H_

class StringUtils {
public:
	StringUtils(){};
	/**
	 * conta le occorrenze del carattere c nella string str
	 */
	 static int countOccurrences(char c, String &str){
		int count = 0;

		for (char i : str)
			if (i == c)
				count++;

		return count;
	};
	/**
	 * dato un separatore c, torna l'elemento in posizione pos nella stringa f
	 */
	 static String getElementAtPos(String &f,char c,int pos){
		int  nrBlock = countOccurrences(c,f)+1;
		int  end = f.indexOf(c);
		int  start=0;
		String temp;
		for(int i=0;i<nrBlock;i++)
		{
			if(i==pos){
				//	printf("token [%s] \n",f.substr(start, end - start).c_str());
				return f.substring(start, end - start);
			}
			start = end + 1;
			end = f.indexOf(c, start);
		}
		return "";
	};

};

#endif /* STRINGUTILS_H_ */
