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
#include "cJobManager.h"
#include "cBrothel.h"
#include "cCentre.h"
#include "cCustomers.h"
#include "cRng.h"
#include "cInventory.h"
#include "sConfig.h"
#include "cRival.h"
#include <sstream>
#include "CLog.h"
#include "cTrainable.h"
#include "cTariff.h"
#include "cGold.h"
#include "cGangs.h"
#include "cMessageBox.h"
#include "libintl.h"

extern cRng g_Dice;
extern CLog g_LogFile;
extern cCustomers g_Customers;
extern cInventory g_InvManager;
extern cBrothelManager g_Brothels;
extern cCentreManager g_Centre;
extern cGangManager g_Gangs;
extern cMessageQue g_MessageQue;

// `J` Centre Job - Therapy
bool cJobManager::WorkCentreTherapy(sGirl* girl, sBrothel* brothel, bool Day0Night1, string& summary)
{
	string message = "";
	u_int job = 0;	
	int msgtype = Day0Night1;


	if (girl->m_YesterDayJob != JOB_THERAPY)	// if she was not in thearpy yesterday, 
	{
		girl->m_WorkingDay = 0;				// rest working days to 0 before proceding
		girl->m_PrevWorkingDay = 0;
	}
	
	// not for patient
	g_Girls.UnequipCombat(girl);

	bool hasDoctor = false;
	if(g_Centre.GetNumGirlsOnJob(brothel->m_id, JOB_DRUGCOUNSELOR, true) >= 1 || g_Centre.GetNumGirlsOnJob(brothel->m_id, JOB_DRUGCOUNSELOR, false) >= 1)
		hasDoctor = true;

	if (!hasDoctor)
	{
		string message = girl->m_Realname + gettext(" you must have a counselor (require 1)");
		if(Day0Night1 == 0)	message += gettext("day");
		else				message += gettext("night");
		message += gettext(" Shift.");

		girl->m_Events.AddMessage(message, IMGTYPE_PROFILE, EVENT_WARNING);
		return true;
	}
	if (!g_Girls.HasTrait(girl, "Nervous") &&		//  if the girl doesnt need this
		!g_Girls.HasTrait(girl, "Dependant") &&
		!g_Girls.HasTrait(girl, "Pessimist"))
	{
		message = girl->m_Realname + gettext(" doesn't need therapy for anything so she was sent to the waiting room.");
		if (Day0Night1 == 0)	girl->m_Events.AddMessage(message, IMGTYPE_PROFILE, EVENT_WARNING);
		girl->m_YesterDayJob = girl->m_YesterNightJob = JOB_CENTREREST;
		girl->m_DayJob = girl->m_NightJob = JOB_CENTREREST;
		girl->m_PrevWorkingDay = girl->m_WorkingDay = 0;
		return true;
	}


	if(Day0Night1 == 0)
	{
		girl->m_WorkingDay++;
		job	= girl->m_DayJob;
	}
	else
	{
		job	= girl->m_NightJob;
	}

	stringstream ss;
	if(girl->m_WorkingDay == 3)
	{
		g_Girls.UpdateStat(girl, STAT_HAPPINESS, -5);
		g_Girls.UpdateStat(girl, STAT_SPIRIT, -5);
		if (girl->health() - 20 < 1 && (g_Centre.GetNumGirlsOnJob(brothel->m_id, JOB_DRUGCOUNSELOR, true) >= 1 || g_Centre.GetNumGirlsOnJob(brothel->m_id, JOB_DRUGCOUNSELOR, false) >= 1))
		{	// Don't kill the girl from therapy if a Drug Counselor is on duty
			g_Girls.SetStat(girl, STAT_HEALTH, 1);
			g_Girls.UpdateStat(girl, STAT_PCFEAR, 1);
			g_Girls.UpdateStat(girl, STAT_PCLOVE, -1);
			g_Girls.UpdateStat(girl, STAT_PCHATE, 5);
			ss << "She almost died in therapy but the Counselor saved her.\n";
			ss << "She hates you a little more for forcing this on her.";
			girl->m_Events.AddMessage(ss.str(), IMGTYPE_PROFILE, EVENT_DANGER);
		}
		else
		{
			g_Girls.UpdateStat(girl, STAT_HEALTH, -5);
		}
		g_Girls.UpdateStat(girl, STAT_MANA, -5);

		if (girl->health()< 1)
		{
			ss << "She died in therapy.";
			msgtype = EVENT_DANGER;
		}
		else
		{
			ss << "The therapy is a success.\n";
			msgtype = EVENT_GOODNEWS;
			if (g_Girls.HasTrait(girl, "Nervous"))
			{
				g_Girls.RemoveTrait(girl, "Nervous");
				ss << "She is no longer nervous all the time.\n";
			}
			else if (g_Girls.HasTrait(girl, "Dependant"))
			{
				g_Girls.RemoveTrait(girl, "Dependant");
				ss << "She is no longer Dependant on others.\n";
			}
			else if (g_Girls.HasTrait(girl, "Pessimist"))
			{
				g_Girls.RemoveTrait(girl, "Pessimist");
				ss << "She is no longer a Pessimist about everything.\n";
			}
		}
		girl->m_PrevWorkingDay = girl->m_WorkingDay = 0;

		if (g_Girls.HasTrait(girl, "Nervous") || g_Girls.HasTrait(girl, "Dependant") || g_Girls.HasTrait(girl, "Pessimist"))
		{
			// stay in therapy for another session
			ss << "\nShe should stay in therapy to treat her other disorders.";
		}
		else // get out of therapy
		{
			ss << "\nShe has been released from therapy.";
			girl->m_DayJob = JOB_CENTREREST;
			girl->m_NightJob = JOB_CENTREREST;
		}
	}
	else
	{
		ss << "The therapy is in progress (" << (3 - girl->m_WorkingDay) << " day remaining).";
	}

	// Improve girl
	int libido = 1;

	if (g_Girls.HasTrait(girl, "Nymphomaniac"))			{ libido += 2; }

	g_Girls.UpdateTempStat(girl, STAT_LIBIDO, libido);
	girl->m_Events.AddMessage(ss.str(), IMGTYPE_PROFILE, msgtype);

	return false;
}