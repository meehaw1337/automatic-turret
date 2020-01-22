#include<windows.h>
#include<opencv2/opencv.hpp>
#include<iostream>
#include<chrono>
#include<string.h>

using namespace std;
using namespace cv;


HANDLE initHandle() {
	HANDLE serialHandle = CreateFile(L"\\\\.\\COM3", GENERIC_READ | GENERIC_WRITE, 0,
		NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	DCB serialParams = { 0 };

	GetCommState(serialHandle, &serialParams);

	serialParams.BaudRate = CBR_9600;
	serialParams.ByteSize = 8;
	serialParams.StopBits = ONESTOPBIT;
	serialParams.Parity = NOPARITY;
	serialParams.fDtrControl = DTR_CONTROL_ENABLE;

	SetCommState(serialHandle, &serialParams);

	return serialHandle;
}

int main()
{
	VideoCapture cap;
	Mat frame;

	auto start = chrono::high_resolution_clock::now();

	HANDLE serialHandle = initHandle();

	if (!cap.open(0))
		return -1;

	DWORD bytesSend;
	char* info = (char*)malloc(sizeof(char) * 2);

	while (true)
	{
		cap >> frame;

		if (frame.empty()) break;
		imshow("USB camera image", frame);

		uint8_t* pixelPtr = (uint8_t*) frame.data;
		int cn = frame.channels();
		Scalar_<uint8_t> bgrPixel;

		Mat hsv;
		cvtColor(frame, hsv, COLOR_BGR2HSV);
		int x = 0;
		int y = 0;
		int n = 0;
		chrono::duration<double> elapsed = chrono::high_resolution_clock::now() - start;
		if (elapsed.count() > 5) {
			for (int i = 0; i < frame.rows; i++)
			{
				for (int j = 0; j < frame.cols; j++)
				{
					//Vec3b hsv = hsv.at<Vec3b>(i, j);

					bgrPixel.val[0] = pixelPtr[i * frame.cols * cn + j * cn + 0]; // B
					bgrPixel.val[1] = pixelPtr[i * frame.cols * cn + j * cn + 1]; // G
					bgrPixel.val[2] = pixelPtr[i * frame.cols * cn + j * cn + 2]; // R

					if ((int) bgrPixel.val[2] > 240 &&
						//(int)bgrPixel.val[2] < 165  &&
						(int)bgrPixel.val[1] > 190 &&
						//(int)bgrPixel.val[1] < 125 &&
						(int) bgrPixel.val[0] > 220
						//&& (int) bgrPixel.val[0] < 140) 
					){

						x += i;
						y += j;
						n++;
						/*
						// Aim down
						if (i < frame.rows / 2 - 20) {
							strcpy(info, "D");
							WriteFile(serialHandle, (void*)info, 2, &bytesSend, 0);
						}
						// Aim up
						if (i > frame.rows / 2 + 20) {
							strcpy(info, "U");
							WriteFile(serialHandle, (void*)info, 2, &bytesSend, 0);
						}
						// Aim right
						if (j < frame.cols / 2 - 20) {
							strcpy(info, "L");
							WriteFile(serialHandle, (void*)info, 2, &bytesSend, 0);
						}
						// Aim left
						if (j > frame.cols / 2 + 20) {
							strcpy(info, "R");
							WriteFile(serialHandle, (void*)info, 2, &bytesSend, 0);
						}

						cout 
							<< "Laser detected at: " << i << "x" << j 
							<< ", RGB = (" << (int)bgrPixel.val[2] << "," << (int)bgrPixel.val[1] 
							<< "," << (int)bgrPixel.val[0] << ")" << endl;

						// Break both loops
						i = frame.rows;
						j = frame.cols;

						Sleep(50);
						*/
					}


				}
			}
			cout << n << endl;
			if (n > 0) {
				x = x / n;
				y = y / n;
				// Aim down
				if (x < frame.rows / 2 - 40) {
					strcpy(info, "D");
					WriteFile(serialHandle, (void*)info, 1, &bytesSend, 0);
				}
				// Aim up
				else if (x > frame.rows / 2 + 40) {
					strcpy(info, "U");
					WriteFile(serialHandle, (void*)info, 1, &bytesSend, 0);
				}
				// Aim right
				else if (y < frame.cols / 2 - 40) {
					strcpy(info, "L");
					WriteFile(serialHandle, (void*)info, 1, &bytesSend, 0);
				}
				// Aim left
				else if (y > frame.cols / 2 + 40) {
					strcpy(info, "R");
					WriteFile(serialHandle, (void*)info, 1, &bytesSend, 0);
				}
				n = 0;
				x = 0;
				y = 0;
				Sleep(50);
			}
		}

		if (waitKey(1) == 27) break;
	}
	

	return 0;
}