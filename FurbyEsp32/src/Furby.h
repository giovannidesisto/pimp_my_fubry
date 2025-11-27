/*
 * Furby.h
 *
 *  Created on: 17 gen 2022
 *      Author: giovanni
 */
#include "FurbyConstants.h"
#include <Arduino.h>
#include <SD.h>
#include <SPI.h>
#include <StringUtils.h>




#include "Objects/Function.h"
#include "Objects/Game.h"

#include "Services/FurbyFtpServer.h"
#include "Services/FurbyWifi.h"

#include "Services/Ota.h"

#include "Services/AudioManager.h"
//#include "Services/EngineManager.h"
//#include "Services/FunctionEvaluator.h"
#include "Services/GameManager.h"
#include "Services/InputManager.h"
#include "Services/LedManager.h"
#include "Services/TalesTeller.h"




#ifndef LIBRARIES_FURBY_SRC_FURBY_H_
#define LIBRARIES_FURBY_SRC_FURBY_H_



boolean enWifi=false;

class Furby {
public:

	Furby(){};
	void setup() {
		Serial.begin(115200);
		SPI.begin(SPI_SCK, SPI_MISO, SPI_MOSI,SD_CS);

		initIO();
		enWifi=digitalRead(WIFI_EN_PIN);
		//delay(500);
		initSD();
		if(enWifi){
			//init wifi
			FurbyWifi::init();

			//ini servizio update sketch via wifi
			Ota::init();
			//init server ftp

			FurbyFtpServer::init();
		}
		//else
		{





			//inizializza i servizi
			begin();
			ledManager.init();

			/*
			populateData("/setup/setup.csv",SETUP_DATA);
			populateData("/setup/engine.csv",MOTION_DATA);
			Serial.println("engine data");
			engineManager.print();
			String first_game=s_games;
			first_game.concat("/0/");
			first_game.concat(s_game_csv);
			populateData(first_game,GAME_DATA);
			String first_game_functions=s_games;
			first_game_functions.concat("/0/");
			first_game_functions.concat(s_functions_csv);
			populateData(first_game_functions,FUNCTION_DATA);
			 */

			//	gameManager.print();



			//	Serial.print("enginePhotoresistorBias ");
			//	Serial.print(enginePhotoresistorBias);
			//	Serial.print(" - volume ");
			//	Serial.println(volume);


			audioManager.audioFile="/audio/wakeup/";
			audioManager.audioFile.concat(random(1, 10));
			audioManager.audioFile.concat(".mp3");
			//engineManager.getMotionRoutineStepCount(0);
		}
	};

