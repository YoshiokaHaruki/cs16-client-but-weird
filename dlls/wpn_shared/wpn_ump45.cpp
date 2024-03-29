/***
*
*	Copyright (c) 1996-2002, Valve LLC. All rights reserved.
*
*	This product contains software technology licensed from Id
*	Software, Inc. ("Id Technology").  Id Technology (c) 1996 Id Software, Inc.
*	All Rights Reserved.
*
*   Use, distribution, and modification of this source code and/or resulting
*   object code is restricted to non-commercial enhancements to products from
*   Valve LLC.  All other use, distribution, or modification is prohibited
*   without written permission from Valve LLC.
*
****/

#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "player.h"
#include "weapons.h"
#include "wpn_ump45.h"

enum ump45_e
{
	UMP45_IDLE1,
	UMP45_RELOAD,
	UMP45_DRAW,
	UMP45_SHOOT1,
	UMP45_SHOOT2,
	UMP45_SHOOT3
};

LINK_ENTITY_TO_CLASS(weapon_ump45, CUMP45)

void CUMP45::Spawn(void)
{
	pev->classname = MAKE_STRING("weapon_ump45");

	Precache();
	m_iId = WEAPON_UMP45;
	SET_MODEL(ENT(pev), "models/w_ump45.mdl");

	m_iDefaultAmmo = UMP45_DEFAULT_GIVE;
	m_flAccuracy = 0;
	m_bDelayFire = false;

	FallInit();
}

void CUMP45::Precache(void)
{
	PRECACHE_MODEL("models/v_ump45.mdl");
	PRECACHE_MODEL("models/w_ump45.mdl");

	PRECACHE_SOUND("weapons/ump45-1.wav");
	PRECACHE_SOUND("weapons/ump45_clipout.wav");
	PRECACHE_SOUND("weapons/ump45_clipin.wav");
	PRECACHE_SOUND("weapons/ump45_boltslap.wav");

	m_iShell = PRECACHE_MODEL("models/pshell.mdl");
	m_usFireUMP45 = PRECACHE_EVENT(1, "events/ump45.sc");
}

int CUMP45::GetItemInfo(ItemInfo *p)
{
	p->pszName = STRING(pev->classname);
	p->pszAmmo1 = "45acp";
	p->iMaxAmmo1 = MAX_AMMO_45ACP;
	p->pszAmmo2 = NULL;
	p->iMaxAmmo2 = -1;
	p->iMaxClip = UMP45_MAX_CLIP;
	p->iSlot = 0;
	p->iPosition = 15;
	p->iId = m_iId = WEAPON_UMP45;
	p->iFlags = 0;
	p->iWeight = UMP45_WEIGHT;

	return 1;
}

BOOL CUMP45::Deploy(void)
{
	m_flAccuracy = 0;
	m_bDelayFire = false;
	iShellOn = 1;

	return DefaultDeploy("models/v_ump45.mdl", "models/p_ump45.mdl", UMP45_DRAW, "carbine", UseDecrement() != FALSE);
}

void CUMP45::PrimaryAttack(void)
{
	if (!FBitSet(m_pPlayer->pev->flags, FL_ONGROUND))
		UMP45Fire((0.24) * m_flAccuracy, 0.1, FALSE);
	else
		UMP45Fire((0.04) * m_flAccuracy, 0.1, FALSE);
}

void CUMP45::UMP45Fire(float flSpread, float flCycleTime, BOOL fUseAutoAim)
{
	m_bDelayFire = true;
	m_iShotsFired++;
	m_flAccuracy = ((float)(m_iShotsFired * m_iShotsFired * m_iShotsFired) / 210.0) + 0.5;

	if (m_flAccuracy > 1)
		m_flAccuracy = 1;

	if (m_iClip <= 0)
	{
		if (m_fFireOnEmpty)
		{
			PlayEmptySound();
			m_flNextPrimaryAttack = UTIL_WeaponTimeBase() + 0.2;
		}

		return;
	}

	m_iClip--;
	m_pPlayer->pev->effects |= EF_MUZZLEFLASH;
#ifndef CLIENT_DLL
	m_pPlayer->SetAnimation(PLAYER_ATTACK1);
#endif

	UTIL_MakeVectors(m_pPlayer->pev->v_angle + m_pPlayer->pev->punchangle);

	Vector vecSrc = m_pPlayer->GetGunPosition();
	Vector vecDir = m_pPlayer->FireBullets3(vecSrc, gpGlobals->v_forward, flSpread, 8192, 1, BULLET_PLAYER_45ACP, 30, 0.82, m_pPlayer->pev, FALSE, m_pPlayer->random_seed);

	int flags;
#ifdef CLIENT_WEAPONS
	flags = FEV_NOTHOST;
#else
	flags = 0;
#endif

	PLAYBACK_EVENT_FULL(flags, m_pPlayer->edict(), m_usFireUMP45, 0, (float *)&g_vecZero, (float *)&g_vecZero, vecDir.x, vecDir.y, (int)(m_pPlayer->pev->punchangle.x * 100), (int)(m_pPlayer->pev->punchangle.y * 100), FALSE, FALSE);

	m_pPlayer->m_iWeaponVolume = NORMAL_GUN_VOLUME;
	m_pPlayer->m_iWeaponFlash = DIM_GUN_FLASH;
	m_flNextPrimaryAttack = m_flNextSecondaryAttack = UTIL_WeaponTimeBase() + flCycleTime;

#ifndef CLIENT_DLL
	if (!m_iClip && m_pPlayer->m_rgAmmo[m_iPrimaryAmmoType] <= 0)
		m_pPlayer->SetSuitUpdate("!HEV_AMO0", FALSE, 0);
#endif
	m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + 2;

	if (!FBitSet(m_pPlayer->pev->flags, FL_ONGROUND))
		KickBack(0.125, 0.65, 0.55, 0.0475, 5.5, 4.0, 10);
	else if (m_pPlayer->pev->velocity.Length2D() > 0)
		KickBack(0.55, 0.3, 0.225, 0.03, 3.5, 2.5, 10);
	else if (FBitSet(m_pPlayer->pev->flags, FL_DUCKING))
		KickBack(0.25, 0.175, 0.125, 0.02, 2.25, 1.25, 10);
	else
		KickBack(0.275, 0.2, 0.15, 0.0225, 2.5, 1.5, 10);
}

void CUMP45::Reload(void)
{
	if (m_pPlayer->ammo_45acp <= 0)
		return;

	if (DefaultReload(UMP45_MAX_CLIP, UMP45_RELOAD, 3.5))
	{
#ifndef CLIENT_DLL
		m_pPlayer->SetAnimation(PLAYER_RELOAD);
#endif
		m_flAccuracy = 0;
		m_iShotsFired = 0;
	}
}

void CUMP45::WeaponIdle(void)
{
	ResetEmptySound();
	m_pPlayer->GetAutoaimVector(AUTOAIM_10DEGREES);

	if (m_flTimeWeaponIdle > UTIL_WeaponTimeBase())
		return;

	m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + 20;
	SendWeaponAnim(UMP45_IDLE1, UseDecrement() != FALSE);
}
