#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>

#define BUF_LEN 32
#define VAL_LEN 123

unsigned char *server_add = "127.0.0.1"; //server addres 127.0.0.1 - loopback 
#define port 502 //modbus port

int main () {

int buf0; // to switch case (write, read, coil, register)
int st_r; 	// st_r - start register adress to first register
//int st_c[VAL_LEN];	// st_c - start register adress to first coil
//int n_c[VAL_LEN];	// n_c - number of coils
int n_r;	// n_r - number of register
uint16_t val[VAL_LEN];	// val - (pointer) to the value

printf ("Write_multiple_regs 0, Write_multiple_coils 1, Read_h_regs 2, Read_coils 2 \n");
scanf("%d", &buf0);

if (buf0 == 0){

	printf ("Start register \n");
	scanf("%d", &st_r);

	printf ("How many registers \n");
	scanf("%d", &n_r);
	
	// read data
	for (int i=0;i<n_r;i++)
	{
		printf ("Value \n");
		scanf("%d", val+i);		
	}
		
	// Write_multiple_regs
	int answer = Write_multiple_regs (server_add, port, st_r, n_r, val);
	
	// check answer
	if (answer<0)
	{
		if (answer == -9)
		{
			printf ("error, timeout \n");
		}
			
		else if (answer == -8)
		{
			printf ("other error socket\n");
		}
		else if (answer == -7)
		{
			printf ("Modbus error: Different transaction ID\n");
		}
		else if (answer == -1)
		{
			printf ("wrong parameters \n");
		}
		else
		{
			printf ("Other error \n");	
		}
	}
	else 
	{
		printf ("\n%d register(s) were successfully write\n",answer);
	}

}

if (buf0 == 2){

	printf ("Start register \n");
	scanf("%d", &st_r);

	printf ("How many registers \n");
	scanf("%d", &n_r);
	
	// Read_h_regs
	
	int answer = Read_h_regs (server_add, port, st_r, n_r, val);
		
	// check answer
	if (answer<0)
	{
		if (answer == -9)
		{
			printf ("error, timeout \n");
		}
			
		else if (answer == -8)
		{
			printf ("other error socket\n");
		}
		else if (answer == -7)
		{
			printf ("Modbus error: Different transaction ID\n");
		}
		else if (answer == -1)
		{
			printf ("wrong parameters \n");
		}
		else
		{
			printf ("Other error \n");	
		}
	}
	else 
	{
		printf ("\n%d register(s) were successfully read\n",answer);
		
	// print read data
		printf ("Value(s) \n");
		for (int i=0;i<answer;i++){
			printf ("%d ",val[i]);
			}
		printf ("\n");
	}
}
	

if (buf0 == 1){

	printf ("Not implemented\n");
	
}

if (buf0 == 3){

	printf ("Not implemented\n");
	
	}

printf ("EXIT \n");
return 0;
}