	int mp3Index=0;
	bool toggleMp3F=false;
	bool toggleMp3R=false;
	bool cucu=false;
	bool wifiLed=false;
	int rainbowCounter=0;
	unsigned long rainbowMillis=0;
	unsigned long currentMillis=0;
	void loop() {
		//sempre e comunque


		/**
		 * TODO abiliatare i comporamenti sulla base di digita io cfg
		 */

		if(enWifi)
		{
			ArduinoOTA.handle();
			ftpSrv.handleFTP();

			if(!wifiLed){
				ledManager.pixels.setPixelColor(0, 0,0,ledManager.ledB);
				ledManager.pixels.show();
				wifiLed=true;
			}
		}
		else
		{


			if(
					(
							op_mode==-1 ||
							talesTeller.talesMachineState==TALSES_ENDED ||
							(op_mode==OP_MODE_MP3 && audioManager.wavPlayingState)
					)
					&& !inputManager.btn_front && !inputManager.btn_rear
			)
			{
				currentMillis=millis();
				if(currentMillis-rainbowMillis>20)
				{
					rainbowMillis=currentMillis;
					if(rainbowCounter<256)
						rainbowCounter++;
					else
						rainbowCounter=0;

					ledManager.pixels.setPixelColor(0, ledManager.Wheel((rainbowCounter) & 255));
					//ledManager.pixels.setBrightness(ledManager.ledB);
					ledManager.pixels.show();


				}

			}

			//lettura ingressi
			readInputs();

			//valutazione ingressi
			inputManager.checkButtonClikingState();

			//calcolo CAM
			//engineManager.cam();

			//forza manualmente eventuali stati --> imposto qua per bypassare la lettura effettiva (che resetterebbe le variabili
			//riportandole al valore dello stato fisico

			readSerialData();//

			//valutazione logica
			if (op_mode==OP_MODE_TALES)
			{
				talesTeller.tellTale();
			}
			else if (op_mode==OP_MODE_PLAY)
			{
				//gameManager.execGameRoutine();
				if(inputManager.btn_tongue && !audioManager.wavPlayingState)
				{


					audioManager.audioFolder="";
					audioManager.audioFile="/audio/dontTouchMyTongue/";
					audioManager.audioFile.concat(String(random(1,11)));
					audioManager.audioFile.concat(".mp3");

				}

				if(inputManager.photoFront<100 && !cucu && !audioManager.wavPlayingState){

					audioManager.audioFolder="";
					audioManager.audioFile="/audio/cucu/cucu.mp3";

					//engineManager.getMotionRoutineStepCount(100);
					cucu=true;
				}
				else if(inputManager.photoFront>100 && cucu  && !audioManager.wavPlayingState){
					audioManager.audioFolder="";
					audioManager.audioFile="/audio/cucu/cucu_end.mp3";
					//engineManager.getMotionRoutineStepCount(0);
					cucu=false;
				}

			}
			else if (op_mode==OP_MODE_MP3)
			{


				if(!inputManager.btn_front)toggleMp3F=false;
				if(!inputManager.btn_rear)toggleMp3R=false;

				if(inputManager.btn_front_click==CLICK_1_SEC){
					if(!audioManager.wavPlayingState &&  !toggleMp3F)
					{
						if(mp3Index<nr_mp3_sd)
							mp3Index++;
						else mp3Index=1;


						delay(250);


						audioManager.audioFolder="";
						audioManager.audioFile="/mp3/";
						audioManager.audioFile.concat(String(mp3Index));
						audioManager.audioFile.concat(".MP3");


						toggleMp3F=true;
					}
					else audioManager.stop();
				}
				else  if(inputManager.btn_rear_click==CLICK_1_SEC){

					if(!audioManager.wavPlayingState && !toggleMp3R)
					{
						inputManager.btn_rear_cycle_counter=0;
						if(mp3Index>1)
							mp3Index--;
						else mp3Index=nr_mp3_sd;

						delay(250);

						audioManager.audioFolder="";
						audioManager.audioFile="/mp3/";
						audioManager.audioFile.concat(String(mp3Index));
						audioManager.audioFile.concat(".MP3");

						toggleMp3R=true;
					}
					else audioManager.stop();


				}

			}




			//valuta lo stato dei pulsanti per cambiare modalità operativa
			setOpMode();

			//	engineManager.parseCurrentCommand();

			//scrittura output
			//	engineManager.driveEngine();

			audioManager.playAudio();

			ledManager.setNexopielState();

			handleVolume();
		}


		if(engineTest){
			Serial.print("encoderAnalogValue:");
			Serial.print(inputManager.encoderAnalogValue);
			Serial.print("\tbtn_cam:");
			Serial.println(inputManager.btn_cam * 10);
		}

	};

	void handleVolume(){
		if(inputManager.front_double_click)
		{
			if(audioManager.volume>0.0)
			{
				audioManager.volume=audioManager.volume-0.1;
				audioManager.setVolume();
				Serial.print("audio - ");
				Serial.println(audioManager.volume);
			}
		}else if(inputManager.rear_double_click)
		{
			if(audioManager.volume<audioManager.maxVolume)
			{
				audioManager.volume=audioManager.volume+0.1;
				audioManager.setVolume();
				Serial.print("audio + ");
				Serial.println(audioManager.volume);
			}
		}
	}

