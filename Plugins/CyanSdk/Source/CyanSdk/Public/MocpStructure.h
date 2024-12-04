

#pragma once
#include "CoreMinimal.h"
#include "Components/SkinnedMeshComponent.h"
#include "MocpConstVar.h"
#include "GroomAsset.h"
#include "Materials/MaterialInterface.h"
#include "GroomBindingAsset.h"
#include "Engine/DeveloperSettings.h"
#include "MocpStructure.generated.h"

UENUM(BlueprintType)
enum class EFingerrotationAngle : uint8
{

	Pitch = 0,
	Yaw,
	Roll

};

 struct POSIT
{
	float x;
	float y;
	float z;
};

 struct ST_BODY
{
	POSIT         POSEJOINT[PoseCount];
	short         CEMAIL[PoseCount];
	POSIT         LHANDJOINT[HandCount];
	POSIT         RHANDJOINT[HandCount + 10];
	short         bFilled;
	int           _fid;
};


 struct FJs_ModelSelect
 {
	 int selectType;
	 int selectID;
	 int selectNum;
	 FString IPaddress;
	 FString Language;

 };

USTRUCT(BlueprintType)
struct FJS_ModelskeletonSetting
{
	GENERATED_BODY()
	FJS_ModelskeletonSetting();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletonBinding", meta = (JsonPropertyName = "naval", DisplayName = "naval", MultiLine = "true", Tooltip = "pelvis上第一个骨骼点,必填"))
	FName naval;															 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletonBinding", meta=(JsonPropertyName = "lowchest",DisplayName = "lowchest"			, MultiLine = "true", Tooltip ="低胸骨骼点,在naval上处的骨骼，没有可不填"))
	FName lowchest;															 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletonBinding", meta=(JsonPropertyName = "chest",DisplayName = "chest"				, MultiLine = "true", Tooltip ="胸部骨骼点，必填"))
	FName chest;															 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletonBinding", meta=(JsonPropertyName = "pelvis",DisplayName = "pelvis"			, MultiLine = "true", Tooltip = "骨盆骨骼点，必填"))
	FName pelvis;															 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletonBinding", meta=(JsonPropertyName = "neck",DisplayName = "neck"				, MultiLine = "true", Tooltip ="脖子骨骼点，必填"))
	FName neck;																 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletonBinding", meta=(JsonPropertyName = "leftclivc",DisplayName = "leftclivc"			, MultiLine = "true", Tooltip ="左锁骨骨骼点，必填"))
	FName leftclivc;														 	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletonBinding", meta=(JsonPropertyName = "leftshld",DisplayName = "leftshld"			, MultiLine = "true", Tooltip ="左上臂骨骼点，必填"))
	FName leftshld;															 										
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletonBinding", meta=(JsonPropertyName = "leftelblow",DisplayName = "leftelblow"			, MultiLine = "true", Tooltip ="左下臂骨骼点，必填"))
	FName leftelblow;														 									
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletonBinding", meta=(JsonPropertyName = "leftwrist",DisplayName = "leftwrist"			, MultiLine = "true", Tooltip ="左手骨骼点，必填"))
	FName leftwrist;														 										
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletonBinding", meta=(JsonPropertyName = "rightclivc",DisplayName = "rightclivc"			, MultiLine = "true", Tooltip ="右锁骨骨骼点，必填"))
	FName rightclivc;														 										
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletonBinding", meta=(JsonPropertyName = "rightshld",DisplayName = "rightshld"			, MultiLine = "true", Tooltip ="右上臂骨骼点，必填"))
	FName rightshld;														 									
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletonBinding", meta=(JsonPropertyName = "rightelblow",DisplayName = "rightelblow"		, MultiLine = "true", Tooltip ="右下臂骨骼点，必填"))
	FName rightelblow;														 										
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletonBinding", meta=(JsonPropertyName = "rightwrist",DisplayName = "rightwrist"			, MultiLine = "true", Tooltip ="右手骨骼点，必填"))
	FName rightwrist;														 											
																			 											
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletonBinding", meta=(JsonPropertyName = "lefthip",DisplayName = "lefthip"			, MultiLine = "true", Tooltip ="左大腿骨骼点，必填"))
	FName lefthip;															 											
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletonBinding", meta=(JsonPropertyName = "leftkneel",DisplayName = "leftkneel"			, MultiLine = "true", Tooltip ="左小腿骨骼点，必填"))
	FName leftkneel;														 										
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletonBinding", meta=(JsonPropertyName = "ankleleft",DisplayName = "ankleleft"			, MultiLine = "true", Tooltip ="左脚踝骨骼点，必填"))
	FName ankleleft;														 										
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletonBinding", meta=(JsonPropertyName = "footleft",DisplayName = "footleft"			, MultiLine = "true", Tooltip ="左脚尖骨骼点，必填"))
	FName footleft;															 										
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletonBinding", meta=(JsonPropertyName = "righthip",DisplayName = "righthip"			, MultiLine = "true", Tooltip ="右大腿骨骼点，必填"))
	FName righthip;															 											
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletonBinding", meta=(JsonPropertyName = "rightkneel",DisplayName = "rightkneel"			, MultiLine = "true", Tooltip ="右小腿骨骼点，必填"))
	FName rightkneel;														 									
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletonBinding", meta=(JsonPropertyName = "ankleright",DisplayName = "ankleright"			, MultiLine = "true", Tooltip ="右脚踝骨骼点，必填"))
	FName ankleright;														 											
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletonBinding", meta=(JsonPropertyName = "footright",DisplayName = "footright"			, MultiLine = "true", Tooltip ="右脚尖骨骼点，必填"))
	FName footright;														 											
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletonBinding", meta=(JsonPropertyName = "head",DisplayName = "head"				, MultiLine = "true", Tooltip ="头部骨骼点，必填"))
	FName head;																 											
																			 											
																			 											
																			 											
																			 											
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletonBinding", meta=(JsonPropertyName = "leftArmAdd03",DisplayName = "leftArmAdd03"		, MultiLine = "true", Tooltip ="左边手肘至手部骨骼之间的节点，选填，可增加手部计算精度"))
	FName leftArmAdd03;														 										
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletonBinding", meta=(JsonPropertyName = "leftArmAdd04",DisplayName = "leftArmAdd04"		, MultiLine = "true", Tooltip ="左边手肘至手部骨骼之间的节点，选填，可增加手部计算精度"))
	FName leftArmAdd04;														 											
																			 											
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletonBinding", meta=(JsonPropertyName = "rightArmAdd03",DisplayName = "rightArmAdd03"		, MultiLine = "true", Tooltip ="右边手肘至手部骨骼之间的节点，选填，可增加手部计算精度"))
	FName rightArmAdd03;													 											
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletonBinding", meta=(JsonPropertyName = "rightArmAdd04",DisplayName = "rightArmAdd04"		, MultiLine = "true", Tooltip ="右边手肘至手部骨骼之间的节点，选填，可增加手部计算精度"))
	FName rightArmAdd04;													 											
																			 											
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletonBinding", meta=(JsonPropertyName = "leftLegAdd01",DisplayName = "leftLegAdd01"		, MultiLine = "true", Tooltip ="左边大腿至小腿之间的骨骼节点，选填，可增加脚部计算精度"))
	FName leftLegAdd01;														 										
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletonBinding", meta=(JsonPropertyName = "leftLegAdd02",DisplayName = "leftLegAdd02"		, MultiLine = "true", Tooltip ="左边大腿至小腿之间的骨骼节点，选填，可增加脚部计算精度"))
	FName leftLegAdd02;														 										
																			 											
																			 											
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletonBinding", meta=(JsonPropertyName = "rightLegAdd01",DisplayName = "rightLegAdd01"		, MultiLine = "true", Tooltip ="右边大腿至小腿之间的骨骼节点，选填，可增加脚部计算精度"))
	FName rightLegAdd01;													 										
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletonBinding", meta=(JsonPropertyName = "rightLegAdd02",DisplayName = "rightLegAdd02"		, MultiLine = "true", Tooltip ="右边大腿至小腿之间的骨骼节点，选填，可增加脚部计算精度"))
	FName rightLegAdd02;													 											
																			 											
																			 										
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletonBinding", meta=(JsonPropertyName = "AvatarIkRoot",DisplayName = "AvatarIkRoot"		, MultiLine = "true", Tooltip ="人物根节点，必填"))
	FName AvatarIkRoot;														 									
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletonBinding", meta=(JsonPropertyName = "l_wrist",DisplayName = "l_wrist"			, MultiLine = "true", Tooltip ="左手骨骼点，必填"))
	FName l_wrist;															 										
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletonBinding", meta=(JsonPropertyName = "l_thumbproximal",DisplayName = "l_thumbproximal"	, MultiLine = "true", Tooltip ="左大拇指第一节，必填"))
	FName l_thumbproximal;													 											
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletonBinding", meta=(JsonPropertyName = "l_thumbintermediate",DisplayName = "l_thumbintermediate", MultiLine = "true", Tooltip ="左大拇指第二节，必填"))
	FName l_thumbintermediate;												 											
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletonBinding", meta=(JsonPropertyName = "l_thumbdistal",DisplayName = "l_thumbdistal"		, MultiLine = "true", Tooltip ="左大拇指第三节，必填"))
	FName l_thumbdistal;													 										
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletonBinding", meta=(JsonPropertyName = "l_thumbtip",DisplayName = "l_thumbtip"			, MultiLine = "true", Tooltip = "左拇指尖，选填"))
	FName l_thumbtip;														 																			
																			 																		
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletonBinding", meta=(JsonPropertyName = "l_indexproximal",DisplayName = "l_indexproximal"	, MultiLine = "true", Tooltip ="左食指第一节，必填"))
	FName l_indexproximal;													 																			
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletonBinding", meta=(JsonPropertyName = "l_indexintermediate",DisplayName = "l_indexintermediate", MultiLine = "true", Tooltip ="左食指第二节，必填"))
	FName l_indexintermediate;												 																		
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletonBinding", meta=(JsonPropertyName = "l_indexdistal",DisplayName = "l_indexdistal"		, MultiLine = "true", Tooltip ="左食指第三节，必填"))
	FName l_indexdistal;													 																		
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletonBinding", meta=(JsonPropertyName = "l_indextip",DisplayName = "l_indextip"			, MultiLine = "true", Tooltip ="左食指尖，选填"))
	FName l_indextip;														 																		
																			 																		
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletonBinding", meta=(JsonPropertyName = "l_middleproximal",DisplayName = "l_middleproximal"	, MultiLine = "true", Tooltip ="左中指第一节，必填"))
	FName l_middleproximal;													 																			
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletonBinding", meta=(JsonPropertyName = "l_middleintermediate",DisplayName = "l_middleintermediate", MultiLine = "true", Tooltip ="左中指第二节，必填"))
	FName l_middleintermediate;												 																			
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletonBinding", meta=(JsonPropertyName = "l_middledistal",DisplayName = "l_middledistal"		, MultiLine = "true", Tooltip ="左中指第三节，必填"))
	FName l_middledistal;													 																		
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletonBinding", meta=(JsonPropertyName = "l_middletip",DisplayName = "l_middletip"		, MultiLine = "true", Tooltip ="左中指尖，选填"))
	FName l_middletip;														 																			
																			 																	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletonBinding", meta=(JsonPropertyName = "l_ringproximal",DisplayName = "l_ringproximal"		, MultiLine = "true", Tooltip ="左无名指第一节，必填"))
	FName l_ringproximal;													 																		
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletonBinding", meta=(JsonPropertyName = "l_ringintermediate",DisplayName = "l_ringintermediate"	, MultiLine = "true", Tooltip ="左无名指第二节，必填"))
	FName l_ringintermediate;												 																		
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletonBinding", meta=(JsonPropertyName = "l_ringdistal",DisplayName = "l_ringdistal"		, MultiLine = "true", Tooltip ="左无名指第三节，必填"))
	FName l_ringdistal;														 																			
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletonBinding", meta=(JsonPropertyName = "l_ringtip",DisplayName = "l_ringtip"			, MultiLine = "true", Tooltip ="左无名指尖，选填"))
	FName l_ringtip;														 																			
																			 																		
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletonBinding", meta=(JsonPropertyName = "l_littleproximal",DisplayName = "l_littleproximal"	, MultiLine = "true", Tooltip ="左小指第一节，必填"))
	FName l_littleproximal;													 																		
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletonBinding", meta=(JsonPropertyName = "l_littleintermediate",DisplayName = "l_littleintermediate", MultiLine = "true", Tooltip ="左小指第二节，必填"))
	FName l_littleintermediate;												 																			
																			 																	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletonBinding", meta=(JsonPropertyName = "l_littledistal",DisplayName = "l_littledistal"		, MultiLine = "true", Tooltip ="左小指第三节，必填"))
	FName l_littledistal;													 																			
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletonBinding", meta=(JsonPropertyName = "l_littletip",DisplayName = "l_littletip"		, MultiLine = "true", Tooltip ="左小指尖，选填"))
	FName l_littletip;																
																					
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletonBinding", meta=(JsonPropertyName = "r_wrist",DisplayName = "r_wrist"			, MultiLine = "true", Tooltip ="右手骨骼点，必填"))
	FName r_wrist;																	
																					
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletonBinding", meta=(JsonPropertyName = "r_thumbproximal",DisplayName = "r_thumbproximal"	, MultiLine = "true", Tooltip ="右大拇指第一节，必填"))
	FName r_thumbproximal;																																
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletonBinding", meta=(JsonPropertyName = "r_thumbintermediate",DisplayName = "r_thumbintermediate", MultiLine = "true", Tooltip ="右大拇指第二节，必填"))
	FName r_thumbintermediate;																															
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletonBinding", meta=(JsonPropertyName = "r_thumbdistal",DisplayName = "r_thumbdistal"		, MultiLine = "true", Tooltip ="右大拇指第三节，必填"))
	FName r_thumbdistal;																																
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletonBinding", meta=(JsonPropertyName = "r_thumbtip",DisplayName = "r_thumbtip"			, MultiLine = "true", Tooltip ="右大拇指尖，选填"))
	FName r_thumbtip;														 																			
   																																					
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletonBinding", meta=(JsonPropertyName = "r_indexproximal",DisplayName = "r_indexproximal"	, MultiLine = "true", Tooltip ="右食指第一节，必填"))
	FName r_indexproximal;																															
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletonBinding", meta=(JsonPropertyName = "r_indexintermediate",DisplayName = "r_indexintermediate", MultiLine = "true", Tooltip ="右食指第二节，必填"))
	FName r_indexintermediate;												  																		
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletonBinding", meta=(JsonPropertyName = "r_indexdistal",DisplayName = "r_indexdistal"		, MultiLine = "true", Tooltip ="右食指第三节，必填"))
	FName r_indexdistal;													 																			
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletonBinding", meta=(JsonPropertyName = "r_indextip",DisplayName = "r_indextip"			, MultiLine = "true", Tooltip ="右食指尖，选填"))
	FName r_indextip;																																
																			 																			
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletonBinding", meta=(JsonPropertyName = "r_middleproximal",DisplayName = "r_middleproximal"	, MultiLine = "true", Tooltip ="右中指第一节，必填"))
	FName r_middleproximal;																															
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletonBinding", meta=(JsonPropertyName = "r_middleintermediate",DisplayName = "r_middleintermediate", MultiLine = "true", Tooltip ="右中指第二节，必填"))
	FName r_middleintermediate;												 																			
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletonBinding", meta=(JsonPropertyName = "r_middledistal",DisplayName = "r_middledistal"	, MultiLine = "true", Tooltip ="右中指第三节，必填"))
	FName r_middledistal;													 																			
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletonBinding", meta=(JsonPropertyName = "r_middletip",DisplayName = "r_middletip"		, MultiLine = "true", Tooltip ="右中指尖，选填"))
	FName r_middletip;														 																			
																																						
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletonBinding", meta=(JsonPropertyName = "r_ringproximal",DisplayName = "r_ringproximal"		, MultiLine = "true", Tooltip ="右无名指第一节，必填"))
	FName r_ringproximal;													  																		
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletonBinding", meta=(JsonPropertyName = "r_ringintermediate",DisplayName = "r_ringintermediate"	, MultiLine = "true", Tooltip ="右无名指第二节，必填"))
	FName r_ringintermediate;												  																		
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletonBinding", meta=(JsonPropertyName = "r_ringdistal",DisplayName = "r_ringdistal"		, MultiLine = "true", Tooltip ="右无名指第三节，必填"))
	FName r_ringdistal;																																
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletonBinding", meta=(JsonPropertyName = "r_ringtip",DisplayName = "r_ringtip"			, MultiLine = "true", Tooltip ="右无名指尖，选填"))
	FName r_ringtip;														 																			
																																						
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletonBinding", meta=(JsonPropertyName = "r_littleproximal",DisplayName = "r_littleproximal"	, MultiLine = "true", Tooltip ="右小指第一节，必填"))
	FName r_littleproximal;													 																		
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletonBinding", meta=(JsonPropertyName = "r_littleintermediate",DisplayName = "r_littleintermediate", MultiLine = "true", Tooltip ="右小指第二节，必填"))
	FName r_littleintermediate;												 																		
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletonBinding", meta=(JsonPropertyName = "r_littledistal",DisplayName = "r_littledistal"	, MultiLine = "true", Tooltip ="右小指第三节，必填"))
	FName r_littledistal;																															
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletonBinding", meta=(JsonPropertyName = "r_littletip",DisplayName = "r_littletip"		, MultiLine = "true", Tooltip ="右小指尖，选填"))
	FName r_littletip;																																	
																																					
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletonBinding", meta=(JsonPropertyName = "l_carpal1",DisplayName = "l_carpal1"			, MultiLine = "true", Tooltip ="腕关节点1，选填"))
	FName l_carpal1;																																	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletonBinding", meta=(JsonPropertyName = "l_carpal2",DisplayName = "l_carpal2"			, MultiLine = "true", Tooltip ="腕关节点2，选填"))
	FName l_carpal2;														 																			
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletonBinding", meta=(JsonPropertyName = "l_carpal3",DisplayName = "l_carpal3"			, MultiLine = "true", Tooltip ="腕关节点3，选填"))
	FName l_carpal3;																																	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletonBinding", meta=(JsonPropertyName = "l_carpal4",DisplayName = "l_carpal4"			, MultiLine = "true", Tooltip ="腕关节点4，选填"))
	FName l_carpal4;														  																		
																																					
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletonBinding", meta=(JsonPropertyName = "r_carpal1",DisplayName = "r_carpal1"			, MultiLine = "true", Tooltip ="腕关节点1，选填"))
	FName r_carpal1;														 																		
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletonBinding", meta=(JsonPropertyName = "r_carpal2",DisplayName = "r_carpal2"			, MultiLine = "true", Tooltip ="腕关节点2，选填"))
	FName r_carpal2;														 																		
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletonBinding", meta=(JsonPropertyName = "r_carpal3",DisplayName = "r_carpal3"			, MultiLine = "true", Tooltip ="腕关节点3，选填"))
	FName r_carpal3;																														
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletonBinding", meta=(JsonPropertyName = "r_carpal4",DisplayName = "r_carpal4"			, MultiLine = "true", Tooltip ="腕关节点4，选填"))
	FName r_carpal4;
};	

