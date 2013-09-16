int hello_world(char* temp){
	volatile register char* temp1,*temp2;
	for(temp1 =temp, temp2= (char*)0xb8000;
	*temp1;temp1 +=1, temp2+= 2)
	*temp2 = *temp1;
	return 0;
	}
