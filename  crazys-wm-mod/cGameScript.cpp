/*
* Copyright 2009, 2010, The Pink Petal Development Team.
* The Pink Petal Devloment Team are defined as the game's coders
* who meet on http://pinkpetal.org     // old site: http://pinkpetal .co.cc
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "cGameScript.h"
#include "interface/cChoiceMessage.h"
#include "src/buildings/cBrothel.h"
#include "GameFlags.h"
#include "cGirls.h"
#include "cInventory.h"
#include "interface/cWindowManager.h"
#include "cGangs.h"
#include "cScriptUtils.h"
#include "cGirlGangFight.h"
#include "cGirlTorture.h"
#include "src/screens/cScreenGirlDetails.h"
#include "src/screens/cScreenBrothelManagement.h"
#include "cCustomers.h"
#include "src/Game.hpp"
#include "src/sStorage.hpp"

extern int g_TalkCount;
extern cRng g_Dice;
extern cScreenGirlDetails* g_GirlDetails;

extern cSurnameList g_SurnameList;

sScript *cGameScript::Process(sScript *Script)
{
	stringstream ss;
	ss << "Script: " << Script->m_Type;
	g_LogFile.write(ss.str());
	// Jump to function based on action type
	switch (Script->m_Type)
	{
	case 0:		return Script_Dialog(Script);
	case 1:		return Script_Init(Script);
	case 2:		return Script_EndInit(Script);
	case 3:		return Script_EndScript(Script);
	case 4:		return Script_Restart(Script);
	case 5:		return Script_ChoiceBox(Script);
	case 6:		return Script->m_Next;	// the TEXT command only is used for adding choice box options, so shouldn't ever be executed
	case 7:		return Script_SetVar(Script);
	case 8:		return Script_SetVarRandom(Script);
	case 9:		return Script_IfVar(Script);
	case 10:	return Script_Else(Script);
	case 11:	return Script_EndIf(Script);
	case 12:	return Script_ActivateChoice(Script);
	case 13:	return Script_IfChoice(Script);
	case 14:	return Script_SetPlayerSuspision(Script);
	case 15:	return Script_SetPlayerDisposition(Script);
	case 16:	return Script_ClearGlobalFlag(Script);
	case 17:	return Script_AddCustToDungeon(Script);
	case 18:	return Script_AddRandomGirlToDungeon(Script);
	case 19:	return Script_SetGlobal(Script);
	case 20:	return Script_SetGirlFlag(Script);
	case 21:	return Script_AddRandomValueToGold(Script);
	case 22:	return Script_AddManyRandomGirlsToDungeon(Script);
	case 23:	return Script_AddTargetGirl(Script);
	case 24:	return Script_AdjustTargetGirlStat(Script);
	case 25:	return Script_PlayerRapeTargetGirl(Script);
	case 26:	return Script_GivePlayerRandomSpecialItem(Script);
	case 27:	return Script_IfPassSkillCheck(Script);
	case 28:	return Script_IfPassStatCheck(Script);
	case 29:	return Script_IfGirlFlag(Script);
	case 30:	return Script_GameOver(Script);
	case 31:	return Script_IfGirlStat(Script);
	case 32:	return Script_IfGirlSkill(Script);
	case 33:	return Script_IfHasTrait(Script);
	case 34:	return Script_TortureTarget(Script);
	case 35:	return Script_ScoldTarget(Script);
	case 36:	return Script_NormalSexTarget(Script);
	case 37:	return Script_BeastSexTarget(Script);
	case 38:	return Script_AnalSexTarget(Script);
	case 39:	return Script_BDSMSexTarget(Script);
	case 40:	return Script_IfNotDisobey(Script);
	case 41:	return Script_GroupSexTarget(Script);
	case 42:	return Script_LesbianSexTarget(Script);
	case 43:	return Script_OralSexTarget(Script);
	case 44:	return Script_StripTarget(Script);
	case 45:	return Script_CleanTarget(Script);
	case 46:	return Script_NudeTarget(Script);
	case 47:	return Script_MastTarget(Script);
	case 48:	return Script_CombatTarget(Script);
	case 49:	return Script_TittyTarget(Script);
	case 50:	return Script_DeathTarget(Script);
	case 51:	return Script_ProfileTarget(Script);
	case 52:	return Script_HandJobTarget(Script);
	case 53:	return Script_EcchiTarget(Script);
	case 54:	return Script_BunnyTarget(Script);
	case 55:	return Script_CardTarget(Script);
	case 56:	return Script_MilkTarget(Script);
	case 57:	return Script_WaitTarget(Script);
	case 58:	return Script_SingTarget(Script);
	case 59:	return Script_TorturePicTarget(Script);
	case 60:	return Script_FootTarget(Script);
	case 61:	return Script_BedTarget(Script);
	case 62:	return Script_FarmTarget(Script);
	case 63:	return Script_HerdTarget(Script);
	case 64:	return Script_CookTarget(Script);
	case 65:	return Script_CraftTarget(Script);
	case 66:	return Script_SwimTarget(Script);
	case 67:	return Script_BathTarget(Script);
	case 68:	return Script_NurseTarget(Script);
	case 69:	return Script_FormalTarget(Script);
	case 70:	return Script_AddFamilyToDungeon(Script);
	case 71:	return Script_AddTrait(Script);
	case 72:	return Script_RemoveTrait(Script);
	case 73:	return Script_AddTraitTemp(Script);
	case 74:	return Script_ShopTarget(Script);
	case 75:	return Script_MagicTarget(Script);
	case 76:	return Script_SignTarget(Script);
	case 77:	return Script_PresentedTarget(Script);
	case 78:	return Script_GetRandomGirl(Script);
	case 79:	return Script_DomTarget(Script);
	case 80:	return Script_AdjustGirlFlag(Script);
	case 81:	return Script_AdjustTraitTemp(Script);
	case 82:	return Script_AdjustTargetGirlSkill(Script);
	case 83:	return Script_DeepTarget(Script);
	case 84:	return Script_EatOutTarget(Script);
	case 85:	return Script_StrapOnTarget(Script);
	case 86:	return Script_Les69ingTarget(Script);
	case 87:	return Script_DildoTarget(Script);
	case 88:	return Script_SubTarget(Script);
	case 89:	return Script_LickTarget(Script);
	case 90:	return Script_SuckBallsTarget(Script);
	case 91:	return Script_CowGirlTarget(Script);
	case 92:	return Script_RevCowGirlTarget(Script);
	case 93:	return Script_SexDoggyTarget(Script);
	case 94:	return Script_NormalSexWithRandomTarget(Script);
	case 95:	return Script_IfGirlHasItem(Script);					// `J` new .06.02.55
	case 96:	return Script_AddItemtoGirl(Script);					// `J` new .06.02.55
	case 97:	return Script_GivePlayerItem(Script);					// `J` new .06.02.55
	case 98:	return Script_IfPlayerHasItem(Script);					// `J` new .06.02.57
	case 99:	return Script_GiveGirlInvItem(Script);					// `J` new .06.02.57
	case 100:	return Script_IfGirlIsSlave(Script);					// `J` new .06.02.57
	case 101:	return Script_IfGirlIsFree(Script);						// `J` new .06.02.57
	case 102:	return Script_GiveGoldToGirl(Script);					// `J` new .06.02.57
	case 103:	return Script_AdjustTargetGirlStatR(Script);			// `J` new .06.02.57
	case 104:	return Script_AdjustTargetGirlSkillR(Script);			// `J` new .06.02.57
	case 105:	return Script_EndIfNew(Script);							// `J` new .06.03.00 to correct for shifting of lines in the old editor
	case 106:	return Script_IfGirlStatus(Script);						// `J` new .06.03.00
	case 107:	return Script_SetGirlStatus(Script);					// `J` new .06.03.00
	case 108:	return Script_EndPregnancy(Script);						// `J` new .06.03.00
	case 109:	return Script_CreatePregnancy(Script);					// `J` new .06.03.00
	case 110:	return Script_ElseNew(Script);							// `J` new .06.03.01
	case 111:	return Script_BrandTarget(Script);						// `J` new .06.03.01 for DarkArk
	case 112:	return Script_RapeTarget(Script);						// `J` new .06.03.01 for DarkArk
	case 113:	return Script_RapeBeastTarget(Script);					// `J` new .06.03.01 for DarkArk
	case 114:	return Script_BirthHumanTarget(Script);					// `J` new .06.03.01 for DarkArk
	case 115:	return Script_BirthHumanMultipleTarget(Script);			// `J` new .06.03.01 for DarkArk
	case 116:	return Script_BirthBeastTarget(Script);					// `J` new .06.03.01 for DarkArk
	case 117:	return Script_ImpregSexTarget(Script);					// `J` new .06.03.01 for DarkArk
	case 118:	return Script_ImpregGroupTarget(Script);				// `J` new .06.03.01 for DarkArk
	case 119:	return Script_ImpregBDSMTarget(Script);					// `J` new .06.03.01 for DarkArk
	case 120:	return Script_ImpregBeastTarget(Script);				// `J` new .06.03.01 for DarkArk
	case 121:	return Script_VirginSexTarget(Script);					// `J` new .06.03.01 for DarkArk
	case 122:	return Script_VirginGroupTarget(Script);				// `J` new .06.03.01 for DarkArk
	case 123:	return Script_VirginBDSMTarget(Script);					// `J` new .06.03.01 for DarkArk
	case 124:	return Script_VirginBeastTarget(Script);				// `J` new .06.03.01 for DarkArk
	case 125:	return Script_EscortTarget(Script);						// `J` new .06.03.02 for grishnak
	case 126:	return Script_SportTarget(Script);						// `J` new .06.03.02 for grishnak
	case 127:	return Script_StudyTarget(Script);						// `J` new .06.03.02 for grishnak
	case 128:	return Script_TeacherTarget(Script);					// `J` new .06.03.02 for grishnak

	// `J` When modifying Scripts, search for "J-Change-Scripts"  :  found in >> cGameScript.h
	// `J` When modifying Image types, search for "J-Change-Image-Types"  :  found in >> cGameScript.h

	default: return Script->m_Next;	// `J` if a script type is not found, skip it.
	}

	return nullptr; // Error executing
}

void cGameScript::RunScript()
{
	m_Leave = false;
	m_NestLevel = 0;
	sScript* curr = m_CurrPos;

	// Scan through script and process functions
	while (curr != nullptr && !m_Leave && m_Active)
	{
		curr = Process(curr);
	}
	if (m_Active == false)
	{
		Release();
	}
}
sScript *cGameScript::Script_Dialog(sScript *Script)
{
	string text = Script->m_Entries[0].m_Text;
	string replacestring = "*GIRLNAME*";
	string replacewith = (m_GirlTarget ? m_GirlTarget->m_Realname : "The Girl");
	int f = -1;
	do {
		f = text.find(replacestring);
		if (f > -1)	text.replace(f, replacestring.length(), replacewith);
	} while (f > -1);

	g_Game->push_message(text, COLOR_BLUE);
	return Script->m_Next; // Go to next script action
}

sScript *cGameScript::Script_Init(sScript *Script)
{
	return Script->m_Next; // Go to next script action
}

sScript *cGameScript::Script_EndInit(sScript *Script)
{
	return Script->m_Next; // Go to next script action
}
sScript *cGameScript::Script_EndScript(sScript *Script)
{
	m_Active = false;
	m_Leave = true;
	return Script->m_Next; // Go to next script action
}

sScript *cGameScript::Script_ChoiceBox(sScript *Script)
{
	int value[2];
	value[0] = (Script->m_Entries[0].m_Var == 1 ? m_Vars[Script->m_Entries[0].m_lValue] : Script->m_Entries[0].m_lValue);
	value[1] = (Script->m_Entries[1].m_Var == 1 ? m_Vars[Script->m_Entries[1].m_lValue] : Script->m_Entries[1].m_lValue);

	std::vector<std::string> options;
	int id = value[0];
	int num = value[1];
	int size = 0;
	Script = Script->m_Next;

	string replacestring = "*GIRLNAME*";
	string replacewith = (m_GirlTarget ? m_GirlTarget->m_Realname : "The Girl");
	for (int i = 0; i < num; i++)
	{
		string text = Script->m_Entries[0].m_Text;
		int f = -1;
		do { f = text.find(replacestring); if (f > -1)	text.replace(f, replacestring.length(), replacewith); } while (f > -1);
		int newlen = text.length();
		if (newlen > size) size = newlen;
		options.push_back(text);
		Script = Script->m_Next;
	}
	while(m_ChoiceBoxes.size() < id + 1) {
        m_ChoiceBoxes.push_back({});
	}
    m_ChoiceBoxes[id] = sChoiceBox{"", std::move(options)};

	return Script; // Go to next script action
}
sScript *cGameScript::Script_Restart(sScript *Script)
{
	m_Leave = true;
	return Script->m_Next; // Go to next script action
}
sScript *cGameScript::Script_SetVar(sScript *Script)
{
	int value[2];
	value[0] = (Script->m_Entries[0].m_Var == 1 ? m_Vars[Script->m_Entries[0].m_lValue] : Script->m_Entries[0].m_lValue);
	value[1] = (Script->m_Entries[1].m_Var == 1 ? m_Vars[Script->m_Entries[1].m_lValue] : Script->m_Entries[1].m_lValue);

	// Set value
	m_Vars[value[0]] = value[1];
	return Script->m_Next; // Go to next script action
}
sScript *cGameScript::Script_SetVarRandom(sScript *Script)
{
	int num = 0;
	int value[3];
	value[0] = (Script->m_Entries[0].m_Var == 1 ? m_Vars[Script->m_Entries[0].m_lValue] : Script->m_Entries[0].m_lValue);
	value[1] = (Script->m_Entries[1].m_Var == 1 ? m_Vars[Script->m_Entries[1].m_lValue] : Script->m_Entries[1].m_lValue);
	value[2] = (Script->m_Entries[2].m_Var == 1 ? m_Vars[Script->m_Entries[2].m_lValue] : Script->m_Entries[2].m_lValue);
	num = (Script->m_Entries[1].m_lValue == 0 ? (g_Dice % (value[2] + 1)) + value[1] : (g_Dice % (value[2] + 1)) + value[1] - 1);

	m_Vars[value[0]] = num;
	return Script->m_Next; // Go to next script action
}
sScript *cGameScript::Script_IfVar(sScript *Script)
{
	bool Skipping = false; // Flag for if...then condition
	m_NestLevel++;
	int Nest = m_NestLevel;
	int value[2];

	value[0] = (Script->m_Entries[0].m_Var == 1 ? m_Vars[Script->m_Entries[0].m_lValue] : Script->m_Entries[0].m_lValue);
	value[1] = (Script->m_Entries[2].m_Var == 1 ? m_Vars[Script->m_Entries[2].m_lValue] : Script->m_Entries[2].m_lValue);

	// See if variable matches second entry
	int sel = (Script->m_Entries[1].m_Var == 1 ? m_Vars[Script->m_Entries[1].m_Selection] : Script->m_Entries[1].m_Selection);
	switch (sel)
	{
	case 0:		Skipping = !(m_Vars[value[0]] == value[1]);		break;
	case 1:		Skipping = !(m_Vars[value[0]] <  value[1]);		break;
	case 2:		Skipping = !(m_Vars[value[0]] <= value[1]);		break;
	case 3:		Skipping = !(m_Vars[value[0]] >  value[1]);		break;
	case 4:		Skipping = !(m_Vars[value[0]] >= value[1]);		break;
	case 5:		Skipping = !(m_Vars[value[0]] != value[1]);		break;
	}

	// At this point, Skipping states if the script actions
	// need to be skipped due to a conditional if...then statement.
	// Actions are further processed if skipped = false, looking
	// for an else to flip the skip mode, or an endif to end
	// the conditional block.
	Script = Script->m_Next; // Go to next action to process
	while (Script != nullptr)
	{
		if (m_Leave) break;
		// if else, flip skip mode
		if ((Script->m_Type == 10 || Script->m_Type == 110) && Nest == m_NestLevel) Skipping = !Skipping;

		// break on end if
		if (Script->m_Type == 11 || Script->m_Type == 105)
		{
			if (Nest == m_NestLevel)
			{
				m_NestLevel--;
				return Script->m_Next;
			}
			m_NestLevel--;
		}

		// Process script function in conditional block
		// making sure to skip actions when condition not met.
		if (Skipping)
		{
			if (IsIfStatement(Script->m_Type)) m_NestLevel++;
			Script = Script->m_Next;
		}
		else
		{
			if ((Script = Process(Script)) == nullptr) return nullptr;
		}
	}
	return nullptr; // End of script reached
}
sScript *cGameScript::Script_Else(sScript *Script)
{
	return Script->m_Next; // Go to next script action
}
sScript *cGameScript::Script_ElseNew(sScript *Script)
{
	return Script->m_Next; // Go to next script action
}
sScript *cGameScript::Script_EndIf(sScript *Script)
{
	return Script->m_Next; // Go to next script action
}
sScript *cGameScript::Script_EndIfNew(sScript *Script)
{
	return Script->m_Next; // Go to next script action
}
sScript *cGameScript::Script_ActivateChoice(sScript *Script)
{
    int id = Script->m_Entries[0].m_lValue;
    window_manager().InputChoice(m_ChoiceBoxes[id].Prompt, m_ChoiceBoxes[id].Options,
            [this]( int selected ){ m_LastSelection = selected; });
	return Script->m_Next; // Go to next script action
}

sScript *cGameScript::Script_IfChoice(sScript *Script)
{
	bool Skipping = false; // Flag for if...then condition
	m_NestLevel++;
	int Nest = m_NestLevel;
	int value[2];

	value[0] = (Script->m_Entries[0].m_Var == 1 ? m_Vars[Script->m_Entries[0].m_lValue] : Script->m_Entries[0].m_lValue);
	value[1] = (Script->m_Entries[1].m_Var == 1 ? m_Vars[Script->m_Entries[1].m_lValue] : Script->m_Entries[1].m_lValue);
	// See if choice flag matches second entry
    Skipping = m_LastSelection != value[1];

	// At this point, Skipping states if the script actions
	// need to be skipped due to a conditional if...then statement.
	// Actions are further processed if skipped = false, looking
	// for an else to flip the skip mode, or an endif to end
	// the conditional block.
	Script = Script->m_Next; // Go to next action to process
	while (Script != nullptr)
	{
		if (m_Leave) break;
		// if else, flip skip mode
		if ((Script->m_Type == 10 || Script->m_Type == 110) && Nest == m_NestLevel) Skipping = !Skipping;

		// break on end if
		if (Script->m_Type == 11 || Script->m_Type == 105)
		{
			if (Nest == m_NestLevel)
			{
				m_NestLevel--;
				return Script->m_Next;
			}
			m_NestLevel--;
		}

		// Process script function in conditional block
		// making sure to skip actions when condition not met.
		if (Skipping)
		{
			if (IsIfStatement(Script->m_Type))
				m_NestLevel++;
			Script = Script->m_Next;
		}
		else
		{
			if ((Script = Process(Script)) == nullptr) return nullptr;
		}
	}
	return nullptr; // End of script reached
}
sScript *cGameScript::Script_SetPlayerSuspision(sScript *Script)
{
	int value;
	value = (Script->m_Entries[0].m_Var == 1 ? m_Vars[Script->m_Entries[0].m_lValue] : Script->m_Entries[0].m_lValue);

    g_Game->player().suspicion(value);

	return Script->m_Next;
}
sScript *cGameScript::Script_SetPlayerDisposition(sScript *Script)
{
	int value;
	value = (Script->m_Entries[0].m_Var == 1 ? m_Vars[Script->m_Entries[0].m_lValue] : Script->m_Entries[0].m_lValue);

    g_Game->player().disposition(value);

	return Script->m_Next;
}
sScript *cGameScript::Script_ClearGlobalFlag(sScript *Script)
{
	int value;
	value = (Script->m_Entries[0].m_Var == 1 ? m_Vars[Script->m_Entries[0].m_lValue] : Script->m_Entries[0].m_lValue);

	ClearGameFlag(value);
	return Script->m_Next;
}
sScript *cGameScript::Script_AddCustToDungeon(sScript *Script)
{
	bool wife = false;
	int value[3];
	value[0] = (Script->m_Entries[0].m_Var == 1 ? m_Vars[Script->m_Entries[0].m_Selection] : Script->m_Entries[0].m_Selection);
	value[1] = (Script->m_Entries[1].m_Var == 1 ? m_Vars[Script->m_Entries[1].m_lValue] : Script->m_Entries[1].m_lValue);
	value[2] = (Script->m_Entries[2].m_Var == 1 ? m_Vars[Script->m_Entries[2].m_lValue] : Script->m_Entries[2].m_lValue);
	if (value[2] == 1)
		wife = true;

	if (value[0] == 0)
		g_Game->dungeon().AddCust(DUNGEON_CUSTNOPAY, value[1], wife);
	else if (value[0] == 1)
		g_Game->dungeon().AddCust(DUNGEON_CUSTBEATGIRL, value[1], wife);

	return Script->m_Next;
}
sScript *cGameScript::Script_AddRandomGirlToDungeon(sScript *Script)
{
	int value[7];
	value[0] = (Script->m_Entries[0].m_Var == 1 ? m_Vars[Script->m_Entries[0].m_Selection] : Script->m_Entries[0].m_Selection);
	value[1] = (Script->m_Entries[1].m_Var == 1 ? m_Vars[Script->m_Entries[1].m_lValue] : Script->m_Entries[1].m_lValue);
	value[2] = (Script->m_Entries[2].m_Var == 1 ? m_Vars[Script->m_Entries[2].m_lValue] : Script->m_Entries[2].m_lValue);
	value[3] = (Script->m_Entries[3].m_Var == 1 ? m_Vars[Script->m_Entries[3].m_lValue] : Script->m_Entries[3].m_lValue);
	value[4] = (Script->m_Entries[4].m_Var == 1 ? m_Vars[Script->m_Entries[4].m_lValue] : Script->m_Entries[4].m_lValue);
	value[5] = (Script->m_Entries[5].m_Var == 1 ? m_Vars[Script->m_Entries[5].m_lValue] : Script->m_Entries[5].m_lValue);
	value[6] = (Script->m_Entries[6].m_Var == 1 ? m_Vars[Script->m_Entries[6].m_lValue] : Script->m_Entries[6].m_lValue);

	bool kidnaped = false;
	int reason = 0;
	if (value[0] == 0)
	{
		kidnaped = true;
		reason = DUNGEON_GIRLKIDNAPPED;
	}
	else if (value[0] == 1)
	{
		kidnaped = true;
		reason = DUNGEON_GIRLCAPTURED;
	}

	bool slave = (value[3] == 1);
	bool allowNonHuman = (value[4] == 1);
	bool arena = (value[5] == 1);
	bool daughter = (value[6] == 1);

	// `J` reworking age
	int age = 18;
	int minage = value[1];
	int maxage = value[2];
	if (minage == 100 || maxage == 100)	age = 100;
	else
	{
		if (minage < 18) minage = 18;			// `J` Legal Note: 18 is the Legal Age of Majority for the USA where I live 
		if (minage > 80) minage = 80;
		if (maxage < minage) maxage = minage;
		if (maxage > 80) maxage = 80;
		age = g_Dice.in_range(minage, maxage);
	}

	sGirl* newgirl = g_Game->CreateRandomGirl(age, false, slave, false, allowNonHuman, kidnaped, arena, daughter);
	stringstream NGmsg;
	NGmsg << "(DEBUG:: " << newgirl->m_Realname << " was added by a script.\nLookup code: D_001)";
	newgirl->m_Events.AddMessage(NGmsg.str(), IMGTYPE_PROFILE, EVENT_WARNING);

	g_Game->dungeon().AddGirl(newgirl, reason);

	return Script->m_Next;
}
sScript *cGameScript::Script_SetGlobal(sScript *Script)
{
	SetGameFlag(Script->m_Entries[0].m_lValue, Script->m_Entries[1].m_lValue);
	return Script->m_Next;
}
sScript *cGameScript::Script_SetGirlFlag(sScript *Script)
{
	int value[2];
	value[0] = (Script->m_Entries[0].m_Var == 1 ? m_Vars[Script->m_Entries[0].m_lValue] : Script->m_Entries[0].m_lValue);
	value[1] = (Script->m_Entries[1].m_Var == 1 ? m_Vars[Script->m_Entries[1].m_lValue] : Script->m_Entries[1].m_lValue);

	if (m_GirlTarget == nullptr) return Script->m_Next;	// this shouldn't happen

	m_GirlTarget->m_Flags[value[0]] = (char)value[1];
	return Script->m_Next;
}
sScript *cGameScript::Script_AdjustGirlFlag(sScript *Script)
{
	int value[2];
	value[0] = (Script->m_Entries[0].m_Var == 1 ? m_Vars[Script->m_Entries[0].m_lValue] : Script->m_Entries[0].m_lValue);
	value[1] = (Script->m_Entries[1].m_Var == 1 ? m_Vars[Script->m_Entries[1].m_lValue] : Script->m_Entries[1].m_lValue);

	if (m_GirlTarget == nullptr) return Script->m_Next;	// this shouldn't happen

	m_GirlTarget->m_Flags[value[0]] += (char)value[1];
	return Script->m_Next;
}
sScript *cGameScript::Script_AddRandomValueToGold(sScript *Script)
{
	int value[2];
	value[0] = (Script->m_Entries[0].m_Var == 1 ? m_Vars[Script->m_Entries[0].m_lValue] : Script->m_Entries[0].m_lValue);
	value[1] = (Script->m_Entries[1].m_Var == 1 ? m_Vars[Script->m_Entries[1].m_lValue] : Script->m_Entries[1].m_lValue);
	long gold = (value[0] == 0 ? (g_Dice % (value[1] + 1)) + value[0] : (g_Dice % (value[1] + 1)) + value[0] - 1);

	g_Game->gold().misc_credit(gold);

	return Script->m_Next;
}
sScript *cGameScript::Script_AddManyRandomGirlsToDungeon(sScript *Script)
{
	int value[7];
	value[0] = (Script->m_Entries[0].m_Var == 1 ? m_Vars[Script->m_Entries[0].m_lValue] : Script->m_Entries[0].m_lValue);		// how many girls?
	value[1] = (Script->m_Entries[1].m_Var == 1 ? m_Vars[Script->m_Entries[1].m_Selection] : Script->m_Entries[1].m_Selection);	// kidnaped or captured?
	value[2] = (Script->m_Entries[2].m_Var == 1 ? m_Vars[Script->m_Entries[2].m_lValue] : Script->m_Entries[2].m_lValue);		// min age
	value[3] = (Script->m_Entries[3].m_Var == 1 ? m_Vars[Script->m_Entries[3].m_lValue] : Script->m_Entries[3].m_lValue);		// max age
	value[4] = (Script->m_Entries[4].m_Var == 1 ? m_Vars[Script->m_Entries[4].m_lValue] : Script->m_Entries[4].m_lValue);		// slave
	value[5] = (Script->m_Entries[5].m_Var == 1 ? m_Vars[Script->m_Entries[5].m_lValue] : Script->m_Entries[5].m_lValue);		// nonhuman
	value[6] = (Script->m_Entries[6].m_Var == 1 ? m_Vars[Script->m_Entries[6].m_lValue] : Script->m_Entries[6].m_lValue);		// arena

	bool kidnaped = false;
	int reason = 0;
	if (value[1] == 0)		{ kidnaped = true; reason = DUNGEON_GIRLKIDNAPPED; }
	else if (value[1] == 1)	{ kidnaped = true; reason = DUNGEON_GIRLCAPTURED; }

	bool slave = (value[4] == 1);
	bool allowNonHuman = (value[5] == 1);
	bool arena = (value[6] == 1);

	for (int i = 0; i < value[0]; i++)
	{
		int age = (value[2] == 0 ? (g_Dice % (value[3] + 1)) + value[2] : (g_Dice % (value[3] + 1)) + value[2] - 1);

		sGirl* newgirl = g_Game->CreateRandomGirl(age, false, slave, false, allowNonHuman, kidnaped, arena);
		stringstream NGmsg;
		NGmsg << "(DEBUG:: " << newgirl->m_Realname << " was added by a script.\nLookup code: D_002)";
		newgirl->m_Events.AddMessage(NGmsg.str(), IMGTYPE_PROFILE, EVENT_WARNING);

		g_Game->dungeon().AddGirl(newgirl, reason);
	}

	return Script->m_Next;
}
sScript *cGameScript::Script_AddFamilyToDungeon(sScript *Script)
{
	int value[6];
	value[0] = (Script->m_Entries[0].m_Var == 1 ? m_Vars[Script->m_Entries[0].m_lValue] : Script->m_Entries[0].m_lValue);		// how many daughters? Base age 18-25
	value[1] = (Script->m_Entries[1].m_Var == 1 ? m_Vars[Script->m_Entries[1].m_Selection] : Script->m_Entries[1].m_Selection);	// mother taken also?
	value[2] = (Script->m_Entries[2].m_Var == 1 ? m_Vars[Script->m_Entries[2].m_lValue] : Script->m_Entries[2].m_lValue);		// kidnaped or captured?
	value[3] = (Script->m_Entries[3].m_Var == 1 ? m_Vars[Script->m_Entries[3].m_lValue] : Script->m_Entries[3].m_lValue);		// slave
	value[4] = (Script->m_Entries[4].m_Var == 1 ? m_Vars[Script->m_Entries[4].m_lValue] : Script->m_Entries[4].m_lValue);		// nonhuman
	value[5] = (Script->m_Entries[5].m_Var == 1 ? m_Vars[Script->m_Entries[5].m_lValue] : Script->m_Entries[5].m_lValue);		// arena

	bool kidnaped = false;
	int reason = 0;
	if (value[3] == 0)		{ kidnaped = true; reason = DUNGEON_GIRLKIDNAPPED; }
	else if (value[3] == 1)	{ kidnaped = true; reason = DUNGEON_GIRLCAPTURED; }

	bool slave = (value[4] == 1);
	bool allowNonHuman = (value[5] == 1);
	bool arena = (value[6] == 1);

	// Set the surname for the family
	string surname;
	for (int i = 0; i < 5; i++)
	{
		surname = g_SurnameList.random();
		if (i > 3) surname = surname + "-" + g_SurnameList.random();
		if (g_Game->SurnameExists(surname)) continue;
		break;
	}

	// `J` zzzzzz - this can probably be done easier
	sGirl* Daughter1 = nullptr;
	sGirl* Daughter2 = nullptr;
	sGirl* Daughter3 = nullptr;
	sGirl* Mother = nullptr;
	stringstream NGmsg1;
	stringstream NGmsg2;
	stringstream NGmsg3;
	stringstream NGmsgM;

	int oldest = 18;	// `J` Legal Note: 18 is the Legal Age of Majority for the USA where I live 
	if (value[0] > 0)
	{
		Daughter1 = g_Game->CreateRandomGirl((g_Dice % 13) + 13, false, slave, false, allowNonHuman, kidnaped, arena);
		if (Daughter1->age() > oldest) oldest = Daughter1->age();
		Daughter1->m_Surname = surname;
		cGirls::CreateRealName(Daughter1);
	}
	if (value[0] > 1)
	{
		Daughter2 = g_Game->CreateRandomGirl((g_Dice % 13) + 13, false, slave, false, allowNonHuman, kidnaped, arena);
		if (Daughter2->age() == Daughter1->age())	// if only 2 daughters and their ages are the same, change that
		{											// if there is a third daughter, her age can be anything (to allow twins)
			if (Daughter1->age() > 20) Daughter2->age(-(g_Dice % 3 + 1));
			else Daughter2->age((g_Dice % 3 + 1));
		}
		if (Daughter2->age() > oldest) oldest = Daughter2->age();
		Daughter2->m_Surname = surname;
		cGirls::CreateRealName(Daughter2);
	}
	if (value[0] > 2)
	{
		Daughter3 = g_Game->CreateRandomGirl((g_Dice % 13) + 13, false, slave, false, allowNonHuman, kidnaped, arena);
		if (Daughter3->age() > oldest) oldest = Daughter3->age();
		Daughter3->m_Surname = surname;
		cGirls::CreateRealName(Daughter3);
	}

	if (value[1])	// there is a mother
	{
		Mother = g_Game->CreateRandomGirl((g_Dice % (50 - (oldest + 18))) + oldest + 18, false, slave, false, allowNonHuman, kidnaped, arena);	// `J` Legal Note: 18 is the Legal Age of Majority for the USA where I live
		Mother->m_Surname = surname;
		cGirls::CreateRealName(Mother);
		if (!g_Dice.percent(Mother->age())) Mother->add_trait("MILF");	// the younger the mother the more likely she will be a MILF
		Mother->lose_virginity();

		string biography = "Daughter of " + Mother->m_Realname + " and a deadbeat brothel client.";

		if (value[0] > 0)	Daughter1->m_Desc = Daughter1->m_Desc + "\n \n" + biography;
		if (value[0] > 1)	Daughter2->m_Desc = Daughter2->m_Desc + "\n \n" + biography;
		if (value[0] > 2)	Daughter3->m_Desc = Daughter3->m_Desc + "\n \n" + biography;

		if (value[0] > 0)
		{
			Mother->m_ChildrenCount[CHILD00_TOTAL_BIRTHS] += value[0];
			Mother->m_ChildrenCount[CHILD02_ALL_GIRLS] += value[0];
			Mother->m_ChildrenCount[CHILD04_CUSTOMER_GIRLS] += value[0];
		}
	}
	string kstring = (kidnaped ? "kidnapped from her family" : "captured");

	if (value[0] > 0)
	{
		NGmsg1 << Daughter1->m_Realname << " was " << kstring;
		if (value[0] > 1 || value[1])
		{
			NGmsg1 << " along with ";
			if (value[1])					NGmsg1 << "her mother " << Mother->m_Realname;
			if (value[0] > 1 && value[1])	NGmsg1 << " and ";
			if (value[0] > 1)
			{
				NGmsg1 << "her sister" << (value[0] > 2 ? "s " : " ") << Daughter2->m_Realname;
				if (value[0] > 2)			NGmsg1 << " and " << Daughter3->m_Realname;
			}
		}
		NGmsg1 << ".";
		Daughter1->m_Events.AddMessage(NGmsg1.str(), IMGTYPE_PROFILE, EVENT_DUNGEON);
	}
	if (value[0] > 1)
	{
		NGmsg2 << Daughter2->m_Realname << " was " << kstring << " along with ";
		if (value[1]) NGmsg2 << "her mother " << Mother->m_Realname << " and ";
		NGmsg2 << "her sister" << (value[0] > 2 ? "s " : " ") << Daughter1->m_Realname;
		if (value[0] > 2) NGmsg2 << " and " << Daughter3->m_Realname;
		NGmsg2 << ".";
		Daughter2->m_Events.AddMessage(NGmsg2.str(), IMGTYPE_PROFILE, EVENT_DUNGEON);
	}
	if (value[0] > 2)
	{
		NGmsg3 << Daughter3->m_Realname << " was " << kstring << " along with ";
		if (value[1]) NGmsg3 << "her mother " << Mother->m_Realname << " and ";
		NGmsg3 << "her sisters " << Daughter1->m_Realname << " and " << Daughter2->m_Realname << ".";
		Daughter3->m_Events.AddMessage(NGmsg3.str(), IMGTYPE_PROFILE, EVENT_DUNGEON);
	}
	if (value[1])
	{
		NGmsgM << Mother->m_Realname << " was " << kstring;
		if (value[0] > 0)
		{
			NGmsgM << " along with her daughter" << (value[0] > 2 ? "s " : " ") << Daughter1->m_Realname;
			if (value[0] == 2)		NGmsgM << " and ";
			else if (value[0] == 3) NGmsgM << ", ";
			if (value[0] > 1)		NGmsgM << Daughter2->m_Realname;
			if (value[0] > 2)		NGmsgM << " and " << Daughter3->m_Realname;
		}
		NGmsgM << ".";
		Mother->m_Events.AddMessage(NGmsgM.str(), IMGTYPE_PROFILE, EVENT_DUNGEON);
	}

	if (value[0] > 0)	g_Game->dungeon().AddGirl(Daughter1, reason);
	if (value[0] > 1)	g_Game->dungeon().AddGirl(Daughter2, reason);
	if (value[0] > 2)	g_Game->dungeon().AddGirl(Daughter3, reason);
	if (value[1])		g_Game->dungeon().AddGirl(Mother, reason);

	return Script->m_Next;
}
sScript *cGameScript::Script_AddTargetGirl(sScript *Script)
{
	cScriptUtils::add_girl_to_brothel(m_GirlTarget);
	return Script->m_Next;
}
sScript *cGameScript::Script_AdjustTargetGirlStat(sScript *Script)
{
	int value[2];
	value[0] = (Script->m_Entries[0].m_Var == 1 ? m_Vars[Script->m_Entries[0].m_Selection] : Script->m_Entries[0].m_Selection);
	value[1] = (Script->m_Entries[1].m_Var == 1 ? m_Vars[Script->m_Entries[1].m_lValue] : Script->m_Entries[1].m_lValue);
	if (m_GirlTarget)
	{
		if (value[0] - (NUM_STATS) > 0)	// `J` correcting for old scripts for the new STAT_NPCLOVE
			m_GirlTarget->upd_stat(value[0] - NUM_STATS, value[1]);
		else
			m_GirlTarget->upd_stat(value[0], value[1]);
	}
	return Script->m_Next;
}
sScript *cGameScript::Script_AdjustTargetGirlSkill(sScript *Script)
{
	int value[2];
	value[0] = (Script->m_Entries[0].m_Var == 1 ? m_Vars[Script->m_Entries[0].m_Selection] : Script->m_Entries[0].m_Selection);
	value[1] = (Script->m_Entries[1].m_Var == 1 ? m_Vars[Script->m_Entries[1].m_lValue] : Script->m_Entries[1].m_lValue);
	if (m_GirlTarget)
	{
		m_GirlTarget->upd_skill(value[0], value[1]);
	}
	return Script->m_Next;
}
sScript *cGameScript::Script_PlayerRapeTargetGirl(sScript *Script)
{
	if (m_GirlTarget == nullptr) return Script->m_Next;

	m_GirlTarget->bdsm(2);
	m_GirlTarget->anal(2);
	m_GirlTarget->normalsex(2);

	m_GirlTarget->happiness(-5);
	m_GirlTarget->health(-10);
	m_GirlTarget->libido(-1);
	m_GirlTarget->upd_temp_stat(STAT_LIBIDO, 2, true);
	m_GirlTarget->confidence(-1);
	m_GirlTarget->obedience(2);
	m_GirlTarget->pcfear(2);
	m_GirlTarget->pclove(-2);
	m_GirlTarget->pchate(3);

	if (g_Dice.percent(2)) m_GirlTarget->add_trait("Broken Will");

	if (m_GirlTarget->check_virginity()) m_GirlTarget->lose_virginity();	// `J` updated for trait/status

	bool preg = !m_GirlTarget->calc_pregnancy(&g_Game->player(), false, 1.0);
	if (preg) g_Game->push_message(m_GirlTarget->m_Realname + " has gotten pregnant", COLOR_BLUE);
	g_GirlDetails->set_image(IMGTYPE_SEX);

	return Script->m_Next;
}
sScript *cGameScript::Script_GivePlayerRandomSpecialItem(sScript *Script)
{
	sInventoryItem* item = g_Game->inventory_manager().GetRandomItem();
	while (item == nullptr)
		item = g_Game->inventory_manager().GetRandomItem();

	bool ok = false;
	while (!ok)
	{
		if (item->m_Rarity >= RARITYSHOP05) ok = true;
		else
		{
			do { item = g_Game->inventory_manager().GetRandomItem(); } while (item == nullptr);
		}
	}

	if(!g_Game->player().inventory().add_item(item)) {
        g_Game->push_message(" Your inventory is full\n", COLOR_RED);
    }
	return Script->m_Next;
}
sScript *cGameScript::Script_IfPassSkillCheck(sScript *Script)
{
	bool Skipping = false; // Flag for if...then condition
	m_NestLevel++;
	int Nest = m_NestLevel;

	int value = Script->m_Entries[0].m_Selection;

	// See if variable matches second entry
	Skipping = !g_Dice.percent(m_GirlTarget->get_skill(value));

	// At this point, Skipping states if the script actions
	// need to be skipped due to a conditional if...then statement.
	// Actions are further processed if skipped = false, looking
	// for an else to flip the skip mode, or an endif to end
	// the conditional block.
	Script = Script->m_Next; // Go to next action to process
	while (Script != nullptr)
	{
		if (m_Leave) break;
		// if else, flip skip mode
		if ((Script->m_Type == 10 || Script->m_Type == 110) && Nest == m_NestLevel) Skipping = !Skipping;

		// break on end if
		if (Script->m_Type == 11 || Script->m_Type == 105)
		{
			if (Nest == m_NestLevel)
			{
				m_NestLevel--;
				return Script->m_Next;
			}
			m_NestLevel--;
		}

		// Process script function in conditional block
		// making sure to skip actions when condition not met.
		if (Skipping)
		{
			if (IsIfStatement(Script->m_Type)) m_NestLevel++;
			Script = Script->m_Next;
		}
		else
		{
			if ((Script = Process(Script)) == nullptr) return nullptr;
		}
	}
	return nullptr; // End of script reached
}
sScript *cGameScript::Script_IfPassStatCheck(sScript *Script)
{
	bool Skipping = false; // Flag for if...then condition
	m_NestLevel++;
	int Nest = m_NestLevel;

	int value = Script->m_Entries[0].m_Selection;

	// See if variable matches second entry
	Skipping = !g_Dice.percent(m_GirlTarget->get_stat(value));

	// At this point, Skipping states if the script actions
	// need to be skipped due to a conditional if...then statement.
	// Actions are further processed if skipped = false, looking
	// for an else to flip the skip mode, or an endif to end
	// the conditional block.
	Script = Script->m_Next; // Go to next action to process
	while (Script != nullptr)
	{
		if (m_Leave) break;
		// if else, flip skip mode
		if ((Script->m_Type == 10 || Script->m_Type == 110) && Nest == m_NestLevel) Skipping = !Skipping;

		// break on end if
		if (Script->m_Type == 11 || Script->m_Type == 105)
		{
			if (Nest == m_NestLevel)
			{
				m_NestLevel--;
				return Script->m_Next;
			}
			m_NestLevel--;
		}

		// Process script function in conditional block
		// making sure to skip actions when condition not met.
		if (Skipping)
		{
			if (IsIfStatement(Script->m_Type)) m_NestLevel++;
			Script = Script->m_Next;
		}
		else
		{
			if ((Script = Process(Script)) == nullptr) return nullptr;
		}
	}
	return nullptr; // End of script reached
}
sScript* cGameScript::Script_IfGirlFlag(sScript* Script)
{
	bool Skipping = false; // Flag for if...then condition
	m_NestLevel++;
	int Nest = m_NestLevel;
	int value[2];

	value[0] = (Script->m_Entries[0].m_Var == 1 ? m_Vars[Script->m_Entries[0].m_lValue] : Script->m_Entries[0].m_lValue);
	value[1] = (Script->m_Entries[2].m_Var == 1 ? m_Vars[Script->m_Entries[2].m_lValue] : Script->m_Entries[2].m_lValue);

	// See if variable matches second entry
	int sel = (Script->m_Entries[1].m_Var == 1 ? m_Vars[Script->m_Entries[1].m_Selection] : Script->m_Entries[1].m_Selection);
	switch (sel)
	{
	case 0:		Skipping = !(m_GirlTarget->m_Flags[value[0]] == value[1]);		break;
	case 1:		Skipping = !(m_GirlTarget->m_Flags[value[0]] <  value[1]);		break;
	case 2:		Skipping = !(m_GirlTarget->m_Flags[value[0]] <= value[1]);		break;
	case 3:		Skipping = !(m_GirlTarget->m_Flags[value[0]] >  value[1]);		break;
	case 4:		Skipping = !(m_GirlTarget->m_Flags[value[0]] >= value[1]);		break;
	case 5:		Skipping = !(m_GirlTarget->m_Flags[value[0]] != value[1]);		break;
	}

	// At this point, Skipping states if the script actions
	// need to be skipped due to a conditional if...then statement.
	// Actions are further processed if skipped = false, looking
	// for an else to flip the skip mode, or an endif to end
	// the conditional block.
	Script = Script->m_Next; // Go to next action to process
	while (Script != nullptr)
	{
		if (m_Leave) break;
		// if else, flip skip mode
		if ((Script->m_Type == 10 || Script->m_Type == 110) && Nest == m_NestLevel) Skipping = !Skipping;

		// break on end if
		if (Script->m_Type == 11 || Script->m_Type == 105)
		{
			if (Nest == m_NestLevel)
			{
				m_NestLevel--;
				return Script->m_Next;
			}
			m_NestLevel--;
		}

		// Process script function in conditional block
		// making sure to skip actions when condition not met.
		if (Skipping)
		{
			if (IsIfStatement(Script->m_Type)) m_NestLevel++;
			Script = Script->m_Next;
		}
		else
		{
			if ((Script = Process(Script)) == nullptr) return nullptr;
		}
	}
	return nullptr; // End of script reached
}
sScript* cGameScript::Script_GameOver(sScript* Script)
{
	g_Game->push_message("GAME OVER", COLOR_RED);
	window_manager().PopAll();
	m_Active = false;
	m_Leave = true;
	return Script->m_Next;
}
sScript* cGameScript::Script_IfGirlStat(sScript* Script)
{
	bool Skipping = false; // Flag for if...then condition
	m_NestLevel++;
	int Nest = m_NestLevel;
	int value[2];

	value[0] = (Script->m_Entries[0].m_Var == 1 ? m_Vars[Script->m_Entries[0].m_lValue] : Script->m_Entries[0].m_lValue);
	value[1] = (Script->m_Entries[2].m_Var == 1 ? m_Vars[Script->m_Entries[2].m_lValue] : Script->m_Entries[2].m_lValue);

	// See if variable matches second entry
	int sel = (Script->m_Entries[1].m_Var == 1 ? m_Vars[Script->m_Entries[1].m_Selection] : Script->m_Entries[1].m_Selection);
	switch (sel)
	{
	case 0:		Skipping = !(m_GirlTarget->get_stat(value[0]) == value[1]);		break;
	case 1:		Skipping = !(m_GirlTarget->get_stat(value[0]) <  value[1]);		break;
	case 2:		Skipping = !(m_GirlTarget->get_stat(value[0]) <= value[1]);		break;
	case 3:		Skipping = !(m_GirlTarget->get_stat(value[0]) >  value[1]);		break;
	case 4:		Skipping = !(m_GirlTarget->get_stat(value[0]) >= value[1]);		break;
	case 5:		Skipping = !(m_GirlTarget->get_stat(value[0]) != value[1]);		break;
	}

	// At this point, Skipping states if the script actions
	// need to be skipped due to a conditional if...then statement.
	// Actions are further processed if skipped = false, looking
	// for an else to flip the skip mode, or an endif to end
	// the conditional block.
	Script = Script->m_Next; // Go to next action to process
	while (Script != nullptr)
	{
		if (m_Leave) break;
		// if else, flip skip mode
		if ((Script->m_Type == 10 || Script->m_Type == 110) && Nest == m_NestLevel) Skipping = !Skipping;

		// break on end if
		if (Script->m_Type == 11 || Script->m_Type == 105)
		{
			if (Nest == m_NestLevel)
			{
				m_NestLevel--;
				return Script->m_Next;
			}
			m_NestLevel--;
		}

		// Process script function in conditional block
		// making sure to skip actions when condition not met.
		if (Skipping)
		{
			if (IsIfStatement(Script->m_Type)) m_NestLevel++;
			Script = Script->m_Next;
		}
		else
		{
			if ((Script = Process(Script)) == nullptr) return nullptr;
		}
	}
	return nullptr; // End of script reached
}
sScript* cGameScript::Script_IfGirlSkill(sScript* Script)
{
	bool Skipping = false; // Flag for if...then condition
	m_NestLevel++;
	int Nest = m_NestLevel;
	int value[2];

	value[0] = (Script->m_Entries[0].m_Var == 1 ? m_Vars[Script->m_Entries[0].m_lValue] : Script->m_Entries[0].m_lValue);
	value[1] = (Script->m_Entries[2].m_Var == 1 ? m_Vars[Script->m_Entries[2].m_lValue] : Script->m_Entries[2].m_lValue);

	// See if variable matches second entry
	int sel = (Script->m_Entries[1].m_Var == 1 ? m_Vars[Script->m_Entries[1].m_Selection] : Script->m_Entries[1].m_Selection);
	switch (sel)
	{
	case 0:		Skipping = !(m_GirlTarget->get_skill(value[0]) == value[1]);		break;
	case 1:		Skipping = !(m_GirlTarget->get_skill(value[0]) < value[1]);		break;
	case 2:		Skipping = !(m_GirlTarget->get_skill(value[0]) <= value[1]);		break;
	case 3:		Skipping = !(m_GirlTarget->get_skill(value[0]) > value[1]);		break;
	case 4:		Skipping = !(m_GirlTarget->get_skill(value[0]) >= value[1]);		break;
	case 5:		Skipping = !(m_GirlTarget->get_skill(value[0]) != value[1]);		break;
	}

	// At this point, Skipping states if the script actions
	// need to be skipped due to a conditional if...then statement.
	// Actions are further processed if skipped = false, looking
	// for an else to flip the skip mode, or an endif to end
	// the conditional block.
	Script = Script->m_Next; // Go to next action to process
	while (Script != nullptr)
	{
		if (m_Leave) break;
		// if else, flip skip mode
		if ((Script->m_Type == 10 || Script->m_Type == 110) && Nest == m_NestLevel) Skipping = !Skipping;

		// break on end if
		if (Script->m_Type == 11 || Script->m_Type == 105)
		{
			if (Nest == m_NestLevel)
			{
				m_NestLevel--;
				return Script->m_Next;
			}
			m_NestLevel--;
		}

		// Process script function in conditional block
		// making sure to skip actions when condition not met.
		if (Skipping)
		{
			if (IsIfStatement(Script->m_Type)) m_NestLevel++;
			Script = Script->m_Next;
		}
		else
		{
			if ((Script = Process(Script)) == nullptr) return nullptr;
		}
	}
	return nullptr; // End of script reached
}
sScript* cGameScript::Script_IfHasTrait(sScript* Script)
{
	bool Skipping = false; // Flag for if...then condition
	m_NestLevel++;
	int Nest = m_NestLevel;

	Skipping = !m_GirlTarget->has_trait(Script->m_Entries[0].m_Text);

	// At this point, Skipping states if the script actions
	// need to be skipped due to a conditional if...then statement.
	// Actions are further processed if skipped = false, looking
	// for an else to flip the skip mode, or an endif to end
	// the conditional block.
	Script = Script->m_Next; // Go to next action to process
	while (Script != nullptr)
	{
		if (m_Leave) break;
		// if else, flip skip mode
		if ((Script->m_Type == 10 || Script->m_Type == 110) && Nest == m_NestLevel) Skipping = !Skipping;

		// break on end if
		if (Script->m_Type == 11 || Script->m_Type == 105)
		{
			if (Nest == m_NestLevel)
			{
				m_NestLevel--;
				return Script->m_Next;
			}
			m_NestLevel--;
		}

		// Process script function in conditional block
		// making sure to skip actions when condition not met.
		if (Skipping)
		{
			if (IsIfStatement(Script->m_Type)) m_NestLevel++;
			Script = Script->m_Next;
		}
		else
		{
			if ((Script = Process(Script)) == nullptr) return nullptr;
		}
	}
	return nullptr; // End of script reached
}
sScript* cGameScript::Script_TortureTarget(sScript* Script)
{
	cGirlTorture gt(m_GirlTarget);
	g_GirlDetails->set_image(IMGTYPE_TORTURE);

	return Script->m_Next;
}
sScript* cGameScript::Script_ScoldTarget(sScript* Script)
{
	if (m_GirlTarget->spirit() <= 10)
	{
		g_Game->push_message("She is bawling the entire time you yell at her, obviously wanting to do her best", 0);
		m_GirlTarget->happiness(-5);
		m_GirlTarget->confidence(-5);
		m_GirlTarget->obedience(10);
		m_GirlTarget->spirit(-3);
		m_GirlTarget->pclove(-4);
		m_GirlTarget->pcfear(2);
		m_GirlTarget->pchate(2);
	}
	else if (m_GirlTarget->spirit() <= 20)
	{
		g_Game->push_message("She sobs a lot while you yell at her and fearfully listens to your every word", 0);
		m_GirlTarget->happiness(-2);
		m_GirlTarget->confidence(-2);
		m_GirlTarget->obedience(6);
		m_GirlTarget->spirit(-2);
		m_GirlTarget->pclove(-1);
		m_GirlTarget->pcfear(1);
	}
	else if (m_GirlTarget->spirit() <= 30)
	{
		g_Game->push_message("She listens with attention and promises to do better", 0);
		m_GirlTarget->happiness(-1);
		m_GirlTarget->confidence(-1);
		m_GirlTarget->obedience(5);
		m_GirlTarget->spirit(-2);
		m_GirlTarget->pclove(-1);
	}
	else if (m_GirlTarget->spirit() <= 50)
	{
		g_Game->push_message("She listens to what you say but barely pays attention", 0);
		m_GirlTarget->obedience(3);
		m_GirlTarget->spirit(-2);
		m_GirlTarget->pchate(1);
	}
	else if (m_GirlTarget->spirit() <= 80)
	{
		g_Game->push_message("She looks at you defiantly while you yell at her", 0);
		m_GirlTarget->obedience(2);
		m_GirlTarget->spirit(-1);
		m_GirlTarget->pclove(-3);
		m_GirlTarget->pchate(2);
	}
	else
	{
		g_Game->push_message("She stares you down while you yell at her, daring you to hit her", 0);
		m_GirlTarget->obedience(-1);
		m_GirlTarget->spirit(-1);
		m_GirlTarget->pclove(-4);
		m_GirlTarget->pchate(5);
	}

	return Script->m_Next;
}
sScript* cGameScript::Script_NormalSexTarget(sScript* Script)
{
	if (m_GirlTarget)
	{
		m_GirlTarget->normalsex(2);

		if (m_GirlTarget->check_virginity()) m_GirlTarget->lose_virginity();	// `J` updated for trait/status

		if (!m_GirlTarget->calc_pregnancy(&g_Game->player(), false, 1.0))
			g_Game->push_message(m_GirlTarget->m_Realname + " has gotten pregnant", 0);
	}
	g_GirlDetails->set_image(IMGTYPE_SEX);

	return Script->m_Next;
}
sScript* cGameScript::Script_BeastSexTarget(sScript* Script)
{
	if (m_GirlTarget)
	{
		m_GirlTarget->beastiality(1);	// `J` divided skill gain

		if (m_GirlTarget->check_virginity()) m_GirlTarget->lose_virginity();	// `J` updated for trait/status

		// mod: added check for number of beasts owned; otherwise, fake beasts could somehow inseminate the girl
		if (g_Game->storage().beasts() > 0)
		{
			m_GirlTarget->beastiality(1);	// `J` divided skill gain
			if (!m_GirlTarget->calc_insemination(*cGirls::GetBeast(), false, 1.0))
				g_Game->push_message(m_GirlTarget->m_Realname + " has gotten inseminated", 0);
		}
	}
	g_GirlDetails->set_image(IMGTYPE_BEAST);

	return Script->m_Next;
}
sScript* cGameScript::Script_AnalSexTarget(sScript* Script)
{
	if (m_GirlTarget) m_GirlTarget->anal(2);
	g_GirlDetails->set_image(IMGTYPE_ANAL);

	return Script->m_Next;
}
sScript* cGameScript::Script_BDSMSexTarget(sScript* Script)
{
	if (m_GirlTarget)
	{
		m_GirlTarget->bdsm(2);

		if (m_GirlTarget->check_virginity()) m_GirlTarget->lose_virginity();	// `J` updated for trait/status
	}

	if (!m_GirlTarget->calc_pregnancy(&g_Game->player(), false, 0.75))
		g_Game->push_message(m_GirlTarget->m_Realname + " has gotten pregnant", 0);
	g_GirlDetails->set_image(IMGTYPE_BDSM);

	return Script->m_Next;
}
sScript* cGameScript::Script_IfNotDisobey(sScript* Script)
{
	bool Skipping = false; // Flag for if...then condition
	m_NestLevel++;
	int Nest = m_NestLevel;

	// See if choice flag matches second entry
	Skipping = m_GirlTarget->disobey_check(ACTION_GENERAL);

	// At this point, Skipping states if the script actions
	// need to be skipped due to a conditional if...then statement.
	// Actions are further processed if skipped = false, looking
	// for an else to flip the skip mode, or an endif to end
	// the conditional block.
	Script = Script->m_Next; // Go to next action to process
	while (Script != nullptr)
	{
		if (m_Leave) break;
		// if else, flip skip mode
		if ((Script->m_Type == 10 || Script->m_Type == 110) && Nest == m_NestLevel) Skipping = !Skipping;

		// break on end if
		if (Script->m_Type == 11 || Script->m_Type == 105)
		{
			if (Nest == m_NestLevel)
			{
				m_NestLevel--;
				return Script->m_Next;
			}
			m_NestLevel--;
		}

		// Process script function in conditional block
		// making sure to skip actions when condition not met.
		if (Skipping)
		{
			if (IsIfStatement(Script->m_Type)) m_NestLevel++;
			Script = Script->m_Next;
		}
		else
		{
			if ((Script = Process(Script)) == nullptr) return nullptr;
		}
	}
	return nullptr; // End of script reached
}
sScript* cGameScript::Script_GroupSexTarget(sScript* Script)
{
	if (m_GirlTarget)
	{
		m_GirlTarget->group(2);

		if (m_GirlTarget->check_virginity()) m_GirlTarget->lose_virginity();	// `J` updated for trait/status

		if (!m_GirlTarget->calc_group_pregnancy(&g_Game->player(), false, 1.0))
			g_Game->push_message(m_GirlTarget->m_Realname + " has gotten pregnant", 0);

		g_GirlDetails->set_image(IMGTYPE_GROUP);
	}

	return Script->m_Next;
}
sScript* cGameScript::Script_LesbianSexTarget(sScript* Script)
{
	if (m_GirlTarget) m_GirlTarget->lesbian(2);
	g_GirlDetails->set_image(IMGTYPE_LESBIAN);
	return Script->m_Next;
}
sScript* cGameScript::Script_OralSexTarget(sScript* Script)
{
	if (m_GirlTarget) m_GirlTarget->oralsex(2);
	g_GirlDetails->set_image(IMGTYPE_ORAL);
	return Script->m_Next;
}
sScript* cGameScript::Script_StripTarget(sScript* Script)
{
	if (m_GirlTarget) m_GirlTarget->strip(2);
	g_GirlDetails->set_image(IMGTYPE_STRIP);
	return Script->m_Next;
}

sScript* cGameScript::Script_CleanTarget(sScript* Script)
{
	if (m_GirlTarget && m_GirlTarget->m_Building)
	{
		int CleanAmt = (m_GirlTarget->service() >= 10 ? ((m_GirlTarget->service() / 10) + 5) * 10 : 50);
        m_GirlTarget->m_Building->m_Filthiness -= CleanAmt;
		stringstream sstemp;
		sstemp << "Cleanliness rating improved by " << (int)CleanAmt;
		m_GirlTarget->service(1);
	}
	g_GirlDetails->set_image(IMGTYPE_MAID);

	return Script->m_Next;
}
sScript* cGameScript::Script_NudeTarget(sScript* Script)
{
	if (m_GirlTarget) m_GirlTarget->strip(1);
	g_GirlDetails->set_image(IMGTYPE_NUDE);
	return Script->m_Next;
}
sScript* cGameScript::Script_MastTarget(sScript* Script)
{
	if (m_GirlTarget) m_GirlTarget->service(2);
	g_GirlDetails->set_image(IMGTYPE_MAST);
	return Script->m_Next;
}
sScript* cGameScript::Script_CombatTarget(sScript* Script)
{
	if (m_GirlTarget) m_GirlTarget->combat(1);
	g_GirlDetails->set_image(IMGTYPE_COMBAT);
	return Script->m_Next;
}
sScript* cGameScript::Script_TittyTarget(sScript* Script)
{
	if (m_GirlTarget) m_GirlTarget->tittysex(2);
	g_GirlDetails->set_image(IMGTYPE_TITTY);
	return Script->m_Next;
}
sScript* cGameScript::Script_DeathTarget(sScript* Script)
{
	if (m_GirlTarget) m_GirlTarget->combat(0);
	g_GirlDetails->set_image(IMGTYPE_DEATH);
	return Script->m_Next;
}
sScript* cGameScript::Script_ProfileTarget(sScript* Script)
{
	g_GirlDetails->set_image(IMGTYPE_PROFILE);
	return Script->m_Next;
}
sScript* cGameScript::Script_HandJobTarget(sScript* Script)
{
	if (m_GirlTarget) m_GirlTarget->handjob(2);
	g_GirlDetails->set_image(IMGTYPE_HAND);
	return Script->m_Next;
}
sScript* cGameScript::Script_EcchiTarget(sScript* Script)
{
	if (m_GirlTarget){}		//m_GirlTarget->handjob(1);
	g_GirlDetails->set_image(IMGTYPE_ECCHI);
	return Script->m_Next;
}
sScript* cGameScript::Script_BunnyTarget(sScript* Script)
{
	if (m_GirlTarget) m_GirlTarget->performance(1);
	g_GirlDetails->set_image(IMGTYPE_BUNNY);
	return Script->m_Next;
}
sScript* cGameScript::Script_CardTarget(sScript* Script)
{
	if (m_GirlTarget) m_GirlTarget->performance(1);
	g_GirlDetails->set_image(IMGTYPE_CARD);
	return Script->m_Next;
}
sScript* cGameScript::Script_MilkTarget(sScript* Script)
{
	if (m_GirlTarget){}		//m_GirlTarget->handjob(1);
	g_GirlDetails->set_image(IMGTYPE_MILK);
	return Script->m_Next;
}
sScript* cGameScript::Script_WaitTarget(sScript* Script)
{
	if (m_GirlTarget) m_GirlTarget->service(1);
	g_GirlDetails->set_image(IMGTYPE_WAIT);
	return Script->m_Next;
}
sScript* cGameScript::Script_SingTarget(sScript* Script)
{
	if (m_GirlTarget) m_GirlTarget->performance(1);
	g_GirlDetails->set_image(IMGTYPE_SING);
	return Script->m_Next;
}
sScript* cGameScript::Script_TorturePicTarget(sScript* Script)
{
	if (m_GirlTarget){}		//m_GirlTarget->performance(1);
	g_GirlDetails->set_image(IMGTYPE_TORTURE);
	return Script->m_Next;
}
sScript* cGameScript::Script_FootTarget(sScript* Script)
{
	if (m_GirlTarget) m_GirlTarget->footjob(1);
	g_GirlDetails->set_image(IMGTYPE_FOOT);
	return Script->m_Next;
}
sScript* cGameScript::Script_BedTarget(sScript* Script)
{
	if (m_GirlTarget){}		//m_GirlTarget->performance(1);
	g_GirlDetails->set_image(IMGTYPE_BED);
	return Script->m_Next;
}
sScript* cGameScript::Script_FarmTarget(sScript* Script)
{
	if (m_GirlTarget) m_GirlTarget->farming(1);
	g_GirlDetails->set_image(IMGTYPE_FARM);
	return Script->m_Next;
}
sScript* cGameScript::Script_HerdTarget(sScript* Script)
{
	if (m_GirlTarget) m_GirlTarget->animalhandling(1);
	g_GirlDetails->set_image(IMGTYPE_HERD);
	return Script->m_Next;
}
sScript* cGameScript::Script_CookTarget(sScript* Script)
{
	if (m_GirlTarget) m_GirlTarget->cooking(1);
	g_GirlDetails->set_image(IMGTYPE_COOK);
	return Script->m_Next;
}
sScript* cGameScript::Script_CraftTarget(sScript* Script)
{
	if (m_GirlTarget) m_GirlTarget->crafting(1);
	g_GirlDetails->set_image(IMGTYPE_CRAFT);
	return Script->m_Next;
}
sScript* cGameScript::Script_SwimTarget(sScript* Script)
{
	if (m_GirlTarget) m_GirlTarget->strength(1);
	g_GirlDetails->set_image(IMGTYPE_SWIM);
	return Script->m_Next;
}
sScript* cGameScript::Script_BathTarget(sScript* Script)
{
	if (m_GirlTarget){}		//m_GirlTarget->crafting(1);
	g_GirlDetails->set_image(IMGTYPE_BATH);
	return Script->m_Next;
}
sScript* cGameScript::Script_NurseTarget(sScript* Script)
{
	if (m_GirlTarget) m_GirlTarget->medicine(1);
	g_GirlDetails->set_image(IMGTYPE_NURSE);
	return Script->m_Next;
}
sScript* cGameScript::Script_FormalTarget(sScript* Script)
{
	if (m_GirlTarget) m_GirlTarget->refinement(1);
	g_GirlDetails->set_image(IMGTYPE_FORMAL);
	return Script->m_Next;
}
sScript* cGameScript::Script_ShopTarget(sScript* Script)
{
	g_GirlDetails->set_image(IMGTYPE_SHOP);
	return Script->m_Next;
}
sScript* cGameScript::Script_MagicTarget(sScript* Script)
{
	g_GirlDetails->set_image(IMGTYPE_MAGIC);
	return Script->m_Next;
}
sScript* cGameScript::Script_SignTarget(sScript* Script)
{
	g_GirlDetails->set_image(IMGTYPE_SIGN);
	return Script->m_Next;
}
sScript* cGameScript::Script_PresentedTarget(sScript* Script)
{
	g_GirlDetails->set_image(IMGTYPE_PRESENTED);
	return Script->m_Next;
}
sScript* cGameScript::Script_DomTarget(sScript* Script)
{
	g_GirlDetails->set_image(IMGTYPE_DOM);
	return Script->m_Next;
}
sScript* cGameScript::Script_AddTrait(sScript* Script)						// `J` new
{
	if (m_GirlTarget && !m_GirlTarget->has_trait(Script->m_Entries[0].m_Text))
		m_GirlTarget->add_trait(Script->m_Entries[0].m_Text);
	return Script->m_Next;
}
sScript* cGameScript::Script_RemoveTrait(sScript* Script)					// `J` new
{
	if (m_GirlTarget && m_GirlTarget->has_trait(Script->m_Entries[0].m_Text))
		m_GirlTarget->remove_trait(Script->m_Entries[0].m_Text);
	return Script->m_Next;
}
sScript* cGameScript::Script_AddTraitTemp(sScript* Script)						// `J` new
{
	if (m_GirlTarget && !m_GirlTarget->has_trait(Script->m_Entries[0].m_Text))
		m_GirlTarget->add_trait(Script->m_Entries[0].m_Text, Script->m_Entries[1].m_lValue);
	return Script->m_Next;
}
sScript* cGameScript::Script_AdjustTraitTemp(sScript* Script)					// `J` new
{
	if (m_GirlTarget)
	{
		cGirls::updateTempTraits(m_GirlTarget, Script->m_Entries[0].m_Text, Script->m_Entries[1].m_lValue);
	}
	return Script->m_Next;
}
sScript* cGameScript::Script_GetRandomGirl(sScript* Script)						// `J` new
{
	m_GirlTarget = nullptr;
	IBuilding* brothel = nullptr;
	switch (Script->m_Entries[0].m_Selection)
	{
	case 1:	// brothel
		brothel = g_Game->buildings().building_with_type(BuildingType::BROTHEL, Script->m_Entries[1].m_Selection);
		break;
	case 2:	// studio
		brothel = g_Game->buildings().building_with_type(BuildingType::STUDIO);
		break;
	case 3:	// arena
		brothel = g_Game->buildings().building_with_type(BuildingType::ARENA);
		break;
	case 4:	// center
		brothel = g_Game->buildings().building_with_type(BuildingType::CENTRE);
		break;
	case 5:	// clinic
		brothel = g_Game->buildings().building_with_type(BuildingType::CLINIC);
		break;
	case 6:	// farm
		brothel = g_Game->buildings().building_with_type(BuildingType::FARM);
		break;
	case 7:	// house
		brothel = g_Game->buildings().building_with_type(BuildingType::HOUSE);
		break;
	case 0:	// anywhere
	default:
	{
	    std::vector<IBuilding*> brothels;
		int totalgirls = 0;
        for (auto& b : g_Game->buildings().buildings())
            brothels.push_back(b.get());

        for(auto b : brothels)
            totalgirls += b->num_girls();

		int choosegirl = g_Dice%totalgirls;
		for(auto b : brothels) {
            if (choosegirl < b->num_girls())
            {
                brothel = b;
                break;
            }
            else choosegirl -= b->num_girls();
		}
	}
	break;
	}

    if (brothel)
        m_GirlTarget = brothel->get_girl(g_Dice%brothel->num_girls());

	if (m_GirlTarget == nullptr)
	{
		m_Active = false;
		m_Leave = true;
	}
	return Script->m_Next;
}
sScript* cGameScript::Script_DeepTarget(sScript* Script)
{
	if (m_GirlTarget) m_GirlTarget->oralsex(1);
	g_GirlDetails->set_image(IMGTYPE_DEEPTHROAT);
	return Script->m_Next;
}
sScript* cGameScript::Script_EatOutTarget(sScript* Script)
{
	if (m_GirlTarget) m_GirlTarget->lesbian(1);
	g_GirlDetails->set_image(IMGTYPE_EATOUT);
	return Script->m_Next;
}
sScript* cGameScript::Script_StrapOnTarget(sScript* Script)
{
	if (m_GirlTarget) m_GirlTarget->lesbian(1);
	g_GirlDetails->set_image(IMGTYPE_STRAPON);
	return Script->m_Next;
}
sScript* cGameScript::Script_Les69ingTarget(sScript* Script)
{
	if (m_GirlTarget) m_GirlTarget->lesbian(1);
	g_GirlDetails->set_image(IMGTYPE_LES69ING);
	return Script->m_Next;
}
sScript* cGameScript::Script_DildoTarget(sScript* Script)
{
	if (m_GirlTarget) m_GirlTarget->service(1);
	g_GirlDetails->set_image(IMGTYPE_DILDO);
	return Script->m_Next;
}
sScript* cGameScript::Script_SubTarget(sScript* Script)
{
	if (m_GirlTarget) m_GirlTarget->bdsm(1);
	g_GirlDetails->set_image(IMGTYPE_SUB);
	return Script->m_Next;
}
sScript* cGameScript::Script_LickTarget(sScript* Script)
{
	if (m_GirlTarget) m_GirlTarget->oralsex(1);
	g_GirlDetails->set_image(IMGTYPE_LICK);
	return Script->m_Next;
}
sScript* cGameScript::Script_SuckBallsTarget(sScript* Script)
{
	if (m_GirlTarget) m_GirlTarget->oralsex(1);
	g_GirlDetails->set_image(IMGTYPE_SUCKBALLS);
	return Script->m_Next;
}
sScript* cGameScript::Script_CowGirlTarget(sScript* Script)
{
	if (m_GirlTarget) m_GirlTarget->normalsex(1);
	g_GirlDetails->set_image(IMGTYPE_COWGIRL);
	return Script->m_Next;
}
sScript* cGameScript::Script_RevCowGirlTarget(sScript* Script)
{
	if (m_GirlTarget) m_GirlTarget->normalsex(1);
	g_GirlDetails->set_image(IMGTYPE_REVCOWGIRL);
	return Script->m_Next;
}
sScript* cGameScript::Script_SexDoggyTarget(sScript* Script)
{
	if (m_GirlTarget) m_GirlTarget->normalsex(1);
	g_GirlDetails->set_image(IMGTYPE_SEXDOGGY);
	return Script->m_Next;
}
sScript* cGameScript::Script_NormalSexWithRandomTarget(sScript* Script)
{
	if (m_GirlTarget)
	{
		m_GirlTarget->normalsex(2);

		if (m_GirlTarget->check_virginity()) m_GirlTarget->lose_virginity();	// `J` updated for trait/status
		sCustomer Cust = g_Game->GetCustomer(*m_GirlTarget->m_Building);
		if (!m_GirlTarget->calc_pregnancy(Cust, false, 1.0))
			g_Game->push_message(m_GirlTarget->m_Realname + " has gotten pregnant", 0);
	}
	g_GirlDetails->set_image(IMGTYPE_SEX);

	return Script->m_Next;
}
sScript* cGameScript::Script_IfGirlHasItem(sScript* Script)					// `J` new .06.02.55
{
	bool Skipping = false; // Flag for if...then condition
	m_NestLevel++;
	int Nest = m_NestLevel;

	Skipping = m_GirlTarget->has_item_j(Script->m_Entries[0].m_Text) == -1;
	// At this point, Skipping states if the script actions
	// need to be skipped due to a conditional if...then statement.
	// Actions are further processed if skipped = false, looking
	// for an else to flip the skip mode, or an endif to end
	// the conditional block.
	Script = Script->m_Next; // Go to next action to process
	while (Script != nullptr)
	{
		if (m_Leave) break;
		// if else, flip skip mode
		if ((Script->m_Type == 10 || Script->m_Type == 110) && Nest == m_NestLevel) Skipping = !Skipping;

		// break on end if
		if (Script->m_Type == 11 || Script->m_Type == 105)
		{
			if (Nest == m_NestLevel)
			{
				m_NestLevel--;
				return Script->m_Next;
			}
			m_NestLevel--;
		}

		// Process script function in conditional block
		// making sure to skip actions when condition not met.
		if (Skipping)
		{
			if (IsIfStatement(Script->m_Type)) m_NestLevel++;
			Script = Script->m_Next;
		}
		else
		{
			if ((Script = Process(Script)) == nullptr) return nullptr;
		}
	}
	return nullptr; // End of script reached

}
sScript* cGameScript::Script_AddItemtoGirl(sScript* Script)					// `J` new .06.02.55
{
	sInventoryItem* item = g_Game->inventory_manager().GetItem(Script->m_Entries[0].m_Text);
	int value[2];
	value[0] = (Script->m_Entries[1].m_Var == 1 ? m_Vars[Script->m_Entries[1].m_lValue] : Script->m_Entries[1].m_lValue);
	value[1] = (Script->m_Entries[2].m_Var == 1 ? m_Vars[Script->m_Entries[2].m_lValue] : Script->m_Entries[2].m_lValue);

	g_LogFile.ss() << "Debug: New script part: AddItemtoGirl  ||  Trying to add " << value[0] << " of item: " << Script->m_Entries[0].m_Text
		<< "  to girl: " << (m_GirlTarget ? m_GirlTarget->m_Name : " !NO GIRL! ") << " | Item " << (item ? "Found" : " !NOT FOUND! ");
	if (cGirls::IsInvFull(m_GirlTarget))
	{
		g_LogFile.ss() << "Her inventory is full"; g_LogFile.ssend();
		g_Game->push_message("Her inventory is full", 0);
		return Script->m_Next;
	}
	if (item && m_GirlTarget && value[0] > 0)
	{
		for (int i = 0; i < value[0]; i++)
		{
			if (value[1] == 0)	m_GirlTarget->add_inv(item);
			else	g_Game->inventory_manager().Equip(m_GirlTarget, m_GirlTarget->add_inv(item), false);
		}
	}
	g_LogFile.ssend();
	return Script->m_Next;
}
sScript* cGameScript::Script_GivePlayerItem(sScript* Script)					// `J` new .06.02.55
{
	sInventoryItem* item = g_Game->inventory_manager().GetItem(Script->m_Entries[0].m_Text);
	int amount = (Script->m_Entries[1].m_Var == 1 ? m_Vars[Script->m_Entries[1].m_lValue] : Script->m_Entries[1].m_lValue);

	g_LogFile.ss() << "Debug: New script part: GivePlayerItem  ||  Trying to add " << amount << " of item: " << Script->m_Entries[0].m_Text
		<< "  to Inventory  |  Item " << (item ? "Found" : " !NOT FOUND! ");

	if (item && amount > 0)
	{
		for (int i = 0; i < amount; i++)
		{
		    g_Game->player().inventory().add_item(item);
		}
	}
	return Script->m_Next;
}
sScript* cGameScript::Script_IfPlayerHasItem(sScript* Script)
{
	bool Skipping = false; // Flag for if...then condition
	m_NestLevel++;
	int Nest = m_NestLevel;

	Skipping = !g_Game->player().inventory().has_item(Script->m_Entries[0].m_Text);


	// At this point, Skipping states if the script actions
	// need to be skipped due to a conditional if...then statement.
	// Actions are further processed if skipped = false, looking
	// for an else to flip the skip mode, or an endif to end
	// the conditional block.
	Script = Script->m_Next; // Go to next action to process
	while (Script != nullptr)
	{
		if (m_Leave) break;
		// if else, flip skip mode
		if ((Script->m_Type == 10 || Script->m_Type == 110) && Nest == m_NestLevel) Skipping = !Skipping;

		// break on end if
		if (Script->m_Type == 11 || Script->m_Type == 105)
		{
			if (Nest == m_NestLevel)
			{
				m_NestLevel--;
				return Script->m_Next;
			}
			m_NestLevel--;
		}

		// Process script function in conditional block
		// making sure to skip actions when condition not met.
		if (Skipping)
		{
			if (IsIfStatement(Script->m_Type)) m_NestLevel++;
			Script = Script->m_Next;
		}
		else
		{
			if ((Script = Process(Script)) == nullptr) return nullptr;
		}
	}
	return nullptr; // End of script reached
}
sScript* cGameScript::Script_GiveGirlInvItem(sScript* Script)
{
    sInventoryItem* item = g_Game->inventory_manager().GetItem(Script->m_Entries[0].m_Text);
    bool has_item = g_Game->player().inventory().has_item(item);
    bool equip = (Script->m_Entries[1].m_Var == 1 ? m_Vars[Script->m_Entries[1].m_lValue] : Script->m_Entries[1].m_lValue) == 1;

	g_LogFile.ss() << "Debug: New script part: GiveGirlInvItem  ||  Trying to give item: " << Script->m_Entries[0].m_Text << "  to girl: ";
	if (!has_item)
	{
		g_LogFile.ss() << " |  !ITEM NOT FOUND! ";
	}
	else if (!m_GirlTarget)
	{
		g_LogFile.ss() << " |  !GIRL NOT FOUND! ";
	}
	else if (cGirls::IsInvFull(m_GirlTarget))
	{
		g_LogFile.ss() << " |  " << m_GirlTarget->m_Name << "'s inventory is full!";
		g_Game->push_message("Her inventory is full", 0);
	}
	else if (item && m_GirlTarget)
	{
		g_LogFile.ss() << " |  " << m_GirlTarget->m_Name << " received item";

		if (!equip)	m_GirlTarget->add_inv(item);
		else
		{
			g_LogFile.ss() << " and used it.";
			g_Game->inventory_manager().Equip(m_GirlTarget, m_GirlTarget->add_inv(item), false);
		}

		g_Game->player().inventory().remove_item(item);
	}


	g_LogFile.ssend();
	return Script->m_Next; // End of script reached
}
sScript* cGameScript::Script_IfGirlIsSlave(sScript* Script)
{
	bool Skipping = false; // Flag for if...then condition
	m_NestLevel++;
	int Nest = m_NestLevel;

	Skipping = !m_GirlTarget->is_slave();

	// At this point, Skipping states if the script actions
	// need to be skipped due to a conditional if...then statement.
	// Actions are further processed if skipped = false, looking
	// for an else to flip the skip mode, or an endif to end
	// the conditional block.
	Script = Script->m_Next; // Go to next action to process
	while (Script != nullptr)
	{
		if (m_Leave) break;
		// if else, flip skip mode
		if ((Script->m_Type == 10 || Script->m_Type == 110) && Nest == m_NestLevel) Skipping = !Skipping;

		// break on end if
		if (Script->m_Type == 11 || Script->m_Type == 105)
		{
			if (Nest == m_NestLevel)
			{
				m_NestLevel--;
				return Script->m_Next;
			}
			m_NestLevel--;
		}

		// Process script function in conditional block
		// making sure to skip actions when condition not met.
		if (Skipping)
		{
			if (IsIfStatement(Script->m_Type)) m_NestLevel++;
			Script = Script->m_Next;
		}
		else
		{
			if ((Script = Process(Script)) == nullptr) return nullptr;
		}
	}
	return nullptr; // End of script reached
}
sScript* cGameScript::Script_IfGirlIsFree(sScript* Script)
{
	bool Skipping = false; // Flag for if...then condition
	m_NestLevel++;
	int Nest = m_NestLevel;

	Skipping = !m_GirlTarget->is_free();

	// At this point, Skipping states if the script actions
	// need to be skipped due to a conditional if...then statement.
	// Actions are further processed if skipped = false, looking
	// for an else to flip the skip mode, or an endif to end
	// the conditional block.
	Script = Script->m_Next; // Go to next action to process
	while (Script != nullptr)
	{
		if (m_Leave) break;
		// if else, flip skip mode
		if ((Script->m_Type == 10 || Script->m_Type == 110) && Nest == m_NestLevel) Skipping = !Skipping;

		// break on end if
		if (Script->m_Type == 11 || Script->m_Type == 105)
		{
			if (Nest == m_NestLevel)
			{
				m_NestLevel--;
				return Script->m_Next;
			}
			m_NestLevel--;
		}

		// Process script function in conditional block
		// making sure to skip actions when condition not met.
		if (Skipping)
		{
			if (IsIfStatement(Script->m_Type)) m_NestLevel++;
			Script = Script->m_Next;
		}
		else
		{
			if ((Script = Process(Script)) == nullptr) return nullptr;
		}
	}
	return nullptr; // End of script reached
}
sScript* cGameScript::Script_GiveGoldToGirl(sScript* Script)
{
	int value[2];
	value[0] = (Script->m_Entries[0].m_Var == 1 ? m_Vars[Script->m_Entries[0].m_lValue] : Script->m_Entries[0].m_lValue);
	value[1] = (Script->m_Entries[1].m_Var == 1 ? m_Vars[Script->m_Entries[1].m_lValue] : Script->m_Entries[1].m_lValue);
	long gold = (value[0] == 0 ? (g_Dice % (value[1] + 1)) + value[0] : (g_Dice % (value[1] + 1)) + value[0] - 1);

	if (gold < 0 && m_GirlTarget->m_Money <= -gold)	// If taking money and she does not have much
	{
		gold = m_GirlTarget->m_Money;				// you can only take what she has
		m_GirlTarget->m_Money = 0;
	}
	else m_GirlTarget->m_Money += gold;

	g_Game->gold().misc_credit(-gold);

	return Script->m_Next;
}
sScript* cGameScript::Script_AdjustTargetGirlStatR(sScript* Script)
{
	int num = 0;
	int value[4];

	value[0] = (Script->m_Entries[0].m_Var == 1 ? m_Vars[Script->m_Entries[0].m_Selection] : Script->m_Entries[0].m_Selection);
	value[1] = (Script->m_Entries[1].m_Var == 1 ? m_Vars[Script->m_Entries[1].m_lValue] : Script->m_Entries[1].m_lValue);
	value[2] = (Script->m_Entries[2].m_Var == 1 ? m_Vars[Script->m_Entries[2].m_lValue] : Script->m_Entries[2].m_lValue);
	value[3] = (Script->m_Entries[3].m_Var == 1 ? m_Vars[Script->m_Entries[3].m_Selection] : Script->m_Entries[3].m_Selection);
	num = (Script->m_Entries[1].m_lValue == 0 ? (g_Dice % (value[2] + 1)) + value[1] : (g_Dice % (value[2] + 1)) + value[1] - 1);

	if (m_GirlTarget)
	{
		if (value[3])	m_GirlTarget->upd_temp_stat(value[0], num);
		else/*     */	m_GirlTarget->upd_stat(value[0], num);
	}
	return Script->m_Next;
}
sScript* cGameScript::Script_AdjustTargetGirlSkillR(sScript* Script)
{
	int num = 0;
	int value[4];
	value[0] = (Script->m_Entries[0].m_Var == 1 ? m_Vars[Script->m_Entries[0].m_Selection] : Script->m_Entries[0].m_Selection);
	value[1] = (Script->m_Entries[1].m_Var == 1 ? m_Vars[Script->m_Entries[1].m_lValue] : Script->m_Entries[1].m_lValue);
	value[2] = (Script->m_Entries[2].m_Var == 1 ? m_Vars[Script->m_Entries[2].m_lValue] : Script->m_Entries[2].m_lValue);
	value[3] = (Script->m_Entries[3].m_Var == 1 ? m_Vars[Script->m_Entries[3].m_Selection] : Script->m_Entries[3].m_Selection);
	num = (Script->m_Entries[1].m_lValue == 0 ? (g_Dice % (value[2] + 1)) + value[1] : (g_Dice % (value[2] + 1)) + value[1] - 1);

	if (m_GirlTarget)
	{
		if (value[3])	m_GirlTarget->upd_temp_skill(value[0], num);
		else/*     */	m_GirlTarget->upd_skill(value[0], num);
	}
	return Script->m_Next;
}
sScript* cGameScript::Script_IfGirlStatus(sScript* Script)			// `J` new .06.03.00
{
	bool Skipping = false; // Flag for if...then condition
	m_NestLevel++;
	int Nest = m_NestLevel;
	int value = Script->m_Entries[0].m_Selection;

	// See if variable matches second entry
	Skipping = !(m_GirlTarget->m_States&(1 << value));

	// At this point, Skipping states if the script actions
	// need to be skipped due to a conditional if...then statement.
	// Actions are further processed if skipped = false, looking
	// for an else to flip the skip mode, or an endif to end
	// the conditional block.
	Script = Script->m_Next; // Go to next action to process
	while (Script != nullptr)
	{
		if (m_Leave) break;
		// if else, flip skip mode
		if ((Script->m_Type == 10 || Script->m_Type == 110) && Nest == m_NestLevel) Skipping = !Skipping;

		// break on end if
		if (Script->m_Type == 11 || Script->m_Type == 105)
		{
			if (Nest == m_NestLevel)
			{
				m_NestLevel--;
				return Script->m_Next;
			}
			m_NestLevel--;
		}

		// Process script function in conditional block
		// making sure to skip actions when condition not met.
		if (Skipping)
		{
			if (IsIfStatement(Script->m_Type)) m_NestLevel++;
			Script = Script->m_Next;
		}
		else
		{
			if ((Script = Process(Script)) == nullptr) return nullptr;
		}
	}
	return nullptr; // End of script reached
}
sScript* cGameScript::Script_SetGirlStatus(sScript* Script)			// `J` new .06.03.00
{
	int status = (Script->m_Entries[0].m_Var == 1 ? m_Vars[Script->m_Entries[0].m_lValue] : Script->m_Entries[0].m_lValue);
	int setto = (Script->m_Entries[1].m_Var == 1 ? m_Vars[Script->m_Entries[1].m_lValue] : Script->m_Entries[1].m_lValue);

	if (status == STATUS_PREGNANT || status == STATUS_PREGNANT_BY_PLAYER || status == STATUS_INSEMINATED)	// if creating pregnancy, remove old pregnancies
	{
		if (!setto)		// trying to end any pregnancy type will end all pregnancy types
		{
			m_GirlTarget->clear_pregnancy();
		}
		else if (m_GirlTarget->is_pregnant()){}	// she is already pregnant so do nothing
		else if (status == STATUS_PREGNANT_BY_PLAYER)
		{
			cGirls::CreatePregnancy(m_GirlTarget, 1, status, g_Game->player().m_Stats, g_Game->player().m_Skills);
		}
		else
		{
			sCustomer Cust{};
			int num = (status == STATUS_INSEMINATED ? max(1, g_Dice.bell(-4, 5)) : 1);
			cGirls::CreatePregnancy(m_GirlTarget, 1, status, Cust.m_Stats, Cust.m_Skills);
		}
	}
	else
	{
		// Set value
		if (setto)	m_GirlTarget->m_States |= (1 << status);
		else 		m_GirlTarget->m_States &= (1 << status);
	}
	return Script->m_Next; // Go to next script action


}
sScript* cGameScript::Script_EndPregnancy(sScript* Script)			// `J` new .06.03.00
{
	m_GirlTarget->clear_pregnancy();
	return Script->m_Next; // Go to next script action
}
sScript* cGameScript::Script_CreatePregnancy(sScript* Script)		// `J` new .06.03.00
{
	int value[3];
	value[0] = (Script->m_Entries[0].m_Var == 1 ? m_Vars[Script->m_Entries[0].m_Selection] : Script->m_Entries[0].m_Selection);
	value[1] = (Script->m_Entries[1].m_Var == 1 ? m_Vars[Script->m_Entries[1].m_lValue] : Script->m_Entries[1].m_lValue);
	value[2] = (Script->m_Entries[2].m_Var == 1 ? m_Vars[Script->m_Entries[2].m_lValue] : Script->m_Entries[2].m_lValue);

	if (value[2])							// force==true remove old pregnancies
	{
		m_GirlTarget->clear_pregnancy();
	}
	if (m_GirlTarget->is_pregnant()){}		// she is already pregnant so do nothing
	else if (!g_Dice.percent(value[1])){}	// if the percent fails do nothing
	else if (value[0] == 0)					// STATUS_PREGNANT_BY_PLAYER
	{
		cGirls::CreatePregnancy(m_GirlTarget, 1, STATUS_PREGNANT_BY_PLAYER, g_Game->player().m_Stats, g_Game->player().m_Skills);
	}
	else
	{
		sCustomer Cust;
		int status = (value[0] == 1 ? STATUS_PREGNANT : STATUS_INSEMINATED);
		int num = (status == STATUS_INSEMINATED ? max(1, g_Dice.bell(-4, 5)) : 1);
		cGirls::CreatePregnancy(m_GirlTarget, 1, status, Cust.m_Stats, Cust.m_Skills);
	}
	return Script->m_Next; // Go to next script action

}