//EngLish Vession
USTRUCT(BlueprintType)
struct FJS_ModelskeletonSetting_E
{
	GENERATED_BODY()
	FJS_ModelskeletonSetting_E();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletonBinding", meta = (DisplayName = "naval", MultiLine = "true", Tooltip = "The first skeletal point on the pelvis,required field."))
		FName naval;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletonBinding", meta = (DisplayName = "lowchest", MultiLine = "true", Tooltip = "The lowchest bone point, above the naval bone point, optional field"))
		FName lowchest;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletonBinding", meta = (DisplayName = "chest", MultiLine = "true", Tooltip = "Chest bone point，required field."))
		FName chest;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletonBinding", meta = (DisplayName = "pelvis", MultiLine = "true", Tooltip = "Pelvis bone point，required field."))
		FName pelvis;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletonBinding", meta = (DisplayName = "neck", MultiLine = "true", Tooltip = "Neck bone point，required field."))
		FName neck;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletonBinding", meta = (DisplayName = "leftclivc", MultiLine = "true", Tooltip = "Left clavicle bone point,required field."))
		FName leftclivc;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletonBinding", meta = (DisplayName = "leftshld", MultiLine = "true", Tooltip = "Left upper arm bone point,required field."))
		FName leftshld;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletonBinding", meta = (DisplayName = "leftelblow", MultiLine = "true", Tooltip = "Left lower arm bone point,required field."))
		FName leftelblow;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletonBinding", meta = (DisplayName = "leftwrist", MultiLine = "true", Tooltip = "Left hand bone point，required field."))
		FName leftwrist;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletonBinding", meta = (DisplayName = "rightclivc", MultiLine = "true", Tooltip = "Right clavicle bone point,required field."))
		FName rightclivc;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletonBinding", meta = (DisplayName = "rightshld", MultiLine = "true", Tooltip = "Right upper arm bone point,required field."))
		FName rightshld;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletonBinding", meta = (DisplayName = "rightelblow", MultiLine = "true", Tooltip = "Right lower arm bone point,required field."))
		FName rightelblow;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletonBinding", meta = (DisplayName = "rightwrist", MultiLine = "true", Tooltip = "Right hand bone point，required field."))
		FName rightwrist;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletonBinding", meta = (DisplayName = "lefthip", MultiLine = "true", Tooltip = "Left thigh bone point,，required field."))
		FName lefthip;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletonBinding", meta = (DisplayName = "leftkneel", MultiLine = "true", Tooltip = "Left calf bone point，required field."))
		FName leftkneel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletonBinding", meta = (DisplayName = "ankleleft", MultiLine = "true", Tooltip = "Left ankle bone point，required field."))
		FName ankleleft;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletonBinding", meta = (DisplayName = "footleft", MultiLine = "true", Tooltip = "Left toe bone point，required field."))
		FName footleft;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletonBinding", meta = (DisplayName = "righthip", MultiLine = "true", Tooltip = "Right thigh bone point,，required field."))
		FName righthip;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletonBinding", meta = (DisplayName = "rightkneel", MultiLine = "true", Tooltip = "Right calf bone point，required field."))
		FName rightkneel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletonBinding", meta = (DisplayName = "ankleright", MultiLine = "true", Tooltip = "Right ankle bone point，required field."))
		FName ankleright;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletonBinding", meta = (DisplayName = "footright", MultiLine = "true", Tooltip = "Right toe bone point，required field."))
		FName footright;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletonBinding", meta = (DisplayName = "head", MultiLine = "true", Tooltip = "Head bone point，required field."))
		FName head;




	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletonBinding", meta = (DisplayName = "leftArmAdd03", MultiLine = "true", Tooltip = "Node between left elbow and left hand bone，optional Fields，can improve the calculation precision for the hand."))
		FName leftArmAdd03;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletonBinding", meta = (DisplayName = "leftArmAdd04", MultiLine = "true", Tooltip = "Node between left elbow and left hand bone，optional Fields，can improve the calculation precision for the hand."))
		FName leftArmAdd04;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletonBinding", meta = (DisplayName = "rightArmAdd03", MultiLine = "true", Tooltip = "Node between right elbow and right hand bone，optional Fields，can improve the calculation precision for the hand."))
		FName rightArmAdd03;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletonBinding", meta = (DisplayName = "rightArmAdd04", MultiLine = "true", Tooltip = "Node between right elbow and right hand bone，optional Fields，can improve the calculation precision for the hand."))
		FName rightArmAdd04;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletonBinding", meta = (DisplayName = "leftLegAdd01", MultiLine = "true", Tooltip = "Node between left hip and left kneel, optional field, can improve the calculation precision for the foot."))
		FName leftLegAdd01;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletonBinding", meta = (DisplayName = "leftLegAdd02", MultiLine = "true", Tooltip = "Node between left hip and left kneel, optional field, can improve the calculation precision for the foot."))
		FName leftLegAdd02;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletonBinding", meta = (DisplayName = "rightLegAdd01", MultiLine = "true", Tooltip = "Node between right hip and right kneel, optional field, can improve the calculation precision for the foot."))
		FName rightLegAdd01;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletonBinding", meta = (DisplayName = "rightLegAdd02", MultiLine = "true", Tooltip = "Node between right hip and right kneel, optional field, can improve the calculation precision for the foot."))
		FName rightLegAdd02;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletonBinding", meta = (DisplayName = "AvatarIkRoot", MultiLine = "true", Tooltip = "The root node of the character, required field."))
		FName AvatarIkRoot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletonBinding", meta = (DisplayName = "l_wrist", MultiLine = "true", Tooltip = "Left hand bone point，required field."))
		FName l_wrist;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletonBinding", meta = (DisplayName = "l_thumbproximal", MultiLine = "true", Tooltip = "First knuckle of the left thumb，required field."))
		FName l_thumbproximal;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletonBinding", meta = (DisplayName = "l_thumbintermediate", MultiLine = "true", Tooltip = "Second knuckle of the left thumb，required field."))
		FName l_thumbintermediate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletonBinding", meta = (DisplayName = "l_thumbdistal", MultiLine = "true", Tooltip = "Third knuckle of the left thumb，required field."))
		FName l_thumbdistal;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletonBinding", meta = (DisplayName = "l_thumbtip", MultiLine = "true", Tooltip = "Left thumb tip, optional field."))
		FName l_thumbtip;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletonBinding", meta = (DisplayName = "l_indexproximal", MultiLine = "true", Tooltip = "First knuckle of the left index finger, required field."))
		FName l_indexproximal;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletonBinding", meta = (DisplayName = "l_indexintermediate", MultiLine = "true", Tooltip = "Second knuckle of the left index finger, required field."))
		FName l_indexintermediate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletonBinding", meta = (DisplayName = "l_indexdistal", MultiLine = "true", Tooltip = "Third knuckle of the left index finger, required field."))
		FName l_indexdistal;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletonBinding", meta = (DisplayName = "l_indextip", MultiLine = "true", Tooltip = "Left index finger tip, optional field."))
		FName l_indextip;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletonBinding", meta = (DisplayName = "l_middleproximal", MultiLine = "true", Tooltip = "First knuckle of the left middle finger, required field."))
		FName l_middleproximal;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletonBinding", meta = (DisplayName = "l_middleintermediate", MultiLine = "true", Tooltip = "Second knuckle of the left middle finger, required field."))
		FName l_middleintermediate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletonBinding", meta = (DisplayName = "l_middledistal", MultiLine = "true", Tooltip = "Third knuckle of the left middle finger, required field."))
		FName l_middledistal;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletonBinding", meta = (DisplayName = "l_middletip", MultiLine = "true", Tooltip = "Left middle fingertip, optional field."))
		FName l_middletip;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletonBinding", meta = (DisplayName = "l_ringproximal", MultiLine = "true", Tooltip = "First knuckle of the left ring finger，required field."))
		FName l_ringproximal;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletonBinding", meta = (DisplayName = "l_ringintermediate", MultiLine = "true", Tooltip = "Second knuckle of the left ring finger，required field."))
		FName l_ringintermediate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletonBinding", meta = (DisplayName = "l_ringdistal", MultiLine = "true", Tooltip = "Third knuckle of the left ring finger，required field."))
		FName l_ringdistal;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletonBinding", meta = (DisplayName = "l_ringtip", MultiLine = "true", Tooltip = "Left ring finger tip, optional field."))
		FName l_ringtip;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletonBinding", meta = (DisplayName = "l_littleproximal", MultiLine = "true", Tooltip = "First knuckle of left little finger, required field."))
		FName l_littleproximal;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletonBinding", meta = (DisplayName = "l_littleintermediate", MultiLine = "true", Tooltip = "Second knuckle of left little finger, required field."))
		FName l_littleintermediate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletonBinding", meta = (DisplayName = "l_littledistal", MultiLine = "true", Tooltip = "Third knuckle of left little finger, required field."))
		FName l_littledistal;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletonBinding", meta = (DisplayName = "l_littletip", MultiLine = "true", Tooltip = "Left Little Finger Tip, optional field"))
		FName l_littletip;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletonBinding", meta = (DisplayName = "r_wrist", MultiLine = "true", Tooltip = "Right hand bone point，required field."))
		FName r_wrist;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletonBinding", meta = (DisplayName = "r_thumbproximal", MultiLine = "true", Tooltip = "First knuckle of the Right thumb，required field."))
		FName r_thumbproximal;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletonBinding", meta = (DisplayName = "r_thumbintermediate", MultiLine = "true", Tooltip = "Second knuckle of the Right thumb，required field."))
		FName r_thumbintermediate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletonBinding", meta = (DisplayName = "r_thumbdistal", MultiLine = "true", Tooltip = "Third knuckle of the Right thumb，required field."))
		FName r_thumbdistal;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletonBinding", meta = (DisplayName = "r_thumbtip", MultiLine = "true", Tooltip = "Right thumb tip, optional field."))
		FName r_thumbtip;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletonBinding", meta = (DisplayName = "r_indexproximal", MultiLine = "true", Tooltip = "First knuckle of the right index finger, required field."))
		FName r_indexproximal;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletonBinding", meta = (DisplayName = "r_indexintermediate", MultiLine = "true", Tooltip = "Second knuckle of the right index finger, required field."))
		FName r_indexintermediate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletonBinding", meta = (DisplayName = "r_indexdistal", MultiLine = "true", Tooltip = "Third knuckle of the right index finger, required field."))
		FName r_indexdistal;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletonBinding", meta = (DisplayName = "r_indextip", MultiLine = "true", Tooltip = "Right index finger tip, optional field."))
		FName r_indextip;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletonBinding", meta = (DisplayName = "r_middleproximal", MultiLine = "true", Tooltip = "First knuckle of the right middle finger, required field."))
		FName r_middleproximal;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletonBinding", meta = (DisplayName = "r_middleintermediate", MultiLine = "true", Tooltip = "Second knuckle of the right middle finger, required field."))
		FName r_middleintermediate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletonBinding", meta = (DisplayName = "r_middledistal", MultiLine = "true", Tooltip = "Third knuckle of the right middle finger, required field."))
		FName r_middledistal;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletonBinding", meta = (DisplayName = "r_middletip", MultiLine = "true", Tooltip = "Right middle fingertip, optional field."))
		FName r_middletip;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletonBinding", meta = (DisplayName = "r_ringproximal", MultiLine = "true", Tooltip = "First knuckle of the right ring finger，required field."))
		FName r_ringproximal;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletonBinding", meta = (DisplayName = "r_ringintermediate", MultiLine = "true", Tooltip = "Second knuckle of the right ring finger，required field."))
		FName r_ringintermediate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletonBinding", meta = (DisplayName = "r_ringdistal", MultiLine = "true", Tooltip = "Third knuckle of the right ring finger，required field."))
		FName r_ringdistal;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletonBinding", meta = (DisplayName = "r_ringtip", MultiLine = "true", Tooltip = "Right ring finger tip, optional field."))
		FName r_ringtip;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletonBinding", meta = (DisplayName = "r_littleproximal", MultiLine = "true", Tooltip = "First knuckle of right little finger, required field."))
		FName r_littleproximal;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletonBinding", meta = (DisplayName = "r_littleintermediate", MultiLine = "true", Tooltip = "Second knuckle of right little finger, required field."))
		FName r_littleintermediate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletonBinding", meta = (DisplayName = "r_littledistal", MultiLine = "true", Tooltip = "Third knuckle of right little finger, required field."))
		FName r_littledistal;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletonBinding", meta = (DisplayName = "r_littletip", MultiLine = "true", Tooltip = "Right Little Finger Tip, optional field"))
		FName r_littletip;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletonBinding", meta = (DisplayName = "l_carpal1", MultiLine = "true", Tooltip = "Left Carpal point 1，optional field"))
		FName l_carpal1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletonBinding", meta = (DisplayName = "l_carpal2", MultiLine = "true", Tooltip = "Left Carpal point 2，optional field"))
		FName l_carpal2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletonBinding", meta = (DisplayName = "l_carpal3", MultiLine = "true", Tooltip = "Left Carpal point 3，optional field"))
		FName l_carpal3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletonBinding", meta = (DisplayName = "l_carpal4", MultiLine = "true", Tooltip = "Left Carpal point 4，optional field"))
		FName l_carpal4;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletonBinding", meta = (DisplayName = "r_carpal1", MultiLine = "true", Tooltip = "Right Carpal point 1，optional field"))
		FName r_carpal1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletonBinding", meta = (DisplayName = "r_carpal2", MultiLine = "true", Tooltip = "Right Carpal point 2，optional field"))
		FName r_carpal2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletonBinding", meta = (DisplayName = "r_carpal3", MultiLine = "true", Tooltip = "Right Carpal point 3，optional field"))
		FName r_carpal3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletonBinding", meta = (DisplayName = "r_carpal4", MultiLine = "true", Tooltip = "Right Carpal point 4，optional field"))
		FName r_carpal4;
};
USTRUCT(BlueprintType)
struct FJS_Modifycharacterparaneters
{
	GENERATED_BODY()
	FJS_Modifycharacterparaneters();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BodyParameterSettings", meta = (JsonPropertyName = "CacheEnabled", DisplayName = "控制是否停用初始化缓存", MultiLine = "true", Tooltip = "默认false为停用，true为启用"))
    bool CacheEnabled = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BodyParameterSettings", meta = (JsonPropertyName = "floor", DisplayName = "调整人物与地板的高度差", MultiLine = "true", Tooltip = "数值越大，人物被抬升越多，反之人物就会不断下降"))
	float floor=0.0f;
	float footdis=0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BodyParameterSettings", meta = (JsonPropertyName = "kneelwide", DisplayName = "调整人物两腿间距", MultiLine = "true", Tooltip = "数值大于1的时候,两腿间距拉长，小于1的时候则两腿间距缩短，建议数值：0.8 ~ 1.2"))
	float kneelwide=0.0f;
	float initialization=0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BodyParameterSettings", meta = (JsonPropertyName = "tyny", DisplayName = "调整小腿拉扯", MultiLine = "true", Tooltip = "当模型腿的尺寸特别小比如卡通类短腿模型，设置成1 ，可防止与真人匹配时拉扯小腿"))
	float tyny=0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BodyParameterSettings", meta = (JsonPropertyName = "elblowrorate", DisplayName = "调整肘关节旋转比率", MultiLine = "true", Tooltip = "默认值为0.45，数值为0-1"))
	float elblowrorate=0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BodyParameterSettings", meta = (JsonPropertyName = "sholder", DisplayName = "调整肩膀凹陷", MultiLine = "true", Tooltip = "默认值为1 ，范围是0-2，参数大于1时肩膀上抬，小于1时肩膀下降"))
	float sholder=0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BodyParameterSettings", meta = (JsonPropertyName = "faceUpward", DisplayName = "调整模型的脖子旋转", MultiLine = "true", Tooltip = "默认值为1 ，范围是0.5-1.5，参数大于1时头往上抬，小于1时头往下旋转"))
	float faceUpward=0.0f;
};

