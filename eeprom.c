/*
  eeprom.c - Library for the SPI Serial EEPROM chip
*/
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


#include "common_macros.h"
#include "common_types.h"

#include "eeprom.h"
#include "spidev_lib.h"

uint32_t EEPROM_open(struct eeprom *pDev, int fdSpiDev, uint32_t model)
{
	// ATMEL
	if (model == 25010) {
		pDev->_totalBytes = 128;
		pDev->_addrBits = 7;
		pDev->_pageSize = 8;
		pDev->_lastPage = (pDev->_totalBytes/pDev->_pageSize)-1;
		strcpy(pDev->_name, "AT25010");
	} else if (model == 25020) {
		pDev->_totalBytes = 256;
		pDev->_addrBits = 8;
		pDev->_pageSize = 8;
		pDev->_lastPage = (pDev->_totalBytes/pDev->_pageSize)-1;
		strcpy(pDev->_name, "AT25020");
	} else if (model == 25040) {
		pDev->_totalBytes = 512;
		pDev->_addrBits = 9;
		pDev->_pageSize = 8;
		pDev->_lastPage = (pDev->_totalBytes/pDev->_pageSize)-1;
		strcpy(pDev->_name, "AT25040");
	} else if (model == 25080) {
		pDev->_totalBytes = 1024;
		pDev->_addrBits = 10;
		pDev->_pageSize = 32;
		pDev->_lastPage = (pDev->_totalBytes/pDev->_pageSize)-1;
		strcpy(pDev->_name, "AT25080");
	} else if (model == 25160) {
		pDev->_totalBytes = 2048;
		pDev->_addrBits = 11;
		pDev->_pageSize = 32;
		pDev->_lastPage = (pDev->_totalBytes/pDev->_pageSize)-1;
		strcpy(pDev->_name, "AT25160");
	} else if (model == 25320) {
		pDev->_totalBytes = 4096;
		pDev->_addrBits = 12;
		pDev->_pageSize = 32;
		pDev->_lastPage = (pDev->_totalBytes/pDev->_pageSize)-1;
		strcpy(pDev->_name, "AT25320");
	} else if (model == 25640) {
		pDev->_totalBytes = 8192;
		pDev->_addrBits = 13;
		pDev->_pageSize = 32;
		pDev->_lastPage = (pDev->_totalBytes/pDev->_pageSize)-1;
		strcpy(pDev->_name, "AT25640");
	} else if (model == 25128) {
		pDev->_totalBytes = 16384;
		pDev->_addrBits = 14;
		pDev->_pageSize = 64;
		pDev->_lastPage = (pDev->_totalBytes/pDev->_pageSize)-1;
		strcpy(pDev->_name, "AT25128");
	} else if (model == 25256) {
		pDev->_totalBytes = 32768;
		pDev->_addrBits = 15;
		pDev->_pageSize = 64;
		pDev->_lastPage = (pDev->_totalBytes/pDev->_pageSize)-1;
		strcpy(pDev->_name, "AT25256");
	} else if (model == 25512) {
		pDev->_totalBytes = 65536;
		pDev->_addrBits = 16;
		pDev->_pageSize = 128;
		pDev->_lastPage = (pDev->_totalBytes/pDev->_pageSize)-1;
		strcpy(pDev->_name, "AT25512");
	}

	// ST Micro
	if (model == 95010) {
		pDev->_totalBytes = 128;
		pDev->_addrBits = 7;
		pDev->_pageSize = 16;
		pDev->_lastPage = (pDev->_totalBytes/pDev->_pageSize)-1;
		strcpy(pDev->_name, "M95010");
	} else if (model == 95020) {
		pDev->_totalBytes = 256;
		pDev->_addrBits = 8;
		pDev->_pageSize = 16;
		pDev->_lastPage = (pDev->_totalBytes/pDev->_pageSize)-1;
		strcpy(pDev->_name, "M95020");
	} else if (model == 95040) {
		pDev->_totalBytes = 512;
		pDev->_addrBits = 9;
		pDev->_pageSize = 16;
		pDev->_lastPage = (pDev->_totalBytes/pDev->_pageSize)-1;
		strcpy(pDev->_name, "M95040");
	} else if (model == 95080) {
		pDev->_totalBytes = 1024;
		pDev->_addrBits = 10;
		pDev->_pageSize = 32;
		pDev->_lastPage = (pDev->_totalBytes/pDev->_pageSize)-1;
		strcpy(pDev->_name, "M95080");
	} else if (model == 95160) {
		pDev->_totalBytes = 2048;
		pDev->_addrBits = 11;
		pDev->_pageSize = 32;
		pDev->_lastPage = (pDev->_totalBytes/pDev->_pageSize)-1;
		strcpy(pDev->_name, "M95160");
	} else if (model == 95320) {
		pDev->_totalBytes = 4096;
		pDev->_addrBits = 12;
		pDev->_pageSize = 32;
		pDev->_lastPage = (pDev->_totalBytes/pDev->_pageSize)-1;
		strcpy(pDev->_name, "M95320");
	} else if (model == 95640) {
		pDev->_totalBytes = 8192;
		pDev->_addrBits = 13;
		pDev->_pageSize = 32;
		pDev->_lastPage = (pDev->_totalBytes/pDev->_pageSize)-1;
		strcpy(pDev->_name, "M95640");
	} else if (model == 95128) {
		pDev->_totalBytes = 16384;
		pDev->_addrBits = 14;
		pDev->_pageSize = 64;
		pDev->_lastPage = (pDev->_totalBytes/pDev->_pageSize)-1;
		strcpy(pDev->_name, "M95128");
	} else if (model == 95256) {
		pDev->_totalBytes = 32768;
		pDev->_addrBits = 15;
		pDev->_pageSize = 64;
		pDev->_lastPage = (pDev->_totalBytes/pDev->_pageSize)-1;
		strcpy(pDev->_name, "M95256");
	}
	pDev->_fdSpiDev = fdSpiDev;
	pDev->_model = model;
	return pDev->_totalBytes;
}

