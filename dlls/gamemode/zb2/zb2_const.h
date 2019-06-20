
#ifndef ZB2_CONST_H
#define ZB2_CONST_H
#ifdef _WIN32
#pragma once
#endif

enum ZB2MessageType : byte
{
	ZB2_MESSAGE_HEALTH_RECOVERY,
	ZB2_MESSAGE_SKILL_INIT,
	ZB2_MESSAGE_SKILL_ACTIVATE

};

enum ZombieSkillSlot : byte
{
	SKILL_SLOT_CLASS,
	SKILL_SLOT_1,
	SKILL_SLOT_2,
	SKILL_SLOT_3,
	SKILL_SLOT_4,
};

enum ZombieClassType : byte
{
	ZOMBIE_CLASS_HUMAN,
	ZOMBIE_CLASS_TANK,
	ZOMBIE_CLASS_SPEED,
	ZOMBIE_CLASS_HEAVY,
	ZOMBIE_CLASS_PC,
	ZOMBIE_CLASS_HEAL,
	ZOMBIE_CLASS_DEIMOS,
	ZOMBIE_CLASS_DEIMOS2,

	MAX_ZOMBIE_CLASS
};

enum ZombieSkillType : byte
{
	ZOMBIE_SKILL_EMPTY,
	ZOMBIE_SKILL_SPRINT,
	ZOMBIE_SKILL_HEADSHOT,
	ZOMBIE_SKILL_KNIFE2X,
	ZOMBIE_SKILL_CRAZY,
	ZOMBIE_SKILL_HIDE,
	ZOMBIE_SKILL_TRAP,
	ZOMBIE_SKILL_SMOKE,
	ZOMBIE_SKILL_HEAL,
	ZOMBIE_SKILL_SHOCK,
	ZOMBIE_SKILL_CRAZY2,

	MAX_ZOMBIE_SKILL
};

enum ZombieSkillStatus : byte
{
	SKILL_STATUS_READY,
	SKILL_STATUS_USING,
	SKILL_STATUS_FREEZING,
	SKILL_STATUS_USED
};

#endif