USTRUCT(BlueprintType)
struct FJS_Modifycharacterparaneters_E
{
	GENERATED_BODY()
		FJS_Modifycharacterparaneters_E();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BodyParameterSettings", meta = (DisplayName = "Control whether to disable initial caching", MultiLine = "true", Tooltip = "By default, false disables it, and true enables it."))
		bool CacheEnabled = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BodyParameterSettings", meta = (DisplayName = "Adjust the height difference between the figure and the floor.", MultiLine = "true", Tooltip = "The higher the value, the more the character is raised, and the lower the value, the more the character is lowered. Recommended values range from -5 to 10."))
		float floor=0.0f;
	float footdis=0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BodyParameterSettings", meta = (DisplayName = "Adjust the distance between the legs of the character", MultiLine = "true", Tooltip = "If the value is greater than 1, the distance between the two legs will be lengthened. If it is less than 1, the distance between the two legs will be shortened. The recommended value range is 0.8 to 1.2."))
		float kneelwide=0.0f;
	float initialization=0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BodyParameterSettings", meta = (DisplayName = "Adjusting calf pulls", MultiLine = "true", Tooltip = "Whether to force the initialization of the  character, if necessary, you can set the value to 1, it will force the initialization of the character, this behavior is to ensure that the  character can be driven properly, if it is not 1, the initialization will be done according to whether there is a cache file or not, if there is a problem with the initialization of the character, you can enable him, do not enable it, then select 0"))
		float tyny=0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BodyParameterSettings", meta = (DisplayName = "Adjusting the elbow rotation ratio", MultiLine = "true", Tooltip = "Default value is 0.45, recommended value is 0-1"))
		float elblowrorate=0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BodyParameterSettings", meta = (DisplayName = "Adjustment of shoulder indentations.", MultiLine = "true", Tooltip = "The default value is 1 , the range is 0-2, the parameter is greater than 1 when the shoulder is raised, less than 1 when the shoulder is lowered"))
		float sholder=0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BodyParameterSettings", meta = (DisplayName = "Modifying the neck rotation of the model.", MultiLine = "true", Tooltip = "The default value is 1, with a range from 0.5 to 1.5. When the parameter exceeds 1, the head is elevated, and when it's below 1, the head is rotated downward."))
		float faceUpward=0.0f;
};

