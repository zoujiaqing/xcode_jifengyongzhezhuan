

#pragma once

#ifndef STATE_EVENT_TYPE_H
#define STATE_EVENT_TYPE_H

//  state event type
enum EventType
{
    ET_None = -1,

    ToIdle	= 0,
    ToRun	= 1,
	ToDie,

	ToSkillBegin,

	ToBaseAttack,

    ToNormalAttack,
	ToSpecialAttack,
	ToGreatFireBall,
	ToCalving,
	ToBlackHole,
	ToTeleport,
	ToChopseries,
	ToAssassination,
	ToFatalDarkKnives,
	ToWhirlwind,
	ToStepSword,
	ToEarthSplitter,
	ToLinkTimeHandler,
	
	// buf
	ToIronDefense,
	ToInvincibility,
	ToInvisibleKiller,
	ToReviveInvincible,
	ToApocalypse,
	ToExtremeSpeed,

	// magic effect
	ToGreatMagicFireBall,
	ToNormalMagicFireBall,
	ToCalvingMagic,

	ToAnimHandler,
	ToMagicAnimHandler,
	ToAttackCheckerHandler,
	ToMagicCheckHandler,
	ToAttackSenderHandler,
	ToEffectAnimHandler,
	ToRoleEffectHandler,
	ToAimDirectionHandler,

	ToSkillBeginSenderHandler,
	ToSkillBeginReceiverHandler,
	ToMoveToHandler,
	ToVariableSpeedMoveToHandler,

	ToWhirlstorm,
	ToFirehalfmoon,
	ToIceGround,
	ToIceGroundMagic,

	ToDoSkill,
	ToDead,
	ToRevival,
	ToLinkTime,

	ToSkillEnd,

	ToElfSkillBegin,

	ToBaseElfAttack,
	ToElfChase,
	ToElfCloseTo,
	ToElfSkillAttack,
	ToElfEffectAnimHandler,
	ToElfAimDirectionHandler,
	ToElfAnimHandler,
	ToElfAttackTrigger,
	ToElfMagicCheckHandler,
	ToAssistElfAttackTrigger,
	ToAssistElfAttack,

	ToElfSkillEnd,

    // add more here!
};


static bool bET_Table[][3] = {
 //         | idle | run | fight|  
/*idle */   {false, false, false },
/*run  */   {false, false, false },
/*fight*/   {false, false, false }
};


#endif
