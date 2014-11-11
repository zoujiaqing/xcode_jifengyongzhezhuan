#pragma once

// Note: 取值范围 1200001 -  1200100
enum{
	SKILL_SLOT_1 = 1200001,
	SKILL_SLOT_2 ,
	SKILL_SLOT_3 ,
	SKILL_SLOT_4 ,
	SKILL_SLOT_5 ,

	NEW_SKILL_ID = 1200010,
};

/// <summary>
//	网络协议 OnceSet 字段Key范围枚举
/// </summary>
enum ENetOnceSetProtoKeyRange
{
	kType_MainLandStory_Key = 2000,
	kType_PopupReward_Key = 3000,
};