USTRUCT(BlueprintType)
struct FJS_ModelSettings
{
	GENERATED_BODY()
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ModelSettings", meta = (JsonPropertyName = "UseManualSkeletonUpdate", DisplayName = "是否启用手动骨骼刷新", MultiLine = "true", Tooltip = "手动骨骼刷新可以减少性能损耗 但是会有可能会使额外的动画蓝图插件不兼容 不勾：不启用 勾：启用"))
		bool UseManualSkeletonUpdate = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ModelSettings", meta = (JsonPropertyName = "BasedAnimTickOptionType", DisplayName = "模型渲染模式设置", MultiLine = "true", Tooltip = "AlwaysTickPoseAndRefreshBones: 无论渲染与否，总是更新和刷新骨骼,AlwaysTickPose:总是更新但骨骼只在渲染时刷新,OnlyTickMontagesWhenNotRendered:当渲染时，更新姿势和刷新骨骼转换， 否则，只需更新蒙太奇，跳过其他一切。 (AnimBP图形不会更新),OnlyTickPoseWhenRendered:只在渲染的时候更新，渲染的时候只会刷骨骼"))
		EVisibilityBasedAnimTickOption BasedAnimTickOptionType = EVisibilityBasedAnimTickOption::AlwaysTickPose;
};

USTRUCT(BlueprintType)
struct FJS_ModelSettings_E
{
	GENERATED_BODY()
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ModelSettings", meta = (DisplayName = "Whether to enable manual bone refresh. ", MultiLine = "true", Tooltip = "Manual bone refreshing reduces performance loss but can potentially make additional animation blueprint plugins incompatible Unchecked: not enabled Checked: enabled"))
		bool UseManualSkeletonUpdate = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ModelSettings", meta = (DisplayName = "Character Rendering Mode Settings", MultiLine = "true", Tooltip = "AlwaysTickPoseAndRefreshBones: Always update and refresh bones whether rendered or not, AlwaysTickPose:Always update but bones are only refreshed at render time,OnlyTickMontagesWhenNotRendered:When rendered Tick Pose and Refresh Bone Transforms,otherwise, just update montages and skip everything else.(AnimBP graph will not be updated),OnlyTickPoseWhenRendered:Tick only when rendered, and it will only RefreshBoneTransforms when rendered."))
		EVisibilityBasedAnimTickOption BasedAnimTickOptionType = EVisibilityBasedAnimTickOption::AlwaysTickPose;
};

