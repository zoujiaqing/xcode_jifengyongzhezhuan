#pragma  once

class SkillUIManager
{
public :	
	virtual ~SkillUIManager();
	static SkillUIManager* getInstance(void);
	static void Destroy();

	void ResetValue();

	void sendGetUserSkillReqMessage(bool bSender);
	void sendAddUserSkillReqMessage(unsigned int skillId,unsigned int levelId);
	void sendUserSkillDiffReqMessage(unsigned int skillId,unsigned int levelId);

	void sendGetSlotReqMessage();
	void sendSetSlotAttributeMessage(unsigned int slotIndex,unsigned int skillId);

	void InsertOneSkillItemToLua(unsigned int skillId,unsigned int skillLevel);	
private:
	SkillUIManager();
private:
	bool mIsSendSkillReqMessage;
};