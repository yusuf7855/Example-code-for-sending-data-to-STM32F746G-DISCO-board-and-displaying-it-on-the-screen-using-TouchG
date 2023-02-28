/*
 * PollingRoutines.c
 *
 *  Created on: Feb 15, 2023
 *      Author: 90531
 */
#include "main.h"
#include "string.h"
#include "PollingRoutines.h"
#include "cmsis_os.h"

extern UART_HandleTypeDef huart6;
extern osSemaphoreId binarySemUartMsgHandle;
extern DMA_HandleTypeDef hdma_usart6_rx;
uint8_t uartMsgBuf[UART_BUF_SIZE];
uint8_t uartMsgData[uartMsgData_SIZE];
uint8_t msgIndex = 0;
uint8_t msgRdyFlag = 0;


void PollingInit(){

	//HAL_UART_Receive_IT(&huart6, uartMsgData, 1);
	__HAL_DMA_DISABLE_IT(&hdma_usart6_rx, DMA_IT_HT);
	HAL_UARTEx_ReceiveToIdle_DMA(&huart6, uartMsgData, sizeof(uartMsgData));


}

void PollingRoutine(){

	if(msgRdyFlag)
	{
		xSemaphoreGive(binarySemUartMsgHandle);
       msgRdyFlag = 0;
	}
}


/*void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){

  if (msgIndex == 0) {
    memset(uartMsgBuf, 0, sizeof(uartMsgBuf));
  }

  if (uartMsgData[0] != 13) {
    uartMsgBuf[msgIndex++] = uartMsgData[0];
  } else {
    uartMsgBuf[msgIndex] = uartMsgData[0];
    msgIndex = 0;
    msgRdyFlag = 1;
  }

  HAL_UART_Receive_IT(huart, uartMsgData, 1);


}*/
uint16_t oldPos = 0;
uint16_t newPos = 0;
int isOK = 0;

//
//void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
//{
//	if (huart->Instance == USART6)
//	{
//		oldPos = newPos;
//		if (oldPos+Size > uartMsgData_SIZE)
//		{
//
////			for(int i =0;i<32;i++){
////			uartMsgBuf[i]=0;
////		}
//			uint16_t datatocopy = uartMsgData_SIZE-oldPos;
//			memcpy ((uint8_t *)uartMsgBuf+oldPos, uartMsgData, datatocopy);
//			oldPos = 0;
//			memcpy ((uint8_t *)uartMsgBuf,uartMsgData+datatocopy, (Size-datatocopy));
//			newPos = (Size-datatocopy);
//		}
//		else
//		{
//
//
//			for(int i =0;i<32;i++){
//						uartMsgBuf[i]=0;
//					}
//			//newPos = 0;
//			newPos = Size+oldPos;
//			memcpy ((uint8_t *)uartMsgBuf+oldPos, uartMsgData, Size);
//			newPos = 0;
//
//
//		}
//		HAL_UARTEx_ReceiveToIdle_DMA(&huart6,  uartMsgData, sizeof(uartMsgData));
//		__HAL_DMA_DISABLE_IT(&hdma_usart6_rx, DMA_IT_HT);
//
//
//	}
//
//}
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
    if (huart->Instance == USART6)
    {
        if (Size >= uartMsgData_SIZE) {
            uint16_t oldPos = newPos;
            memcpy((uint8_t *)uartMsgBuf + oldPos, uartMsgData, uartMsgData_SIZE - oldPos);
            newPos = uartMsgData_SIZE;
            oldPos = 0;
            memcpy((uint8_t *)uartMsgBuf + newPos, uartMsgData + (uartMsgData_SIZE - oldPos), Size - (uartMsgData_SIZE - oldPos));
            newPos += Size - (uartMsgData_SIZE - oldPos);
        } else {
            newPos = 0;
            memset(uartMsgBuf, 0, uartMsgData_SIZE);
            memcpy((uint8_t *)uartMsgBuf, uartMsgData, Size);
            newPos = Size;
        }
        HAL_UARTEx_ReceiveToIdle_DMA(&huart6, uartMsgData, sizeof(uartMsgData));
        __HAL_DMA_DISABLE_IT(&hdma_usart6_rx, DMA_IT_HT);
    }
}