USTRUCT(BlueprintType)
struct FHandSettings
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HandSettings", meta = (JsonPropertyName = "isEnable", DisplayName = "手部设置开关", MultiLine = "true", Tooltip = "默认关闭，启用的时候手部设置参数修改才生效，注：仅握拳错误的模型才需要启用该设置！"))
	bool isEnable = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HandSettings", meta = (JsonPropertyName = "bR90L", DisplayName = "左手手指外翻修正", MultiLine = "true", Tooltip = "如果左手手指外翻，可以优先更改该设置 "))
	bool bR90L = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HandSettings", meta = (JsonPropertyName = "bR90R", DisplayName = "右手手指外翻修正", MultiLine = "true", Tooltip = "如果右手手指外翻，可以优先更改该设置 "))
	bool bR90R = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HandSettings", meta = (JsonPropertyName = "ifingbend", DisplayName = "调整模型手指握拳的旋转角度", MultiLine = "true", Tooltip = "用于调整该模型手指握拳的旋转角度 "))
		EFingerrotationAngle ifingbend = EFingerrotationAngle::Pitch;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HandSettings", meta = (JsonPropertyName = "ifingshift", DisplayName = "调整模型手指水平移动时的角度", MultiLine = "true", Tooltip = "用于调整该模型手指水平移动时的角度 "))
		EFingerrotationAngle ifingshift = EFingerrotationAngle::Pitch;
	
};

