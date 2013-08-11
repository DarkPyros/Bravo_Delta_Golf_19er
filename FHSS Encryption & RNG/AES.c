
#include <msp430.h>
#include "AES.h"


unsigned char AES_setCipherKey (unsigned int baseAddress,
	 const unsigned char * CipherKey
	 )
{
	unsigned char i = 0;
	unsigned int tempVariable = 0;

	// Wait until AES accelerator is busy
	while(AESBUSY == (HWREG(baseAddress + OFS_AESASTAT) & AESBUSY) );

	for (i = 0; i < 16; i = i + 2)
	{
		//HWREG(baseAddress + OFS_AESAKEY) = ( unsigned int)(( unsigned int)CipherKey[i] | ( unsigned int) (CipherKey[i + 1] << 8));
		tempVariable = (unsigned int)(CipherKey[i]);
		tempVariable = tempVariable | ((unsigned int)(CipherKey[i + 1]) << 8);
		HWREG(baseAddress + OFS_AESAKEY) = tempVariable;
	}

    // Wait until key is written
	while(0x00 == (HWREG(baseAddress + OFS_AESASTAT) & AESKEYWR ));

    return STATUS_SUCCESS;
}

unsigned char AES_encryptData (unsigned int baseAddress,
	const unsigned char * Data,
	unsigned char * encryptedData)
{
	unsigned char i;
	unsigned int tempData = 0;
	unsigned int tempVariable = 0;

	// Set module to encrypt mode
	HWREG(baseAddress + OFS_AESACTL0) &= ~AESOP_3;


	// Write data to encrypt to module
	for (i = 0; i < 16; i = i + 2)
	{
		//HWREG(baseAddress + OFS_AESADIN) = ( unsigned int)(( unsigned int)Data[i] | ( unsigned int) (Data[i + 1] << 8));
		tempVariable = (unsigned int)(Data[i]);
		tempVariable = tempVariable | ((unsigned int)(Data[i+1]) << 8);
		HWREG(baseAddress + OFS_AESADIN) = tempVariable;
	}

	// Key that is already written shall be used
	// Encryption is initialized by setting AESKEYWR to 1
	HWREG(baseAddress + OFS_AESASTAT) |= AESKEYWR;

	// Wait unit finished ~167 MCLK
	while(AESBUSY == (HWREG(baseAddress + OFS_AESASTAT) & AESBUSY) );

	// Write encrypted data back to variable
	for (i = 0; i < 16; i = i + 2)
	{
		tempData = HWREG(baseAddress + OFS_AESADOUT);
		*(encryptedData + i) = (unsigned char)tempData;
		*(encryptedData +i + 1) = (unsigned char)(tempData >> 8);


	}

    return STATUS_SUCCESS;
}

unsigned char AES_decryptDataUsingEncryptionKey (unsigned int baseAddress,
	const unsigned char * Data,
	unsigned char * decryptedData)
{
	unsigned char i;
	unsigned int tempData = 0;
	unsigned int tempVariable = 0;

	// Set module to decrypt mode
	HWREG(baseAddress + OFS_AESACTL0) &= ~(AESOP1);
	HWREG(baseAddress + OFS_AESACTL0) |= AESOP0;

	// Write data to decrypt to module
	for (i = 0; i < 16; i = i + 2)
	{
		//HWREG(baseAddress + OFS_AESADIN) = ( unsigned int)(( unsigned int)Data[i] | ( unsigned int) (Data[i + 1] << 8));
		tempVariable = (unsigned int)(Data[i+1]  << 8);
		tempVariable = tempVariable | ((unsigned int)(Data[i]));
		HWREG(baseAddress + OFS_AESADIN) = tempVariable;
	}

	// Key that is already written shall be used
	// Now decryption starts
	HWREG(baseAddress + OFS_AESASTAT) |= AESKEYWR;

	// Wait unit finished ~214 MCLK
	while(AESBUSY == (HWREG(baseAddress + OFS_AESASTAT) & AESBUSY) );

	// Write encrypted data back to variable
	for (i = 0; i < 16; i = i + 2)
	{
		tempData = HWREG(baseAddress + OFS_AESADOUT);
		*(decryptedData + i) = (unsigned char)tempData;
		*(decryptedData +i+1) = (unsigned char)(tempData >> 8);
	}

	return STATUS_SUCCESS;
}

