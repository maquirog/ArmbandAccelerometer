

#include "mraa.hpp"

#include <iostream>
#include <unistd.h>
#include <fstream>
#include "SFE_LSM9DS0.h"
#include <stdlib.h>
using namespace std;

int main() {

	//Declaration of PWM and 9DOF to be used
	LSM9DS0 *imu;
	imu = new LSM9DS0(0x6B, 0x1D);
	uint16_t imuResult = imu->begin();
	cout<<hex<<"Chip ID: 0x"<<imuResult<<dec<<" (should be 0x49d4)"<<endl;

	bool isHigh = 0;
	bool isZero = 1;
	bool isLow = 0;

	std::cout << "Turn power on now" << std::endl;

	//Wait 8 seconds so we have time to move physicaly stabilize 9DOF module
	sleep(2);

	std::cout << "Finished sleeping" << std::endl;

//	ofstream gyroscopeFile;
//	gyroscopeFile.open("ori.txt");

	for(;;)
	{
		//Get each reading from 9DOF
		imu->readAccel();
		imu->readMag();
		imu->readGyro();
		imu->readTemp();

//	    cout<<"Accel x: "<<imu->calcAccel(imu->ax)<<" g\t"<<
//	    	"Accel y: "<<imu->calcAccel(imu->ay)<<" g\t"<<
//			"Accel z: "<<imu->calcAccel(imu->az)<<" g\t"<<
//			"Gyro x: "<<imu->calcGyro(imu->gx)<<" deg/s\t"<<
//			"Gyro y: "<<imu->calcGyro(imu->gy)<<" deg/s\t"<<
//			"Gyro z: "<<imu->calcGyro(imu->gz)<<" deg/s"<<endl;

//		gyroscopeFile << imu->calcAccel(imu->ax) << ", " << imu->calcAccel(imu->ay) << ", " << imu->calcAccel(imu->az) << ", " << std::endl;
		//Calculate every DC depending on 9DOF accelerometer position
		if((imu->calcAccel(imu->az) > 1) && isZero && ~isLow)
		{
			cout << "Got High-----------------------------------------------------------------------" << endl;
			isZero = 0;
			isHigh = 1;
			system("echo 'N' > /dev/rfcomm0");
		}
		if((imu->calcAccel(imu->az) < 0.2) && (imu->calcAccel(imu->az) > -0.2))
		{
//			cout << "Is Zero" << endl;
			isZero = 1;
			isHigh = 0;
			isLow = 0;
		}
		if((imu->calcAccel(imu->az) < -1) && isZero && ~isHigh)
		{
			cout << "Got Low?????????????????????????????????????????????????????????????????????????" << endl;
			isZero = 0;
			isLow = 1;
			system("echo 'P' > /dev/rfcomm0");
		}
//		cout << imu->calcAccel(imu->ax) << endl;

		//Repeat last steps every 5 ms
		usleep(50000);

	}


	return mraa::SUCCESS;

}
