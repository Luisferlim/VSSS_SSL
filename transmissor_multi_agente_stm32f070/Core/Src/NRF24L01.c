/*
 * NRF24L01.c
 *
 *  Created on: May 13, 2024
 *      Author: ruisodebian
 */

//defining the CE and CSN pins

#include "stm32f0xx_hal.h"
#include "NRF24L01.h"

extern SPI_HandleTypeDef hspi1;
#define NRF24_SPI &hspi1

#define NRF24_CE_PORT GPIOA
#define NRF24_CE_PIN GPIO_PIN_1

#define NRF24_CSN_PORT GPIOA
#define NRF24_CSN_PIN GPIO_PIN_2


//CS pin low to select the device

void CS_Select(void){
	HAL_GPIO_WritePin(NRF24_CSN_PORT, NRF24_CSN_PIN, GPIO_PIN_RESET);
}

void CS_UnSelect(void){
	HAL_GPIO_WritePin(NRF24_CSN_PORT, NRF24_CSN_PIN, GPIO_PIN_SET);
}

//RX or TX
void CE_Enable(void){
	HAL_GPIO_WritePin(NRF24_CE_PORT, NRF24_CE_PIN, GPIO_PIN_SET);
}

void CE_Disable(void){
	HAL_GPIO_WritePin(NRF24_CE_PORT, NRF24_CE_PIN, GPIO_PIN_RESET);
}

//write a single bite to a particular register
void nrf24_WriteReg(uint8_t Reg, uint8_t Data){
	uint8_t buf[2];
	buf[0] = Reg|1<<5; //bitwise
	buf[1] = Data;

	//selecting a device
	CS_Select();

	HAL_SPI_Transmit(NRF24_SPI, buf, 2, 1000);

	// pull the cs pin to high to release the device
	CS_UnSelect();
}

//write multiple bytes to a particular register
void nrf24_WriteRegMulti (uint8_t Reg, uint8_t* data, int size){
	uint8_t buf[2];
	buf[0] = Reg|1<<5; //bitwise
//	buf[1] = Data;

	//selecting a device
	CS_Select();

	HAL_SPI_Transmit(NRF24_SPI, buf, 1, 100);
	HAL_SPI_Transmit(NRF24_SPI, data, size, 1000);

	// pull the cs pin to high to release the device
	CS_UnSelect();
}

//return the data
uint8_t nrf24_ReadReg(uint8_t Reg){
	uint8_t data = 0;
	//selecting a device
	CS_Select();

	HAL_SPI_Transmit(NRF24_SPI, &Reg, 1, 100);
	HAL_SPI_Receive(NRF24_SPI, &data, 1, 100);

	// pull the cs pin to high to release the device
	CS_UnSelect();

	return data;
}

void nrf24_ReadRegMulti(uint8_t Reg, uint8_t *data, int size){

	//selecting a device
	CS_Select();

	HAL_SPI_Transmit(NRF24_SPI, &Reg, 1, 100);
	HAL_SPI_Receive(NRF24_SPI, data, size, 1000);

	// pull the cs pin to high to release the device
	CS_UnSelect();
}

//send  the command to the NRF
void nrfsendCmd (uint8_t cmd){
	//selecting a device
	CS_Select();

	HAL_SPI_Transmit(NRF24_SPI, &cmd, 1, 100);

	// pull the cs pin to high to release the device
	CS_UnSelect();
}