e_ReturnValue EEPROM_waitReady(struct eeprom *pDev)
{
    e_ReturnValue l_nResult = e_RETURNVALUE_Success;
    //Wait until device is READY.
    uint8_t data[2];
    uint32_t l_nCounter = 0;
    int l_nIntResult;

#define RDY_MAXRETRIES 100000

    while(++l_nCounter < RDY_MAXRETRIES)
    {
        data[0] = EEPROM_RDSR_CMD;
        data[1] = 0x0;
        l_nIntResult=spi_xfer( pDev->_fdSpiDev, data, 2, data, 2 );
        if ((data[1] & 0x01) == 0) break;
        usleep(DELAY_WAIT);
    }

    if ( RDY_MAXRETRIES <= l_nCounter )
    {
        printf("%s: Timeout waiting for rdy\r\n", __FUNCTION__);
        l_nResult = e_RETURNVALUE_Failure;
    }
    return l_nResult;

}

e_ReturnValue EEPROM_writeEnable(struct eeprom *pDev)
{
    e_ReturnValue l_nResult = e_RETURNVALUE_Success;
    //printf("EEPROM_writeEnable start\n");
    //Wait until device is WRITE ENABLED.
    uint8_t data[2];
    int l_nIntResult;
    uint32_t l_nCounter = 0;
#define WE_MAXRETRIES 100000

    while(++l_nCounter < WE_MAXRETRIES)
    {
        data[0] = EEPROM_WREN_CMD;
        l_nIntResult=spi_xfer( pDev->_fdSpiDev, data, 1, data, 1 );
        if (l_nIntResult < 0)
        {
            printf("%s: Error WREN_CMD: %d\r\n", __FUNCTION__, l_nIntResult);
            l_nResult = e_RETURNVALUE_Failure;
            goto lbl_err;
        }

        usleep(DELAY_WAIT);

        data[0] = EEPROM_RDSR_CMD;
        data[1] = 0x0;
        l_nIntResult=spi_xfer( pDev->_fdSpiDev, data, 2, data, 2 );
        if (l_nIntResult < 0)
        {
            printf("%s: Error EEPROM_RDSR_CMD: %d\r\n", __FUNCTION__, l_nIntResult);
            l_nResult = e_RETURNVALUE_Failure;
            goto lbl_err;
        }
        //printf("EEPROM_writeEnable data[1]=%x\n",data[1]);
        //if ((data[1] & 0x02) == 0x02) break;
        if (data[1] == 0x02) break;
        usleep(DELAY_WAIT);
    }

    if ( WE_MAXRETRIES <= l_nCounter )
    {
        printf("%s: Failed to set WE\r\n", __FUNCTION__);
        l_nResult = e_RETURNVALUE_Failure;
    }
lbl_err:
    return l_nResult;
}

