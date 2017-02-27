

#include <drivers/uart/adi_uart.h>
#include <drivers/spi/adi_spi.h>
#include "common.h"
#include "Communications.h"
#include <services/gpio/adi_gpio.h>

uint8_t                 UartDeviceMem[UART_MEMORY_SIZE];//UART memory size
ADI_UART_HANDLE         hUartDevice;//UART device handle
ADI_UART_RESULT         eUartResult;//UART error variable
static bool data_sent =        false;//UART data_sent flag
bool data_received =    false;//UART data_recieved flag


ADI_SPI_RESULT          eSpiResult; //SPI error variable
static ADI_SPI_HANDLE   hSPIDevice; //SPI handle
static uint8_t          SPIMem[ADI_SPI_MEMORY_SIZE];//SPI memory size
static ADI_SPI_TRANSCEIVER transceive;//transceive struct for SPI Read/Writes


/********************************************************************
* UART Interrupt callback                                            *
*********************************************************************/
void UARTCallback( void *pAppHandle, uint32_t nEvent, void *pArg)
{

   //CASEOF (event type)
    switch (nEvent)
    {
        //CASE (TxBuffer has been cleared, Data sent) 
        case ADI_UART_EVENT_TX_BUFFER_PROCESSED:
                adi_uart_EnableTx(hUartDevice, false);//disable tx buffer
                data_sent = true;
                break;
                
        //CASE (RxBuffer has been cleared, Data recieved) 
        case ADI_UART_EVENT_RX_BUFFER_PROCESSED:
                adi_uart_EnableRx(hUartDevice, false);//disable rx buffer
                data_received = true;
                break;
                
    default: break;
    }
}



/**********************************************************************************************
* Function Name: UART_Init                                                                   
* Description  : This function initializes an instance of the UART driver for UART_DEVICE_NUM 
* Arguments    : void                                                                         
* Return Value : 0 = Success                                                                    
*                1 = Failure (See eUartResult in debug mode for adi micro specific info)     
**********************************************************************************************/
unsigned char Uart_Init(void)
{
 
  //open Uart
  eUartResult = adi_uart_Open(UART_DEVICE_NUM,ADI_UART_DIR_BIDIRECTION,
                UartDeviceMem,
                UART_MEMORY_SIZE,
                &hUartDevice);
  DEBUG_RESULT("UART open failed",eUartResult,ADI_UART_SUCCESS);
  
  //configure UART device with NO-PARITY, ONE STOP BIT and 8bit word length. 
  eUartResult = adi_uart_SetConfiguration(hUartDevice,
                            ADI_UART_NO_PARITY,
                            ADI_UART_ONE_STOPBIT,
                            ADI_UART_WORDLEN_8BITS);
  DEBUG_RESULT("UART config failed",eUartResult,ADI_UART_SUCCESS);
  
  //set baud rate at 115200
  eUartResult = adi_uart_ConfigBaudRate(hUartDevice,
                          UART_DIV_C_115200,
                          UART_DIV_M_115200,
                          UART_DIV_N_115200,
                          UART_OSR_115200);
	DEBUG_RESULT("UART config failed",eUartResult,ADI_UART_SUCCESS);
	
	//register callback
  adi_uart_RegisterCallback(hUartDevice,UARTCallback,hUartDevice);
  DEBUG_RESULT("UART callback not registered",eUartResult,ADI_UART_SUCCESS);
	
  //init ble flow control uart pins
  adi_gpio_InputEnable(BLE_RTS_PORT, BLE_RTS_PIN, true);
  adi_gpio_OutputEnable(BLE_CTS_PORT, BLE_CTS_PIN, true);
  
  //init mux control
  adi_gpio_OutputEnable(MUX_PORT, MUX_PIN, true);
	
  //turn on transfer status LED
  transferLedOn();
  
    return 0;
}

	
/**********************************************************************************************
* Function Name: UART_Close                                                                   
* Description  : This function releases a UART handle as initialised by Uart_Init
* Arguments    : void                                                                         
* Return Value : 0 = Success                                                                    
*                1 = Failure (See eUartResult in debug mode for adi micro specific info)     
**********************************************************************************************/
unsigned char Uart_Close(void)
{
  //close Uart device
  eUartResult = adi_uart_Close(hUartDevice);
  DEBUG_RESULT("UART not closed",eUartResult,ADI_UART_SUCCESS);
  
  //turn off transfer status LED
  transferLedOff();
  
    return 0;
}