unsigned char AES_encryptDataBlock(unsigned int baseAddress,
        unsigned char * Data,
        unsigned char * encryptedData,
        unsigned char * IV,
        unsigned int Length )
{
    // crypto operations must be carried out locally in case we are encrypting in-place
    unsigned char inbuff[16], outbuff[16];

    char i;

    if(Length % 16)
        return STATUS_FAIL;

    while(Length)
    {
        // apply IV to input data
        for(i= 0 ; i < 16 ; ++i)
        	inbuff[i]= Data[i] ^ IV[i];

        // do the encryption
        AES_encryptData(baseAddress, inbuff, outbuff);

        // maintain IV & feedback
        for(i= 0 ; i < 16 ; ++i)
        	IV[i]= outbuff[i];

        // copy encrypted data to final output
        for(i= 0 ; i < 16 ; ++i)
            encryptedData[i]= outbuff[i];

        Data += 16;
        encryptedData += 16;
        Length -= 16;
    }

    return STATUS_SUCCESS;
}

unsigned char AES_decryptDataBlock(unsigned int baseAddress,
        unsigned char * Data,
        unsigned char * decryptedData,
        unsigned char * IV,
        unsigned int Length,
        const unsigned char Mode)
{
    // crypto operations must be carried out locally in case we are encrypting in-place
    unsigned char inbuff[16], outbuff[16];

    char i;

    if(Length % 16)
        return STATUS_FAIL;

    while(Length)
    {
        // apply IV to input data
        for(i= 0 ; i < 16 ; ++i)
        	inbuff[i]= Data[i];

        // do the 'decryption'
        AES_decryptData(baseAddress, inbuff, outbuff);


        // maintain IV & feedback
        for(i= 0 ; i < 16 ; ++i)
        {
                    outbuff[i] ^= IV[i];
                    IV[i]= Data[i];
        }
        for(i= 0 ; i < 16 ; ++i)
            decryptedData[i]= outbuff[i];

        Data += 16;
        decryptedData += 16;
        Length -= 16;
    }

    return STATUS_SUCCESS;
}

unsigned char AES_decryptData (unsigned int baseAddress,
	const unsigned char * Data,
	unsigned char * decryptedData)
{
	unsigned char i;
	unsigned int tempData = 0;
	unsigned int tempVariable = 0;

	// Set module to decrypt mode
	HWREG(baseAddress + OFS_AESACTL0) |= (AESOP_3);

	// Write data to decrypt to module
	for (i = 0; i < 16; i = i + 2)
	{
		tempVariable = (unsigned int)(Data[i+1]  << 8);
		tempVariable = tempVariable | ((unsigned int)(Data[i]));
		HWREG(baseAddress + OFS_AESADIN) = tempVariable;
	}

	// Key that is already written shall be used
	// Now decryption starts
	HWREG(baseAddress + OFS_AESASTAT) |= AESKEYWR;

	// Wait unit finished ~167 MCLK
	while(AESBUSY == (HWREG(baseAddress + OFS_AESASTAT) & AESBUSY ));

	// Write encrypted data back to variable
	for (i = 0; i < 16; i = i + 2)
	{
		tempData = HWREG(baseAddress + OFS_AESADOUT);
		*(decryptedData + i) = (unsigned char)tempData;
		*(decryptedData +i + 1) = (unsigned char)(tempData >> 8);
	}

	return STATUS_SUCCESS;
}

unsigned char AES_generateFirstRoundKey (unsigned int baseAddress,
	const unsigned char * CipherKey)
{
	unsigned char i;
	unsigned int tempVariable = 0;

	// Set module to decrypt mode
	HWREG(baseAddress + OFS_AESACTL0) &= ~(AESOP0);
	HWREG(baseAddress + OFS_AESACTL0) |= AESOP1;


	// Write cipher key to key register
	for (i = 0; i < 16; i = i + 2)
	{
		tempVariable = (unsigned int)(CipherKey[i]);
		tempVariable = tempVariable | ((unsigned int)(CipherKey[i + 1]) << 8);
		HWREG(baseAddress + OFS_AESAKEY) = tempVariable;
	}

	// Wait until key is processed ~52 MCLK
	while((HWREG(baseAddress + OFS_AESASTAT) & AESBUSY) == AESBUSY);

	return STATUS_SUCCESS;
}
