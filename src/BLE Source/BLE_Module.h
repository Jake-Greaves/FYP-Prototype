
#ifndef __DIALOG_SPI_M350_H
#define __DIALOG_SPI_M350_H


/******************************************************************************/
/* Include Files                                                              */
/******************************************************************************/

#include <services/gpio/adi_gpio.h>

/******************************************************************************/
/* BLE parameters                                                             */
/******************************************************************************/


#define SPI_ACK 0x02
#define SPI_NACK 0x20

#define RESET_LENGTH     10 //ms
#define MAX_ATTEMPTS     5000

#define BLE_RST_PORT    ADI_GPIO_PORT0
#define BLE_RST_PIN     ADI_GPIO_PIN_12

#define RESET_LENGTH     10 //ms

/******************************************************************************/
/* Shared Global flags                                                        */
/******************************************************************************/
extern  bool_t Ble_Sleep;

/******************************************************************************/
/* Function Prototypes                                                       */
/******************************************************************************/

//boot BLE module using SPI interface
uint8_t Ble_Spi_Boot(uint8_t const * bin, uint32_t length);

//calculate check value
uint8_t calc_crc(uint8_t const * bin, uint32_t length);

//Write to BLE over UART
uint8_t Ble_Uart_Write(char* TxBuffer);

//Read from BLE over UART
uint8_t Ble_Uart_Read(char* RxBuffer, int length);

//Read/Write BLE over UART
uint8_t Ble_Uart_ReadWrite(char *TxBuffer, char* RxBuffer, int length);

//wait until BLE is awake
uint8_t Ble_WaitUntilAvailable(void);

//wake ble by toggling cts pin
uint8_t Ble_Wake(void);


#endif /* __DIALOG_SPI_M350_H */
