void KeyExpansion(bool show, int temp3[256], unsigned char Key[32]);
void encrypt(unsigned char temp[16], unsigned char temp2[16], int temp3[256], unsigned char P[4]);
void decrypt(unsigned char temp[16], unsigned char temp2[16], int temp3[256], unsigned char P[4]);
void modprod(unsigned char P1[4], unsigned char P2[4]);
bool verify(unsigned char P1[4], unsigned char P2[4]);


