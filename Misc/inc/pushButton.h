/*
 * pushButton.h
 *
 *  Created on: Mar 31, 2023
 *      Author: Otavio
 */

#ifndef INC_PUSHBUTTON_H_
#define INC_PUSHBUTTON_H_

#include "main.h"

#define MIN_PRESSED_COUNTER	0
#define MAX_PRESSED_COUNTER	65000

typedef enum UNPRESSED_STATE
{
	NORMALLY_LOW = 0x00,
	NORMALLY_HIGH
} UnpressedState;

typedef enum PRESS_STATE
{
	UNPRESSED	= 0x00,
	PRESSED
} PressState;

typedef struct
{
	GPIO_TypeDef* port;
	uint16_t pin;
	UnpressedState unpressedState;
	PressState pressState;
	uint16_t timeSinglePressCounter;
	uint16_t timeContinuousPressCounter;
	uint16_t debounceTimeSinglePressCounter;
	uint16_t debounceTimeContinuousPressCounter;
	uint16_t continuousPressedCounter;
} PushButton;

void pushButtonInit(PushButton *pushButton, GPIO_TypeDef *pushButtonPort, uint16_t pushButtonPin, UnpressedState unpressedState, uint16_t debounceTimeSinglePressCounter, uint16_t debounceTimeContinuousPressCounter);
PressState pushButtonReadSinglePressInRelease(PushButton *pushButton);
PressState pushButtonReadContinuousPress(PushButton *pushButton);
PressState pushButtonReadSingleAndContinuousPress(PushButton *pushButton);

#endif /* INC_PUSHBUTTON_H_ */
