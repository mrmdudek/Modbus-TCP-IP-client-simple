#include <stdint.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/select.h>

#define t_id_max 64 // max transaction ID
#define unit_id 1 // slave ID, unit ID

static int t_id=25;
uint8_t PDU[300];
uint8_t PDU_R[300];
int APDU_Rlen;

Send_Modbus_request (char *server_add,int port,uint8_t *APDU,int APDUlen, uint8_t *APDU_R)
{
	// generates TI (trans.ID →sequence number)
		
		if (t_id < t_id_max)
		{
			t_id++;
		}
		else
		{
			t_id = 0;
		}
		
	// assembles PDU = APDU(SDU) + MBAP
		//2 bytes transaction ID
		PDU[0] = (uint8_t)(t_id >> 8);
		PDU[1] = (uint8_t)(t_id);
		//2 bytes protocol ID
		PDU[2] = 0;
		PDU[3] = 0;
		// length 2 bytes
		PDU[4] = (uint8_t)((1+APDUlen)>>8);
		PDU[5] = (uint8_t)(1+APDUlen);
		// Unit ID
		PDU[6] = unit_id;
		// data from APDU
		for (int j=0;j<APDUlen;j++)
		{
			PDU[j+7] = (uint8_t)(APDU[j]);
		}
		
		int PDUlen = APDUlen+7;
		
	// for debug PDU
	
		printf ("\nPDU ");
		for (int k=0;k<PDUlen;k++)
		{
			printf ("%d ",PDU[k]);
		}

	// opens TCP client socket and connects to server (*)
		
		int sock = open_tcp(server_add, port);
		
		write (sock, PDU, PDUlen); // sends Modbus TCP PDU

		fd_set set;
		struct timeval timeout;
		int rv;
		
		FD_ZERO(&set); /* clear the set */
		FD_SET(sock, &set); /* add our file descriptor to the set */

		timeout.tv_sec = 0;
		timeout.tv_usec = 100000;

		rv = select(sock + 1, &set, NULL, NULL, &timeout);
		if(rv == -1)
		{
			return -8; // perror("select"); /* an error accured */
		}
		else if(rv == 0)
		{
			return -9; // timeout
		}
		else
		{
			read (sock, PDU_R, 7);
			APDU_Rlen = ((PDU_R[4]<<8)+(PDU_R[5]))-1;
			read (sock, APDU_R, APDU_Rlen);
		}
		
	// closes TCP client socket with server (*)
		
		close_tcp(sock);
		
	// for debug PDU_R
	
		printf ("\nPDU_R ");
		for (int i=0;i<7;i++)
		{
			printf ("%d ",PDU_R[i]);
		}
		for (int i=0;i<APDU_Rlen;i++)
		{
			printf ("%d ",APDU_R[i]);
		}
		printf ("\n");
		
	//Check transaction ID 
		if (PDU[0] != PDU_R[0] || PDU[1] != PDU_R[1]) 
		{
		return -1; // Different transaction ID
		}
		
	// returns: APDU_R and 0 – ok, <0 – error (timeout)
	
		return 0;
		
}