sScript* cGameScript::Script_BrandTarget(sScript* Script)
{
	if (m_GirlTarget)
	{
		m_GirlTarget->pchate(g_Dice % 3 + 1);
		m_GirlTarget->pcfear(g_Dice % 3 + 1);
		m_GirlTarget->pclove(-(g_Dice % 3));
	}
	g_GirlDetails->set_image(IMGTYPE_BRAND);
	return Script->m_Next;
}
sScript* cGameScript::Script_RapeTarget(sScript* Script)
{
	if (m_GirlTarget)
	{
		m_GirlTarget->pchate(g_Dice % 5 + 1);
		m_GirlTarget->pcfear(g_Dice % 5 + 1);
		m_GirlTarget->pclove(-(g_Dice % 5));
	}
	g_GirlDetails->set_image(IMGTYPE_RAPE);
	return Script->m_Next;
}
sScript* cGameScript::Script_RapeBeastTarget(sScript* Script)
{
	if (m_GirlTarget)
	{
		m_GirlTarget->pchate(g_Dice % 4 + 1);
		m_GirlTarget->pcfear(g_Dice % 6 + 1);
		m_GirlTarget->pclove(-(g_Dice % 3));
	}
	g_GirlDetails->set_image(IMGTYPE_RAPEBEAST);
	return Script->m_Next;
}
sScript* cGameScript::Script_BirthHumanTarget(sScript* Script)
{
	g_GirlDetails->set_image(IMGTYPE_BIRTHHUMAN);
	return Script->m_Next;
}
sScript* cGameScript::Script_BirthHumanMultipleTarget(sScript* Script)
{
	g_GirlDetails->set_image(IMGTYPE_BIRTHHUMANMULTIPLE);
	return Script->m_Next;
}
sScript* cGameScript::Script_BirthBeastTarget(sScript* Script)
{
	g_GirlDetails->set_image(IMGTYPE_BIRTHBEAST);
	return Script->m_Next;
}
sScript* cGameScript::Script_ImpregSexTarget(sScript* Script)
{
	if (m_GirlTarget) m_GirlTarget->normalsex(g_Dice % 3 + 1);
	g_GirlDetails->set_image(IMGTYPE_IMPREGSEX);
	return Script->m_Next;
}
sScript* cGameScript::Script_ImpregGroupTarget(sScript* Script)
{
	if (m_GirlTarget) m_GirlTarget->group(g_Dice % 3 + 1);
	g_GirlDetails->set_image(IMGTYPE_IMPREGGROUP);
	return Script->m_Next;
}
sScript* cGameScript::Script_ImpregBDSMTarget(sScript* Script)
{
	if (m_GirlTarget) m_GirlTarget->bdsm(g_Dice % 3 + 1);
	g_GirlDetails->set_image(IMGTYPE_IMPREGBDSM);
	return Script->m_Next;
}
sScript* cGameScript::Script_ImpregBeastTarget(sScript* Script)
{
	if (m_GirlTarget) m_GirlTarget->beastiality(g_Dice % 3 + 1);
	g_GirlDetails->set_image(IMGTYPE_IMPREGBEAST);
	return Script->m_Next;
}
sScript* cGameScript::Script_VirginSexTarget(sScript* Script)
{
	if (m_GirlTarget) m_GirlTarget->normalsex(1);
	g_GirlDetails->set_image(IMGTYPE_VIRGINSEX);
	return Script->m_Next;
}
sScript* cGameScript::Script_VirginGroupTarget(sScript* Script)
{
	if (m_GirlTarget) m_GirlTarget->group(1);
	g_GirlDetails->set_image(IMGTYPE_VIRGINGROUP);
	return Script->m_Next;
}
sScript* cGameScript::Script_VirginBDSMTarget(sScript* Script)
{
	if (m_GirlTarget) m_GirlTarget->bdsm(1);
	g_GirlDetails->set_image(IMGTYPE_VIRGINBDSM);
	return Script->m_Next;
}
sScript* cGameScript::Script_VirginBeastTarget(sScript* Script)
{
	if (m_GirlTarget) m_GirlTarget->beastiality(1);
	g_GirlDetails->set_image(IMGTYPE_VIRGINBEAST);
	return Script->m_Next;
}
sScript* cGameScript::Script_EscortTarget(sScript* Script)
{
	if (m_GirlTarget) m_GirlTarget->upd_skill(STAT_CHARISMA, 1);
	g_GirlDetails->set_image(IMGTYPE_ESCORT);
	return Script->m_Next;
}
sScript* cGameScript::Script_SportTarget(sScript* Script)
{
	if (m_GirlTarget) m_GirlTarget->upd_skill(STAT_CONSTITUTION, 1);
	g_GirlDetails->set_image(IMGTYPE_SPORT);
	return Script->m_Next;
}
sScript* cGameScript::Script_StudyTarget(sScript* Script)
{
	if (m_GirlTarget) m_GirlTarget->upd_skill(STAT_INTELLIGENCE, 1);
	g_GirlDetails->set_image(IMGTYPE_STUDY);
	return Script->m_Next;
}
sScript* cGameScript::Script_TeacherTarget(sScript* Script)
{
	if (m_GirlTarget) m_GirlTarget->performance(1);
	g_GirlDetails->set_image(IMGTYPE_TEACHER);
	return Script->m_Next;
}






//sScript* cGameScript::Script_GirlNameTarget(sScript* Script)
//{
//	if(m_GirlTarget)
//	{
//		sGirl* girl;
//		string girlName = girl->m_Realname;
//		stringstream sstemp;
//		sstemp << ("Im ") << girlName;
//	}
//		//g_GirlDetails->set_image(IMGTYPE_TORTURE);
//
//	return Script->m_Next;
//}
