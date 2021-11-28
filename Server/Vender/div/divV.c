#include"divV.h"

int main(int argc, char* argv[])
{
	printf("%s: DIV Begin \n",__func__);

	int fd;
	if(argc > 0){
		fd = *(int*)argv[2];
		printf("fd received is %d \n",fd);
	}
	else{
		printf("Insufficient argument received.\n");
		goto OUT;
	}
	CLIENT_REQUEST_DATA *pcrd;	
	int res = read(fd, pcrd,sizeof(CLIENT_REQUEST_DATA));	

	printf("Data Received as below clinet_id = %d, operand1 = %d, operand2 = %d, operator_id =%d \n ",pcrd->client_id,pcrd->operand1,pcrd->operand2,pcrd->vender_reuest);	
	printf("%s: DIV End \n",__func__);
	return 0;

OUT:
	return -1;
}
