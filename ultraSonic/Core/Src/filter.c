/*
 * filter.c
 *
 *  Created on: Mar 18, 2024
 *      Author: myhg1
 */

#include "filter.h"

#define maxValue	100

uint16_t movingAvgFilter(uint16_t inData){
   static uint16_t filterBuffer[maxValue];
   static uint32_t sumValue = 0;
   static uint16_t bufPos = 0;
   static _Bool	isFirst = 0;
   if(isFirst ==0){
  	 isFirst = 1;
  	 for(int i=0;i<maxValue;i++)
  		 filterBuffer[i] = inData;
   }
   // 합계에서 현재위치의 버퍼값을 빼기
   sumValue -= filterBuffer[bufPos];
   // 현재위치의 버퍼값을
   filterBuffer[bufPos] = inData;
   // 합계에 현재위치의 버퍼값을 더하기
   sumValue += filterBuffer[bufPos];
   // 현재위치 증가
   bufPos++;
   // 현재위치 리셋
   bufPos %= 100;
   return sumValue / 100;
}

double Kalman(double inData) {
// Kalman filter setup
static double P = 1.0;//														추정 오차의 공분산
static double varP = 0.0001; // pow(0.01, 2)				프로세스 변동성
static double R = 0.25;//pow(0.5, 2);								측정치 오차의 공분산
static double K = 1.0;//														칼만 이득
static double X = 20.0;//														현재 추정된 상태
// Kalman Simple Filter
P = P + varP;
K = P / (P + R);
X = (K * inData) + (1 - K) * X;
P = (1 - K) * P;
return X;
}
