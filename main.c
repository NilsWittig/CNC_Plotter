#include <stdlib.h>
#include <stdio.h>

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

#include "rs232.h"



int main(int argc, char* argv[]) // call like this: main "text.txt"
{
	char const* const fileName = argv[1]; /* should check that argc > 1 */
	int thingy = argc;
	printf("Starting with %i and file %s\n", thingy, fileName);
	char line[512];
	FILE *fptr;
	
	if( (fptr = fopen(fileName, "r")) == NULL){
		printf("ERROR, no file");
		return 1;
	}
	
  int i, n,
      cport_nr=5,        /* /dev/ttyS0 (COM1 on windows) */
      bdrate=9600;       /* 9600 baud */

  unsigned char buf[4096];

  char mode[]={'8','N','1',0};
  
  char startBit[520];
  startBit[0] = '@';
  char endBit[2];
  endBit[0] = '@';
  endBit[1] = 0;
  
  if(RS232_OpenComport(cport_nr, bdrate, mode))
  {
    printf("Can not open comport\n");

    return(0);
  }

  while(1){
	 //##################### listen to the COM port ######################
    n = RS232_PollComport(cport_nr, buf, 4095);
    if(n > 0){
      buf[n] = 0;   /* always put a "null" at the end of a string! */
      for(i=0; i < n; i++){
		if(buf[i] < 32)  /* replace unreadable control-codes by dots */
			{buf[i] = '.';}
      }
      printf("received %i bytes: %s\n", n, (char *)buf);
    }
	//##################### send command if plotter is ready ##########################
	/*
	if(buf[0] == '>'){
		if( fgets(line, sizeof(line), fptr) ){ //read line 
			if(line[0] == 'G'){ // if line is a gcode block send it
				RS232_cputs(cport_nr, line);
				printf("send gcode block: %s", line);
			}
		}else{printf("end of file\n"); fclose(fptr);return 0;} //end of file exit
	}
	*/
	int j;
	for(j = 0; j < 4095; j++){ // search for ready code of the plotter
		if(buf[j] == 0){break;}
		if( buf[j] == '>' ){ //found ready code
			printf("FOUND READY CODE\n");
			printf(buf);
			printf("\n");
			int search = 1;
			while(search){ //search for gcode block
				if( fgets(line, sizeof(line), fptr) ){ //read line 
					if(line[0] == 'G'){ // if line is a gcode block send it
						//RS232_cputs(cport_nr, startBit);
						startBit[0] = '@';
						int try;
						for(try = 1; try < 520; try++){startBit[try] = 0;}
						startBit[1] = 0;
						strcat(startBit, line); // place startBit at the start of the command
						strcat(startBit, endBit);
						RS232_cputs(cport_nr, startBit);
						printf("send gcode block: %s", startBit);
						search = 0;
					}
				}else{printf("end of file\n"); fclose(fptr);return 0;} //end of file exit
			}
			break;
		}
	}
	int tryHard;
	for(tryHard = 1; tryHard < 4095; tryHard++){buf[tryHard] = 0;}
	
#ifdef _WIN32
    Sleep(100);
#else
    usleep(100000);  /* sleep for 100 milliSeconds */
#endif
  }

  return(0);
}
//@G03 X499.761643 Y646.316745 Z-0.125000 I-201.539455 J-29.289516 @