	void setOpModeLed(){
		if(!inputManager.btn_front && !inputManager.btn_rear){

			switch(op_mode)
			{
			case OP_MODE_TALES:
				if(talesTeller.talesMachineState!=TALSES_ENDED)//in fase di esecuzione, il led è rainbow
					ledManager.pixels.setPixelColor(0, ledManager.ledB,ledManager.ledB,0);

				break;
			case OP_MODE_PLAY:
				ledManager.pixels.setPixelColor(0, ledManager.ledB,0,ledManager.ledB);

				break;
			case OP_MODE_MP3:
				if(!audioManager.wavPlayingState)//in fase di esecuzione il led è rainbow
					ledManager.pixels.setPixelColor(0, 10,ledManager.ledB,30);
				break;


			}


			ledManager.pixels.show();
		}
	}
	void stopEngine(){
		analogWrite(DC_ENGINE_FWD_COMMAND_PIN,0);
		analogWrite(DC_ENGINE_BKW_COMMAND_PIN,0);
		digitalWrite(CMD_ENCODER_LED_PIN, LOW);
	};
private:
	/*
	 * FORMATO FILE
	 *
	 * VOLUME AUDIO
	 * SOGLIA FOTORESISTENZA
	 */
	char setupData[1][2];



	bool routineDone=false;
	const char* s_games="/games";
	const char* s_game_csv="game.csv";
	const char* s_functions_csv="functions.csv";
	const char* s_tales="/tales";
	const char* s_audio="/audio";



	//EngineManager engineManager;
	InputManager inputManager;
	AudioManager audioManager;
	LedManager ledManager;
	TalesTeller talesTeller;
	//GameManager gameManager;
	//FunctionEvaluator functionEvaluator;
	int op_mode=-1;
	int current_op_mode=OP_MODE_TALES;

	void begin(){

		//engineManager.setInputManager(inputManager);
		ledManager.setInputManager(inputManager);
		talesTeller.setInputManager(inputManager);
		talesTeller.setAudioManager(audioManager);
		//functionEvaluator.setInputManager(inputManager);
		//functionEvaluator.setAudioManager(audioManager);
		//gameManager.setAudioManager(audioManager);
		//gameManager.setTalesTeller(talesTeller );
		//gameManager.setEngineManager(engineManager);
		//gameManager.setFunctionEvaluator(functionEvaluator);

		audioManager.init();


	};

	void setOpMode(){


		if (inputManager.ext_btn_front_click==CLICK_2_SEC) {

			if(op_mode<2)
				op_mode++;
			else
				op_mode=0;

			switch(op_mode)
			{
			case OP_MODE_TALES:
				Serial.println("OP_MODE_TALES");

				break;
			case OP_MODE_PLAY:
				Serial.println("OP_MODE_PLAY");

				break;
			case OP_MODE_MP3:

				Serial.println("OP_MODE_MP3");
				break;


			}

		}

		announceOpMode();
		setOpModeLed();
	};
	void announceOpMode(){
		//la modalità viene annunciata solo al cambio
		if (current_op_mode!=op_mode) {


			//stoppo eventuali motion in corso
			//engineManager.gotoSpecifiedPositionFlag=false;
			/**
			 * REMOVE CON DFP
			 *
			 */
			if (audioManager.wavPlayingState)
				audioManager.stop();//interrompo eventuali frasi

			if (op_mode==OP_MODE_TALES)
			{

				audioManager.audioFile="mod_storia.mp3";
				audioManager.audioFolder="/audio/";
				talesTeller.talesMachineState=TALSES_NOT_ANNOUNCED;
				talesTeller.current_tale_index=random(0,talesTeller.nr_tales_in_sd);


			}
			else if (op_mode==OP_MODE_PLAY)
			{
				audioManager.audioFile="mod_gioco.mp3";
				audioManager.audioFolder="/audio/";
				//gameManager.game_nextStep=0;

			}
			else if (op_mode==OP_MODE_MP3)
			{

			}



			current_op_mode=op_mode;
			stopEngine();

		}

	};

