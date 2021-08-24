//*****************************************************************************
//
// blinky.c - Simple example to blink the on-board LED.
//
// Copyright (c) 2012-2020 Texas Instruments Incorporated.  All rights reserved.
// Software License Agreement
// 
// Texas Instruments (TI) is supplying this software for use solely and
// exclusively on TI's microcontroller products. The software is owned by
// TI and/or its suppliers, and is protected under applicable copyright
// laws. You may not combine this software with "viral" open-source
// software in order to form a larger program.
// 
// THIS SOFTWARE IS PROVIDED "AS IS" AND WITH ALL FAULTS.
// NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT
// NOT LIMITED TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. TI SHALL NOT, UNDER ANY
// CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR CONSEQUENTIAL
// DAMAGES, FOR ANY REASON WHATSOEVER.
// 
// This is part of revision 2.2.0.295 of the EK-TM4C123GXL Firmware Package.
//
//*****************************************************************************

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"

//*****************************************************************************
//
//! \addtogroup example_list
//! <h1>Blinky (blinky)</h1>
//!
//! A very simple example that blinks the on-board LED using direct register
//! access.
//
//*****************************************************************************

//*****************************************************************************
//
// The error routine that is called if the driver library encounters an error.
//
//*****************************************************************************
#ifdef DEBUG
void
__error__(char *pcFilename, uint32_t ui32Line)
{
    while(1);
}
#endif

// When system starts. Let out a beep to indicate system started.
void GoodBeep(void)
{
    volatile uint32_t ui32Loop;
    volatile uint32_t i;
    for (i = 0; i < 3; i++)
    {
        GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_6, GPIO_PIN_6);
        for(ui32Loop = 0; ui32Loop < 110000; ui32Loop++)
        {
        }
        GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_6, 0x00);
        for(ui32Loop = 0; ui32Loop < 110000; ui32Loop++)
        {
        }
    }

}

// This bad beep is used to indicate the solenoid has stayed open for over 60 seconds.
void BadBeep(void)
{
    volatile uint32_t ui32Loop;
    volatile uint32_t i;
    volatile uint32_t iTotal;

    for (iTotal = 0; iTotal < 4; iTotal++)
    {
        for (i = 0; i < 3; i++)
        {
            GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_6, GPIO_PIN_6);
            for(ui32Loop = 0; ui32Loop < 410000; ui32Loop++)
            {
            }
            GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_6, 0x00);
            for(ui32Loop = 0; ui32Loop < 410000; ui32Loop++)
            {
            }
        }

        for(ui32Loop = 0; ui32Loop < 1610000; ui32Loop++)
        {
        }
    }

    GoodBeep();
}

void SetLEDRed()
{
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 0x0);
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, GPIO_PIN_1);
}

void SetLEDGreen()
{
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0x0);
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, GPIO_PIN_3);
}

//*****************************************************************************
//
// Blink the on-board LED.
//
//*****************************************************************************
int
main(void)
{
    volatile uint32_t ui32Loop;
    volatile uint32_t nRedDuration;
    volatile int32_t nPF4;
    volatile int32_t nPD7;
    //
    // Enable the GPIO port that is used for the on-board LED.
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

    //
    // Check if the peripheral access is enabled.
    //
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF))
    {
    }

    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOA))
    {
    }
    //
    // Enable the GPIO pin for the LED (PF3).  Set the direction as output, and
    // enable the GPIO pin for digital function.
    //
    GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_4);
    GPIOPinTypeGPIOInput(GPIO_PORTA_BASE, GPIO_PIN_5);
    GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, GPIO_PIN_6);
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1);
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_3);
    //
    // Loop forever.
    //
    bool bRedSet = false;
    bool bGreenSet = false;
    bool bGreen = true;

    //Following value was used to check how to get 1Hz frequency in oscillation.
    //int32_t nDuration = 805000; //1610000 is delay required for 1 second.
    int32_t nSettleDurationInSeconds = 1;

    bool bWaitForSettleTime = false;

    GoodBeep();

    while(1)
    {
        nPF4 = GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4);
        nPD7 = GPIOPinRead(GPIO_PORTA_BASE, GPIO_PIN_5);

        if (nPF4 == 0)
            bRedSet = false;

        if (nPD7 == 0)
            bGreenSet = false;

        if (nPF4 > 0 && !bRedSet && bGreen)
        {
            bRedSet = true;
            bWaitForSettleTime = true;
            bGreen = false;
        }

        if (nPD7 > 0 && !bGreenSet && !bGreen)
        {
            bGreenSet = true;
            bWaitForSettleTime = true;
            bGreen = true;
        }

        //
        // Set LED color.
        //

        if (bGreen)
        {
            SetLEDGreen();

            nRedDuration = 0;
        }
        else
        {
            SetLEDRed();

            nRedDuration += 1;
        }

        if (bWaitForSettleTime)
        {
            bWaitForSettleTime = false;
            for(ui32Loop = 0; ui32Loop < nSettleDurationInSeconds * 1610000; ui32Loop++)
            {
            }
        }
        else
        {
            for(ui32Loop = 0; ui32Loop < 1000; ui32Loop++)
            {
            }
        }

        // If red for 30 seconds.
        if (nRedDuration >= 1300 * 30)
        {
            SetLEDGreen();
            bGreen = true;
            nRedDuration = 0;
            BadBeep();
        }
    }
}
