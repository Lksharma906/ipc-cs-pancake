#include"modV.h"

int main(int argc, char* argv[])
{
	printf("%s: MOD Begin \n",__func__);

	int fd;
	if(argc == 2){
		fd = atoi(argv[1]);
		printf("fd received is %d \n",fd);
	}
	else{
		printf("Insufficient argument received.\n");
		goto OUT;
	}
	CLIENT_REQUEST_DATA pcrd;
	int res = read(fd, &pcrd,sizeof(CLIENT_REQUEST_DATA));
	if(res == -1){
		perror("Read:");
		exit(EXIT_FAILURE);
	}
	//close(fd);
	//printf("Data Received as below clinet_id = %d, operand1 = %d, operand2 = %d, operator_id =%d \n ",pcrd->client_id,pcrd->operand1,pcrd->operand2,pcrd->vender_reuest);
	printf("%s: MOD End \n",__func__);
	return 0;
OUT:
	return -1;

}
