#ifndef AES_H_
#define AES_H_

#define HWREG(x)     	(*((volatile unsigned int *)(x)))

#define STATUS_SUCCESS  0x01
#define STATUS_FAIL     0x00

#define OFS_AESACTL0           (0x0000)       /* AES accelerator control register 0 */
#define OFS_AESACTL0_L         OFS_AESACTL0
#define OFS_AESACTL0_H         OFS_AESACTL0+1
#define OFS_AESASTAT           (0x0004)       /* AES accelerator status register */
#define OFS_AESASTAT_L         OFS_AESASTAT
#define OFS_AESASTAT_H         OFS_AESASTAT+1
#define OFS_AESAKEY            (0x0006)       /* AES accelerator key register */
#define OFS_AESAKEY_L          OFS_AESAKEY
#define OFS_AESAKEY_H          OFS_AESAKEY+1
#define OFS_AESADIN            (0x0008)       /* AES accelerator data in register */
#define OFS_AESADIN_L          OFS_AESADIN
#define OFS_AESADIN_H          OFS_AESADIN+1
#define OFS_AESADOUT           (0x000A)       /* AES accelerator data out register  */
#define OFS_AESADOUT_L         OFS_AESADOUT
#define OFS_AESADOUT_H         OFS_AESADOUT+1

typedef struct {
	int year;
	char month;
	char day;
	char hour;
	char minute;
	char second;
	char seed_channel;
	unsigned long long counter;
} NONCE;

unsigned char AES_setCipherKey (unsigned int,const unsigned char *);
unsigned char AES_encryptData (unsigned int,const unsigned char *,unsigned char *);
unsigned char AES_decryptDataUsingEncryptionKey (unsigned int,const unsigned char *, unsigned char *);

#endif /* AES_H_ */