USTRUCT(BlueprintType)
struct FHandSettings_E
{
	GENERATED_BODY()
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HandSettings", meta = (DisplayName = "Hand Setting Switch", MultiLine = "true", Tooltip = "The default setting is off. This setting should only be enabled for character where the five fingers of the hand make an error in making a fist."))
		bool isEnable = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HandSettings", meta = (DisplayName = "Correction of finger ectropion on the left hand", MultiLine = "true", Tooltip = "If the fingers of the left hand have an externally rotated error, you can prioritize changing this setting."))
		bool bR90L = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HandSettings", meta = (DisplayName = "Correction of finger ectropion on the right hand", MultiLine = "true", Tooltip = "If the fingers of the right hand have an externally rotated error, you can prioritize changing this setting."))
		bool bR90R = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HandSettings", meta = (DisplayName = "Adjusting the rotation angle of the character's finger grip", MultiLine = "true", Tooltip = "Used to adjust the angle of rotation of the character's finger clenched fist."))
		EFingerrotationAngle ifingbend = EFingerrotationAngle::Pitch;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HandSettings", meta = (DisplayName = "Adjust the angle of the character's finger when moving horizontally", MultiLine = "true", Tooltip = "Used to adjust the angle of the character's finger when moving horizontally."))
		EFingerrotationAngle ifingshift = EFingerrotationAngle::Pitch;

};
USTRUCT(BlueprintType)
struct FJS_ModelhandSettings
{
	GENERATED_BODY()
	bool isEnable = false;
	bool bR90L = false;
	bool bR90R = false;
	int ifingbend= 0;
	int ifingshift=0;
};