	/*
	void populateData(String fileName, int dataArray){
		int counter=0;
		File file = SD.open(fileName); // @suppress("Abstract class cannot be instantiated")
		char byte=0;
		int columnCounter=0;
		String partial="";


		switch(dataArray){
		case MOTION_DATA:
			engineManager.initEngineData();
			break;
		case GAME_DATA:
			gameManager.initGameData();
			break;
		case FUNCTION_DATA:
			gameManager.initFunctionData();
			break;
		}


		if (file)
		{

			while ( file.available() )
			{
				byte=file.read();
				if(byte=='\n'){
					counter++;
					columnCounter=0;
				}
				else if(byte!=','){
					partial+=byte;
				}
				else if(byte==',')
				{

					switch(dataArray)
					{
					case MOTION_DATA:
						engineManager.motionData[counter][columnCounter]=atoi(partial.c_str());
						break;
					case GAME_DATA:
					{
						switch(columnCounter)
						{
						case GAME_ID_COLUMN:
							gameManager.gameData[counter].id=atoi(partial.c_str());
							break;
						case GAME_CURRENT_STEP_COMMAND:
							gameManager.gameData[counter].command=atoi(partial.c_str());
							break;
						case GAME_AUDIO_FILE_COLUMN:
							gameManager.gameData[counter].audioFile=atoi(partial.c_str());
							break;
						case GAME_ENGINE_COLUMN:
							gameManager.gameData[counter].engineRoutine=atoi(partial.c_str());
							break;
						case GAME_NEXT_STEP_COLUMN:
							gameManager.gameData[counter].nextStep=atoi(partial.c_str());
							break;
						case GAME_FUNCTION_COLUMN:
							gameManager.gameData[counter].function=atoi(partial.c_str());
							break;
						}
					}
					break;

					case FUNCTION_DATA:
					{
						switch(columnCounter){
						case FUNCTION_ID_COLUMN:
							gameManager.gameFunctions[counter].functionId=atoi(partial.c_str());
							break;
						case FUNCTION_STRING:
							gameManager.gameFunctions[counter].functionToEvaluate=partial.c_str();
							break;
						case FUNCIOTN_NEXT:
							gameManager.gameFunctions[counter].nextStep=atoi(partial.c_str());
							break;

						}
					}
					break;


					case SETUP_DATA:
						setupData[counter][columnCounter]=atoi(partial.c_str());
						break;
					}
					columnCounter++;
					partial="";
				}
			}
			file.close();
		}


	};

	 */
	/**
	 * Init modules for normal play mode
	 */
	void initIO() {

		pinMode(BTN_FRONT_PIN, INPUT);
		pinMode(BTN_TONGUE_PIN, INPUT);
		pinMode(BTN_CAM_PIN, INPUT);


		pinMode(BTN_ENCODER_PIN, INPUT);
		pinMode(BTN_REAR_PIN, INPUT);

		pinMode(WIFI_EN_PIN, INPUT);
		//init engine control pin
		pinMode(DC_ENGINE_FWD_COMMAND_PIN, OUTPUT);
		pinMode(DC_ENGINE_BKW_COMMAND_PIN, OUTPUT);
		pinMode(CMD_ENCODER_LED_PIN, OUTPUT);

		//set engine state to off {even if it will be off..}
		digitalWrite(DC_ENGINE_BKW_COMMAND_PIN, LOW);
		digitalWrite(DC_ENGINE_FWD_COMMAND_PIN, LOW);


	};


	void printDirectory(File &dir, int numTabs) {
		while (true) {

			File entry =  dir.openNextFile();
			if (! entry) {
				// no more files
				break;
			}
			for (uint8_t i = 0; i < numTabs; i++) {
				Serial.print('\t');
			}
			Serial.print(entry.name());
			if (entry.isDirectory()) {
				Serial.println("/");
				printDirectory(entry, numTabs + 1);
			} else {
				// files have sizes, directories do not
				Serial.print("\t\t");
				Serial.println(entry.size(), DEC);
			}
			entry.close();
		}
	};

