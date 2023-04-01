/*
 * pushButton.c
 *
 *  Created on: Mar 31, 2023
 *      Author: Otavio
 */

#include "pushButton.h"

void pushButtonInit(PushButton *pushButton, GPIO_TypeDef *pushButtonPort, uint16_t pushButtonPin, UnpressedState unpressedState, uint16_t debounceTimeSinglePressCounter, uint16_t debounceTimeContinuousPressCounter)
{
	pushButton->port = pushButtonPort;
	pushButton->pin = pushButtonPin;
	pushButton->unpressedState = unpressedState;
	pushButton->pressState = UNPRESSED;
	pushButton->timeSinglePressCounter = 0;
	pushButton->timeContinuousPressCounter = 0;
	pushButton->continuousPressedCounter = 0;
	pushButton->debounceTimeSinglePressCounter = debounceTimeSinglePressCounter;
	pushButton->debounceTimeContinuousPressCounter = debounceTimeContinuousPressCounter;

	if(pushButton->debounceTimeSinglePressCounter < MIN_PRESSED_COUNTER)
	{
		pushButton->debounceTimeSinglePressCounter = MIN_PRESSED_COUNTER;
	}
	else if(pushButton->debounceTimeSinglePressCounter > MAX_PRESSED_COUNTER)
	{
		pushButton->debounceTimeSinglePressCounter = MAX_PRESSED_COUNTER;
	}

	if(pushButton->debounceTimeContinuousPressCounter < MIN_PRESSED_COUNTER)
	{
		pushButton->debounceTimeContinuousPressCounter = MIN_PRESSED_COUNTER;
	}
	else if(pushButton->debounceTimeContinuousPressCounter > MAX_PRESSED_COUNTER)
	{
		pushButton->debounceTimeContinuousPressCounter = MAX_PRESSED_COUNTER;
	}
}

PressState pushButtonReadSinglePressInRelease(PushButton *pushButton)
{
	GPIO_PinState currentPushButtonPinState = HAL_GPIO_ReadPin(pushButton->port, pushButton->pin);

	if((uint8_t) currentPushButtonPinState == (uint8_t) pushButton->unpressedState)
	{
		if(pushButton->timeSinglePressCounter >= pushButton->debounceTimeSinglePressCounter)
		{
			pushButton->timeSinglePressCounter = MIN_PRESSED_COUNTER;
			pushButton->pressState = PRESSED;
			return PRESSED;
		}

		pushButton->timeSinglePressCounter = MIN_PRESSED_COUNTER;
		pushButton->pressState = UNPRESSED;
		return UNPRESSED;
	}

	pushButton->timeSinglePressCounter += 1;
	if(pushButton->timeSinglePressCounter < MIN_PRESSED_COUNTER)
	{
		pushButton->timeSinglePressCounter = MIN_PRESSED_COUNTER;
	}
	else if(pushButton->timeSinglePressCounter > MAX_PRESSED_COUNTER)
	{
		pushButton->timeSinglePressCounter = MAX_PRESSED_COUNTER;
	}

	pushButton->pressState = UNPRESSED;
	return UNPRESSED;
}

PressState pushButtonReadContinuousPress(PushButton *pushButton)
{
	GPIO_PinState currentPushButtonPinState = HAL_GPIO_ReadPin(pushButton->port, pushButton->pin);

	if((uint8_t) currentPushButtonPinState == (uint8_t) pushButton->unpressedState)
	{
		pushButton->continuousPressedCounter = 0;
		pushButton->timeContinuousPressCounter = MIN_PRESSED_COUNTER;
		pushButton->pressState = UNPRESSED;
		return UNPRESSED;
	}

	if(pushButton->continuousPressedCounter >= 20)
	{
		pushButton->timeContinuousPressCounter += 4;
	}
	else if(pushButton->continuousPressedCounter >= 10)
	{
		pushButton->timeContinuousPressCounter += 2;
	}
	else
	{
		pushButton->timeContinuousPressCounter += 1;
	}

	if(pushButton->timeContinuousPressCounter < MIN_PRESSED_COUNTER)
	{
		pushButton->timeContinuousPressCounter = MIN_PRESSED_COUNTER;
	}
	else if(pushButton->timeContinuousPressCounter > MAX_PRESSED_COUNTER)
	{
		pushButton->timeContinuousPressCounter = MAX_PRESSED_COUNTER;
	}

	if(pushButton->timeContinuousPressCounter >= pushButton->debounceTimeContinuousPressCounter)
	{
		if(pushButton->continuousPressedCounter < 20)
		{
			pushButton->continuousPressedCounter++;
		}

		pushButton->timeContinuousPressCounter = MIN_PRESSED_COUNTER;
		pushButton->pressState = PRESSED;
		return PRESSED;
	}

	pushButton->pressState = UNPRESSED;
	return UNPRESSED;
}

PressState pushButtonReadSingleAndContinuousPress(PushButton *pushButton)
{
	PressState pushButtonSinglePressStateInRelease = pushButtonReadSinglePressInRelease(pushButton);
	PressState pushButtonContinuousPressState = pushButtonReadContinuousPress(pushButton);

	return (pushButtonSinglePressStateInRelease || pushButtonContinuousPressState);
}
