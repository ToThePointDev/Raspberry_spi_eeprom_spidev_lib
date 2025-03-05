/*
  eeprom.h - Library for the SPI Serial EEPROM chip
*/
#ifndef _EEPROM_H_
#define _EEPROM_H_

#define EEPROM_WREN_CMD           0x06        /* Set Write Enable Latch */
#define EEPROM_WRDI_CMD           0x04        /* Reset Write Enable Latch */
#define EEPROM_RDSR_CMD           0x05        /* Read Status Register */
#define EEPROM_WRSR_CMD           0x01        /* Write Status Register */
#define EEPROM_READ_CMD           0x03        /* Read Data from Memory Array */
#define EEPROM_WRITE_CMD          0x02        /* Write Data to Memory Array */

#define DELAY_WAIT    1
#define	DEBUG	0


struct eeprom
{
	int      _fdSpiDev;
	uint32_t _model;
	uint32_t _totalBytes;     // eeprom memory size
	int16_t _addrBits;        // number of address bit
	int16_t _pageSize;        // bytes per page
	int16_t _lastPage;        // last page
	char 	_name[16];
};

uint32_t EEPROM_open(struct eeprom *pDev, int fdSpiDev, uint32_t model);
e_ReturnValue EEPROM_waitReady(struct eeprom *pDev);
e_ReturnValue EEPROM_writeEnable(struct eeprom *pDev);
uint8_t EEPROM_readStatus(struct eeprom *dev);
void EEPROM_writeStatus(struct eeprom *dev, uint8_t status);
e_ReturnValue EEPROM_writeByte(struct eeprom *pDev, uint16_t address, uint8_t data);
e_ReturnValue EEPROM_writePage(struct eeprom *pDev, uint16_t nPageNb, uint8_t *pData);
e_ReturnValue EEPROM_readByte(struct eeprom *pDev, uint16_t address, uint8_t *pData);
uint32_t EEPROM_totalBytes(struct eeprom *dev);
int16_t EEPROM_lastPage(struct eeprom *dev);
uint16_t EEPROM_pageSize(struct eeprom *dev);
char * EEPROM_name(struct eeprom *dev);

#endif

