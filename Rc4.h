#ifndef RC4_H_
#define RC4_H_

struct rc4_state {
	unsigned char	perm[256];
	unsigned char	index1;
	unsigned char	index2;
};

void rc4_init(struct rc4_state *state, const unsigned char *key, int keylen);
void rc4_crypt(struct rc4_state *state, const unsigned char *inbuf, unsigned char *outbuf, int buflen);

#endif