/**********************************************************************************************
* Function Name: UART_ReadWrite                                                                   
* Description  : This function parses a string to the TxBuffer, submits it and submits an 
*                empty RxBuffer before enabling the dataflow for both buffers.
*                data_recieved/sent flag set by callback
* Arguments    : char* string = string to be sent                                                                       
* Return Value : 0 = Success                                                                    
*                1 = Failure (See eUartResult in debug mode for adi micro specific info)     
**********************************************************************************************/
unsigned char Uart_ReadWrite(char *TxBuffer, char *RxBuffer, int length)
{


  //Indicate transfer is occuring
  transferLedOff();
  
  //clear flags
  data_sent = false;
  data_received = false;
  
  //ensure data transfer is disabled for submitting buffers
  eUartResult = adi_uart_EnableRx(hUartDevice,false);
  DEBUG_RESULT("RxBuffer not disabled",eUartResult,ADI_UART_SUCCESS);
  
  eUartResult = adi_uart_EnableTx(hUartDevice,false); 
  DEBUG_RESULT("TxBuffer not disabled",eUartResult,ADI_UART_SUCCESS);
  
  //length of string
  int16_t size_l = 0; 
  size_l = strlen(TxBuffer);

  if(RxBuffer != NULL)
  {
    //'empty' RxBuffer using NULL char
    RxBuffer[0] = '\0';
    
    //submit RxBuffer to receive data
    eUartResult = adi_uart_SubmitRxBuffer(hUartDevice, RxBuffer, length);
    DEBUG_RESULT("RxBuffer not submitted",eUartResult,ADI_UART_SUCCESS);
  }
  
  if(TxBuffer != NULL)
  {
    //submit TxBuffer for sending data
    eUartResult = adi_uart_SubmitTxBuffer(hUartDevice, TxBuffer, size_l);
    DEBUG_RESULT("TxBuffer not submitted",eUartResult,ADI_UART_SUCCESS);
  }
  
  if(RxBuffer != NULL)
  {
    // Enable the Data flow for Rx. This is disabled by UARTCallback
    eUartResult = adi_uart_EnableRx(hUartDevice,true);
    DEBUG_RESULT("RxBuffer not enabled",eUartResult,ADI_UART_SUCCESS);
  }
  
  else
    data_received = true;
  
  if(TxBuffer != NULL)
  {
    // Enable the Data flow for Tx. This is disabled by UARTCallback
    eUartResult = adi_uart_EnableTx(hUartDevice,true);
    DEBUG_RESULT("TxBuffer not enabled",eUartResult,ADI_UART_SUCCESS);
  }
  
  else
    data_sent = true;

  //wait for data sent and received
  while((data_sent & data_received)== false)
	  Delay_ms(10);
		
  //Indicate transfer has stopped
  transferLedOn();
  
    return 0;
}
     

/**********************************************************************************************
* Function Name: UART_Read                                                                  
* Description  : This function submits an empty RxBuffer before enabling the dataflow for the receive
*                buffer. data_recieved flag set by callback
* Arguments    : void                                                                         
* Return Value : 0 = Success                                                                    
*                1 = Failure (See eUartResult in debug mode for adi micro specific info)     
**********************************************************************************************/
unsigned char Uart_Read(char* RxBuffer, int length)
{
	transferLedOff();

	//call read write function
	adi_uart_Read(hUartDevice, RxBuffer, length);

	transferLedOn();
  
    return 0;
}


/**********************************************************************************************
* Function Name: UART_Write                                                                   
* Description  : This function parses a string to the TxBuffer, submits it before 
*                enabling the dataflow for the transfer buffers. data_sent flag set by callback
* Arguments    : char* string = string to be sent                                                                        
* Return Value : 0 = Success                                                                    
*                1 = Failure (See eUartResult in debug mode for adi micro specific info)     
**********************************************************************************************/
unsigned char Uart_Write(char* TxBuffer)
{
	transferLedOff();

	//call read write function
	adi_uart_Write(hUartDevice, TxBuffer, strlen(TxBuffer));

  	transferLedOn();
    return 0;
}


/**********************************************************************************************
* Function Name: UART_Mux                                                                  
* Description  : This function muxes UART Rx and Tx signals to seperate devices
* Arguments    : Device = selects action on Mux                                                                       
* Return Value : 0 = Success                                                                    
*                1 = Failure (See eUartResult in debug mode for adi micro specific info)     
**********************************************************************************************/
unsigned char Uart_Mux(int Device)
{
  switch(Device)
  {
    case BLE_MUX: adi_gpio_SetLow(MUX_PORT, MUX_PIN);
    break;
          
    case LibIIO_MUX: adi_gpio_SetHigh(MUX_PORT, MUX_PIN);
    break;
          
    default: return 1;
  }
  
  return 0;
}


