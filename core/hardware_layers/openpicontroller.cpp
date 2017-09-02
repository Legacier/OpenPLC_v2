#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <pcf8574.h>
#include <ads1115.h>

#define DOUT_PINBASE                    100
#define DIN_PINBASE                     90
#define ADC_PINBASE                     65
#define DAC_PINBASE                     75
#define MAX_INPUT                       8

int adcBuffer[4];
int data[2]; // DAC

void *readAdcThread(void *args)
{
    while(1)
    {
        for(int i=0; i<4; i++)
        {
            pthread_mutex_lock(&adcBufferLock);
            adcBuffer[i] = analogRead(ADC_PINBASE + i);
            pthread_mutex_unlock(&adcBufferLock);
        }
    }
}
//-----------------------------------------------------------------------------
// ADS1115 Reading
//-----------------------------------------------------------------------------
int ads_adcRead(int chan)
{
        int returnValue;
        if (chan < 4)
        {
            pthread_mutex_lock(&adcBufferLock);
            returnValue = adcBuffer[chan];
            pthread_mutex_unlock(&adcBufferLock);
            return returnValue;
        }
        return 0;
}
//-----------------------------------------------------------------------------
// This function is called to initialize MCP4725
//-----------------------------------------------------------------------------
void mcp4725Setup(int addr)
{
        dac_fd = wiringPiI2CSetup(addr);
}
//-----------------------------------------------------------------------------
// MCP4725 Writing
//-----------------------------------------------------------------------------
void mcp_dacWrite(int value)
{
        data[0] = (value >> 8) & 0xFF; // [0 0 0 0 D11 D10 D9 D8] (first bits are modes for our use 0 is fine)
        data[1] = value; // [D7 D6 D5 D4 D3 D2 D1 D0]
        wiringPiI2CWrite(dac_fd, 0x40);
        wiringPiI2CWriteReg8(dac_fd, data[0], data[1]);
}
//-----------------------------------------------------------------------------
// This function is called by the main OpenPLC routine when it is initializing.
// Hardware initialization procedures should be here.
//-----------------------------------------------------------------------------
void initializeHardware()
{
        pcf8574Setup(DOUT_PINBASE, 0x20); // Digital Output Expansion
        pcf8574Setup(DIN_PINBASE, 0x21); // Digital Input Expansion
        ads1115Setup(ADC_PINBASE, 0x49); // ADC Expansion
        mcp4725Setup(0x60); // DAC Expansion
        
        for (int i = 0; i < MAX_OUTPUT; i++)
        {
            pinMode(DOUT_PINBASE + i, OUTPUT);
        }
        
        for (int i = 0; i < MAX_INPUT; i++)
        {
            pinMode(DIN_PINBASE + i, INPUT);
        }
        
        pthread_t ADCthread;
        pthread_create(&ADCthread, NULL, readAdcThread, NULL);
}
//-----------------------------------------------------------------------------
// This function is called by the OpenPLC in a loop. Here the internal buffers
// must be updated to reflect the actual I/O state. The mutex buffer_lock
// must be used to protect access to the buffers on a threaded environment.
//-----------------------------------------------------------------------------
void updateBuffers()
{
        pthread_mutex_lock(&bufferLock); //lock mutex
        
        for (int i = 0; i < MAX_INPUT; i++)
        {
                if (bool_input[i/8][i%8] != NULL) *bool_input[i/8][i%8] = !digitalRead(DIN_PINBASE + i); 
        }
        
        for (int i = 0; i < MAX_OUTPUT; i++)
        {
                if (bool_output[i/8][i%8] != NULL) digitalWrite(DOUT_PINBASE + i, *bool_output[i/8][i%8]); 
        }
       
        for (int i = 0; i < 4; i++)
        {
                if (int_input[i] != NULL) *int_input[i] = ads_adcRead(i);
        }
        
        // Analog OUT.
        if (int_output[0] != NULL) mcp_dacWrite((*int_output[0] / 16.0));
        
        pthread_mutex_unlock(&bufferLock); //unlock mutex
}