USTRUCT(BlueprintType)
//ARkit face standard structure
struct FFaceChangeValue
{
	GENERATED_BODY()
	UPROPERTY()
	//ARkit 52 expressions
	float   eyeBlinkLeft=0.0f;      //左眼眨眼
	UPROPERTY()
	float   eyeLookDownLeft=0.0f;      //左眼目视下方
	UPROPERTY()
	float   eyeLookInLeft=0.0f;      //左眼注视鼻尖
	UPROPERTY()
	float   eyeLookOutLeft=0.0f;      //左眼向左看
	UPROPERTY()
	float   eyeLookUpLeft=0.0f;      //左眼目视上方
	UPROPERTY()
	float   eyeSquintLeft=0.0f;      //左眼眯眼
	UPROPERTY()
	float   eyeWideLeft=0.0f;      //左眼睁大
	UPROPERTY()
	float   eyeBlinkRight = 0.0f;      //右眼眨眼
	UPROPERTY()
	float   eyeLookDownRight = 0.0f;      //右眼目视下方
	UPROPERTY()
	float   eyeLookInRight = 0.0f;      //右眼注视鼻尖
	UPROPERTY()
	float   eyeLookOutRight = 0.0f;      //右眼向左看
	UPROPERTY()
	float   eyeLookUpRight = 0.0f;      //右眼目视上方
	UPROPERTY()
	float   eyeSquintRight = 0.0f;      //右眼眯眼
	UPROPERTY()
	float   eyeWideRight = 0.0f;      //右眼睁大
	UPROPERTY()
	float   jawForward = 0.0f;      //努嘴时下巴向前
	UPROPERTY()
	float   jawLeft = 0.0f;      //撇嘴时下巴向左
	UPROPERTY()
	float   jawRight = 0.0f;      //撇嘴时下巴向右
	UPROPERTY()
	float   jawOpen = 0.0f;      //张嘴时下巴向下
	UPROPERTY()
	float   mouthClose = 0.0f;      //闭嘴
	UPROPERTY()
	float   mouthFunnel = 0.0f;      //稍张嘴并双唇张开
	UPROPERTY()
	float   mouthPucker = 0.0f;      //抿嘴
	UPROPERTY()
	float   mouthLeft = 0.0f;      //向左撇嘴
	UPROPERTY()
	float   mouthRight = 0.0f;      //向右撇嘴
	UPROPERTY()
	float   mouthSmileLeft = 0.0f;      //左撇嘴笑
	UPROPERTY()
	float   mouthSmileRight = 0.0f;      //右撇嘴笑
	UPROPERTY()
	float   mouthFrownLeft = 0.0f;      //左嘴唇下压
	UPROPERTY()
	float   mouthFrownRight = 0.0f;      //右嘴唇下压
	UPROPERTY()
	float   mouthDimpleLeft = 0.0f;      //左嘴唇向后
	UPROPERTY()
	float   mouthDimpleRight = 0.0f;      //右嘴唇向后
	UPROPERTY()
	float   mouthStretchLeft = 0.0f;      //左嘴角向左
	UPROPERTY()
	float   mouthStretchRight = 0.0f;      //右嘴角向右
	UPROPERTY()
	float   mouthRollLower = 0.0f;      //下嘴唇卷向里
	UPROPERTY()
	float   mouthRollUpper = 0.0f;      //下嘴唇卷向上
	UPROPERTY()
	float   mouthShrugLower = 0.0f;      //下嘴唇向下
	UPROPERTY()
	float   mouthShrugUpper = 0.0f;      //上嘴唇向上
	UPROPERTY()
	float   mouthPressLeft = 0.0f;      //下嘴唇压向左
	UPROPERTY()
	float   mouthPressRight = 0.0f;      //下嘴唇压向右
	UPROPERTY()
	float   mouthLowerDownLeft = 0.0f;      //下嘴唇压向左下
	UPROPERTY()
	float   mouthLowerDownRight = 0.0f;      //下嘴唇压向右下
	UPROPERTY()
	float   mouthUpperUpLeft = 0.0f;      //上嘴唇压向左上
	UPROPERTY()
	float   mouthUpperUpRight = 0.0f;      //上嘴唇压向右上
	UPROPERTY()
	float   browDownLeft = 0.0f;      //左眉向外
	UPROPERTY()
	float   browDownRight = 0.0f;      //右眉向外
	UPROPERTY()
	float   browInnerUp = 0.0f;      //蹙眉(描述两个眉毛内部向上运动的系数。)
	UPROPERTY()
	float   browOuterUpLeft = 0.0f;      //左眉向左上(描述左眉毛外侧向上运动的系数。)
	UPROPERTY()
	float   browOuterUpRight = 0.0f;      //右眉向右上(描述右眉外侧向上运动的系数。)
	UPROPERTY()
	float   cheekPuff = 0.0f;      //脸颊向外
	UPROPERTY()
	float   cheekSquintLeft = 0.0f;      //左脸颊向上并回旋
	UPROPERTY()
	float   cheekSquintRight = 0.0f;      //右脸颊向上并回旋
	UPROPERTY()
	float   noseSneerLeft = 0.0f;      //左蹙鼻子
	UPROPERTY()
	float   noseSneerRight = 0.0f;      //右蹙鼻子
	UPROPERTY()
	float   tongueOut = 0.0f;      //吐舌头


