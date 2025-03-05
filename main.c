/*
  AT25010  cc -o main main.c eeprom.c spidev_lib.c -DAT25010
  AT25020  cc -o main main.c eeprom.c spidev_lib.c -DAT25020
  AT25040  cc -o main main.c eeprom.c spidev_lib.c -DAT25040
  AT25080  cc -o main main.c eeprom.c spidev_lib.c -DAT25080
  AT25160  cc -o main main.c eeprom.c spidev_lib.c -DAT25160
  AT25320  cc -o main main.c eeprom.c spidev_lib.c -DAT25320
  AT25640  cc -o main main.c eeprom.c spidev_lib.c -DAT25640
  AT25128  cc -o main main.c eeprom.c spidev_lib.c -DAT25128
  AT25256  cc -o main main.c eeprom.c spidev_lib.c -DAT25256
  AT25512  cc -o main main.c eeprom.c spidev_lib.c -DAT25512

  M95010  cc -o main main.c eeprom.c spidev_lib.c -DM95010
  M95020  cc -o main main.c eeprom.c spidev_lib.c -DM95020
  M95040  cc -o main main.c eeprom.c spidev_lib.c -DM95040
  M95080  cc -o main main.c eeprom.c spidev_lib.c -DM95080
  M95160  cc -o main main.c eeprom.c spidev_lib.c -DM95160
  M95320  cc -o main main.c eeprom.c spidev_lib.c -DM95320
  M95640  cc -o main main.c eeprom.c spidev_lib.c -DM95640
  M95128  cc -o main main.c eeprom.c spidev_lib.c -DM95128
  M95256  cc -o main main.c eeprom.c spidev_lib.c -DM95256
  M95512  cc -o main main.c eeprom.c spidev_lib.c -DM95512


Commands for spi0 reading out of airbag controller eeprom:
sudo dtoverlay -r spi0-1cs
sudo dtoverlay spi0-1cs cs0_pin=8
cc -o main main.c eeprom.c spidev_lib.c -DM95640
./main -o contents_fault.bin

./main -i contents_cleared.bin

*/

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/stat.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>
#include <errno.h>
#include <unistd.h>


#include "common_macros.h"
#include "common_types.h"
#include "spidev_lib.h"
#include "eeprom.h"




/**
  * Select the spi device and chipselect according to /dev/spidev{SPI_DEV}{SPI_CS}
  * sudo dtoverlay spi0-1s cs0_pin=8
  */
#define SPI_DEV		0
#define SPI_CS 		0




static spi_config_t m_sSpiConfig;



void dump(char * title, int bits, uint8_t *pData, uint16_t nDataSize)
{
	int clm = 0;
	uint16_t data;
	uint16_t saddr =0;
	uint16_t eaddr =nDataSize-1;

	printf("-------------------- [%s] --------------------\r\n", title);
	uint16_t addr;
	for (addr = saddr; addr <= eaddr; addr++) {
		data = pData[addr];
		if (clm == 0) {
			printf("%05x: ",addr);
		}

		if (bits == 8)	printf("%02x ",data);
		if (bits == 16) printf("%04x ",data);

		clm++;
		if (bits == 8 && clm == 16) {
			printf("\n");
			clm = 0;
		}
		if (bits == 16 && clm == 8) {
			printf("\n");
			clm = 0;
		}
	}
	printf("-------------------- [%s] --------------------\r\n", title);
}

e_ReturnValue lclDump2File(char * strFile, uint8_t *pData, uint16_t nDataSize)
{
    e_ReturnValue   l_nResult = e_RETURNVALUE_Success;
    int             l_nIntResult;
    FILE            *l_fpWrite;
    struct stat 	l_stStat;

    //Abort if the file already exists
	l_nIntResult = stat( strFile, &l_stStat );
	//printf("%s=%d\n", device,ret);
	if (l_nIntResult == 0)
	{
        printf("%s: Targe file %s already exists, aborting\r\n", __FUNCTION__, strFile);
		l_nResult =  e_RETURNVALUE_IllegalArgument;
		goto lbl_err;
	}

	l_fpWrite = fopen( strFile,"wb");  // w for write, b for binary
	if ( NULL == l_fpWrite  )
	{
        printf("%s: Error opening target file %s : %d [%s]\r\n", __FUNCTION__, strFile, errno, strerror( errno ) );
        l_nResult =  e_RETURNVALUE_Failure;
        goto lbl_err;
	}

	l_nIntResult = fwrite( pData, nDataSize, 1, l_fpWrite ); // write  bytes from our buffer, returns the number of elements written
	if ( l_nIntResult != 1 )
	{
        printf("%s: Error writing bytes to target file %s : %d\r\n", __FUNCTION__, strFile, l_nIntResult );
        l_nResult =  e_RETURNVALUE_Failure;
	}

	l_nIntResult = fclose( l_fpWrite );
	if ( l_nIntResult != 0 )
	{
        printf("%s: Error closing file %s : %d\r\n", __FUNCTION__, strFile, l_nIntResult );
        l_nResult =  e_RETURNVALUE_Failure;
	}

lbl_err:
    return l_nResult;
}

