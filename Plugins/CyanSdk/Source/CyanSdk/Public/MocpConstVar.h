// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"


/**
 * 
 */

//body count
static const int PoseCount = 32;
static const int PoseAddCount = 20;
static const int HandCount = 21;
static const int vstatusCount = 3;
static const int FaceCount = /*140*/27;

const int ankleleft = 20;
const int ankleright = 24;
const int leftkneel = 19;
const int rightkneel = 23;
const int footleft = 21;
const int footright = 25;
const int lefthip = 18;
const int righthip = 22;
const int leftshld = 5;
const int rightshld = 12;
const int leftclivc = 4;
const int rightclivc = 11;
const int leftelblow = 6;
const int rightelblow = 13;
const int leftwrist = 7;
const int rightwrist = 14;
const int leftArmMid = 8;
const int pelvis = 9;
const int  lowchest = 10;
const int  rightArmMid = 15;
const int  leftHandstatus = 16;
const int  rightHandstatus = 17;
const int leftears = 29;
const int rightears = 31;
const int head = 26;
const int nose = 27;
const int leftLegMid = 28;
const int rightLegMid = 30;
const int hipc = 0;//real root
const int naval = 1;
const int chest = 2;
const int neck = 3;
// add ik
const int leftArmAdd01 = 0;
const int leftArmAdd02 = 1;
const int leftArmAdd03 = 2;
const int leftArmAdd04 = 3;

const int rightArmAdd01 = 4;
const int rightArmAdd02 = 5;
const int rightArmAdd03 = 6;
const int rightArmAdd04 = 7;

// add ik
const int leftLegAdd01 = 8;
const int leftLegAdd02 = 9;
const int leftLegAdd03 = 10;
const int leftLegAdd04 = 11;

const int rightLegAdd01 = 12;
const int rightLegAdd02 = 13;
const int rightLegAdd03 = 14;
const int rightLegAdd04 = 15;


//left hand 
const int l_wrist = 0;
const int l_thumbproximal = 1;
const int l_thumbintermediate = 2;
const int l_thumbdistal = 3;
const int l_thumbtip = 4;
const int l_indexproximal = 5;
const int l_indexintermediate = 6;
const int l_indexdistal = 7;
const int l_indextip = 8;
const int l_middleproximal = 9;
const int l_middleintermediate = 10;
const int l_middledistal = 11;
const int l_middletip = 12;
const int l_ringproximal = 13;
const int l_ringintermediate = 14;
const int l_ringdistal = 15;
const int l_ringtip = 16;
const int l_littleproximal = 17;
const int l_littleintermediate = 18;
const int l_littledistal = 19;
const int l_littletip = 20;
//right hand
const int r_wrist = 21 + 0;
const int r_thumbproximal = 21 + 1;
const int r_thumbintermediate = 21 + 2;
const int r_thumbdistal = 21 + 3;
const int r_thumbtip = 21 + 4;
const int r_indexproximal = 21 + 5;
const int r_indexintermediate = 21 + 6;
const int r_indexdistal = 21 + 7;
const int r_indextip = 21 + 8;
const int r_middleproximal = 21 + 9;
const int r_middleintermediate = 21 + 10;
const int r_middledistal = 21 + 11;
const int r_middletip = 21 + 12;
const int r_ringproximal = 21 + 13;
const int r_ringintermediate = 21 + 14;
const int r_ringdistal = 21 + 15;
const int r_ringtip = 21 + 16;
const int r_littleproximal = 21 + 17;
const int r_littleintermediate = 21 + 18;
const int r_littledistal = 21 + 19;
const int r_littletip = 21 + 20;

const int l_carpal1 = 21 + 21;
const int l_carpal2 = 21 + 22;
const int l_carpal3 = 21 + 23;
const int l_carpal4 = 21 + 24;

const int r_carpal1 = 21 + 25;
const int r_carpal2 = 21 + 26;
const int r_carpal3 = 21 + 27;
const int r_carpal4 = 21 + 28;


const int  leye_left = 8;
const int  leye_right = 15;
const int  leye_up0 = 11;
const int  leye_up1 = 12;
const int  leye_up2 = 13;
const int  leye_down0 = 3;
const int  leye_down1 = 4;
const int  leye_down2 = 5;
const int  reye_left = 31;
const int  reye_right = 24;
const int  reye_up0 = 29;
const int  reye_up1 = 28;
const int  reye_up2 = 27;
const int  reye_down0 = 21;
const int  reye_down1 = 20;
const int  reye_down2 = 19;
const int  leyeboll0 = 48;
const int  reyeboll0 = 52;


const int  leyebrow0 = 32;
const int  leyebrow1 = 36;
const int  leyebrow2 = 35;
const int  leyebrow3 = 39;

const int  reyebrow0 = 43;
const int  reyebrow1 = 47;
const int  reyebrow2 = 40;
const int  reyebrow3 = 44;

const int  noseleftFixed = 121;
const int  noserightFixed = 113;

const int  hleftFixed = 128;
const int  hleft1Fixed = 126;
const int  hcenterFixed = 125;
const int  hrightFixed = 122;
const int  hright1Fixed = 124;


const int  browleftFixed = 127;
const int  browrightFixed = 123;

const int  mouthupFixed = 105;
const int  mouthdownFixed = 111;

const int  vtopFixed = 102;
const int  vupFixed = 105;
const int  vdownFixed = 106;

const int  mouleft = 95;
const int  mouright = 86;
const int  mouthup8 = 87;
const int  mouthup7 = 88;
const int  mouthup6 = 89;
const int  mouthup5 = 90;
const int  mouthup4 = 91;
const int  mouthup3 = 92;
const int  mouthup2 = 93;
const int  mouthup1 = 94;
const int  mouthup0 = 56;

const int  mouthdown8 = 77;
const int  mouthdown7 = 78;
const int  mouthdown6 = 79;
const int  mouthdown5 = 80;
const int  mouthdown4 = 81;
const int  mouthdown3 = 82;
const int  mouthdown2 = 83;
const int  mouthdown1 = 84;
const int  mouthdown0 = 85;


const int  mouCornerleft = 97;
const int  mouCornerright = 100;
const int  vbot = 134;
const int  vleft = 130;

const int  vleft1 = 131;
const int  vleft2 = 132;
const int  vleft3 = 133;

const int  vright = 138;

const int  vright1 = 137;
const int  vright2 = 136;
const int  vright3 = 135;


extern bool IsTaskwork;
//5.3 diff
const FString EV5_0 = "5.0";
const FString EV5_1 = "5.1";
const FString EV5_2 = "5.2";
const FString EV5_3 = "5.3";
const FString EV5_4 = "5.4";
const FString EV5_5 = "5.5";