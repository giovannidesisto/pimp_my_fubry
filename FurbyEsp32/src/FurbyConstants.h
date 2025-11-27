/////costanti switch popolazione aree di memoria
#define MOTION_DATA 0
#define GAME_DATA 1
#define FUNCTION_DATA 2
#define SETUP_DATA 3


////costanti modalità operative
#define OP_MODE_PLAY 0
#define OP_MODE_TALES 1
#define OP_MODE_MP3 2


////costanti gestione tempistiche pressione pulsanti
#define NUMPIXELS 1
#define NOT_CLICKING 0
#define CLICKING 1
#define CLICK_1_SEC 2
#define CLICK_2_SEC 3
#define CLICK_3_SEC 4
#define NUM_CYCLE_1_SEC_CLICK 1000
#define NUM_CYCLE_2_SEC_CLICK 2000
#define NUM_CYCLE_3_SEC_CLICK 3000
#define NUM_CYCLE_4_SEC_CLICK 4000
//#define NUM_CYCLE_BLINK 1250


/////////////////////////////PIN VARS////////////////////////////////
//1-2 rx tx


//comunicazione spi SD CARD
#define SD_CS 5
#define SPI_MOSI 23
#define SPI_MISO 19
#define SPI_SCK 18

//comunicazione audio i2s
#define I2S_DOUT 25
#define I2S_BCLK 27
#define I2S_LRC 26


/*
#define NEOPIXEL_COMMAND_PIN 12
#define DC_ENGINE_FWD_COMMAND_PIN 2
#define DC_ENGINE_BKW_COMMAND_PIN 15

#define BTN_FRONT_PIN 14
#define BTN_REAR_PIN 35
#define BTN_ENCODER_PIN 32 //analogico
#define BTN_CAM_PIN 39
#define BTN_IR_PIN 36

#define BTN_TONGUE_PIN 4
#define CMD_ENCODER_LED_PIN 16
*/


#define DC_ENGINE_FWD_COMMAND_PIN 4
#define DC_ENGINE_BKW_COMMAND_PIN 16

#define BTN_ENCODER_PIN 36 //analogico
#define BTN_TONGUE_PIN 39
#define BTN_FRONT_PIN 32
#define NEOPIXEL_COMMAND_PIN 33
#define WIFI_EN_PIN 21
#define BTN_IR_PIN 35

#define BTN_CAM_PIN 2
#define BTN_REAR_PIN 15



#define CMD_ENCODER_LED_PIN 17


#define FRONT_PHOTORESISTOR_SIGNAL_PIN 34//analogico

/////////////////////////////GAME VARS////////////////////////////////
#define functionRows 10
#define gameRows 10

////// valori apppicabili per la colonna GAME_CURRENT_STEP_COMMAND
///// ovvero passi eseguibili in una routine di tipo gioco
#define GAME_TYPE_AUDIO 0
#define GAME_TYPE_AUDIO_ENGINE 1
#define GAME_TYPE_ENGINE 2
#define GAME_TYPE_EVAL_INPUT 3
#define GAME_TYPE_EVAL_INPUT_AUDIO 4


/**
 * FORMATO FILE
 *
 * ID_PASSO
 * COMANDO
 * PARAM 1 / AUDIO_FILE
 * PARAM 2 / ENGINE_ROUTINE
 * NEXT_STEP
 * ID_BLOCCO_FUNZIONE
 *
 */
#define GAME_ID_COLUMN 0
#define GAME_CURRENT_STEP_COMMAND 1
#define GAME_AUDIO_FILE_COLUMN 2
#define GAME_ENGINE_COLUMN 3
#define GAME_NEXT_STEP_COLUMN 4
#define GAME_FUNCTION_COLUMN 5


#define FUNCTION_ID_COLUMN 0
#define FUNCTION_STRING 1
#define FUNCIOTN_NEXT 2