e_ReturnValue EEPROM_writeByte(struct eeprom *pDev, uint16_t address, uint8_t data)
{
    e_ReturnValue l_nResult = e_RETURNVALUE_Success;
    e_ReturnValue l_nIntermediateResult;
    int l_nIntResult;
    uint8_t index;
    uint8_t work[4];

    l_nResult = EEPROM_writeEnable(pDev);
    if (l_nResult !=  e_RETURNVALUE_Success)
    {
        printf("%s: Error setting WE: %d\r\n", __FUNCTION__, l_nResult);
      goto lbl_err;
    }


    work[0] = EEPROM_WRITE_CMD;
    if (pDev->_addrBits == 9 && address > 0xff) work[0] = work[0] | 0x8;
    if(DEBUG)printf("[write_byte] address=0x%x work[0]=0x%x\n",address, work[0]);

    if (pDev->_addrBits < 10)
    {
        work[1] = (address & 0xFF);			//send LSByte address
        work[2] = data;
        int l_nIntResult=spi_xfer( pDev->_fdSpiDev, work, 3, work, 3 );
        if (l_nIntResult < 0)
        {
            printf("%s: Error writing byte (short address): %d\r\n", __FUNCTION__, l_nIntResult);
            l_nResult = e_RETURNVALUE_Failure;
            goto lbl_err;
        }
    }
    else
    {
        work[1] = (address & 0x0FF00) >> 8;	//send MSByte address first
        work[2] = (address & 0x0FF);		//send LSByte address
        work[3] = data;
        int l_nIntResult=spi_xfer( pDev->_fdSpiDev, work, 4, work, 4 );
        if (l_nIntResult < 0)
        {
            printf("%s: Error writing byte (long address): %d\r\n", __FUNCTION__, l_nIntResult);
            l_nResult = e_RETURNVALUE_Failure;
            goto lbl_err;
        }
    }


lbl_err:
    l_nIntermediateResult = EEPROM_waitReady(pDev);
    if (l_nIntermediateResult  !=  e_RETURNVALUE_Success)
    {
        printf("%s: Error waiting for flash ready : %d\r\n", __FUNCTION__, l_nIntermediateResult);
        if ( e_RETURNVALUE_Success == l_nResult )
        {
          l_nResult = l_nIntermediateResult;
        }
    }

    return l_nResult;
}