/**
  * Read the contents of strFile into the databuffer pData that will be malloc-ed by this function
  *
  * @param strFile  The path to the file to read
  * @param pData    The path to the file to read

  */
e_ReturnValue lclReadFile(char * strFile, uint8_t **ppData, uint32_t *pnDataSize)
{
    e_ReturnValue   l_nResult = e_RETURNVALUE_Success;
    int             l_nIntResult;
    FILE            *l_fpRead;
    struct stat 	  l_stStat;
    uint32_t        l_nSize;

    (*ppData) = NULL;
    //Abort if the file already exists
    l_nIntResult = stat( strFile, &l_stStat );
    //printf("%s=%d\n", device,ret);
    if (l_nIntResult != 0)
    {
        printf("%s: Targe file %s does not exists, aborting\r\n", __FUNCTION__, strFile);
        l_nResult =  e_RETURNVALUE_IllegalArgument;
        goto lbl_err;
    }

    l_nSize= l_stStat.st_size;
    (*ppData)= (uint8_t* )malloc(l_nSize);
    if ( NULL == (*ppData) )
    {
        printf("%s: Error mallocing memory to read file %s of size: %d\r\n", __FUNCTION__, strFile, l_nSize );
        l_nResult =  e_RETURNVALUE_NoMem;
        goto lbl_err;
    }

    l_fpRead = fopen( strFile,"rb");  // w for write, b for binary
    if ( NULL == l_fpRead )
    {
        printf("%s: Error opening sourcefile %s : %d [%s]\r\n", __FUNCTION__, strFile, errno, strerror( errno ) );
        l_nResult =  e_RETURNVALUE_Failure;
        goto lbl_err;
    }

    l_nIntResult = fread( (*ppData), 1, l_nSize, l_fpRead ); // read  bytes to our buffer, returns the number of elements read
    if ( l_nIntResult != l_nSize )
    {
        printf("%s: Error reading bytes from source file %s : %d\r\n", __FUNCTION__, strFile, l_nIntResult );
        l_nResult =  e_RETURNVALUE_Failure;
    }

    l_nIntResult = fclose( l_fpRead );
    if ( l_nIntResult != 0 )
    {
        printf("%s: Error closing file %s : %d\r\n", __FUNCTION__, strFile, l_nIntResult );
        l_nResult =  e_RETURNVALUE_Failure;
    }


lbl_err:

    if (l_nResult !=  e_RETURNVALUE_Success)
    {
        if ( (*ppData) != NULL )
        {
            // Clean up in case of error
            free((*ppData));
        }
        *pnDataSize = 0;
    }
    else
    {
        *pnDataSize = l_nSize;
    }

    return l_nResult;
}



