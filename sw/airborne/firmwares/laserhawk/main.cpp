// Simple app to print hokuyo range values

#include <stdio.h>
#include <stdlib.h>
#include "chokuyoplus.h"

CHokuyoPlus *HokuyoSensor;

int main(int argc, char *argv[]) {	
	int doprint = 0;
	
	if (argc <= 1) {
		printf("hokuyomti hokuyodevice\n ex: getrange /dev/ttyACM0\n");
		exit(EXIT_SUCCESS);
	}
	
	printf("print %d\n", atoi(argv[4]));	
	if (atoi(argv[4])) doprint = 1;	
		
	HokuyoSensor = new CHokuyoPlus(argv[1]);
	HokuyoSensor->setRealSensor();
	HokuyoSensor->open_port();
	
	if (!HokuyoSensor->port_open){
		exit(EXIT_SUCCESS);
	}

	HokuyoSensor->firstStep = atoi(argv[2]);
	HokuyoSensor->lastStep = atoi(argv[3]);
	/*HokuyoSensor->startAngle = 0;
	HokuyoSensor->endAngle = 2.35619;*/
	
	printf("scanning from step %d to %d\n",HokuyoSensor->firstStep, HokuyoSensor->lastStep );

	while(1) {   
		HokuyoSensor->getRange();
		int goodcnt = 0;
		int maxdist = 0;
		for (int i = 0 ; i <= HokuyoSensor->lastStep-HokuyoSensor->firstStep; i++) {
//			if (HokuyoSensor->data.depth[i] > 100) {
				if (doprint) {
					//printf("%d: %d ",i,HokuyoSensor->data.depth[i]);
					printf("%d\n",HokuyoSensor->data.depth[i]);
				/*Packet : Header,Length,payload,checksum
				  Skip dists < threshold and restart packet
				  Packets limited to PACBYTES bytes, restart packet if length attained*/
				}
				//if (HokuyoSensor->data.depth[i] > maxdist) maxdist = HokuyoSensor->data.depth[i];
				//goodcnt++;
//			}
		}
		//printf(" pts: %d max: %d\n", goodcnt, maxdist);
	}

}
