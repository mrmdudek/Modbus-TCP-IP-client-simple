#include <stdint.h>

uint8_t APDU[300];

Write_multiple_regs (char *server_add, int port, int st_r, int n_r, uint16_t *val)
{
	// check consistency of parameters

		if (n_r < 0)
		{
			return -1;
		}

		if (val < 0)
		{
			return -1;
		}

		if (st_r < 0)
		{
			return -1;
		}

	// assembles APDU (MODBUS PDU)
		
		APDU[0]=0x10; //Function code 1 Byte
		APDU[1]=(uint8_t)(st_r>>8);//Read Starting Address 2 Bytes 0x0000 to 0xFFFF
		APDU[2]=(uint8_t)(st_r);
		APDU[3]=(uint8_t)((n_r)>>8);//Quantity of Registers 2 Bytes 0x0001 to 0x007B
		APDU[4]=(uint8_t)(n_r);
		APDU[5]=(uint8_t)(2*n_r);//Byte Count 1 Byte 2 x N*

		for (int i=0;i<n_r;i++)
		{
			APDU[2*i+6] = (uint8_t)(val[i]>>8);//Registers Value N* x 2 Bytes value
			APDU[2*i+7] = (uint8_t)val[i];
		}

		int APDUlen = 6+2*n_r;
		uint8_t APDU_R[255];

	//Send_Modbus_request
		
		int answer_AP = Send_Modbus_request(server_add,port,APDU,APDUlen,APDU_R);
	
	// checks the reponse (APDU_R or error_code)
		
		if (answer_AP < 0)
		{
			if (answer_AP == -9)
			{
				return -9;// timeout
			}
			
			if (answer_AP == -8)
			{
				return -8;// socket error: perror("select"); /* an error accured */
			}
			
			if (answer_AP == -1)
			{
				return -7;//Different transaction ID
			}
		}	
	
	// returns: number of written regs/ coils – ok, <0 – error
		
			return ((APDU_R[3]<<8)+(APDU_R[4]));

}

Read_h_regs (char *server_add, int port, int st_r, int n_r, uint16_t *val)
{
	// check consistency of parameters
		
		if (n_r < 0)
		{
			return -1;
		}

		if (st_r < 0)
		{
			return -1;
		}
		
	// assembles APDU (MODBUS PDU)
		
		APDU[0]=0x03;
		APDU[1]=(uint8_t)(st_r>>8);
		APDU[2]=(uint8_t)(st_r);
		APDU[3]=(uint8_t)(n_r>>8);
		APDU[4]=(uint8_t)(n_r);

		int APDUlen = 5;
		uint8_t APDU_R[255];
		
	//Send_Modbus_request
		
		int answer_AP = Send_Modbus_request (server_add,port,APDU,APDUlen,APDU_R);

	// checks the reponse (APDU_R or error_code)
		
// checks the reponse (APDU_R or error_code)
		
		if (answer_AP < 0)
		{
			if (answer_AP == -9)
			{
				return -9;// timeout
			}
			
			if (answer_AP == -8)
			{
				return -8;// socket error: perror("select"); /* an error accured */
			}
			
			if (answer_AP == -1)
			{
				return -7;//Different transaction ID
			}
		}	
	
	// returns: number of written regs/ coils – ok, <0 – error
	
		for (int i=0;i<((APDU_R[1])/2);i++)
		{
			val[i] = (uint16_t)((APDU_R[2*i+2]>>8)+APDU_R[2*i+3]);
		}
		
		return ((APDU_R[1])/2);

}

/*
Write_multiple_coils (server_add, port, st_c, n_c, val)
{
// check consistency of parameters

// assembles APDU (MODBUS PDU)
APDU[0]=0x0F;
APDU[1]=(uint8_t)(st_c>>8);
APDU[2]=(uint8_t)(st_c);
APDU[3]=(uint8_t)(n_c>>8);
APDU[4]=(uint8_t)(n_c);

for (i=0;i<n_c;i++)
{
	APDU[i+5] = (uint8_t)val[i];
}

int APDUlen = sizeof(APDU);
int APDU_R;

//Send_Modbus_request
Send_Modbus_request (server_add,port,APDU,APDUlen,APDU_R)

// checks the reponse (APDU_R or error_code)
if (APDU_R < 0){
	printf ("error");
}	 

// returns: number of written regs/ coils – ok, <0 – error
if (APDU_R < 0 ){
	return (-1);
}
else 
{
	return (sizeof(APDU_R);
}
}
*/ 


/*

Read_coils (server_add, port, st_c, n_c, val)
{
// check consistency of parameters

// assembles APDU (MODBUS PDU)
APDU[0]=0x10;
APDU[1]=(uint8_t)(st_c>>8);
APDU[2]=(uint8_t)(st_c);
APDU[3]=(uint8_t)(n_c>>8);
APDU[4]=(uint8_t)(n_c);

for (i=0;i<n_c;i++)
{
	APDU[i+5] = (uint8_t)val[i];
}

int APDUlen = sizeof(APDU);
int APDU_R;

//Send_Modbus_request
Send_Modbus_request (server_add,port,APDU,APDUlen,APDU_R)

// checks the reponse (APDU_R or error_code)
if (APDU_R < 0){
	printf ("error");
}	 

// returns: number of written regs/ coils – ok, <0 – error
if (APDU_R < 0 ){
	return (-1);
}
else 
{
	return (sizeof(APDU_R);
}
}*/