int main(int argc, char *argv[])
{
  e_ReturnValue l_nResult;
	// set EEPROM memory size
	int eeprom_model = 0;

  char * l_sInputFile = NULL;
  char * l_sOutputFile = NULL;

	int opt;
  while ((opt = getopt (argc, argv, "i:o:m:")) != -1)
  {
    switch (opt)
    {
      case 'i':
        printf("Input file: \"%s\"\n", optarg);
        l_sInputFile = strdup( optarg );
        break;
      case 'o':
        printf("Output file: \"%s\"\n", optarg);
        l_sOutputFile = strdup( optarg );
        break;
      case 'm':
        {
          char * l_strEnd;
          eeprom_model = strtol( optarg , &l_strEnd, 10);
        }
        break;
    }
  }


// ATMEL
#ifdef AT25010
	eeprom_model = 25010;
#endif
#ifdef AT25020
	eeprom_model = 25020;
#endif
#ifdef AT25040
	eeprom_model = 25040;
#endif
#ifdef AT25080
	eeprom_model = 25080;
#endif
#ifdef AT25160
	eeprom_model = 25160;
#endif
#ifdef AT25320
	eeprom_model = 25320;
#endif
#ifdef AT25640
	eeprom_model = 25640;
#endif
#ifdef AT25128
	eeprom_model = 25128;
#endif
#ifdef AT25256
	eeprom_model = 25256;
#endif
#ifdef AT25512
	eeprom_model = 25512;
#endif

// ST Micro
#ifdef M95010
	eeprom_model = 95010;
#endif
#ifdef M95020
	eeprom_model = 95020;
#endif
#ifdef M95040
	eeprom_model = 95040;
#endif
#ifdef M95080
	eeprom_model = 95080;
#endif
#ifdef M95160
	eeprom_model = 95160;
#endif
#ifdef M95320
	eeprom_model = 95320;
#endif
#ifdef M95640
	eeprom_model = 95640;
#endif
#ifdef M95128
	eeprom_model = 95128;
#endif
#ifdef M95256
	eeprom_model = 95256;
#endif

  if (  ((NULL != l_sInputFile ) && (NULL != l_sOutputFile ))
    ||  ((NULL == l_sInputFile ) && (NULL == l_sOutputFile ))
    )
  {
    printf("./main -i infile -o outfile -m modelnb\n");
  	printf("Specify either an input file or an outputfile, not both, not neither\n");
		return 1;
  }
	printf("eeprom_model=%d\n",eeprom_model);
	if (eeprom_model == 0) {
    printf("./main -i infile -o outfile -m modelnb\n");
		printf("EEPROM model not found\n");
		return 1;
	}



	// open SPI device
	int 			l_fdSpiDev;

	struct eeprom 	l_stEepromDev;
	char 			l_strDevice[ 20 ];
	struct stat 	l_stStat;
	int ret;
	snprintf(l_strDevice, 19, "/dev/spidev%d.%d", SPI_DEV, SPI_CS );
	ret = stat( l_strDevice, &l_stStat );
	//printf("%s=%d\n", device,ret);
	if (ret != 0) {
		printf("spi device[%s] not found\r\nHave you enabled the device in the device tree?\r\nEg 'sudo dtoverlay spi%d-1cs' for a non permanent enable with 1 CS at the default GPIO\r\n", l_strDevice, SPI_DEV);
		return 1;
	}

	// Start SPI with 2MHz
	m_sSpiConfig.mode=0;
	m_sSpiConfig.speed=2000000;
	m_sSpiConfig.delay=0;
	m_sSpiConfig.bits_per_word=8;

	l_fdSpiDev = spi_open( l_strDevice, m_sSpiConfig );


	uint32_t l_nEepromTotalBytes = EEPROM_open(&l_stEepromDev, l_fdSpiDev, eeprom_model);
	int16_t lastPage = EEPROM_lastPage(&l_stEepromDev);
	uint16_t pageSize = EEPROM_pageSize(&l_stEepromDev);
	char *name = EEPROM_name(&l_stEepromDev);
	printf("name=%s\n", name);
	printf("l_nEepromTotalBytes=%d lastPage=%d pageSize=%d\n",l_nEepromTotalBytes, lastPage, pageSize);


#if 1
  /* Read the contents of the eeprom and write it to the output file */
  if (NULL != l_sOutputFile )
  {
    uint16_t l_nMemAddr;
    uint8_t data;
    uint8_t * l_pReadDataBuffer = (uint8_t *) malloc( l_nEepromTotalBytes );
    uint16_t i;

    if ( NULL == l_pReadDataBuffer  )
    {
      printf("Error : could not allocate memory to hold data\r\n");
    }
    else
    {
      char title[64];

      // read all bytes
      memset(l_pReadDataBuffer, 0, sizeof(l_pReadDataBuffer));
      l_nMemAddr = 0;
      for( i=0; i<l_nEepromTotalBytes; i++ )
      {
        l_nResult = EEPROM_readByte(&l_stEepromDev, l_nMemAddr, &l_pReadDataBuffer[i]);
        if (e_RETURNVALUE_Success != l_nResult)
        {
                  printf("%s: Error reading byte %d: %d\r\n", __FUNCTION__, i, l_nResult);
                  goto lbl_err;
        }
        l_nMemAddr++;
      }

      sprintf(title,"address 0-%d", l_nEepromTotalBytes - 1 );
      dump( title, 8, l_pReadDataBuffer, l_nEepromTotalBytes );
      lclDump2File( l_sOutputFile,  l_pReadDataBuffer, l_nEepromTotalBytes );
    }

    free( l_sOutputFile );
    free( l_pReadDataBuffer );
  }
  else if (NULL != l_sInputFile )
  {
    uint8_t * l_pWriteDataBuffer = NULL;
    uint32_t l_nFileSize;
    uint16_t l_nMemAddr;
    l_nResult = lclReadFile( l_sInputFile, &l_pWriteDataBuffer, &l_nFileSize);
    if ((e_RETURNVALUE_Success != l_nResult) || (l_nFileSize != l_nEepromTotalBytes) )
    {
        printf("%s: Error reading input file or not right size : %d, %d\r\n", __FUNCTION__, l_nResult, l_nFileSize );
        goto lbl_err;
    }
    l_nMemAddr = 0;
    for( uint32_t i=0; i < l_nFileSize; i++ )
    {
      l_nResult = EEPROM_writeByte( &l_stEepromDev, l_nMemAddr, l_pWriteDataBuffer[i] );
      if (e_RETURNVALUE_Success != l_nResult)
      {
        printf("%s: Error writing byte %d: %d\r\n", __FUNCTION__, i, l_nResult);
        goto lbl_err;
      }
      l_nMemAddr++;
    }


  }
#else
	uint16_t l_nMemAddr;
	uint8_t data;
	uint8_t rdata[128];
	uint16_t i;


	// clear first 64 bytes
	l_nMemAddr = 0;
	for(i=0;i<64;i++) {
		data = 0xff;
		EEPROM_writeByte(&l_stEepromDev, l_nMemAddr, data);
		l_nMemAddr++;
	}

	// write first 64 bytes
	l_nMemAddr = 0;
	for(i=0;i<64;i++) {
		data = i;
		EEPROM_writeByte(&l_stEepromDev, l_nMemAddr, data);
		l_nMemAddr++;
	}

	// read first 64 bytes
	memset(rdata, 0, sizeof(rdata));
	l_nMemAddr = 0;
	for(i=0;i<64;i++) {
		rdata[i] = EEPROM_readByte(&l_stEepromDev, l_nMemAddr);
		l_nMemAddr++;
	}
	char title[64];
	sprintf(title,"address 0-63");
	dump(title, 8, rdata, 64);

	// clear last 64 bytes
	l_nMemAddr = l_nEepromTotalBytes - 64;
	for(i=0;i<64;i++) {
		data = 0xff;
		EEPROM_writeByte(&l_stEepromDev, l_nMemAddr, data);
		l_nMemAddr++;
	}

	// write last 64 bytes
	l_nMemAddr = l_nEepromTotalBytes - 64;
	for(i=0;i<64;i++) {
		data = 0x3f - i;
		EEPROM_writeByte(&l_stEepromDev, l_nMemAddr, data);
		l_nMemAddr++;
	}

	// read last 64 bytes
	memset(rdata, 0, sizeof(rdata));
	l_nMemAddr = l_nEepromTotalBytes - 64;
	for(i=0;i<64;i++) {
		rdata[i] = EEPROM_readByte(&l_stEepromDev, l_nMemAddr);
		l_nMemAddr++;
	}
	sprintf(title,"address %d-%d", l_nEepromTotalBytes - 64, l_nEepromTotalBytes);
	dump(title, 8, rdata, 64);

	// write last page
	memset(rdata, 0xff, 128);
	EEPROM_writePage(&l_stEepromDev, lastPage, rdata);

	// read last 64 bytes
	memset(rdata, 0, sizeof(rdata));
	l_nMemAddr = l_nEepromTotalBytes - 64;
	for(i=0;i<64;i++) {
		rdata[i] = EEPROM_readByte(&l_stEepromDev, l_nMemAddr);
		l_nMemAddr++;
	}
	sprintf(title,"address %d-%d", l_nEepromTotalBytes - 64, l_nEepromTotalBytes);
	dump(title, 8, rdata, 64);
#endif

    goto lbl_done;
lbl_err:
    return (-1);
lbl_done:
    return (0);
}
