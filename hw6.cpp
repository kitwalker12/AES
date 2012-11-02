#include <stdio.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include "aes.h"

using namespace std;

bool sCheck(int sbox[256]) {
	int sum=0;
	for(int i=0;i<256;i++) {
		sum+=sbox[i];
	}
	if(sum==0x7f80) return true;
	return false;
}
int main(int argc, char *argv[])
{
	//key
	//unsigned char temp[16] = {0x00  ,0x01  ,0x02  ,0x03  ,0x04  ,0x05  ,0x06  ,0x07  ,0x08  ,0x09  ,0x0a  ,0x0b  ,0x0c  ,0x0d  ,0x0e  ,0x0f};
	//unsigned char temp[16] = {0x2b,0x7e,0x15,0x16,0x28,0xae,0xd2,0xa6,0xab,0xf7,0x15,0x88,0x09,0xcf,0x4f,0x3c};
	//input
	unsigned char temp2[16]= {0x00  ,0x11  ,0x22  ,0x33  ,0x44  ,0x55  ,0x66  ,0x77  ,0x88  ,0x99  ,0xaa  ,0xbb  ,0xcc  ,0xdd  ,0xee  ,0xff};
	int temp3[256];
	unsigned char P[4],INVP[4];
	int result=0;
	if(argc==3) {
		if(strcmp(argv[1],"tablecheck")==0) {
			string option,param,temp;
			FILE *tf;
			temp=argv[2];
			option=temp.substr(0,3);
			if(option.compare("-t=")==0) {
				param=temp.substr(3,temp.length());
				if(param.compare("")==0) {
					fputs("Malformed Command\n",stderr);
					exit(1);
				}
				tf=fopen(param.c_str(),"rb");
				if(tf==NULL) {fputs("Error in opening table file\n",stderr);exit(1);}
				//int result=0;
				unsigned char *buff=new unsigned char;
				int i=0;
				while(true) {
					result=fread(buff,1,1,tf);
					if(result==0) {
						break;
					}
					if(*buff=='S') continue;
					if(*buff=='=') continue;
					if(i==256) break;
					unsigned char tmp=*buff;
					result=fread(buff,1,1,tf);
					if(result==0) {
						break;
					}
					unsigned char tmp2=*buff;
					if(tmp>='0' && tmp<='9') {
						tmp-='0';
					} else
					if(tmp>='a' && tmp<='f') {
						tmp-=87;
					} else {fputs("Invalid value\n",stderr);exit(1);}
					if(tmp2>='0' && tmp2<='9') {
						tmp2-='0';
					} else
					if(tmp2>='a' && tmp2<='f') {
						tmp2-=87;
					} else {fputs("Invalid value\n",stderr);exit(1);}
					temp3[i++]=(tmp*0x10)+tmp2;	//sbox
					//printf("%02x",temp3[i-1]);
				}
				if(i!=256 || !sCheck(temp3)) {
					fputs("Tablecheck Failed\n",stderr);exit(1);
				}
				//printf("\n");
				i=0;
				while(true) {
					result=fread(buff,1,1,tf);
					if(result==0) {
						break;
					}
					if(*buff=='P') continue;
					if(*buff=='=') continue;
					if(i==4) break;
					unsigned char tmp=*buff;
					result=fread(buff,1,1,tf);
					if(result==0) {
						break;
					}
					unsigned char tmp2=*buff;
					if(tmp>='0' && tmp<='9') {
						tmp-='0';
					} else
					if(tmp>='a' && tmp<='f') {
						tmp-=87;
					} else {fputs("Invalid value\n",stderr);exit(1);}
					if(tmp2>='0' && tmp2<='9') {
						tmp2-='0';
					} else
					if(tmp2>='a' && tmp2<='f') {
						tmp2-=87;
					} else {fputs("Invalid value\n",stderr);exit(1);}
					P[i++]=(tmp*0x10)+tmp2;	//P
					//printf("%02x",P[i-1]);
				}
				if(i!=4) {
					fputs("Tablecheck Failed\n",stderr);exit(1);
				}
				i=0;
				while(true) {
					result=fread(buff,1,1,tf);
					if(result==0) {
						break;
					}
					if(*buff=='I') continue;
					if(*buff=='N') continue;
					if(*buff=='V') continue;
					if(*buff=='P') continue;
					if(*buff=='=') continue;
					if(i==4) break;
					unsigned char tmp=*buff;
					result=fread(buff,1,1,tf);
					if(result==0) {
						break;
					}
					unsigned char tmp2=*buff;
					if(tmp>='0' && tmp<='9') {
						tmp-='0';
					} else
					if(tmp>='a' && tmp<='f') {
						tmp-=87;
					} else {fputs("Invalid value\n",stderr);exit(1);}
					if(tmp2>='0' && tmp2<='9') {
						tmp2-='0';
					} else
					if(tmp2>='a' && tmp2<='f') {
						tmp2-=87;
					} else {fputs("Invalid value\n",stderr);exit(1);}
					INVP[i++]=(tmp*0x10)+tmp2;	//P
					//printf("%02x",P[i-1]);
				}
				if(i!=4) {
					fputs("Tablecheck Failed\n",stderr);exit(1);
				}
				if(!verify(P,INVP)) {
					fputs("Tablecheck Failed\n",stderr);exit(1);
				}
				return 0;
			}
		}//tablecheck
		if(strcmp(argv[1],"inverse")==0) {
			string option,param,temp;
			char *p;
			unsigned char P[4];
			temp=argv[2];
			option=temp.substr(0,3);
			if(option.compare("-p=")==0) {
				param=temp.substr(3,temp.length());
				if(param.compare("")==0) {
					fputs("Malformed Command\n",stderr);
					exit(1);
				}
				if(param.length()!=8){
					fputs("Invalid Polynomial length\n",stderr);
					exit(1);
				}
				p = new char[param.length()];
				strcpy(p,param.c_str());
				int j=0;
				for(int i=0;i<8;i+=2) {
					unsigned char tmp=(unsigned char)p[i];
					unsigned char tmp2=(unsigned char)p[i+1];
					if(p[i]>='0' && p[i]<='9') {
						tmp-='0';
					} else
					if(p[i]>='a' && p[i]<='f') {
						tmp-=87;
					} else {fputs("Invalid value\n",stderr);exit(1);}
					if(p[i+1]>='0' && p[i+1]<='9') {
						tmp2-='0';
					} else
					if(p[i+1]>='a' && p[i+1]<='f') {
						tmp2-=87;
					} else {fputs("Invalid value\n",stderr);exit(1);}
					P[j++]=(tmp*0x10)+tmp2;
				}
				return 0;
			}//-p
		}//inverse
	}//argc==3
	if(argc==4 || argc==5) {
		if(strcmp(argv[1],"keyexpand")==0) {
			string option,param,temp;
			char *key;
			FILE *tf;
			temp=argv[2];
			option=temp.substr(0,3);
			if(option.compare("-k=")==0) {
				param=temp.substr(3,temp.length());
				if(param.compare("")==0) {
					fputs("Malformed Command\n",stderr);
					exit(1);
				}
				if(param.length()!=32){
					fputs("Invalid Key length\n",stderr);
					exit(1);
				}
				key = new char[param.length()];
				strcpy(key,param.c_str());
				unsigned char Key[32];
				int j=0;
				for(int i=0;i<32;i+=2) {
					unsigned char tmp=(unsigned char)key[i];
					unsigned char tmp2=(unsigned char)key[i+1];
					if(key[i]>='0' && key[i]<='9') {
						tmp-='0';
					} else
					if(key[i]>='a' && key[i]<='f') {
						tmp-=87;
					} else {fputs("Invalid value\n",stderr);exit(1);}
					if(key[i+1]>='0' && key[i+1]<='9') {
						tmp2-='0';
					} else
					if(key[i+1]>='a' && key[i+1]<='f') {
						tmp2-=87;
					} else {fputs("Invalid value\n",stderr);exit(1);}
					Key[j++]=(tmp*0x10)+tmp2;
				}
				temp=argv[3];
				option=temp.substr(0,3);
				if(option.compare("-t=")==0) {
					param=temp.substr(3,temp.length());
					if(param.compare("")==0) {
						fputs("Malformed Command\n",stderr);
						exit(1);
					}
					tf=fopen(param.c_str(),"rb");
					if(tf==NULL) {fputs("Error in opening table file\n",stderr);exit(1);}
					//int result=0;
					unsigned char *buff=new unsigned char;
					int i=0;
					while(true) {
						result=fread(buff,1,1,tf);
						if(result==0) {
							break;
						}
						if(*buff=='S') continue;
						if(*buff=='=') continue;
						if(i==256) break;
						unsigned char tmp=*buff;
						result=fread(buff,1,1,tf);
						if(result==0) {
							break;
						}
						unsigned char tmp2=*buff;
						if(tmp>='0' && tmp<='9') {
							tmp-='0';
						} else
						if(tmp>='a' && tmp<='f') {
							tmp-=87;
						} else {fputs("Invalid value\n",stderr);exit(1);}
						if(tmp2>='0' && tmp2<='9') {
							tmp2-='0';
						} else
						if(tmp2>='a' && tmp2<='f') {
							tmp2-=87;
						} else {fputs("Invalid value\n",stderr);exit(1);}
						temp3[i++]=(tmp*0x10)+tmp2;
						//printf("%02x",temp3[i-1]);
					}
					if(i!=256 || !sCheck(temp3)) {
						fputs("Tablecheck failed\n",stderr);exit(1);
					}
					KeyExpansion(true,temp3,Key);
					return 0;
				}
			}
		}//keyexpand
		if(strcmp(argv[1],"encrypt")==0) {
			string option,param,temp;
			char *key;
			FILE *tf,*f;
			temp=argv[2];
			option=temp.substr(0,3);
			if(option.compare("-k=")==0) {
				param=temp.substr(3,temp.length());
				if(param.compare("")==0) {
					fputs("Malformed Command\n",stderr);
					exit(1);
				}
				if(param.length()!=32){
					fputs("Invalid Key length\n",stderr);
					exit(1);
				}
				key = new char[param.length()];
				strcpy(key,param.c_str());
				unsigned char Key[32];
				int j=0;
				for(int i=0;i<32;i+=2) {
					unsigned char tmp=(unsigned char)key[i];
					unsigned char tmp2=(unsigned char)key[i+1];
					if(key[i]>='0' && key[i]<='9') {
						tmp-='0';
					} else
					if(key[i]>='a' && key[i]<='f') {
						tmp-=87;
					} else {fputs("Invalid value\n",stderr);exit(1);}
					if(key[i+1]>='0' && key[i+1]<='9') {
						tmp2-='0';
					} else
					if(key[i+1]>='a' && key[i+1]<='f') {
						tmp2-=87;
					} else {fputs("Invalid value\n",stderr);exit(1);}
					Key[j++]=(tmp*0x10)+tmp2;
				}
				temp=argv[3];
				option=temp.substr(0,3);
				if(option.compare("-t=")==0) {
					param=temp.substr(3,temp.length());
					if(param.compare("")==0) {
						fputs("Malformed Command\n",stderr);
						exit(1);
					}
					tf=fopen(param.c_str(),"rb");
					if(tf==NULL) {fputs("Error in opening table file\n",stderr);exit(1);}
					//int result=0;
					unsigned char *buff=new unsigned char;
					int i=0;
					while(true) {
						result=fread(buff,1,1,tf);
						if(result==0) {
							break;
						}
						if(*buff=='S') continue;
						if(*buff=='=') continue;
						if(i==256) break;
						unsigned char tmp=*buff;
						result=fread(buff,1,1,tf);
						if(result==0) {
							break;
						}
						unsigned char tmp2=*buff;
						if(tmp>='0' && tmp<='9') {
							tmp-='0';
						} else
						if(tmp>='a' && tmp<='f') {
							tmp-=87;
						} else {fputs("Invalid value\n",stderr);exit(1);}
						if(tmp2>='0' && tmp2<='9') {
							tmp2-='0';
						} else
						if(tmp2>='a' && tmp2<='f') {
							tmp2-=87;
						} else {fputs("Invalid value\n",stderr);exit(1);}
						temp3[i++]=(tmp*0x10)+tmp2;	//sbox
						//printf("%02x",temp3[i-1]);
					}
					if(i!=256 || !sCheck(temp3)) {
						fputs("Tablecheck Failed\n",stderr);exit(1);
					}
					//printf("\n");
					i=0;
					while(true) {
						result=fread(buff,1,1,tf);
						if(result==0) {
							break;
						}
						if(*buff=='P') continue;
						if(*buff=='=') continue;
						if(i==4) break;
						unsigned char tmp=*buff;
						result=fread(buff,1,1,tf);
						if(result==0) {
							break;
						}
						unsigned char tmp2=*buff;
						if(tmp>='0' && tmp<='9') {
							tmp-='0';
						} else
						if(tmp>='a' && tmp<='f') {
							tmp-=87;
						} else {fputs("Invalid value\n",stderr);exit(1);}
						if(tmp2>='0' && tmp2<='9') {
							tmp2-='0';
						} else
						if(tmp2>='a' && tmp2<='f') {
							tmp2-=87;
						} else {fputs("Invalid value\n",stderr);exit(1);}
						P[i++]=(tmp*0x10)+tmp2;	//P
						//printf("%02x",P[i-1]);
					}
					if(i!=4) {
						fputs("Tablecheck Failed\n",stderr);exit(1);
					}
					//printf("\n");
					if(argc==5) {
						f=fopen(argv[4],"rb");
						if(f==NULL) {fputs("Error in opening plaintext file\n",stderr);exit(1);}
					}
					else {
						f=stdin;
					}
					i=0;
					while(true) {
						result=fread(buff,1,1,f);
						if(result==0) {
							break;
						}
						if(i==16) break;
						temp2[i++]=*buff;
						//printf("%02x",*buff);
					}
					if(i<16) {
						fputs("Input file too small\n",stderr);exit(1);
					}
					encrypt(Key,temp2,temp3,P);
					return 0;
				}
			}
		}//encrypt
		if(strcmp(argv[1],"decrypt")==0) {
			string option,param,temp;
			char *key;
			FILE *tf,*f;
			temp=argv[2];
			option=temp.substr(0,3);
			if(option.compare("-k=")==0) {
				param=temp.substr(3,temp.length());
				if(param.compare("")==0) {
					fputs("Malformed Command\n",stderr);
					exit(1);
				}
				if(param.length()!=32){
					fputs("Invalid Key length\n",stderr);
					exit(1);
				}
				key = new char[param.length()];
				strcpy(key,param.c_str());
				unsigned char Key[32];
				int j=0;
				for(int i=0;i<32;i+=2) {
					unsigned char tmp=(unsigned char)key[i];
					unsigned char tmp2=(unsigned char)key[i+1];
					if(key[i]>='0' && key[i]<='9') {
						tmp-='0';
					} else
					if(key[i]>='a' && key[i]<='f') {
						tmp-=87;
					} else {fputs("Invalid value\n",stderr);exit(1);}
					if(key[i+1]>='0' && key[i+1]<='9') {
						tmp2-='0';
					} else
					if(key[i+1]>='a' && key[i+1]<='f') {
						tmp2-=87;
					} else {fputs("Invalid value\n",stderr);exit(1);}
					Key[j++]=(tmp*0x10)+tmp2;
				}
				temp=argv[3];
				option=temp.substr(0,3);
				if(option.compare("-t=")==0) {
					param=temp.substr(3,temp.length());
					if(param.compare("")==0) {
						fputs("Malformed Command\n",stderr);
						exit(1);
					}
					tf=fopen(param.c_str(),"rb");
					if(tf==NULL) {fputs("Error in opening table file\n",stderr);exit(1);}
					//int result=0;
					unsigned char *buff=new unsigned char;
					int i=0;
					while(true) {
						result=fread(buff,1,1,tf);
						if(result==0) {
							break;
						}
						if(*buff=='S') continue;
						if(*buff=='=') continue;
						if(i==256) break;
						unsigned char tmp=*buff;
						result=fread(buff,1,1,tf);
						if(result==0) {
							break;
						}
						unsigned char tmp2=*buff;
						if(tmp>='0' && tmp<='9') {
							tmp-='0';
						} else 
						if(tmp>='a' && tmp<='f') {
							tmp-=87;
						} else {fputs("Invalid value\n",stderr);exit(1);}
						if(tmp2>='0' && tmp2<='9') {
							tmp2-='0';
						} else
						if(tmp2>='a' && tmp2<='f') {
							tmp2-=87;
						} else {fputs("Invalid value\n",stderr);exit(1);}
						temp3[i++]=(tmp*0x10)+tmp2;	//sbox
						//printf("%02x",temp3[i-1]);
					}
					if(i!=256 || !sCheck(temp3)) {
						fputs("Tablecheck Failed\n",stderr);exit(1);
					}
					//printf("\n");
					i=0;
					while(true) {
						result=fread(buff,1,1,tf);
						if(result==0) {
							break;
						}
						if(*buff=='P') continue;
						if(*buff=='=') continue;
						if(i==4) break;
						unsigned char tmp=*buff;
						result=fread(buff,1,1,tf);
						if(result==0) {
							break;
						}
						unsigned char tmp2=*buff;
						if(tmp>='0' && tmp<='9') {
							tmp-='0';
						} else
						if(tmp>='a' && tmp<='f') {
							tmp-=87;
						} else {fputs("Invalid value\n",stderr);exit(1);}
						if(tmp2>='0' && tmp2<='9') {
							tmp2-='0';
						} else
						if(tmp2>='a' && tmp2<='f') {
							tmp2-=87;
						} else {fputs("Invalid value\n",stderr);exit(1);}
						P[i++]=(tmp*0x10)+tmp2;	//P
						//printf("%02x",P[i-1]);
					}
					if(i!=4) {
						fputs("Tablecheck failed\n",stderr);exit(1);
					}
					//printf("\n");
					i=0;
					while(true) {
						result=fread(buff,1,1,tf);
						if(result==0) {
							break;
						}
						if(*buff=='I') continue;
						if(*buff=='N') continue;
						if(*buff=='V') continue;
						if(*buff=='P') continue;
						if(*buff=='=') continue;
						if(i==4) break;
						unsigned char tmp=*buff;
						result=fread(buff,1,1,tf);
						if(result==0) {
							break;
						}
						unsigned char tmp2=*buff;
						if(tmp>='0' && tmp<='9') {
							tmp-='0';
						} else
						if(tmp>='a' && tmp<='f') {
							tmp-=87;
						} else {fputs("Invalid value\n",stderr);exit(1);}
						if(tmp2>='0' && tmp2<='9') {
							tmp2-='0';
						} else
						if(tmp2>='a' && tmp2<='f') {
							tmp2-=87;
						} else {fputs("Invalid value\n",stderr);exit(1);}
						P[i++]=(tmp*0x10)+tmp2;	//P
						//printf("%02x",P[i-1]);
					}
					if(i!=4) {
						fputs("Tablecheck Failed\n",stderr);exit(1);
					}
					//printf("\n");
					if(argc==5) {
						f=fopen(argv[4],"rb");
						if(f==NULL) {fputs("Error in opening plaintext file\n",stderr);exit(1);}
					}
					else {
						f=stdin;
					}
					i=0;
					while(true) {
						result=fread(buff,1,1,f);
						if(result==0) {
							break;
						}
						if(i==16) break;
						temp2[i++]=*buff;
						//printf("%02x",*buff);
					}
					if(i<16) {
						fputs("Input file too small\n",stderr);exit(1);
					}
					decrypt(Key,temp2,temp3,P);
					return 0;
				}
			}
		}//decrypt
		if(strcmp(argv[1],"modprod")==0) {
			string option,param,temp;
			char *p1,*p2;
			unsigned char P1[4],P2[4];
			temp=argv[2];
			option=temp.substr(0,4);
			if(option.compare("-p1=")==0) {
				param=temp.substr(4,temp.length());
				if(param.compare("")==0) {
					fputs("Malformed Command\n",stderr);
					exit(1);
				}
				if(param.length()!=8){
					fputs("Invalid Polynomial length\n",stderr);
					exit(1);
				}
				p1 = new char[param.length()];
				strcpy(p1,param.c_str());
				int j=0;
				for(int i=0;i<8;i+=2) {
					unsigned char tmp=(unsigned char)p1[i];
					unsigned char tmp2=(unsigned char)p1[i+1];
					if(p1[i]>='0' && p1[i]<='9') {
						tmp-='0';
					} else
					if(p1[i]>='a' && p1[i]<='f') {
						tmp-=87;
					} else {fputs("Invalid value\n",stderr);exit(1);}
					if(p1[i+1]>='0' && p1[i+1]<='9') {
						tmp2-='0';
					} else
					if(p1[i+1]>='a' && p1[i+1]<='f') {
						tmp2-=87;
					} else {fputs("Invalid value\n",stderr);exit(1);}
					P1[j++]=(tmp*0x10)+tmp2;
				}
			//P2
				temp=argv[3];
				option=temp.substr(0,4);
				if(option.compare("-p2=")==0) {
					param=temp.substr(4,temp.length());
					if(param.compare("")==0) {
						fputs("Malformed Command\n",stderr);
						exit(1);
					}
					if(param.length()!=8){
						fputs("Invalid Polynomial length\n",stderr);
						exit(1);
					}
					p2 = new char[param.length()];
					strcpy(p2,param.c_str());
					int j=0;
					for(int i=0;i<8;i+=2) {
						unsigned char tmp=(unsigned char)p2[i];
						unsigned char tmp2=(unsigned char)p2[i+1];
						if(p2[i]>='0' && p2[i]<='9') {
							tmp-='0';
						} else
						if(p2[i]>='a' && p2[i]<='f') {
							tmp-=87;
						} else {fputs("Invalid value\n",stderr);exit(1);}
						if(p2[i+1]>='0' && p2[i+1]<='9') {
							tmp2-='0';
						} else
						if(p2[i+1]>='a' && p2[i+1]<='f') {
							tmp2-=87;
						} else {fputs("Invalid value\n",stderr);exit(1);}
						P2[j++]=(tmp*0x10)+tmp2;
					}
					modprod(P1,P2);
					return 0;
				}//p2
			}	//P1
		}//modprod
	}//argc==4
	fputs("Malformed Command\n",stderr);
	return 1;
}