	// 重置所有浮点数属性到初始值
	void ResetValues()
	{
		for (TFieldIterator<FProperty> PropIt(FFaceChangeValue::StaticStruct()); PropIt; ++PropIt)
		{
			FProperty* Property = *PropIt;
			if (FNumericProperty* NumProperty = CastField<FNumericProperty>(Property))
			{
				if (NumProperty->IsFloatingPoint()) // 仅重置浮点数属性
				{
					NumProperty->SetFloatingPointPropertyValue(Property->ContainerPtrToValuePtr<void>(this), 0.0f);
				}
			}
		}
	}




};

struct FMathFingerName
{
	FMathFingerName();

	//Left
	FName l_ringproximal;
	FName l_ringintermediate;
	FName l_ringdistal;

	FName l_middleproximal;
	FName l_middleintermediate;
	FName l_middledistal;

	FName l_littleproximal;
	FName l_littleintermediate;
	FName l_littledistal;

	//Right
	FName r_ringproximal;
	FName r_ringintermediate;
	FName r_ringdistal;

	FName r_middleproximal;
	FName r_middleintermediate;
	FName r_middledistal;

	FName r_littleproximal;
	FName r_littleintermediate;
	FName r_littledistal;

};
USTRUCT(BlueprintType)
struct FModelPartResource
{
	GENERATED_BODY()
		// 组件的名称，用于识别对应的USkeletalMeshComponent
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Part Resources")
		FName PartName;
		// 组件的骨骼网格
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Part Resources")
		TSoftObjectPtr<USkeletalMesh> Mesh;

	    // 组件使用的材料
	    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Part Resources")
		TArray<TSoftObjectPtr<UMaterialInterface>> Materials;
};

USTRUCT(BlueprintType)
struct FGroomResource
{
	GENERATED_BODY()
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Groom")
		FName ComponentName;
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Groom")
		TSoftObjectPtr<UGroomAsset> GroomAsset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Groom")
		TSoftObjectPtr<UGroomBindingAsset> GroomBindingAsset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Groom")
		TArray<TSoftObjectPtr<UMaterialInterface>> Materials;
};
USTRUCT(BlueprintType)
struct FChildActorResource
{
	GENERATED_BODY()

	// 子角色组件的蓝图
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChildActor")
	TSoftClassPtr<AActor> ChildActorBlueprint;

	// 子角色名称，用于唯一标识和查找特定子角色组件
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChildActor")
		FName ChildActorName;
};

USTRUCT(BlueprintType)
struct FModelResource
{
	GENERATED_BODY()

		// 身体部分的骨骼网格
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resources")
		TSoftObjectPtr<USkeletalMesh> BodyMesh;

	// 身体部分的动画蓝图
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resources")
		TSoftClassPtr<UAnimInstance> BodyAnimationBlueprint;

	// 身体部分使用的材料
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resources")
		TArray<TSoftObjectPtr<UMaterialInterface>> BodyMaterials;

	// 脸部的骨骼网格
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resources")
		TSoftObjectPtr<USkeletalMesh> FaceMesh;

	// 脸部的动画蓝图
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resources")
		TSoftClassPtr<UAnimInstance> FaceAnimationBlueprint;

	// 脸部使用的材料
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resources")
		TArray<TSoftObjectPtr<UMaterialInterface>> FaceMaterials;

	// 其他组件的骨骼网格和材料
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resources")
		TArray<FModelPartResource> OtherParts;
	//毛发相关：
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resources")
		TArray<FGroomResource> GroomResources;

	// 子角色组件资源 毛发 灯光 装饰等 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resources")
		TArray<FChildActorResource> ChildActorResources;
};

USTRUCT(BlueprintType)
struct FBodyLimitStruct
{
	GENERATED_BODY()
	FBodyLimitStruct();
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BodyLimit", meta = (DisplayName = "LimitedBoneName"))
	FName BoneName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BodyLimit", meta = (DisplayName = "UseLimit"))
	bool IsLimit;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BodyLimit",meta = (DisplayName = "LimitedMinVal",ClampMin = "-360.0",ClampMax = "360.0",UIMin = "-360.0", UIMax = "360.0"))
	FRotator LimitedMinVal;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BodyLimit", meta = (DisplayName = "LimitedMaxVal", ClampMin = "-360.0", ClampMax = "360.0", UIMin = "-360.0", UIMax = "360.0"))
	FRotator LimitedMaxVal;
};

