	int nr_mp3_sd=0;
	void initSD() {
		Serial.println("[initSD] START");
		if (!SD.begin(SD_CS,SPI)) {
			Serial.println("Unable to start SD");
			return;
		}
		else{

			Serial.println("[initSD] SD initialization done.");
			/*
			File root =SD.open("/");
			printDirectory(root,0);
			root.close();
			 */
			File talesFolder= SD.open(s_tales);
			talesTeller.nr_tales_in_sd = countFile(talesFolder, false,false);
			talesFolder.close();

			//			File gamesFolder= SD.open(s_games);
			//			gameManager.nr_games_in_sd = countFile(gamesFolder,false,false);
			//			gamesFolder.close();

			File mp3Folder= SD.open("/mp3");
			nr_mp3_sd = countFile(mp3Folder,false,true);
			mp3Folder.close();



			Serial.print("[initSD] nr_tales_in_sd ");
			Serial.println(talesTeller.nr_tales_in_sd);

			talesTeller.current_tale_index = random(0, talesTeller.nr_tales_in_sd);
			//			Serial.print("[initSD] nr_games_in_sd ");
			//			Serial.println(gameManager.nr_games_in_sd );
			Serial.print("[initSD] nr_mp3_sd ");
			Serial.println(nr_mp3_sd);



			Serial.println("[initSD] END");
		}

	};




	//char readedCharFromStream=0;
	String currentCommand="";


