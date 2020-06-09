//
//  main.c
//  CEromupdater
//
//  Created by LogicalJoe on 5/31/20.
//  Copyright © 2020 LogicalJoe. All rights reserved.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void error1(int n, char* str) {
	if (n) {
		printf("Error: %s\n",str);
		exit(1);
	}
}

int main(int argc, const char * argv[]) {
	uint64_t n,romsize,hl;
	char romversion[11];
	FILE *romin, *osin, *romout;
	
	error1(argc==1,"Requires arguments");
	
	if (!strcmp(argv[1],"-h") || !strcmp(argv[1],"--help")) {
		printf("CEromupdater v1.0 - by LogicalJoe (c) 2020\n"
			   "Usage: CEromupdater ([rom in] [os in] [rom out]) | -h\n"
			   "  -h, --help\tDisplay a useful help message\n"
			   "  [rom in] [os in] [rom out]\tUse boot [rom in] boot and [os in] to make [rom out]\n");
		return 0;
	};
	
	error1(argc!=4,"Requires three arguments");
	
	error1(!strcmp(argv[1],argv[3]),"romin == romout");
	
	romin=fopen(argv[1],"r");
	error1(romin==NULL,"Romin does not exist");
	
	fseek(romin,0,SEEK_END);
	romsize=ftell(romin);
	error1(romsize!=0x400000,"Invalid rom size");
	
	osin=fopen(argv[2],"r");
	if (osin==NULL) {
		printf("Error: osin does not exist\n");
		return 1;
	}
	
	fseek(osin,74,SEEK_SET);
	// 75 bytes in = size
	printf("400000 byte rom\n20000 byte boot\n%llX byte OS\n",romsize=getc(osin)+getc(osin)*0x100+getc(osin)*0x10000+getc(osin)*0x1000000);
	
	fseek(romin,0,SEEK_SET);
	romout=fopen(argv[3],"w+");
	
	for(n=0x20000;n--;) {
		putc(getc(romin),romout);
	}
	fclose(romin);
	for(n=0x3E0000;n--;) {
		putc(0xFF,romout);
	}
	fseek(romout,0x20000,SEEK_SET);
	for(n=romsize;n--;) {
		putc(getc(osin),romout);
	}
	fclose(osin);
	
	// Why's this call never finished in some OSs?
	fseek(romout,0x20100,SEEK_SET);
	putc(0x5A,romout); // 0b01011010
	putc(0xA5,romout); // 0b10100101
	
	
	// BOOT Code ((0000394h [_PutBootVersion:] +1)+17)
	printf("BOOT Code ");
	hl=0x000395;
	fseek(romout,hl,SEEK_SET);
	hl=17+getc(romout)+getc(romout)*0x100+getc(romout)*0x10000;
	fseek(romout,hl,SEEK_SET);
	hl=getc(romout)+getc(romout)*0x100+getc(romout)*0x10000;
	fseek(romout,hl,SEEK_SET);
	for (hl=10;hl--;)
		putc(getc(romout),stdout);
	printf("\n");
	
	// BASE Code ((00219DCh [_EnterSelfTest] +1)+25)
	hl=0x0219DD;
	fseek(romout,hl,SEEK_SET);
	hl=25+getc(romout)+getc(romout)*0x100+getc(romout)*0x10000;
	fseek(romout,hl,SEEK_SET);
	hl=getc(romout)+getc(romout)*0x100+getc(romout)*0x10000;
	fseek(romout,hl,SEEK_SET);
	for (hl=20;hl--;)
		putc(getc(romout),stdout);
	printf("\n");
	
	fclose(romout);
	
	return 0;
}
