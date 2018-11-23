#include "stdlib.h"
#include "string.h"
#include "Des.h"

static int IP[] = {
	58, 50, 42, 34, 26, 18, 10, 2,	60, 52, 44, 36, 28, 20, 12, 4,
	62, 54, 46, 38, 30, 22, 14, 6,	64, 56, 48, 40, 32, 24, 16, 8,
	57, 49, 41, 33, 25, 17, 9, 1,	59, 51, 43, 35, 27, 19, 11, 3,
	61, 53, 45, 37, 29, 21, 13, 5,	63, 55, 47, 39, 31, 23, 15, 7
};

static int IP_1[] = {
	40, 8, 48, 16, 56, 24, 64, 32,	39, 7, 47, 15, 55, 23, 63, 31,
	38, 6, 46, 14, 54, 22, 62, 30,	37, 5, 45, 13, 53, 21, 61, 29,
	36, 4, 44, 12, 52, 20, 60, 28,	35, 3, 43, 11, 51, 19, 59, 27,
	34, 2, 42, 10, 50, 18, 58, 26,	33, 1, 41, 9, 49, 17, 57, 25
};

static int PC_1[] = {
	57, 49, 41, 33, 25, 17, 9,	1, 58, 50, 42, 34, 26, 18,
	10, 2, 59, 51, 43, 35, 27,	19, 11, 3, 60, 52, 44, 36,
	63, 55, 47, 39, 31, 23, 15,	7, 62, 54, 46, 38, 30, 22,
	14, 6, 61, 53, 45, 37, 29,	21, 13, 5, 28, 20, 12, 4
};

static int PC_2[] = {
	14, 17, 11, 24, 1, 5,	3, 28, 15, 6, 21, 10,
	23, 19, 12, 4, 26, 8,	16, 7, 27, 20, 13, 2,
	41, 52, 31, 37, 47, 55,	30, 40, 51, 45, 33, 48,
	44, 49, 39, 56, 34, 53,	46, 42, 50, 36, 29, 32
};

static int shiftBits[] = { 1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1 };

static int E[] = {
	32, 1, 2, 3, 4, 5,	4, 5, 6, 7, 8, 9,
	8, 9, 10, 11, 12, 13,	12, 13, 14, 15, 16, 17,
	16, 17, 18, 19, 20, 21,	20, 21, 22, 23, 24, 25,
	24, 25, 26, 27, 28, 29,	28, 29, 30, 31, 32, 1
};

static int S_BOX[8][4][16] = {
	{
		{ 14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7 },
		{ 0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8 },
		{ 4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0 },
		{ 15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13 }
	},
	{
		{ 15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10 },
		{ 3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5 },
		{ 0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15 },
		{ 13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9 }
	},
	{
		{ 10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8 },
		{ 13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1 },
		{ 13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7 },
		{ 1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12 }
	},
	{
		{ 7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15 },
		{ 13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9 },
		{ 10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4 },
		{ 3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14 }
	},
	{
		{ 2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9 },
		{ 14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6 },
		{ 4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14 },
		{ 11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3 }
	},
	{
		{ 12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11 },
		{ 10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8 },
		{ 9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6 },
		{ 4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13 }
	},
	{
		{ 4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1 },
		{ 13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6 },
		{ 1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2 },
		{ 6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12 }
	},
	{
		{ 13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7 },
		{ 1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2 },
		{ 7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8 },
		{ 2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11 }
	}
};

static int P[] = {
	16, 7, 20, 21,	29, 12, 28, 17,
	1, 15, 23, 26,	5, 18, 31, 10,
	2, 8, 24, 14,	32, 27, 3, 9,
	19, 13, 30, 6,	22, 11, 4, 25
};


static void Des_edCryption(char input[8], char output[8], short mode);
static void make_SubKey(char key[8]);
static void leftMoveKey(unsigned short keyarray[56], int offset);
static void arrayCryption(unsigned short * data64, short mode, char * out);
static void LoopF(unsigned short data64[64], short time, short flag);
static void Bit64ToByte(unsigned short data64[64], unsigned char *out);


static void byteTo64Int(char data[8], unsigned short out[64])
{
	for (int i = 0; i < 8; ++i) {
		for (int j = 7; j >= 0; --j) {
			out[i * 8 + j] = data[i] >> (7 - j) & 1;
		}
	}
}

static void initKey(unsigned short key[64], unsigned short outkey[16][48])
{
	unsigned short int key56[56];
	for (int i = 0; i < 56; ++i) {
		key56[i] = key[PC_1[i] - 1];
	}
	for (int j = 0; j < 16; ++j) {
		leftMoveKey(key56, shiftBits[j]);
		for (int i = 0; i < 48; ++i) {
			outkey[j][i] = key56[PC_2[i] - 1];
		}
	}
}

static unsigned short keyset[16][48];

static void make_SubKey(char key[8])
{
	unsigned short key64[64];
	byteTo64Int(key, key64);
	initKey(key64, keyset);
}

