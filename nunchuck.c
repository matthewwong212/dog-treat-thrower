#include "nunchuck.h"

uint8_t SlaveAddress = 0xA4;
uint8_t Data_Receive[6];
uint8_t Data_Send;
uint8_t Data_Send_Array[5];

void Nunchuck_Init(void){
		// Initialize Nunchuck
	Data_Send_Array[0] = 0xF0;
	Data_Send_Array[1] = 0x55;
	I2C_SendData(I2C1, SlaveAddress, Data_Send_Array, 2);
	Data_Send_Array[0] = 0xFB;
	Data_Send_Array[1] = 0x00;
	I2C_SendData(I2C1, SlaveAddress, Data_Send_Array, 2);
}

void Nunchuck_Get_Data(uint8_t *dataAdd){
	Data_Send = 0x00;
	I2C_SendData(I2C1, SlaveAddress, &Data_Send, 1);
	I2C_ReceiveData(I2C1, SlaveAddress, dataAdd, 6);
}

// returns 1 if button is pressed
int Nunchuck_Z_Button(void) {
	Nunchuck_Get_Data(Data_Receive);
	if ( (Data_Receive[5] & 0x1) == 1) {
		return 0;
	} else {
		return 1;
	}
}

// returns 1 if button is pressed
int Nunchuck_C_Button(void) {
	Nunchuck_Get_Data(Data_Receive);
	if ( (Data_Receive[5] & 0x2) == 2) {
		return 0;
	} else {
		return 1;
	}
}
