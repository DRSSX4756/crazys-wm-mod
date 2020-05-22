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
#include "src/buildings/cBrothel.h"
#include "cRng.h"
#include "cInventory.h"
#include <sstream>
#include "cObjectiveManager.hpp"
#include "src/Game.hpp"
#include "src/sStorage.hpp"
#include "CLog.h"

// `J` Job Brothel - General
bool cJobManager::WorkExploreCatacombs(sGirl* girl, bool Day0Night1, string& summary, cRng& rng)
{
    auto brothel = girl->m_Building;

	int actiontype = ACTION_COMBAT;
	stringstream ss; string girlName = girl->m_Realname; ss << girlName;
	if (girl->disobey_check(actiontype, JOB_EXPLORECATACOMBS))
	{
		ss << " refused to go into the catacombs during the " << (Day0Night1 ? "night" : "day") << " shift.";
		girl->m_Events.AddMessage(ss.str(), IMGTYPE_PROFILE, EVENT_NOWORK);
		return true;
	}
	ss << " went into the catacombs to see what she can find.\n \n";

	int num_monsters = 0;
	int type_monster_girls = 0;
	int type_unique_monster_girls = 0;
	int type_beasts = 0;
	int num_items = 0;
	long gold = 0;
	int wages = 0, tips = 0;
	bool raped = false;
	string UGirls_list;
	string Girls_list;
	string item_list;

	cGirls::EquipCombat(girl);	// ready armor and weapons!

#if 1
	if (cfg.catacombs.control_girls())	// start controlled girl results - aka the new code
	{
		int haulcount = 2 + ((girl->strength() + girl->constitution()) / 10);	// how much she can bring back			- max 22 points
		double girlspercent = cfg.catacombs.girl_gets_girls();	// each girl costs 5 haul points						- max 5 girls
		double beastpercent = cfg.catacombs.girl_gets_beast();	// each beast costs 3 haul points						- max 8 beasts
		double itemspercent = cfg.catacombs.girl_gets_items();	// each item costs 2 if an item is found or 1 if not	- max 11 items
		int numgirls = 0, numitems = 0;

		while (haulcount > 0 && girl->health() > 40)
		{
			gold += rng % 150;
			double roll = (rng % 10000) / 100.0;
			int getwhat = 0;								// 0=girl, 1=beast, 2=item
			if (roll < beastpercent)						getwhat = 1;
			else if (roll < beastpercent + itemspercent)	getwhat = 2;

			std::uint8_t fight_outcome = 0;
			// she may be able to coax a beast or if they are looking for an item, it may be guarded
			if ((getwhat == 1 && rng.percent((girl->animalhandling() + girl->beastiality()) / 3))
				|| (getwhat == 2 && rng.percent(50)))
				fight_outcome = 1;	// no fight so auto-win
			else		// otherwise do the fight
			{
				sGirl* tempgirl = g_Game->CreateRandomGirl(18, false, false, false, true);
				if (tempgirl)		// `J` reworked incase there are no Non-Human Random Girls
				{
					fight_outcome = cGirls::girl_fights_girl(girl, tempgirl);
				}
				else // `J` this should have been corrected with the addition of the default random girl but leaving it in just in case.
				{
					g_LogFile.write("Error: You have no Non-Human Random Girls for your girls to fight\n");
					g_LogFile.write("Error: You need a Non-Human Random Girl to allow WorkExploreCatacombs randomness");
					fight_outcome = 7;
				}
				if (fight_outcome == 7)
				{
					if (rng % girl->get_skill(SKILL_COMBAT) < 5) fight_outcome = 2;
					else
					{
						stringstream sse;
						sse << "(Error: You need a Non-Human Random Girl to allow WorkExploreCatacombs randomness)";
						girl->m_Events.AddMessage(sse.str(), IMGTYPE_PROFILE, Day0Night1);
						fight_outcome = 1;
					}
				}
				if (tempgirl) delete tempgirl; tempgirl = nullptr;
			}

			if (fight_outcome == 1)  // If she won
			{
				if (getwhat == 0)		{ haulcount -= 5;	numgirls++; }						// Catacombs girl type
				else if (getwhat == 1)	{ haulcount -= 3;	type_beasts++;	num_monsters++; }	// Beast type
				else
				{
					haulcount--;
					int chance = (girl->intelligence() + girl->agility()) / 2;
					if (rng.percent(chance))		// percent chance she will find an item
					{
						haulcount--;
						numitems++;
					}
				}
			}
			else if (fight_outcome == 2) // she lost
			{
				haulcount -= 50;
				raped = true;
				break;
			}
			else if (fight_outcome == 0) // it was a draw
			{
				haulcount -= 1 + rng % 5;
			}
		}

		if (raped)
		{
			ss.str("");
			int NumMon = rng % 6 + 1;
			ss << girl->m_Realname << " was defeated then" << ((NumMon <= 3) ? "" : " gang") << " raped and abused by " << NumMon << " monsters.";
			int health = -NumMon, happy = -NumMon * 5, spirit = -NumMon, sex = -NumMon * 2, combat = -NumMon * 2, injury = 9 + NumMon;

			if (girl->check_virginity())
			{
				ss << " That's a hell of a way to lose your virginity; naturally, she's rather distressed by this fact.";
				girl->lose_virginity();					// Virginity both attrib & trait now, 04/15/13
				health -= 1, happy -= 10, spirit -= 2, sex -= 2, combat -= 2, injury += 2;
			}
			girl->m_Events.AddMessage(ss.str(), IMGTYPE_DEATH, EVENT_DANGER);

			if (!girl->calc_insemination(*cGirls::GetBeast(), false, 1.0 + (NumMon * 0.5)))
			{
				g_Game->push_message(girl->m_Realname + " has gotten inseminated", 0);
				health -= 1, happy -= 10, spirit -= 4, sex -= 4, combat -= 2, injury += 2;
			}

			girl->health(health);
			girl->happiness(happy);
			girl->spirit(spirit);
			cGirls::GirlInjured(girl, injury);
			girl->upd_Enjoyment(ACTION_SEX, sex);
			girl->upd_Enjoyment(actiontype, combat);

			return false;
		}

		g_Game->storage().add_to_beasts(type_beasts);
		while (numgirls > 0)
		{
			numgirls--;
			sGirl* ugirl = nullptr;
			if (rng.percent(cfg.catacombs.unique_catacombs()))	// chance of getting unique girl
			{
				ugirl = g_Game->GetRandomGirl(false, true);				// Unique monster girl type
			}
			if (ugirl == nullptr)		// if not unique or a unique girl can not be found
			{
				ugirl = g_Game->CreateRandomGirl(0, false, false, true, true);	// create a random girl
				if (ugirl)
				{
					type_monster_girls++;
					Girls_list += ((Girls_list.empty()) ? "   " : ",\n   ") + ugirl->m_Realname;
				}
			}
			else				// otherwise set the unique girls stuff
			{
				ugirl->m_States &= ~(1 << STATUS_CATACOMBS);
				type_unique_monster_girls++;
				UGirls_list += ((UGirls_list.empty()) ? "   " : ",\n   ") + ugirl->m_Realname;
			}
			if (ugirl)
			{
				num_monsters++;
				if (g_Game->get_objective() && g_Game->get_objective()->m_Objective == OBJECTIVE_CAPTUREXCATACOMBGIRLS)
				{
                    g_Game->get_objective()->m_SoFar++;
				}
				stringstream Umsg;
				ugirl->add_trait("Kidnapped", 2 + rng % 15);
				Umsg << ugirl->m_Realname << " was captured in the catacombs by " << girlName << ".\n";
				ugirl->m_Events.AddMessage(Umsg.str(), IMGTYPE_PROFILE, EVENT_DUNGEON);
				g_Game->dungeon().AddGirl(ugirl, DUNGEON_GIRLCAPTURED);	// Either type of girl goes to the dungeon
			}
		}
		while (numitems > 0)
		{
			numitems--;
			int ItemPlace = 0;  // Place in 0..299
			sInventoryItem* TempItem = g_Game->inventory_manager().GetRandomCatacombItem();
			if(g_Game->player().inventory().add_item(TempItem)) {
                item_list += ((item_list.empty()) ? "   " : ",\n   ") + TempItem->m_Name;
                num_items++;
            }
		}
	}	// end controlled girl results

	else	// start uncontrolled girl results - aka the old code
#endif
	{
		// determine if they fight any monsters
		if (!rng.percent(max(girl->combat(), girl->magic())))	// WD:	Allow best of Combat or Magic skill
		{
			ss << "The first creature she encountered looked way to strong for her to fight so she ran away. She stops by your office later, \"I think I need more fight training before I can go back there.\"";
			girl->m_Events.AddMessage(ss.str(), IMGTYPE_COMBAT, Day0Night1);
			return false;
		}
		num_monsters = max(1, (rng % 6) - 1);

		// fight/capture monsters here
		for (int i = num_monsters; i > 0; i--)
		{
			bool getagirl = rng.percent(55);	// 0 is beast, 1 is girl (human or non-human)

			sGirl* tempgirl = g_Game->CreateRandomGirl(18, false, false, false, true, false, false);
			std::uint8_t fight_outcome = 0;
			if (tempgirl)		// `J` reworked incase there are no Non-Human Random Girls
			{
				fight_outcome = cGirls::girl_fights_girl(girl, tempgirl);
			}
			else // `J` this should have been corrected with the addition of the default random girl but leaving it in just in case.
			{
				g_LogFile.write("Error: You have no Non-Human Random Girls for your girls to fight\n");
				g_LogFile.write("Error: You need a Non-Human Random Girl to allow WorkExploreCatacombs randomness");
				fight_outcome = 7;
			}
			if (fight_outcome == 7)
			{
				if (rng%girl->get_skill(SKILL_COMBAT) < 5) raped = true;
				else
				{
					ss << "She came back with one animal today.\n \n";
					ss << "(Error: You need a Non-Human Random Girl to allow WorkExploreCatacombs randomness)";
					girl->m_Events.AddMessage(ss.str(), IMGTYPE_PROFILE, Day0Night1);
					g_Game->storage().add_to_beasts(1);
					type_beasts++;
				}
			}
			else if (fight_outcome == 1)  // If she won
			{
				if (getagirl)  // Catacombs girl type
				{				// NOTE that defeating tempgirl and capturing unrelated girl is
					// the only way to ever capture rare girls like those with incoporeal trait.
					// Some rationilization could be done, but is probably not necessary. DustyDan
					sGirl* ugirl = nullptr;
					if (rng.percent(cfg.catacombs.unique_catacombs()))	// chance of getting unique girl
					{
						ugirl = g_Game->GetRandomGirl(false, true);				// Unique monster girl type
					}
					if (ugirl == nullptr)		// if not unique or a unique girl can not be found
					{
						ugirl = g_Game->CreateRandomGirl(0, false, false, true, true);	// create a random girl
						if (ugirl)
						{
							type_monster_girls++;
							Girls_list += ((Girls_list.empty()) ? "   " : ",\n   ") + ugirl->m_Realname;
						}
					}
					else				// otherwise set the unique girls stuff
					{
						ugirl->m_States &= ~(1 << STATUS_CATACOMBS);
						type_unique_monster_girls++;
						UGirls_list += ((UGirls_list.empty()) ? "   " : ",\n   ") + ugirl->m_Realname;
					}
					if (ugirl)
					{
						if (g_Game->get_objective() && g_Game->get_objective()->m_Objective == OBJECTIVE_CAPTUREXCATACOMBGIRLS)
						{
                            g_Game->get_objective()->m_SoFar++;
						}
						stringstream Umsg;
						Umsg << ugirl->m_Realname << " was captured in the catacombs by " << girlName << ".\n";
						ugirl->m_Events.AddMessage(Umsg.str(), IMGTYPE_PROFILE, EVENT_DUNGEON);
						g_Game->dungeon().AddGirl(ugirl, DUNGEON_GIRLCAPTURED);	// Either type of girl goes to the dungeon
					}
				}
				else  // Beast type
				{
					g_Game->storage().add_to_beasts(1);
					type_beasts++;
				}
			}
			else if (fight_outcome == 2)  // she lost
			{
				raped = true;
			}
			else if (fight_outcome == 0)  // it was a draw just get a beast
			{
				g_Game->storage().add_to_beasts(1);
				type_beasts++;
			}

			if (tempgirl) delete tempgirl; tempgirl = nullptr;
			if (raped) break;
		} // # of monsters to fight loop

		if (raped)
		{
			ss.str("");
			int NumMon = rng % 6 + 1;
			ss << girl->m_Realname << " was defeated then" << ((NumMon <= 3) ? "" : " gang") << " raped and abused by " << NumMon << " monsters.";
			int health = -NumMon, happy = -NumMon * 5, spirit = -NumMon, sex = -NumMon * 2, combat = -NumMon * 2, injury = 9 + NumMon;

			if (girl->check_virginity())
			{
				ss << " That's a hell of a way to lose your virginity; naturally, she's rather distressed by this fact.";
				girl->lose_virginity();					// Virginity both attrib & trait now, 04/15/13
				health -= 1, happy -= 10, spirit -= 2, sex -= 2, combat -= 2, injury += 2;
			}
			girl->m_Events.AddMessage(ss.str(), IMGTYPE_DEATH, EVENT_DANGER);

			if (!girl->calc_insemination(*cGirls::GetBeast(), false, 1.0 + (NumMon * 0.5)))
			{
				g_Game->push_message(girl->m_Realname + " has gotten inseminated", 0);
				health -= 1, happy -= 10, spirit -= 4, sex -= 4, combat -= 2, injury += 2;
			}

			girl->health(health);
			girl->happiness(happy);
			girl->spirit(spirit);
			cGirls::GirlInjured(girl, injury);
			girl->upd_Enjoyment(ACTION_SEX, sex);
			girl->upd_Enjoyment(actiontype, combat);

			return false;
		}

		int ItemPlace = 0;  // Place in 0..299

		for (int i = num_monsters; i > 0; i--)
		{
			gold += rng % 150;

			// get any items
			while (rng.percent(50 - (num_items * 5))) {
                sInventoryItem* TempItem = g_Game->inventory_manager().GetRandomCatacombItem();

                if (g_Game->player().inventory().add_item(TempItem)) {
                    item_list += ((item_list.empty()) ? "   " : ",\n   ") + TempItem->m_Name;
                    num_items++;
                }
            }
		}
	}		// end uncontrolled girls results


	if (num_monsters > 0)
	{
		ss << "She encountered " << num_monsters << " monster" << (num_monsters > 1 ? "s" : "") << " and captured:\n";
		if (type_monster_girls > 0)
		{
			ss << type_monster_girls << " catacomb girl" << (type_monster_girls > 1 ? "s" : "") << ":\n" << Girls_list << ".\n";
		}
		if (type_unique_monster_girls > 0)
		{
			ss << type_unique_monster_girls << " unique girl" << (type_unique_monster_girls > 1 ? "s" : "") << ":\n" << UGirls_list << ".\n";
		}
		if (type_beasts > 0)
			ss << type_beasts << " beast" << (type_beasts > 1 ? "s." : ".");
		ss << "\n \n";
	}
	if (num_items > 0)
	{
		ss << (num_monsters > 0 ? "Further, she" : "She") << " came out with ";
		if (num_items == 1) ss << "one item:\n";
		else	ss << num_items << " items:\n";
		ss << item_list << ".\n \n";
	}
	if (gold > 0) ss << "She " << (num_monsters + num_items > 0 ? "also " : "") << "came out with " << gold << " gold.";

	if (num_monsters + num_items + gold < 1) ss << "She came out empty handed.";

	girl->m_Events.AddMessage(ss.str(), IMGTYPE_COMBAT, Day0Night1);

	ss.str("");
	if (girl->get_stat(STAT_LIBIDO) > 90 && type_monster_girls + type_unique_monster_girls > 0 && brothel->is_sex_type_allowed(SKILL_LESBIAN))
	{
		ss << girl->m_Realname << " was real horny so she had a little fun with the girl" << (type_monster_girls + type_unique_monster_girls > 1 ? "s" : "") << " she captured.";
		girl->upd_temp_stat(STAT_LIBIDO, -50, true);
		girl->lesbian(type_monster_girls + type_unique_monster_girls);
		girl->m_Events.AddMessage(ss.str(), IMGTYPE_LESBIAN, Day0Night1);
	}
	else if (girl->get_stat(STAT_LIBIDO) > 90 && type_beasts > 0 && brothel->is_sex_type_allowed(SKILL_BEASTIALITY))
	{
		ss << girl->m_Realname << " was real horny so she had a little fun with the beast" << (type_beasts > 1 ? "s" : "") << " she captured.";
		girl->upd_temp_stat(STAT_LIBIDO, -50, true);
		girl->beastiality(type_beasts);
		girl->m_Events.AddMessage(ss.str(), IMGTYPE_BEAST, Day0Night1);
		if (!girl->calc_insemination(*cGirls::GetBeast(), false, 1.0))
		{
			g_Game->push_message(girl->m_Realname + " has gotten inseminated", 0);
		}
	}

	if (girl->is_pregnant())
	{
		if (girl->strength() >= 60)
		{
			ss << "\n \nFighting monsters and exploring the catacombs proved to be quite exhausting for a pregnant girl, even for one as strong as " << girlName << " .\n";
		}
		else
		{
			ss << "\n \nFighting monsters and exploring the catacombs was quite exhausting for a pregnant girl like " << girlName << " .\n";
		}
		girl->tiredness(10 - girl->strength() / 20 );
	}

	wages += gold;
	girl->m_Tips = max(0, tips);
	girl->m_Pay = max(0, wages);

	// Improve girl
	int num = type_monster_girls + type_unique_monster_girls + type_beasts + 1;
	int xp = 5 * num, libido = 5, skill = num;

	if (girl->has_trait( "Quick Learner"))		{ skill += 1; xp += 5; }
	else if (girl->has_trait( "Slow Learner"))	{ skill -= 1; xp -= 5; }
	if (girl->has_trait( "Nymphomaniac"))			libido += 2;
	if (girl->has_trait( "Lesbian"))				libido += type_monster_girls + type_unique_monster_girls;

	girl->exp(xp);
	girl->combat((rng % skill) + 1);
	girl->magic((rng % skill) + 1);
	girl->agility(rng % skill);
	girl->constitution(rng % skill);
	girl->strength(rng % skill);
	girl->upd_temp_stat(STAT_LIBIDO, libido);
	girl->upd_Enjoyment(actiontype, (rng % skill) + 2);

	// Myr: Turned trait gains into functions
	cGirls::PossiblyGainNewTrait(girl, "Tough", 30, actiontype, "She has become pretty Tough from all of the fights she's been in.", Day0Night1);
	cGirls::PossiblyGainNewTrait(girl, "Adventurer", 40, actiontype, "She has been in enough tough spots to consider herself an Adventurer.", Day0Night1);
	cGirls::PossiblyGainNewTrait(girl, "Aggressive", 60, actiontype, "She is getting rather Aggressive from her enjoyment of combat.", Day0Night1);
	if (rng.percent(25) && girl->strength() >= 60 && girl->combat() > girl->magic())
	{
		cGirls::PossiblyGainNewTrait(girl, "Strong", 60, ACTION_COMBAT, girlName + " has become pretty Strong from all of the fights she's been in.", Day0Night1);
	}

	//lose traits
	cGirls::PossiblyLoseExistingTrait(girl, "Fragile", 75, actiontype, girl->m_Realname + " has had to heal from so many injuries you can't say she is fragile anymore.", Day0Night1);

	return false;
}

double cJobManager::JP_ExploreCatacombs(sGirl* girl, bool estimate)
{
	double jobperformance = 0.0;
	if (estimate)	// for third detail string
	{
		jobperformance = girl->combat() +
			girl->agility() / 3 +
			girl->constitution() / 3 +
			girl->magic() / 3;

		if (girl->has_trait( "Incorporeal")) jobperformance += 100;

	}
	else			// for the actual check		// not used
	{

	}

    jobperformance += girl->get_trait_modifier("work.explorecatacombs");

	return jobperformance;
}