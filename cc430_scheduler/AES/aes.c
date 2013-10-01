#include "aes.h"

static tByte Cipher_Key[AES_SIZE] = { 0x2b, 0x7e, 0x15, 0x16,
									  0x28, 0xae, 0xd2, 0xa6,
									  0xab, 0xf7, 0x15, 0x88,
									  0x09, 0xcf, 0x4f, 0x3c };

void AES_Init (void)
{
	//LED_ON;
	// 12.8 us
	AES_setCipherKey(AES_ADDR, Cipher_Key);
	//LED_OFF;
}

tByte AES_setCipherKey (tWord const baseAddress,
	 tByte const * const CipherKey
	 )
{
	tByte i = 0;
	tWord tempVariable = 0;

	// Wait until AES accelerator is not busy
	while(AESBUSY == (HWREG(baseAddress + OFS_AESASTAT) & AESBUSY) );

	for (i = 0; i < 16; i = i + 2)
	{
		//HWREG(baseAddress + OFS_AESAKEY) = ( unsigned int)(( unsigned int)CipherKey[i] | ( unsigned int) (CipherKey[i + 1] << 8));
		tempVariable = (tWord)(CipherKey[i]);
		tempVariable = tempVariable | ((tWord)(CipherKey[i + 1]) << 8);
		HWREG(baseAddress + OFS_AESAKEY) = tempVariable;
	}

    // Wait until key is written
	while(0x00 == (HWREG(baseAddress + OFS_AESASTAT) & AESKEYWR ));

    return STATUS_SUCCESS;
}

tByte AES_Encrypt_Data (tWord const baseAddress,
	tByte const * const Data,
	tByte * const encryptedData)
{
	tByte i;
	tWord tempData = 0;
	tWord tempVariable = 0;

	// Wait until AES accelerator is not busy
	while(AESBUSY == (HWREG(baseAddress + OFS_AESASTAT) & AESBUSY) );

	// Set module to encrypt mode
	HWREG(baseAddress + OFS_AESACTL0) &= ~AESOP_3;


	// Write data to encrypt to module
	for (i = 0; i < AES_SIZE; i = i + 2)
	{
		//HWREG(baseAddress + OFS_AESADIN) = ( unsigned int)(( unsigned int)Data[i] | ( unsigned int) (Data[i + 1] << 8));
		tempVariable = (tWord)(Data[i]);
		tempVariable = tempVariable | ((tWord)(Data[i+1]) << 8);
		HWREG(baseAddress + OFS_AESADIN) = tempVariable;
	}

	// Key that is already written shall be used
	// Encryption is initialized by setting AESKEYWR to 1
	HWREG(baseAddress + OFS_AESASTAT) |= AESKEYWR;

	// Wait unit finished ~167 MCLK
	while(AESBUSY == (HWREG(baseAddress + OFS_AESASTAT) & AESBUSY) );

	// Write encrypted data back to variable
	for (i = 0; i < AES_SIZE; i = i + 2)
	{
		tempData = HWREG(baseAddress + OFS_AESADOUT);
		*(encryptedData + i) = (tByte)tempData;
		*(encryptedData + i + 1) = (tByte)(tempData >> 8);
	}

    return STATUS_SUCCESS;
}

tByte AES_decryptDataUsingEncryptionKey (tWord const baseAddress,
	tByte const * const Data,
	tByte * const decryptedData)
{
	tByte i;
	tWord tempData = 0;
	tWord tempVariable = 0;

	// Set module to decrypt mode
	HWREG(baseAddress + OFS_AESACTL0) &= ~(AESOP1);
	HWREG(baseAddress + OFS_AESACTL0) |= AESOP0;

	// Write data to decrypt to module
	for (i = 0; i < AES_SIZE; i = i + 2)
	{
		//HWREG(baseAddress + OFS_AESADIN) = ( unsigned int)(( unsigned int)Data[i] | ( unsigned int) (Data[i + 1] << 8));
		tempVariable = (tWord)(Data[i+1]  << 8);
		tempVariable = tempVariable | ((tWord)(Data[i]));
		HWREG(baseAddress + OFS_AESADIN) = tempVariable;
	}

	// Key that is already written shall be used
	// Now decryption starts
	HWREG(baseAddress + OFS_AESASTAT) |= AESKEYWR;

	// Wait unit finished ~214 MCLK
	while(AESBUSY == (HWREG(baseAddress + OFS_AESASTAT) & AESBUSY) );

	// Write encrypted data back to variable
	for (i = 0; i < AES_SIZE; i = i + 2)
	{
		tempData = HWREG(baseAddress + OFS_AESADOUT);
		*(decryptedData + i) = (tByte)tempData;
		*(decryptedData + i + 1) = (tByte)(tempData >> 8);
	}

	return STATUS_SUCCESS;
}