static void Des_edCryption(char * input, char * output, short mode)
{
	unsigned short data64[64];
	memset(output, 0, 8);
	byteTo64Int(input, data64);
	arrayCryption(data64, mode, output);
}

static void arrayCryption(unsigned short * data64, short mode, char * out)
{
	unsigned short ipdata[64];
	for (int i = 0; i < 64; ++i) {
		ipdata[i] = data64[IP[i] - 1];
	}
	for (int j = 0; j < 16; ++j) {
		LoopF(ipdata, (mode ? j : 15 - j), mode);
	}
	for (int k = 0; k < 64; ++k) {
		data64[k] = ipdata[IP_1[k] - 1];
	}
	Bit64ToByte(data64, (unsigned char*)out);
}

static void Bit64ToByte(unsigned short data64[64], unsigned char *out)
{
	for (int i = 0; i < 8; ++i) {
		for (int j = 0; j < 8; ++j) {
			out[i] |= data64[i * 8 + j] << (7 - j);
		}
	}
}


static void LoopF(unsigned short data64[64], short time, short flag)
{
	unsigned short tmp = 0;
	unsigned short R[32];
	unsigned short RE[48];
	unsigned short sValue[32];
	int i;

	for (int m = 0; m < 32; ++m) {
		R[m] = data64[32 + m];
	}
	for (i = 0; i < 48; ++i) {
		RE[i] = R[E[i] - 1] ^ keyset[time][i];
	}
	for (int j = 0; j < 8; ++j) {
		tmp = S_BOX[j][(RE[j * 6] << 1) | (RE[j * 6 + 5])][(RE[j * 6 + 1] << 3) | (RE[j * 6 + 2] << 2) | (RE[j * 6 + 3] << 1) | RE[j * 6 + 4]];
		for (int k = 0; k < 4; ++k) {
			sValue[j * 4 + 3 - k] = (tmp >> k) & 1;
		}
	}
	for (i = 0; i < 32; i++) {
		tmp = sValue[P[i] - 1] ^ data64[i];
		if ((!flag &&!time) || (flag && time == 15)) {
			data64[i] = tmp;
		}
		else {
			data64[i] = R[i];
			data64[i + 32] = tmp;
		}
	}
}

static void leftMoveKey(unsigned short keyarray[56], int offset)
{
	short temp1 = keyarray[0];
	short temp2 = keyarray[28];
	short temp3 = keyarray[1];
	short temp4 = keyarray[29];
	for (int i = 0; i < 26; ++i) {
		keyarray[i] = keyarray[offset + i];
		keyarray[i + 28] = keyarray[offset + 28 + i];
	}
	if (offset == 1)
	{
		keyarray[26] = keyarray[27];
		keyarray[27] = temp1;
		keyarray[54] = keyarray[55];
		keyarray[55] = temp2;
	}
	else{
		keyarray[26] = temp1;
		keyarray[27] = temp3;
		keyarray[54] = temp2;
		keyarray[55] = temp4;
	}
}

static char* ByteDataFormat(char* data, int len, int *piLen, int flag)
{
	int padlen = 8 - (len % 8);
	int newlen = len + padlen;
	char* newdata = (char*)malloc(newlen);
	memset(newdata, 0x00, newlen);
	memcpy(newdata, data, len);

	for (int i = len; i < newlen; i++) {
		newdata[i] = (char)padlen;
	}
	*piLen = newlen;
	return newdata;
}

static char* DES_CODE(char *sourceData, int sourceSize, char *keyStr, int *resultSize, int flag)
{
	int format_keyLen = 0;
	char* format_key = ByteDataFormat(keyStr, strlen(keyStr), &format_keyLen, flag);
	int format_dataLen = 0;
	char* format_data = ByteDataFormat(sourceData, sourceSize, &format_dataLen, flag);
	int datalen = format_dataLen;
	int unitcount = datalen / 8;
	char* result_data = (char*)malloc(datalen);
	memset(result_data, 0x00, datalen);

	make_SubKey(format_key);

	for (int i = 0; i < unitcount; i++) {
		Des_edCryption(format_data + i * 8, result_data + i * 8, flag);
	}

	if (flag == 0) {
		int total_len = datalen;
		int delete_len = result_data[total_len - 8 - 1];
		delete_len = ((delete_len >= 1) && (delete_len <= 8)) ? delete_len : 0;

		bool del_flag = true;
		for (int k = 0; k < delete_len; k++) {
			if (delete_len != result_data[total_len - 8 - (k + 1)]){
				del_flag = false;
			}
		}

		if (del_flag == true) {
			datalen = total_len - delete_len - 8;
			result_data[datalen] = 0; //ÖØÖÃ0
		}
	}

	free(format_key);
	free(format_data);

	*resultSize = datalen;
	return result_data;
}

char* DES_Encrypt(char *sourceData, int sourceSize, char *keyStr, int *resultSize)
{
	return DES_CODE(sourceData, sourceSize, keyStr, resultSize, 1);
}

char* DES_Decrypt(char *sourceData, int sourceSize, char *keyStr, int* resultSize)
{
	return DES_CODE(sourceData, sourceSize, keyStr, resultSize, 0);
}