/**********************************************************************************************
* Function Name: Spi_Init                                                                   
* Description  : This function initializes SPI and creates a handle which is configured accordingly
* Arguments    : void                                                                       
* Return Value : 0 = Success                                                                    
*                1 = Failure (See eUartResult in debug mode for adi micro specific info)     
**********************************************************************************************/
unsigned char Spi_Init(void)
{
  eSpiResult = adi_spi_Open(SPI_DEV_NUM,SPIMem,ADI_SPI_MEMORY_SIZE,&hSPIDevice);
  if(eSpiResult != ADI_SPI_SUCCESS)
    return 1;
  
  //Set the SPI clock rate 
  eSpiResult = adi_spi_SetBitrate(hSPIDevice,SPI_BITRATE);
  if(eSpiResult != ADI_SPI_SUCCESS)
    return 1;
  
  //Set the chip select 
  eSpiResult = adi_spi_SetChipSelect(hSPIDevice, SPI_CS_NUM);
  if(eSpiResult != ADI_SPI_SUCCESS)
    return 1;
  
  //Enable continue mode (Chip Select remains low until the end of the transaction
  eSpiResult = adi_spi_SetContinousMode(hSPIDevice, true);
  if(eSpiResult != ADI_SPI_SUCCESS)
    return 1;
  
  //Disable DMA
  eSpiResult = adi_spi_EnableDmaMode(hSPIDevice, false);
  if(eSpiResult != ADI_SPI_SUCCESS)
    return 1;
    
  //Indicate comms are up
  transferLedOn();
  
    return 0;  
}


/**********************************************************************************************
* Function Name: Spi_Close                                                                   
* Description  : This function closes the existing SPI handle
* Arguments    : void                                                                       
* Return Value : 0 = Success                                                                    
*                1 = Failure (See eUartResult in debug mode for adi micro specific info)     
**********************************************************************************************/
unsigned char Spi_Close(void)
{
  eSpiResult = adi_spi_Close(hSPIDevice);
  if(eSpiResult != ADI_SPI_SUCCESS)
    return 1;

  //Indicate comms are closed
  transferLedOff();
    return 0;
}


/**********************************************************************************************
* Function Name: Spi_ReadWrite                                                                   
* Description  : This function configures a transceive struct to write and read from the SPI object
* Arguments    : uint8_t const* TxArray = Transmit Array
*                uint16_t TxLength = Transmit length (bytes)
*                uint8_t* RxArray = Receive Array
*                uint16_t RxLength = Recieve length (bytes)                                                                   
* Return Value : 0 = Success                                                                    
*                1 = Failure (See eUartResult in debug mode for adi micro specific info)     
**********************************************************************************************/
unsigned char Spi_ReadWrite(uint8_t const* TxArray, uint16_t TxLength, uint8_t* RxArray, uint16_t RxLength)
{  
  //Indicate transfer is occuring
  transferLedOff();
  
  //enable DMA mode to manage transfers in background
   eSpiResult = adi_spi_EnableDmaMode(hSPIDevice, true);
   if(eSpiResult != ADI_SPI_SUCCESS)
    return 1;

   //setup transceive struct with Tx and Rx buffers
   transceive.TransmitterBytes = TxLength;
   transceive.ReceiverBytes = RxLength;
   transceive.nTxIncrement = true;
   transceive.nRxIncrement = true;
   transceive.pReceiver = RxArray;
   transceive.pTransmitter = (uint8_t *)TxArray;
   
   //commit transeive struct to write read operation
   eSpiResult = adi_spi_ReadWrite(hSPIDevice,&transceive);
   if(eSpiResult != ADI_SPI_SUCCESS)
    return 1;
   
   //disable DMA mode
   eSpiResult = adi_spi_EnableDmaMode(hSPIDevice, false);
   if(eSpiResult != ADI_SPI_SUCCESS)
    return 1;
      
  //Indicate transfer is complete
  transferLedOn();
  
    return 0;
}


/**********************************************************************************************
* Function Name: Spi_Write                                                                   
* Description  : This function configures a transceive struct to write to the SPI object
* Arguments    : void                                                                       
* Return Value : 0 = Success                                                                    
*                1 = Failure (See eUartResult in debug mode for adi micro specific info)     
**********************************************************************************************/
unsigned char Spi_Write(uint8_t const * TxArray, uint8_t TxLength)
{
   //call read write function
   Spi_ReadWrite(TxArray, TxLength, NULL, 0);
  
    return 0;
}

void transferLedOn(void)
{
  adi_gpio_SetHigh(BLE_LED_PORT, BLE_LED_PIN);
  adi_gpio_SetLow(BLE_LED_PORT, BLE_LED_PIN);
}

void transferLedOff(void)
{
  adi_gpio_SetHigh(BLE_LED_PORT, BLE_LED_PIN);
}