e_ReturnValue EEPROM_writePage(struct eeprom *pDev, uint16_t nPageNb, uint8_t *pData)
{
    e_ReturnValue l_nResult = e_RETURNVALUE_Success;
    e_ReturnValue l_nIntermediateResult;
    int l_nIntResult;
        uint8_t index;
    uint8_t *work = (uint8_t *)malloc(128);
    uint16_t address = nPageNb * pDev->_pageSize;

    if ( NULL == work )
    {
        printf("%s: Error allocating page memory\r\n", __FUNCTION__);
        l_nResult = e_RETURNVALUE_NoMem;
        goto lbl_err;
    }

    l_nResult = EEPROM_writeEnable(pDev);
    if (l_nResult !=  e_RETURNVALUE_Success)
    {
        printf("%s: Error setting WE: %d\r\n", __FUNCTION__, l_nResult);
      goto lbl_err;
    }


    work[0] = EEPROM_WRITE_CMD;
    if (pDev->_addrBits == 9 && address > 0xff) work[0] = work[0] | 0x8;
    if(DEBUG)printf("[write_pageSize] address=%x work[0]=%x\n",address, work[0]);

    if (pDev->_addrBits < 10)
    {
        work[1] = (address & 0xFF);		 //send LSByte address
        index = 2;
    }
    else
    {
        work[1] = (address & 0x0FF00) >> 8;   //send MSByte address first
        work[2] = (address & 0x0FF);	  //send LSByte address
        index = 3;
    }

    if(DEBUG)printf("[write_pageSize] _pageSize=%d\n", pDev->_pageSize);
    for (int i=0; i<pDev->_pageSize; i++)
        work[index+i] = pData[i];
    l_nIntResult=spi_write (pDev->_fdSpiDev, work, index+pDev->_pageSize);
    if (l_nIntResult < 0)
    {
        printf("%s: Error writing page: %d\r\n", __FUNCTION__, l_nIntResult);
        l_nResult = e_RETURNVALUE_Failure;
        goto lbl_free;
    }




lbl_free:
    free (work);
lbl_err:
    l_nIntermediateResult = EEPROM_waitReady(pDev);
    if (l_nIntermediateResult  !=  e_RETURNVALUE_Success)
    {
        printf("%s: Error waiting for flash ready : %d\r\n", __FUNCTION__, l_nIntermediateResult);
        if ( e_RETURNVALUE_Success == l_nResult )
        {
          l_nResult = l_nIntermediateResult;
        }
    }

    return l_nResult;
}

e_ReturnValue EEPROM_readByte(struct eeprom *pDev, uint16_t address, uint8_t *pData)
{
    e_ReturnValue l_nResult = e_RETURNVALUE_Success;
    int l_nIntResult;
	uint8_t work[4];
	if (( NULL == pDev) || (NULL == pData) )
	{
        printf("%s: Null argument(s)\r\n", __FUNCTION__);
        l_nResult = e_RETURNVALUE_IllegalArgument;
        goto lbl_err;
	}
	work[0] = EEPROM_READ_CMD;
	if ( (pDev->_addrBits == 9) && (address > 0xff) )
	{
        work[0] = work[0] | 0x8;
    }
    if(DEBUG)printf("[read_byte] address=0x%x work[0]=0x%x\n",address, work[0]);

	uint8_t data;
	if (pDev->_addrBits < 10)
	{
		work[1] = (address & 0xFF);			//send LSByte address
		work[2] = 0;
		l_nIntResult=spi_xfer( pDev->_fdSpiDev, work, 3, work, 3 );
		if (l_nIntResult < 0)
		{
            printf("%s: Error sending short address: %d\r\n", __FUNCTION__, l_nIntResult);
            l_nResult = e_RETURNVALUE_Failure;
            goto lbl_err;
		}
		data = work[2];
	}
	else
	{
		work[1] = (address & 0x0FF00) >> 8;	//send MSByte address first
		work[2] = (address & 0x0FF);		//send LSByte address
		work[3] = 0;
		l_nIntResult=spi_xfer( pDev->_fdSpiDev, work, 4, work, 4 );
		if (l_nIntResult < 0)
		{
            printf("%s: Error sending long address: %d\r\n", __FUNCTION__, l_nIntResult);
            l_nResult = e_RETURNVALUE_Failure;
            goto lbl_err;
		}
		data = work[3];
	}

	*pData= (data);

lbl_err:
    return l_nResult;
}

uint32_t EEPROM_totalBytes(struct eeprom *pDev)
{
	return pDev->_totalBytes;
}

int16_t EEPROM_lastPage(struct eeprom *pDev)
{
	return pDev->_lastPage;
}

uint16_t EEPROM_pageSize(struct eeprom *pDev)
{
	return pDev->_pageSize;
}

char * EEPROM_name(struct eeprom *pDev)
{
	return pDev->_name;
}


