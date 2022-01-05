#include <Windows.h>
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
void menu();
void serial_Connection();



HANDLE hComm;
BOOL Status;
DWORD dwEventMask;

int main(void) {
	// Defining variables 
	
	char dosya[15];
	FILE *input;
	/*
	input=fopen("result.txt","r+");

	fscanf(input,"%s",dosya);**/
    char number[10]; // for square number 
	char quit[10]; 
	char choice[10]; // choice variable
	char dataFromCard;
	char data[256]; // data from arduino

	serial_Connection(); // I create connection with arduino which connects COM4 port
    while(1) {
		input=fopen("../3_app/result.txt","r+");

		fscanf(input,"%s",dosya);
		printf("%s",dosya);
		DWORD dNoOFBytestoWrite;    
		DWORD dNoOfBytesWritten = 0; 
		DWORD NoBytesRead;
		dNoOFBytestoWrite = sizeof(choice);
		int press_counter = 0;
		
		if(dosya[0] == '1' || dosya[0] == '2' || dosya[0] == '3'){
			// write user's choice to arduino 
			Status = WriteFile(hComm,                 
							dosya,          
							dNoOFBytestoWrite,  
							&dNoOfBytesWritten, 
							NULL);
		}
		else if(dosya[0] == '1'){
			// write user's choice to arduino 
			Status = WriteFile(hComm,                 
							choice,          
							dNoOFBytestoWrite,  
							&dNoOfBytesWritten, 
							NULL);
			
			printf("Please enter the number to be squared by the arduino :");
			scanf("%s", number); // taking number 
			// write user's number to arduino 
			Status = WriteFile(hComm,                 
							dosya,          
							dNoOFBytestoWrite,  
							&dNoOfBytesWritten, 
							NULL);
			// taking squared number from arduino 				
			printf("Result is ");
			int i = 0;
			Status = WaitCommEvent(hComm, &dwEventMask, NULL);
			
			do{
            	Status = ReadFile(hComm,
								&dataFromCard, 
								sizeof(dataFromCard), 
								&NoBytesRead, 
								NULL);
            	data[i] = dataFromCard;     
            	i++;
        	} while (NoBytesRead > 0);
			
			
			for (int a = 0; a < i - 1; a++) 
				printf("%c",data[a]);
				
		}
		else if(choice[0] == '5'){
			// write user's choice to arduino 
			Status = WriteFile(hComm,                 
							choice,          
							dNoOFBytestoWrite,  
							&dNoOfBytesWritten, 
							NULL);
							
			
			printf("\nPlease enter how many times to press the button : ");
			scanf("%d",&press_counter);
			printf("\nThe counter started to counting.\nYou can press button\n");
			
			
			int i = 0;
			float a = 0;
			int flush_control = 0;
			// taking press number from arduino
			do{
				Status = WaitCommEvent(hComm, &dwEventMask, NULL);
				do{
					Status = ReadFile(hComm,
									&dataFromCard, 
									sizeof(dataFromCard), 
									&NoBytesRead, 
									NULL);
					data[i] = dataFromCard;
					
					if((int)data[i]<50 || flush_control == 1){
						printf("%c",data[i]);		
						flush_control = 1;
					}
					i++;
				} while (NoBytesRead > 0);
				a+=0.5;
			}while(press_counter != a);
	
		}
		
		else if(choice[0] == '6') menu();
		else if(choice[0] == '0') break;
		//else printf("Please enter right number! ");
		
	}	
	//closing port	
	CloseHandle(hComm);
	return 0;
}


void menu(){
	printf("##\tWELCOME TO GTU ARDUINO LAB       ##"
		   "\n##\tStudent Name : Ahmet Yigit       ##"
		   "\n##\tPlease Select From Following     ##");  
    printf("\n\t\t\t MENU\n \n(1) TURN ON LED ON ARDUINO \n(2) TURN OFF LED ON ARDUINO\n(3) FLASH ARDUINO LED 3 TIMES\n(4) SEND A NUMBER TO ARDUINO TO COMPUTE SQUARE BY ARDUINO\n(5) Button press counter (bonus item)\n(6) Display Menu\n(0) EXIT");    
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

