#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//xxd <-- this is a hex dump

int init(unsigned char* img_name){
	if(!img_name){
		printf("Error: Missing the image name\n");
		return(-1);
	}
		
	FILE *img;
	/*
	if(img = fopen(img_name, "r")){
		fclose(img);
		pringf("Error: File already exists\n");
		return(-1);
	} keeping this bc it might come in handy later
	*/
	
	img = fopen(img_name, "w");
	time_t current_time;
	time(&current_time);
	struct tm *current_time_struct;
	current_time_struct = localtime(&current_time);
	for(int i = 0; i < 256; i++){
		unsigned char buffer[512] = {0x00};
		if(i < 200){
			//user data
			fwrite(buffer, 1, sizeof(buffer), img);
		}else if(i < 241){
			//normally unussed area
			fwrite(buffer, 1, sizeof(buffer), img);
		}else if(i < 254){
			//single level directory
			fwrite(buffer, 1, sizeof(buffer), img);
		}else if(i < 255){
			//file allocation table
			for(int x = 0; x < 512; x += 2){
				if(x < 481){
					buffer[x + 1] = 0xFF;
					buffer[x] = 0xFC;
				}else if(x < 508){
					int nextpoint = (x - 482);
					if(nextpoint == 0){
						buffer[x + 1] = 0xFF;
						buffer[x] = 0xFA;
					}else{
						nextpoint = (nextpoint/2) + 240;
						buffer[x + 1] = 0x00;
						buffer[x] = 	nextpoint;				
						//buffer[x] = (nextpoint/1000) << 4) | ((nextpoint%1000)/100);
						//buffer[x] = (((nextpoint%100)/10) << 4) | (nextpoint%10);
					}
				}else if(x < 512){
					buffer[x + 1] = 0xFF;
					buffer[x] = 0xFA;
				}
			}
			fwrite(buffer, 1, sizeof(buffer), img);
		}else if(i < 256){
			//root block
			for(int x = 0; x < 512; x++){
				if(x < 16){
					//Filesystem Signature
					buffer[x] = 0x55;
				}else if(x < 17){
					buffer[x] = 0x00;
				}else if(x < 18){
					//Color code blue componenet
					buffer[x] = 0x00;
				}else if(x < 19){
					//Color code green componenet
					buffer[x] = 0x00;
				}else if(x < 20){
					//Color code red componenet
					buffer[x] = 0x00;
				}else if(x < 21){
					//Color code alpha componenet
					buffer[x] = 0x00;
				}else if(x < 48){
					//unused
					buffer[x] = 0x00;
				}else if(x < 56){
					//Timestamp
					if(x == 48){
						//century
						int cen = (current_time_struct->tm_year/100 + 19);
						buffer[x] = ((cen/10) << 4) | (cen%10);
					}else if(x == 49){
						//year
						int year = (current_time_struct->tm_year%100);
						buffer[x] = ((year/10) << 4) | (year%10);
					}else if(x == 50){
						//Month
						int Month = (current_time_struct->tm_mon + 1);
						buffer[x] = ((Month/10) << 4) | (Month%10);
					}else if(x == 51){
						//Day
						int Day = (current_time_struct->tm_mday);
						buffer[x] = ((Day/10) << 4) | (Day%10);
					}else if(x == 52){
						//Hour
						int Hour = (current_time_struct->tm_hour);
						buffer[x] = ((Hour/10) << 4) | (Hour%10);
					}else if(x == 53){
						//Minute
						int Min = (current_time_struct->tm_min);
						buffer[x] = ((Min/10) << 4) | (Min%10);
					}else if(x == 54){
						//Second
						int Sec = (current_time_struct->tm_sec);
						buffer[x] = ((Sec/10) << 4) | (Sec%10);
					}else if(x == 55){
						//Day of week
						int whatDay = (current_time_struct->tm_wday);
						buffer[x] = ((whatDay/10) << 4) | (whatDay%10);
					}
				}else if(x < 70){
					//unused
					buffer[x] = 0x00;
				}else if(x < 72){
					//Starting block of the FAT
					if(x == 71){
						buffer[x] = 0x00;
					}else if(x == 70){
						buffer[x] = 0xFE;
					}
				}else if(x < 74){
					//size of fat
					if(x == 73){
						buffer[x] = 0x00;
					}else if(x == 72){
						buffer[x] = 0x01;
					}
				}else if(x < 76){
					//Last block of the directory
					if(x == 75){
						buffer[x] = 0x00;
					}else if(x == 74){
						buffer[x] = 0xFD;
					}
				}else if(x < 78){
					//Size of the directory
					if(x == 77){
						buffer[x] = 0x00;
					}else if(x == 76){
						buffer[x] = 0x0D;
					}
				}else if(x < 80){
					//Icon for the filesystem
					if(x == 79){
						buffer[x] = 0x00;
					}else if(x == 78){
						buffer[x] = 0x00;
					}
				}else if(x < 82){
					//Number of user blocks
					if(x == 81){
						buffer[x] = 0x00;
					}else if(x == 80){
						buffer[x] = 0xC8;
					}
				}else if(x < 512){
					//Unused
					buffer[x] = 0x00;
				}
			}
			fwrite(buffer, 1, sizeof(buffer), img);

		}
	}
	fclose(img);
	//200 blocks for the user data
	//41 blocks of normally unussed area
	//13 blocks single level directory
	//1 block file allocation table
	//1 block root block

	return(0);
}

int main(int argc, char **argv){
	if(argc == 1){
		printf("ERROR: No file name\n");
		return(0);
	}
	int x;
	x = init(argv[1]);
	return(x);
}