//reseting the device
void nrf24_reset(uint8_t REG)
{
	if (REG == STATUS)
	{
		nrf24_WriteReg(STATUS, 0x00);
	}

	else if (REG == FIFO_STATUS)
	{
		nrf24_WriteReg(FIFO_STATUS, 0x11);
	}

	else {
	nrf24_WriteReg(CONFIG, 0x08);
	nrf24_WriteReg(EN_AA, 0x3F);
	nrf24_WriteReg(EN_RXADDR, 0x03);
	nrf24_WriteReg(SETUP_AW, 0x03);
	nrf24_WriteReg(SETUP_RETR, 0x03);
	nrf24_WriteReg(RF_CH, 0x02);
	nrf24_WriteReg(RF_SETUP, 0x0E);
	nrf24_WriteReg(STATUS, 0x00);
	nrf24_WriteReg(OBSERVE_TX, 0x00);
	nrf24_WriteReg(CD, 0x00);
	uint8_t rx_addr_p0_def[5] = {0xE7, 0xE7, 0xE7, 0xE7, 0xE7};
	nrf24_WriteRegMulti(RX_ADDR_P0, rx_addr_p0_def, 5);
	uint8_t rx_addr_p1_def[5] = {0xC2, 0xC2, 0xC2, 0xC2, 0xC2};
	nrf24_WriteRegMulti(RX_ADDR_P1, rx_addr_p1_def, 5);
	nrf24_WriteReg(RX_ADDR_P2, 0xC3);
	nrf24_WriteReg(RX_ADDR_P3, 0xC4);
	nrf24_WriteReg(RX_ADDR_P4, 0xC5);
	nrf24_WriteReg(RX_ADDR_P5, 0xC6);
	uint8_t tx_addr_def[5] = {0xE7, 0xE7, 0xE7, 0xE7, 0xE7};
	nrf24_WriteRegMulti(TX_ADDR, tx_addr_def, 5);
	nrf24_WriteReg(RX_PW_P0, 0);
	nrf24_WriteReg(RX_PW_P1, 0);
	nrf24_WriteReg(RX_PW_P2, 0);
	nrf24_WriteReg(RX_PW_P3, 0);
	nrf24_WriteReg(RX_PW_P4, 0);
	nrf24_WriteReg(RX_PW_P5, 0);
	nrf24_WriteReg(FIFO_STATUS, 0x11);
	nrf24_WriteReg(DYNPD, 0);
	nrf24_WriteReg(FEATURE, 0);
	}
}

void NRF24_Init (void){
	//configuring the nrf24
	//disable the chip before configuring device
	CE_Disable();

	//reset
	nrf24_reset (0);

	nrf24_WriteReg(CONFIG, 0); //later

	//non-using shockburst auto ack
	nrf24_WriteReg(EN_AA, 0);

	nrf24_WriteReg(EN_RXADDR, 0); // non pipe receiving

	nrf24_WriteReg(SETUP_AW, 0x03); // 5 bytes for rx/tx address

	nrf24_WriteReg(SETUP_RETR, 0); // non re-transsmit with ack mode

	nrf24_WriteReg(RF_CH, 0); //setup in the rx tx modes

	nrf24_WriteReg(RF_SETUP, 0x0E); //power = 0db, data rate = 2mbps

	//enable the chip
	CE_Enable();
}

// set up the tx mode
void NRF24_TxMode(uint8_t *Address, uint8_t channel){
	//disable the chip before configuring device
	CE_Disable();

	nrf24_WriteReg(RF_CH, channel);

	nrf24_WriteRegMulti(TX_ADDR, Address, 5); //write the tx address

	//pwr up the device, dont modifying other bits
	uint8_t config = nrf24_ReadReg(CONFIG);
	config = config & (0xF2);
	config = config | (1<<1); //bitwise operation
	nrf24_WriteReg(CONFIG, config);

	//enable the chip
	CE_Enable();
}


//transmite the data
uint8_t NRF24_Transmit (uint8_t* data){
	uint8_t cmdToSend = 0;

	//select the device
	CS_Select();

	//payload command
	cmdToSend = W_TX_PAYLOAD;
	HAL_SPI_Transmit(NRF24_SPI, &cmdToSend, 1, 100);

	//send to payload
	HAL_SPI_Transmit(NRF24_SPI, data, 32, 1000); //32 BITS?

	//unselect the device
	CS_UnSelect();

	//make sure that the pipe is empty
	HAL_Delay(1);
	uint8_t fifoStatus = nrf24_ReadReg(FIFO_STATUS);

	if 	((fifoStatus&(1<<4)) && (!(fifoStatus&(1<<3)))) {
		cmdToSend = FLUSH_TX;
		nrfsendCmd(cmdToSend);

		return 1;
	}

	//flag to failed operation
	return 0;
}