	/**
	 * solo per finalità di test
	 */
	boolean engineTest=false;
	boolean motionTest=false;
	void readSerialData() {
		//compone il comando leggendo dallo stin, fintanto che non viene intercettato il char new line
		//non prende in considerazione ulteriori comandi fintanto che non è stato analizzato il comando corrente

		//!engineManager.gotoSpecifiedPositionFlag && ???
		if (Serial.available() > 0)
		{
			String currentCommand = Serial.readString();


			if(currentCommand.equals("mode")){
				if(op_mode<2)
					op_mode++;
				else
					op_mode=0;

				switch(op_mode)
				{
				case OP_MODE_TALES:
					Serial.println("OP_MODE_TALES");

					break;
				case OP_MODE_PLAY:
					Serial.println("OP_MODE_PLAY");

					break;
				case OP_MODE_MP3:

					Serial.println("OP_MODE_MP3");
					break;


				}
			}
			//test engine motion (type engine@0)
			/*
			else if(currentCommand.startsWith("engine")){
				engineManager.commandToParseAvailable=true;
				engineManager.command=(StringUtils::getElementAtPos(currentCommand, '@', 1) );

				Serial.print("test engine motion");
				Serial.println(engineManager.command);
				currentCommand="";


			}
			 */
			else if(currentCommand.startsWith("x")){

				inputManager.encoderLimit= atoi(StringUtils::getElementAtPos(currentCommand, '@', 1).c_str());
				currentCommand="";

			}
			/*
			else if(currentCommand.startsWith("y")){

				engineManager.gotoSpecifiedPositionFlag=false;

				if(!engineTest)

					digitalWrite(CMD_ENCODER_LED_PIN,HIGH);

				else
					digitalWrite(CMD_ENCODER_LED_PIN,LOW);
				engineTest=!engineTest;

			}

			else if(currentCommand.startsWith("z")){



				if(!motionTest)
					analogWrite(DC_ENGINE_FWD_COMMAND_PIN,255);
				else
					analogWrite(DC_ENGINE_FWD_COMMAND_PIN,0);
				motionTest=!motionTest;
			} */
			else if(currentCommand.startsWith("l")){

				if(StringUtils::getElementAtPos(currentCommand, '@', 1).equals("ON"))
					ledManager.pixels.setPixelColor(0, 0, 50, 0);//off
				else
					ledManager.pixels.setPixelColor(0, 0, 0, 0);//off

				ledManager.pixels.show();

			}

			//test audio file(type  audio@/path/to/audio.mp3)
			else if(currentCommand.startsWith( "audio")){


				audioManager.audioFolder="";
				audioManager.audioFile=(StringUtils::getElementAtPos(currentCommand, '@', 1) );


				Serial.print("test audio file ");
				Serial.println(	audioManager.audioFile);
				currentCommand="";
			}
			// increase volume
			else if(currentCommand.equals("+")){
				if(audioManager.volume<1.0)
				{
					audioManager.volume=audioManager.volume+0.05;
					audioManager.setVolume();
					Serial.printf("+ audio volume %f \r\n",audioManager.volume);
				}
				currentCommand="";
			}
			//decrease volume
			else if(currentCommand.equals("-")){
				if(audioManager.volume>0.0)
				{
					audioManager.volume=audioManager.volume-0.05;
					audioManager.setVolume();
					Serial.printf("- audio volume %f \r\n",audioManager.volume);
				}
				currentCommand="";
			}
			else if(currentCommand.equals("ls")){
				File root=SD.open("/");
				//printDirectory(root, 0);
				root.close();
				currentCommand="";
			}
			else if(currentCommand.equals("fn")){
				inputManager.ext_btn_front_click=NOT_CLICKING;
			}
			else if(currentCommand.equals("fc")){
				inputManager.ext_btn_front_click=CLICKING;
			}
			else if(currentCommand.equals("f1")){
				inputManager.ext_btn_front_click=CLICK_1_SEC;
			}
			else if(currentCommand.equals("f2")){
				inputManager.ext_btn_front_click=CLICK_2_SEC;
			}
			else if(currentCommand.equals("f3")){
				inputManager.ext_btn_front_click=CLICK_3_SEC;
			}
			else if(currentCommand.equals("rn")){
				inputManager.ext_btn_rear_click=NOT_CLICKING;
			}
			else if(currentCommand.equals("rc")){
				inputManager.ext_btn_rear_click=CLICKING;
			}
			else if(currentCommand.equals("r1")){
				inputManager.ext_btn_rear_click=CLICK_1_SEC;
			}
			else if(currentCommand.equals("r2")){
				inputManager.ext_btn_rear_click=CLICK_2_SEC;
			}
			else if(currentCommand.equals("r3")){
				inputManager.ext_btn_rear_click=CLICK_3_SEC;
			}
			else if(currentCommand.equals("i")){
				Serial.print(" photoFront ");Serial.print(inputManager.photoFront);
				Serial.print("\tbtn_front ");Serial.print(inputManager.btn_front);
				Serial.print("\tbtn_rear ");Serial.print(inputManager.btn_rear);
				Serial.print("\tbtn_tongue ");Serial.print(inputManager.btn_tongue);
				Serial.print("\tbtn_encoder ");Serial.print(inputManager.btn_encoder);
				Serial.print("\tbtn_cam ");Serial.print(inputManager.btn_cam);
				Serial.print("\tencoderAnalogValue ");Serial.println(inputManager.encoderAnalogValue);

			}
			else //if(currentCommand.length()>0)
			{
				Serial.print(F("unknown command "));
				Serial.println(currentCommand);
				currentCommand="";

			}


		}




	};






	void readInputs() {

		inputManager.readInputValues();
		audioManager.setPlaiyngState();

	};







	//////////////////////////////////////////////////////
	/*
	 * UTILITY
	 */
	//////////////////////////////////////////////////////

	int countFile(File &dir, boolean recursive,boolean returnFileCount) { //otherwise returns dir count // @suppress("Abstract class cannot be instantiated")
		int dirCounter=0;
		int fileCounter=0;
		while (true) {

			File entry =  dir.openNextFile(); // @suppress("Abstract class cannot be instantiated")
			if (! entry) {

				break;
			}

			if (entry.isDirectory()) {

				dirCounter++;
				if (recursive) {
					if (returnFileCount)
						fileCounter+=countFile(entry,recursive,returnFileCount);
					else
						dirCounter+=countFile(entry,recursive,returnFileCount);
				}
			} else {
				fileCounter++;
			}
			entry.close();
		}
		dir.close();
		return (returnFileCount?fileCounter:dirCounter);



	};



};



#endif /* LIBRARIES_FURBY_SRC_FURBY_H_ */
