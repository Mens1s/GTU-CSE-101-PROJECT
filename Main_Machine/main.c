#include <Windows.h>
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
void serial_Connection();

HANDLE hComm;
BOOL Status;
DWORD dwEventMask;

int main(void) {
	// Defining variables 
	
	char dosya[15];//python write human status and we get it in this variable
	FILE *input; // human status file

	char quit[10]; 
	char choice[10]; // choice variable
	char dataFromCard;
	char data[256]; // data from arduino
	int temp = 0;
	serial_Connection(); // We create connection with arduino which connects COM4 port
	Sleep(3000);
    while(1) {
		input=fopen("../3_app/result.txt","r+");
		fscanf(input,"%s",dosya);// we get human status 
		DWORD dNoOFBytestoWrite;    
		DWORD dNoOfBytesWritten = 0; 
		DWORD NoBytesRead;
		dNoOFBytestoWrite = sizeof(choice);

		int unmasked_people = 0;
		
		if(dosya[0] == '1' && temp != 1){
			
			unmasked_people += 1;
			temp = 1;
			// write user's choice to arduino 
			Status = WriteFile(hComm,                 
							dosya,        // send human status to arduino   
							dNoOFBytestoWrite,  
							&dNoOfBytesWritten, 
							NULL);
		}
		else if(dosya[0] == '2' && temp != 2){
			temp = 2;
			// write user's choice to arduino 
			Status = WriteFile(hComm,                 
							dosya,  // send human status to arduino        
							dNoOFBytestoWrite,  
							&dNoOfBytesWritten, 
							NULL);
		}
		fclose(input);
		Sleep(3000);
	}	
	//closing port	
	CloseHandle(hComm);
	return 0;
}

void serial_Connection(){
		                     
    char ComPortName[] = "\\\\.\\COM4"; //Our port name
                           
     
	DWORD NoBytesRead; 
	hComm = CreateFile(ComPortName,                  
                       GENERIC_READ | GENERIC_WRITE,
                       0,                            
                       NULL,                         
                       OPEN_EXISTING,               
                       0,                            
                       NULL);
	DCB dcbSerialParams = {0}; 
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);

    Status = GetCommState(hComm, &dcbSerialParams); 

    dcbSerialParams.BaudRate = CBR_9600;   
    dcbSerialParams.ByteSize = 8;          
    dcbSerialParams.StopBits = ONESTOPBIT; 
    dcbSerialParams.Parity = NOPARITY;     

    Status = SetCommState(hComm, &dcbSerialParams);
	
	COMMTIMEOUTS timeouts = {0};
	
    timeouts.ReadIntervalTimeout = 50;
    timeouts.ReadTotalTimeoutConstant = 50;
    timeouts.ReadTotalTimeoutMultiplier = 10;
    timeouts.WriteTotalTimeoutConstant = 50;
    timeouts.WriteTotalTimeoutMultiplier = 10;
    
    if (SetCommTimeouts(hComm, &timeouts) == FALSE)
        printf("\n\n~~~Error~~~ Please check your connection! ~~~Error~~~");

    /*------------------------------------ Setting Receive Mask ----------------------------------------------*/

    Status = SetCommMask(hComm, EV_RXCHAR);	
}

