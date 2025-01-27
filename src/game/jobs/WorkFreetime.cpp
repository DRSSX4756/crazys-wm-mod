/*
* Copyright 2009, 2010, The Pink Petal Development Team.
* The Pink Petal Devloment Team are defined as the game's coders
* who meet on http://pinkpetal.org
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
#include "buildings/cBuildingManager.h"
#include "cRng.h"
#include "cInventory.h"
#include <sstream>
#include "events.h"

#include "IGame.h"
#include "cGirls.h"
#include "cShop.h"
#include "cJobManager.h"
#include "character/predicates.h"
#include "buildings/cDungeon.h"

/*    First we give her all the possible choices in the freetimechoice enum
   *    start each name with "FT_"
   *    Don't assign anything to the names, the enum will do that for you.
   //*/
enum freetimechoice
{
    FT_Nothing,            // do nothing special, standard free time only
    FT_Shopping,        // if she has any money she will go shopping
    FT_WindowShopping,    // if she does not have any money or does spend any money
    FT_BuyDrugs,        // if she is an addict or unhappy, she may turn to drugs
    FT_Bath,            // take a bath
    FT_Bed,                // stay in bed
    FT_Church,            // go to church - possibly update to Morality
    FT_Salon,            // go to the salon - checks if the girl has enough money
    FT_Pool,            // go to the local public pool
    FT_Cook,            // cook herself dinner
    FT_ClinicCheckup,    // go to the Clinic
    FT_ClinicVisit,        // go to the Clinic to visit
    FT_WorkOut,            // she works out to stay in shape
    FT_HasTraining,        // she has been trained to be a pet

    // Crazy started adding these but didn't finish them yet
    FT_WatchMovie,        // go to see a movie
    FT_Concert,            // go see a concert
    FT_Picnic,            // go on a picnic
    FT_VisitBar,        // go to the bar
    FT_Club,            // go to a dance club
    FT_Quest,            // go on a quest
    FT_Hobby,            // does she have a hobby?

    // Suggestions to be added
    FT_Counseling,        // she goes to counseling
    FT_WatchFights,        // she goes to the arena as a spectator
    FT_StrollInCity,    // she goes for a walk in the city
    FT_Casino,            // she goes to the casino and gambles
    FT_CountrySide,        // she goes out into the country side for a walk
    FT_GoOnDate,        // she goes out on a date with someone
    FT_VisitKid,        // she goes to visit her kid
    FT_MakeExtraMoney,    // she goes and does odd jobs for extra money



    FT_NumberOfFreeTimeChoices    // Leave this as the last thing on the list to allow for random choices.
};                                // When the choice gets handled, the switch will use the "FT_name" as the case.


const char* event_from_ft(freetimechoice choice) {
    switch (choice)
    {
        case FT_Bath:    return events::GIRL_FREE_TIME_BATH;
        case FT_Bed:     return events::GIRL_FREE_TIME_BED;
        case FT_Church:  return events::GIRL_FREE_TIME_CHURCH;
        case FT_Cook:    return events::GIRL_FREE_TIME_COOKING;
        case FT_WindowShopping:   return events::GIRL_FREE_TIME_WINDOW_SHOPPING;
        case FT_Salon:   return events::GIRL_FREE_TIME_SALON;
        case FT_Picnic:  return events::GIRL_FREE_TIME_PICNIC;
        case FT_Concert: return events::GIRL_FREE_TIME_CONCERT;
        case FT_WorkOut: return events::GIRL_FREE_TIME_WORK_OUT;
        case FT_Hobby:   return events::GIRL_FREE_TIME_HOBBY;
        case FT_WatchMovie:   return events::GIRL_FREE_TIME_MOVIE;
        default:         return nullptr;
    }
}

// `J` Job Brothel - General
sWorkJobResult WorkFreetime(sGirl& girl, bool Day0Night1, cRng& rng)
{
    auto brothel = girl.m_Building;
    girl.exp(100);

    std::stringstream ss;

    int U_Tiredness = -(10 + rng % 21);
    int U_Health = 10 + (girl.constitution() / 10);
    int U_Happiness = 10 + rng % 11;
    int U_Mana = 5 + girl.magic() / 5;
    int U_EXP = 1;    // Just because!
    int U_Beauty = 0;
    int U_Money = 0;
    int U_Morality = 0;
    int U_NPCLove = 0;

    // `J` NOTE: one message to tell she is resting - use a separate one to tell the anything else she does
    ss << "${name} rested and recovered some energy.";
    girl.AddMessage(ss.str(), EImageBaseType::PROFILE, Day0Night1 ? EVENT_NIGHTSHIFT : EVENT_DAYSHIFT);

    sImagePreset imagetype = EImageBaseType::PROFILE;
    auto messagetype = Day0Night1 ? EVENT_NIGHTSHIFT : EVENT_DAYSHIFT;
    ss.str("");
    std::stringstream girldiedmsg;


    int roll = rng.d100();
    int roll_a = rng.d100();
    int roll_c = rng.d100();
    int roll_d = rng.d100();
    int HateLove = 0;
    HateLove = girl.pclove();

    freetimechoice choice = FT_Nothing;    bool choicemade = false;

    if (rng % 2)    // half of the time she will just stay home and rest
    {

        // `J` the test for if the girl can act on that choice is done next

#if 0    // change this to 1 and add your choice to debug a choice

        choice = FT_WorkOut;
        choicemade = true;
#endif

        while (!choicemade) {
#if 1
            // First check if there are some things she will more likely do
            if (girl.health() < 30 &&
                g_Game->has_building(BuildingType::CLINIC))    // if she is in bad health and you own a clinic
            {
                choice = FT_ClinicCheckup;
                choicemade = true;
            } else if (is_addict(girl, true)                    // `J` changed it so only hard drugs will trigger this
                       &&
                       rng.percent(90 - girl.happiness()))                // and the less happy she is, the more likely
            {
                choice = FT_BuyDrugs;
                choicemade = true;
            } else if (girl.get_training(TRAINING_PUPPY) >= 35)    //
            {
                choice = FT_HasTraining;
                choicemade = true;
            } else    // If none of the above take precedence, randomly choose
            {

                choice = freetimechoice(rng % FT_NumberOfFreeTimeChoices);    // randomly choose from all of the choices
                switch (choice) {
                    // these don't need a test
                    case FT_Bath:
                    case FT_Bed:
                    case FT_Church:
                    case FT_Pool:
                    case FT_WindowShopping:
                    case FT_ClinicVisit:
                    case FT_WorkOut:
                    case FT_VisitBar:
                    case FT_Club:
                    case FT_Hobby:
                        choicemade = true;    // ready so continue
                        break;

                        // if there are restrictions for a choice, check them here
                    case FT_Cook:
                        if (girl.m_Enjoyment[ACTION_WORKCOOKING] > -10) {
                            choicemade = true;    // She is not going to cook if she hates it
                        }
                        break;
                    case FT_Shopping:
                        if (girl.m_Money >= 10) {
                            choicemade = true;    // She has enough money for it, so continue
                        } else {
                            choice = FT_WindowShopping;
                            choicemade = true;    // She does not have enough money for it but looking is free
                        }
                        // if she can not afford it, reroll.
                        break;
                    case FT_Salon:
                        if (girl.m_Money >= 10) {
                            choicemade = true;    // She has enough money for it, so continue
                        }
                        // if she can not afford it, reroll.
                        break;
                    case FT_BuyDrugs:
                        if (is_addict(girl) || (girl.happiness() < 20 && !rng.percent(girl.intelligence()))) {
                            choicemade = true;    // if she is an addict or unhappy, she may turn to drugs
                        }
                        break;
                    case FT_ClinicCheckup:
                        if (g_Game->has_building(BuildingType::CLINIC)) {
                            choicemade = true;    // if you own a Clinic, she will go
                        } else if (girl.m_Money >= 25) {
                            choicemade = true;    // if you don't own a Clinic, she will go if she has enough money to pay 25 for a checkup
                        } else if (girl.morality() > 50) {
                            choice = FT_ClinicVisit;
                            choicemade = true;    // if she is a good girl, she will go to help out
                        }
                        break;
                    case FT_WatchMovie:
                        if (girl.m_Money >= 10) {
                            choicemade = true;    // She has enough money for it, so continue
                        }
                        // if she can not afford it, reroll.
                        break;
                    case FT_Concert:
                        if (girl.m_Money >= 25) {
                            choicemade = true;    // She has enough money for it, so continue
                        }
                        // if she can not afford it, reroll.
                        break;
                    case FT_Counseling:
                        if (is_addict(girl) || girl.is_fighter(true) || girl.has_active_trait(traits::SHY) ||
                            girl.has_active_trait(traits::PESSIMIST)) {
                            choicemade = true;
                        }
                        // if she dont need it, reroll.
                        break;
                    case FT_WatchFights:
                        if (girl.m_Money >= 15 && g_Game->has_building(BuildingType::ARENA)) {
                            choicemade = true;    // She has enough money for it, so continue
                        }
                        // if she can not afford it, reroll.
                        break;

                    case FT_GoOnDate:
                        if (girl.any_active_trait({traits::HAS_BOY_FRIEND, traits::HAS_GIRL_FRIEND})) {
                            choicemade = true;    // Doesn't need money
                        }
                        // if she doesn't have one, reroll.
                        break;


                        // These are not ready so reroll.
                    case FT_Picnic:
                    case FT_Quest:
                        //case FT_Counseling:
                    case FT_StrollInCity:
                    case FT_Casino:
                    case FT_CountrySide:
                    case FT_VisitKid:
                    case FT_MakeExtraMoney:
                        break;


                        // anything not in the list gets changed to FT_Nothing and continues
                    case FT_Nothing:
                    default:
                        choice = FT_Nothing;
                        choicemade = true;    // ready so continue
                        break;
                }
            }
#endif
        };


        // `J` Finally, let her do what she chooses.

        // Check if it is a Lua-based event.
        const char* event_ft = event_from_ft(choice);
        if (event_ft) {
            sImageSpec spec;
            spec.BasicImage = EImageBaseType::PROFILE;
            scripting::sLuaEventResult er{&spec};
            girl.CallScriptFunction(event_ft, er);
            ss << er.Text << "\n";
            imagetype = spec.BasicImage;
        } else {
            // no - ok, go the full C++ route
            switch (choice) {
                case FT_Pool:
#if 1
                {
                    imagetype = EImageBaseType::SWIM;
                    U_Happiness += 5;
                    ss << "${name} went to the local pool.\n";

                    if (girl.tiredness() > 70 && rng.percent(10))        // 'MUTE'
                    {
                        ss
                                << "Not realizing how tired she was, she tried to jump into to pool from the high dive but she didn't realize ";
                        if ((rng.percent(25) || girl.any_active_trait({traits::MIND_FUCKED, traits::RETARDED}))) {
                            ss << "the water had been drained out of the pool.";
                            if (girl.has_active_trait(traits::INCORPOREAL)) {
                                ss
                                        << " Lucky for her, not haveing a totally physical body allowed her to survive the sudden stop at the bottom of the pool.\n";
                            } else if (girl.has_active_trait(traits::ABNORMALLY_LARGE_BOOBS) && girl.health() > 50) {
                                ss
                                        << " Luckily her Abnormally Large Boobs cushioned her fall. She may need reconstructive surgery but at least she will live.\n";
                                U_Health -= 45;
                                cGirls::AdjustTraitGroupBreastSize(girl, -2);
                            } else if (girl.has_active_trait(traits::ZOMBIE) && girl.health() > 30) {
                                U_Health -= 25;
                                ss
                                        << " She landed with a thud on the bottom of the pool. Several people around the pool screamed at the sight and then screamed even more as your zombie girl got up and shambled her way out of the pool.\n";
                            } else if (girl.has_active_trait(traits::SKELETON) && girl.health() > 30) {
                                U_Health -= 25;
                                ss
                                        << " She made an almost musical racket as her bare bones crashed onto the concrete pool bed. Several people around the pool looked on curiosly but then started screaming as your skeleton girl pulled herself together and made her way out of the dry pool.\n";
                            } else        // 'MUTE' this is meant to kill the girl
                            {
                                if (girl.has_active_trait(traits::SKELETON)) {
                                    ss
                                            << " She made an almost musical racket as her bare bones crashed onto the concrete pool bed.\n";
                                    girldiedmsg
                                            << "Your Skeleton girl ${name} shattered all her bones in a pool accident and was unable to recover.";
                                } else {
                                    ss
                                            << " She landed on the pool's floor with a loud crack from all her bones shattering.\n";
                                    girldiedmsg << girl.FullName()
                                                << " died from internal bleeding due to an incident at the pool.";
                                }
                                U_Health -= 500;
                                imagetype = EImageBaseType::DEATH;
                                messagetype = EVENT_WARNING;
                            }
                        } else {
                            ss << "how high up the board was and hit the water in a straight belly flop.\n";
                            U_Health -= 10;
                            imagetype = EImageBaseType::SWIM;
                            messagetype = EVENT_WARNING;
                        }
                    } else if (girl.tiredness() > 25) {
                        ss
                                << "Being on the tired side, she just decided to lay around the pool and get some sun.  She is going to have a tan for a few days.\n";
                        U_Beauty += 5;
                        if (rng.percent(20)) {
                            bool success = false;
                            ss << "A rather brave young man saw her laying there and decide to try and remove her top.";
                            if (girl.has_active_trait(traits::PSYCHIC)) {
                                ss << " But using her Psychic skills she stopped him before he could do it.";
                            } else if (!rng.percent(girl.agility()) && rng.percent(40)) {
                                ss << " He succeed and was able to expose her ";
                                if (girl.breast_size() >= BreastSize::MASSIVE_MELONS) {
                                    ss << "gigantic boobs.";
                                } else if (girl.breast_size() >= BreastSize::BIG_BOOBS) {
                                    ss << "large breasts.";
                                } else if (girl.any_active_trait({traits::SMALL_BOOBS, traits::PETITE_BREASTS})) {
                                    ss << "small tits.";
                                }
                                else if (girl.has_active_trait(traits::FLAT_CHEST)) { ss << "rather Flat Chest."; }
                                else { ss << "breasts."; }
                                imagetype = EImageBaseType::NUDE;
                                success = true;
                            } else {
                                ss << " He missed his oppertunity and ran away with her towel.";
                            }
                            if (girl.any_active_trait(
                                    {traits::AGGRESSIVE, traits::ASSASSIN, traits::BRAWLER, traits::FEARLESS,
                                     traits::MERCILESS, traits::TOUGH, traits::TSUNDERE, traits::YANDERE})) {
                                ss << "\n ${name} chased him down and beat the crap out of him. She took back her ";
                                if (girl.has_active_trait(traits::EXHIBITIONIST)) {
                                    ss << (success ? "top" : "towel") << " and";
                                }
                                else { ss << (success ? "top, put it back on then " : "towel and "); }
                                ss << " kicked him in the balls.";
                            }
                            if (success && girl.has_active_trait(traits::EXHIBITIONIST)) {
                                ss << "\n ${name} continused with her day as if nothing had happened, topless.";
                            }
                        }
                    } else {
                        ss << "She took a dive into the pool and swam for awhile enjoying herself.\n";
                    }
                }
#endif
                    break;    // end FT_Pool

                case FT_BuyDrugs:    // `J` zzzzzz - this section needs work
#if 1
                {
                    // 1. if she is addicted she will first attempt to purchase drugs until she has no money
                    if (girl.has_active_trait(traits::VIRAS_BLOOD_ADDICT) ||
                        (girl.happiness() < 20 && rng.percent(10)))    // 10% chance an unhappy girl will try this
                    {
                        cJobManager::AddictBuysDrugs(traits::VIRAS_BLOOD_ADDICT, "Viras Blood", girl, brothel,
                                                     Day0Night1);
                        break;
                    }
                    if (girl.has_active_trait(traits::FAIRY_DUST_ADDICT) ||
                        (girl.happiness() < 20 && rng.percent(50)))    // 50% chance an unhappy girl will try this
                    {
                        cJobManager::AddictBuysDrugs(traits::FAIRY_DUST_ADDICT, "Fairy Dust", girl, brothel,
                                                     Day0Night1);
                        break;
                    }
                    if (girl.has_active_trait(traits::SHROUD_ADDICT) || (girl.happiness() < 20)) {
                        cJobManager::AddictBuysDrugs(traits::SHROUD_ADDICT, "Shroud Mushroom", girl, brothel,
                                                     Day0Night1);
                        break;
                    }
                    if (girl.has_active_trait(traits::ALCOHOLIC) || (girl.happiness() < 30)) {
                        cJobManager::AddictBuysDrugs(traits::ALCOHOLIC, "Alcohol", girl, brothel, Day0Night1);
                        break;
                    }
                }
#endif
                    break;    // end FT_BuyDrugs

                case FT_Shopping:
#if 1
                {
                    // 2. buy any items that catch her fancy
                    int numberToBuy = rng % 10;    // try to buy up to 10 things
                    int itemsBought = 0;
                    std::string buyList;
                    imagetype = EImageBaseType::SHOP;

                    for (int i = 0; i < numberToBuy; i++) {
                        auto item = g_Game->shop().GetRandomItem();
                        if (!item) continue;
                        bool wants = g_Game->shop().GirlWantsItem(girl, *item);

                        if (wants && g_Game->shop().GirlBuyItem(girl, *item)) {
                            buyList += ((buyList.empty()) ? "" : ", ") + item->m_Name;
                            itemsBought++;
                        }

                        if (itemsBought >= 3) break;    // if she already has 3 items stop shopping
                    }         // for # buy chances

                    if (itemsBought > 0) {
                        U_Happiness += 5 + (itemsBought * 2);
                        ss << "She did some shopping, and bought: " << buyList << ".";
                        imagetype = EImageBaseType::SHOP;
                    } else {
                        int hap = rng % 10 - 4;
                        U_Happiness += hap;
                        ss
                                << "${name} wandered around the shops trying things on. Though she wasted the shop girl's time, ";
                        if (hap > 0) ss << " she enjoyed herself a " << (hap > 3 ? "bit." : "lot.");
                        else ss << " she was sad because she didn't find what she liked for a price she could afford.";

                    }
                }
#endif
                    break;    //end FT_Shopping

                case FT_ClinicVisit:
#if 1
                {
                    ss << "${name}" << ((girl.m_Building && girl.m_Building->type() == BuildingType::CLINIC) ?
                                        " hangs out around" : " goes to") << " the Clinic";

                    if (is_addict(girl, true) && girl.get_stat(STAT_MORALITY) <= -60)        // 'Mute'
                    {
                        ss << " to try and steal drugs.\n";
                        if (!rng.percent((girl.agility() + girl.intelligence()) / 2))    // 'Mute' Fail to steal drugs
                        {
                            if (g_Game->has_building(BuildingType::CLINIC) &&
                                rng.percent(50))        // She tries stealing from your clinic
                            {
                                ss
                                        << "She got caught by the guards in your clinic and they brought her to your dungeon.\n";
                                girl.AddMessage(ss.str(), EImageBaseType::PROFILE, EVENT_WARNING);
                                girl.m_DayJob = girl.m_NightJob = JOB_INDUNGEON;
                                assert(girl.m_Building);
                                g_Game->dungeon().AddGirl(girl.m_Building->remove_girl(&girl), DUNGEON_GIRLSTEAL);
                                return {false, 0, 0, 0};
                            } else if (girl.m_Money >= 200 && rng.percent(
                                    50))            // 'Mute' Pay 200 gold fine, if not enough gold goes to prision
                            {
                                ss << "She got caught by the clinic guards and was forced to pay 200 Gold.\n";
                                girl.m_Money -= 200;
                                U_Happiness -= 50;
                            } else if (rng.percent(50)) {
                                ss
                                        << "She got caught by the clinic guards and was unable to pay so they sent her to jail.\n";
                                girl.AddMessage(ss.str(), EImageBaseType::PROFILE, EVENT_WARNING);
                                g_Game->GetPrison().AddGirl(girl.m_Building->remove_girl(&girl));
                                return {false, 0, 0, 0};
                            } else if (rng.percent(20)) // 'Mute' Gets raped by guards
                            {
                                ss << "Unfortunately she got caught and was raped by the guards.\n";
                                U_Happiness -= 50;
                                U_Health -= 10;
                                cJobManager::customer_rape(girl, 5);
                            }
                        } else // 'Mute' Manages to steal Drugs
                        {
                            std::string itemname;
                            std::string itemprefix = "a";
                            /* */if (girl.has_active_trait(traits::FAIRY_DUST_ADDICT)) {
                                itemprefix = "a vial of";
                                itemname = "Fairy Dust";
                            }
                            else if (girl.has_active_trait(traits::SHROUD_ADDICT)) {
                                itemprefix = "a";
                                itemname = "Shroud Mushroom";
                            }
                            else if (girl.has_active_trait(traits::VIRAS_BLOOD_ADDICT)) {
                                itemprefix = "a vial of";
                                itemname = "Vira Blood";
                            }
                            const sInventoryItem* item = g_Game->inventory_manager().GetItem(itemname);
                            while (!item) {
                                int numtry = rng % 6;
                                if (numtry >= 3) numtry = rng % 15;

                                itemprefix = "a ";
                                /* */if (numtry == 0 || numtry == 2) itemprefix = "a vial of ";
                                else if (numtry == 12 || numtry == 13) itemprefix = "an ";

                                switch (numtry) {
                                    case 0:
                                        itemname = "Fairy Dust";
                                        break;
                                    case 1:
                                        itemname = "Shroud Mushroom";
                                        break;
                                    case 2:
                                        itemname = "Vira Blood";
                                        break;
                                    case 3:
                                        if (rng.percent(5)) {
                                            itemname = "Healing Salve (L)";
                                            break;
                                        }
                                    case 4:
                                        if (rng.percent(20)) {
                                            itemname = "Healing Salve (M)";
                                            break;
                                        }
                                    case 5:
                                        if (rng.percent(50)) {
                                            itemname = "Healing Salve (S)";
                                            break;
                                        }
                                    case 6:
                                        itemname = "Healing Salve (T)";
                                        break;
                                    case 7:
                                        if (rng.percent(5)) {
                                            itemname = "Incense of Serenity(L)";
                                            break;
                                        }
                                    case 8:
                                        if (rng.percent(20)) {
                                            itemname = "Incense of Serenity(M)";
                                            break;
                                        }
                                    case 9:
                                        if (rng.percent(50)) {
                                            itemname = "Incense of Serenity(S)";
                                            break;
                                        }
                                    case 10:
                                        itemname = "Incense of Serenity(T)";
                                        break;
                                    case 11:
                                        itemname = "Mana Potion";
                                        break;
                                    case 12:
                                        if (rng.percent(5)) {
                                            itemname = "Oil of Greater Scar Removing";
                                            break;
                                        }
                                    case 13:
                                        if (rng.percent(10)) {
                                            itemname = "Oil of Lesser Scar Removing";
                                            break;
                                        }
                                    default:
                                        break;
                                }
                                item = g_Game->inventory_manager().GetItem(itemname);
                            }
                            ss << "She managed to steal " << itemprefix << itemname << ".\n";
                            girl.add_item(item);
                        }
                    } else if (girl.morality() >= 40) {
                        ss << " to cheer up the patients.";
                        U_Morality += 5;
                    } else if (girl.morality() <= -40) {
                        ss << " to mock the patients for been sick.";
                        U_Morality -= 2;
                    } else {
                        ss << " to check on one of her friends.";// not the best
                    }
                    U_Happiness += 5;
                    // needs more options
                }
#endif
                    break;    // end FT_ClinicVisit

                case FT_ClinicCheckup:
#if 1
                {
                    bool playerclinic = g_Game->has_building(BuildingType::CLINIC);
                    sGirl* doctoronduty = nullptr;
                    sGirl* nurseonduty = nullptr;
                    std::string doctorname = "the Doctor";    // Who?
                    std::string nursename = "the Nurse";
                    if (playerclinic) {
                        doctoronduty = random_girl_on_job(g_Game->buildings(), JOB_DOCTOR, Day0Night1);
                        doctorname = (doctoronduty ? "Doctor " + doctoronduty->FullName() + "" : "the Doctor");
                        nurseonduty = random_girl_on_job(g_Game->buildings(), JOB_NURSE, Day0Night1);
                        nursename = (nurseonduty ? "Nurse " + nurseonduty->FullName() + "" : "the Nurse");
                    }

                    if ((playerclinic && (doctoronduty || nurseonduty)) || girl.m_Money >= 25)    // get a checkup
                    {
                        ss << "${name} went to the Clinic for a checkup.\n";
                        if (girl.has_active_trait(traits::AIDS) || girl.has_active_trait(traits::CHLAMYDIA) ||
                            girl.has_active_trait(traits::HERPES) ||
                            girl.has_active_trait(traits::SYPHILIS)) {
                            if (!doctorname.empty()) ss << doctorname << " told her";
                            else if (!nursename.empty()) ss << nursename << " told her";
                            else ss << "She was told";
                            ss << " she has an STD." << (girl.is_pregnant()
                                                         ? " Her unborn child will most likely be born with it if she can't find a way to cure it"
                                                         : "") << ".\n";
                            U_Happiness -= 5;
                            ss << "Other than that, ";
                        }
                        if (girl.health() >= 90) {
                            ss << "Her check up went wonderful. She was told she was in near perfect health"
                               << (girl.is_pregnant() ? " and her unborn child was growing as expected" : "") << ".\n";
                            U_Happiness += 5;
                        } else if (girl.health() >= 70) {
                            ss << "Her check up went well. She was told she was in good health"
                               << (girl.is_pregnant() ? " and her unborn child was doing well" : "") << ".\n";
                        } else if (girl.health() >= 50) {
                            ss << "Her check up was routine. She was told she was in fair health" << (girl.is_pregnant()
                                                                                                      ? " and she needs to start taking better care of herself for her babys sake"
                                                                                                      : "") << ".\n";
                        } else if (girl.health() >= 20) {
                            ss << "Her check up went poorly. She was told she was in bad health"
                               << (girl.is_pregnant() ? " and that they are concerned for her unborn childs saftey"
                                                      : "") << ".\n";
                            U_Happiness -= 5;
                        } else {
                            ss << "Her check up took a long time. She was in such bad health ";
                            if (!doctorname.empty()) ss << doctorname;
                            else if (!nursename.empty()) ss << nursename;
                            else ss << "it was";
                            ss << " recommended that she get checked into the Clinic for full time treatment.\n";
                            if (girl.is_pregnant())
                                ss << "If she remains in such grave condition, her baby may not survive.\n";
                        }

                        if (!playerclinic)    // if she goes to a clinic not owned by you...
                        {
                            int cost = 25;        // base cost for the checkup
                            if (girl.health() + U_Health <= 90) {
                                ss << "The Doctor decides to give her a booster shot.\n";
                                cost += 25;        // additional cost fot 10 healing
                                U_Health += 10;
                            }
                            ss << "\n${name} was charged " << cost << " gold for her checkup";
                            if (girl.m_Money - cost < 0) {
                                ss
                                        << " but she did not have enough money. The Doctor unzipped his pants and allowed her to work it off";
                                imagetype = EImagePresets::BLOWJOB;
                                girl.oralsex(1);
                            } else {
                                U_Money -= cost;
                            }
                            ss << ".\n";
                        } else if ((doctoronduty || nurseonduty) && girl.health() + U_Health <= 90) {
                            ss << (doctoronduty ? doctorname : nursename) << " gives her a booster shot.";
                            U_Health += 10;
                        }
                    }
                }
#endif
                    break;    // end FT_ClinicCheckup

                case FT_VisitBar:
#if 1
                {
                    sGirl* barmaidonduty = random_girl_on_job(g_Game->buildings(), JOB_BARMAID, Day0Night1);
                    std::string barmaidname = (barmaidonduty ? "Barmaid " + barmaidonduty->FullName() + ""
                                                             : "the Barmaid");
                    ss << "${name} decides to go to the bar.\n";
                    if (girl.has_active_trait(traits::ALCOHOLIC)) {
                        ss << "As an Alcoholic she loves coming to the bar.\n";
                        U_Happiness += 15;
                    }
                    if (girl.happiness() < 50) {
                        ss << "${name} feeling a little down, decide to get drunk while she was at the bar.\n";
                        U_Health -= 5;
                        if (barmaidonduty) {
                            ss << "She sits and talks to " << barmaidname
                               << " most of the night while getting drunk.\n"; //
                        } else {
                            ss << "She sits and talks to the bartender while getting drunk.\n"; //
                        }
                    } else {
                        ss
                                << "${name} was in a good mood so she had a few drinks and talked to the people around her.\n";
                        U_Happiness += 5;
                    }
                }
#endif
                    break;    // end FT_VisitBar

                case FT_Club:
#if 1
                {
                    sGirl* clubbaronduty = random_girl_on_job(g_Game->buildings(), JOB_SLEAZYBARMAID, Day0Night1);
                    std::string clubbarname = (clubbaronduty ? "Bartender " + clubbaronduty->FullName() + ""
                                                             : "the Bartender");
                    sGirl* stripperonduty = random_girl_on_job(g_Game->buildings(), JOB_BARSTRIPPER, Day0Night1);
                    std::string strippername = (stripperonduty ? "Stripper " + stripperonduty->FullName() + ""
                                                               : "the Stripper");
                    sGirl* clubwaitonduty = random_girl_on_job(g_Game->buildings(), JOB_SLEAZYWAITRESS, Day0Night1);
                    std::string clubwaitname = (clubwaitonduty ? "Waitress " + clubwaitonduty->FullName() + ""
                                                               : "the Waitress");

                    ss << "${name} puts on her best dress before leaving and going to the club.";
                    if (clubbaronduty) {
                        ss << "${name} says hi to " << clubbarname << ".\n"; //
                    }
                    if (girl.has_active_trait(traits::LESBIAN)) {
                        ss << " She takes in some exotic dancing from some of the strippers there.";
                        if (roll <= 15 && stripperonduty) {
                            ss << " She ends up getting a lap dance from " << strippername << ".\n";
                        }
                    } else if (clubwaitonduty) {
                        ss << clubwaitname
                           << " took her order then ${name} talked to some of the people around her before heading home.";
                    } else {
                        ss << "She had a few drinks and talked to some of the locals before going home.";
                    }
                    imagetype = EImageBaseType::FORMAL;
                }
#endif
                    break;    // end FT_Club

                case FT_Counseling:
#if 1
                {
                    ss << "${name} decided to go and get some counseling for her ";
                    if (is_addict(girl)) {
                        ss << "addiction.";
                        if (is_addict(girl, true) && rng.percent(20))//doesnt make it
                        {
                            ss << "But on her way their she ran into an old friend who invited her to do some ";
                            if (girl.has_active_trait(traits::SHROUD_ADDICT)) {
                                girl.add_item(g_Game->inventory_manager().GetItem("Shroud Mushroom"));
                                ss << "Shroud Mushrooms.";
                            }
                            if (girl.has_active_trait(traits::FAIRY_DUST_ADDICT)) {
                                girl.add_item(g_Game->inventory_manager().GetItem("Fairy Dust"));
                                ss << "Fairy Dust.";
                            }
                            if (girl.has_active_trait(traits::VIRAS_BLOOD_ADDICT)) {
                                girl.add_item(g_Game->inventory_manager().GetItem("Vira Blood"));
                                ss << "Vira Blood.";
                            }
                        } else//does make it
                        {
                            if (rng.percent(
                                    2))//helps 2% seems about right to me... dont want it to help to often or rehab would be worthless
                            {
                                ss << "She ended up having a major breakthru and ";
                                if (girl.lose_trait(traits::FAIRY_DUST_ADDICT, true)) {
                                    ss << "she is no longer a fairy dust addict.\n";
                                } else if (girl.lose_trait(traits::SHROUD_ADDICT, true)) {
                                    ss << "she is no longer a shroud addict.\n";
                                } else if (girl.lose_trait(traits::VIRAS_BLOOD_ADDICT, true)) {
                                    ss << "she is no longer a viras blood addict.\n";
                                } else if (girl.lose_trait(traits::ALCOHOLIC, true)) {
                                    ss << "she is no longer an alcoholic.\n";
                                } else if (girl.lose_trait(traits::SMOKER, true)) {
                                    ss << "she is no longer a smoker.\n";
                                } else if (girl.lose_trait(traits::CUM_ADDICT, true)) {
                                    ss << "she is no longer a cum addict.\n";
                                }
                            } else//doesnt
                            {
                                ss << "She managed to make it. But it didn't help her any.\n";
                            }
                        }
                    } else if (girl.is_fighter(true)) {
                        ss << "anger problems.";
                        if (rng.percent(20))//doesnt make it
                        {
                            ss
                                    << "But on her way their she ran into someone who pissed her off and she ended up chasing them trying to fight with them.";
                        } else//does make it
                        {
                            if (rng.percent(2))//helps
                            {
                                ss << "She ended up having a major breakthru and ";
                                if (girl.lose_trait(traits::AGGRESSIVE)) {
                                    ss << "she is no longer Aggressive.\n";
                                } else if (girl.lose_trait(traits::TSUNDERE)) {
                                    ss << "she is no longer a Tsundere.\n";
                                } else if (girl.lose_trait(traits::YANDERE)) {
                                    ss << "she is no longer a Yandere.\n";
                                }
                            } else//doesnt
                            {
                                ss << "She managed to make it. But it didn't help her any.\n";
                            }
                        }
                    } else if (girl.has_active_trait(traits::SHY)) {
                        ss << "shyness.\n";
                        if (rng.percent(20))//doesnt make it
                        {
                            ss << "But she couldn't work up the courage to go.";
                        } else//does make it
                        {
                            if (girl.lose_trait(traits::SHY, true, 2))//helps
                            {
                                ss << "She ended up having a major breakthru and she is no longer shy.\n";
                            } else//doesnt
                            {
                                ss << "She managed to make it. But it didn't help her any.\n";
                            }
                        }
                    } else if (girl.has_active_trait(traits::PESSIMIST)) {
                        ss << "pessimistic nature.\n";
                        if (rng.percent(20))//doesnt make it
                        {
                            ss << "But she decided it was pointless.";
                        } else//does make it
                        {
                            if (girl.lose_trait(traits::PESSIMIST, true, 2))//helps
                            {
                                ss << "She ended up having a major breakthru and she is no longer a Pessimist.\n";
                            } else//doesnt
                            {
                                ss << "She managed to make it. But didn't help her any.\n";
                            }
                        }
                    }
                }
#endif
                    break;    // end FT_Counseling

                case FT_WatchFights:
#if 1
                {
                    sGirl* fightgirlonduty = random_girl_on_job(g_Game->buildings(), JOB_FIGHTARENAGIRLS, Day0Night1);
                    std::string fightgirlname = (fightgirlonduty ? "Gladiator " + fightgirlonduty->FullName() + ""
                                                                 : "the Gladiator");
                    sGirl* fightbeastonduty = random_girl_on_job(g_Game->buildings(), JOB_FIGHTBEASTS, Day0Night1);
                    std::string fightbeastname = (fightbeastonduty ? "Beast fighter " + fightbeastonduty->FullName() +
                                                                     "" : "the Beast fighter");
                    ss << "${name} decides to go to the arena and watch some fights.\n";
                    if (fightgirlonduty && fightbeastonduty) {
                        ss << "${name} watched " << fightgirlname << " during her match.";
                        ss << " Then watched " << fightbeastname << " fight a beast.\n";
                    } else if (fightbeastonduty) {
                        ss << "${name} watched " << fightbeastname << " during her match against a beast.\n";
                    } else if (fightgirlonduty) {
                        ss << "${name} watched " << fightgirlname << " during her match.";
                    } else {
                        ss << "${name} took in a few fights before going home.";
                    }
                    if (girl.is_fighter() && rng.percent(40)) {
                        ss << "She felt she could have put on a better show.";
                    } else if (girl.has_active_trait(traits::MEEK)) {
                        ss << "The sight of blood made her feel faint. She really didn't care for this.";
                    }
                }
#endif
                    break;    // end FT_WatchFights

                case FT_Casino:
#if 1
                {
                    sGirl* dealeronduty = random_girl_on_job(g_Game->buildings(), JOB_DEALER, Day0Night1);
                    std::string dealername = (dealeronduty ? "Dealer " + dealeronduty->FullName() + "" : "the Dealer");
                    sGirl* enteronduty = random_girl_on_job(g_Game->buildings(), JOB_ENTERTAINMENT, Day0Night1);
                    std::string entername = (enteronduty ? "Entertainment " + enteronduty->FullName() + ""
                                                         : "the Entertainment");
                    sGirl* xxxonduty = random_girl_on_job(g_Game->buildings(), JOB_XXXENTERTAINMENT, Day0Night1);
                    std::string xxxname = (xxxonduty ? "XXX Entertainment " + xxxonduty->FullName() + ""
                                                     : "the XXX Entertainment");
                    ss << "${name} decides to go to ";
                    int gamble = rng % 1000 + 1;
                    if (roll > 75) {
                        ss << "one of your rivials casinos.\n";
                        //if she gambles and loses more then she has I want it to fire a script where rival brings girl to you and says pay her bill or he keeps her
                        if (roll_a >= 50)//won.. may have her skills at dealing or maybe intel affect this
                        {
                            ss << "${name} won " << gamble << " gold at the card tables.";
                            U_Money += gamble;
                        } else//she loses
                        {
                            ss << "${name} lost " << gamble << " gold at the card tables. ";
                            if (girl.m_Money < gamble) {
                                ss
                                        << "She didn't have enough to pay them so they brought her to you to see if you would.";
                                girl.TriggerEvent(events::GIRL_GAMBLING_LOST_RIVAL);
                            } else {
                                U_Money -= gamble;
                            }
                        }
                    } else {
                        ss << "your casino. ";
                        //if she loses more then she has they bring her in front of you and you can forgive her debt make her a slave or make her "work it off"
                        if (roll_a >= 60)//gambles
                        {
                            if (dealeronduty) {
                                ss << "She sits down at " << dealername << "'s table and decides to gamble some.";
                                /*This could either be based off the dealers skill or gambling odds as it does nothing currently as to if she wins or not */
                                if (girl.m_Money < gamble) {
                                    ss
                                            << "She didn't have enough to pay so they brought her to you to decide her fate.";
                                    girl.TriggerEvent(events::GIRL_GAMBLING_LOST_OWN);
                                } else {
                                    U_Money -= gamble;
                                }
                            } else {
                                ss << "Was going do some gambling but you didn't have any dealers so she went home.";
                            }
                        }
                        if (roll_a >= 30)//enter
                        {
                            if (enteronduty) {
                                ss << "She watchs " << entername << " put on her show.";
                            } else {
                                ss << "She takes in a show.";
                            }
                        } else//xxx
                        {
                            if (xxxonduty) {
                                ss << "She takes in " << xxxname << "'s exotic show.";
                            } else {
                                ss << "She takes in an exotic show.";
                            }
                        }
                    }
                }
#endif
                    break;    // end FT_Casino

                case FT_GoOnDate:
#if 1
                {
                    int enjoy = 0;
                    bool breakup = false;
                    ss << "${name} went on a date with her ";
                    if (girl.has_active_trait(traits::HAS_BOY_FRIEND)) {
                        ss << "boy friend.\n";
                    } else if (girl.has_active_trait(traits::HAS_GIRL_FRIEND)) {
                        ss << "girl friend.\n";
                    }
                    if (rng.percent(10))//is she excited to go?
                    {
                        if (girl.has_active_trait(traits::OPTIMIST)) {
                            ss << "Excited to go on a date with someone she couldn't wait to get the date started.\n";
                            enjoy += 1;
                        } else if (girl.has_active_trait(traits::PESSIMIST)) {
                            ss
                                    << "Already thinking the date would go bad she dragged her feet and didn't really want to go.\n";
                            enjoy -= 1;
                        } else {
                        }
                    } else {
                        if (roll >= 80)//dinner
                        {
                            ss << "They went to dinner. ";
                            if (girl.has_status(STATUS_DATING_PERV))//perv
                            {
                                ss
                                        << "During dinner her date told several dirty jokes and made many lewd comments which she ";
                                if (girl.has_active_trait(traits::SHY)) {
                                    ss << "didn't like at all.\n";
                                    enjoy -= 2;
                                } else if (is_sex_crazy(girl)) {
                                    ss << "really liked.\n";
                                    enjoy += 2;
                                } else if (girl.has_active_trait(traits::BIMBO)) {
                                    ss << "which she didn't get at all.\n";
                                } else {
                                    ss << "didn't care for much either way.\n";
                                }
                                ss << "After dinner ";
                                if (girl.has_active_trait(traits::HAS_BOY_FRIEND)) {
                                    ss << "he walks ${name} home, ";
                                    if (rng.percent(50))//perv so higher chance of trying to get some
                                    {
                                        ss
                                                << "but before getting her home he whips out his cock and asks for a blow job.\n";
                                        if (girl.has_active_trait(traits::SHY)) {

                                            ss
                                                    << "She couldn't believe he would do that and ran off her face red from the thought.\n";
                                            enjoy -= 2;
                                        } else if (girl.has_active_trait(traits::NYMPHOMANIAC)) {
                                            ss << "She loves cock so she agrees and sucks him off.\n";
                                            enjoy += 2;
                                            imagetype = EImagePresets::BLOWJOB;
                                            girl.oralsex(1);
                                        } else if (girl.is_fighter()) {
                                            if (g_Dice.percent(50)) {
                                                ss
                                                        << "She told him he was lucky she kinda liked him if not he would be in a world of pain right now.\n";
                                            } else {
                                                ss
                                                        << "She punched him in the junk and told him next time he might not get off so easy.";
                                                enjoy -= 2;
                                            }
                                        } else {
                                            if (enjoy > 2) {
                                                ss
                                                        << "Told him not to ruin a good night gave him a kiss on the check and walked home alone after that.\n";
                                            } else {
                                                ss << "Told him not to be an ass and walked home alone after that.\n";
                                            }
                                        }
                                    } else {
                                        ss << "and wishes her a good night.\n";
                                    }
                                } else if (girl.has_active_trait(traits::HAS_GIRL_FRIEND)) {
                                    ss << "she walks ${name} home, ";
                                    if (rng.percent(50))//perv so higher chance of trying to get some
                                    {
                                        ss
                                                << "before getting her home she pulls her into an alley and whips out a didlo asking if she wants to have some fun.\n";
                                        if (girl.has_active_trait(traits::SHY)) {
                                            ss
                                                    << "She couldn't believe she would do that and ran off her face red from the thought.\n";
                                            enjoy -= 2;
                                        } else if (girl.has_active_trait(traits::NYMPHOMANIAC)) {
                                            ss << "She loves to have fun so she agrees and they enjoy each other.\n";
                                            enjoy += 2;
                                            imagetype = EImagePresets::LESBIAN;
                                            girl.lesbian(1);
                                        } else if (girl.is_fighter()) {
                                            if (g_Dice.percent(50)) {
                                                ss
                                                        << "She told her she was lucky she kinda liked her if not she would be in a world of pain right now.\n";
                                            } else {
                                                ss
                                                        << "She punched her in the cunt and told her next time she might not get off so easy.";
                                                enjoy -= 2;
                                            }
                                        } else {
                                            if (enjoy > 2) {
                                                ss
                                                        << "Told her not to ruin a good night gave her a kiss on the check and walked home alone after that.\n";
                                            } else {
                                                ss << "Told her not to be an ass and walked home alone after that.\n";
                                            }
                                        }
                                    } else {
                                        ss << "and wishes her a good night.\n";
                                    }
                                }
                            } else if (girl.has_status(STATUS_DATING_MEAN))//mean
                            {
                                ss
                                        << "During dinner her date was very aggressive and overly jealous of everyone which she ";
                                if (girl.has_active_trait(traits::SHY)) {
                                    ss << "didn't like at all.\n";
                                    enjoy -= 1;
                                } else if (girl.has_active_trait(traits::BIMBO)) {
                                    ss << "didn't seem to notice.\n";
                                } else {
                                    ss << "didn't care for much either way.\n";
                                }
                                ss << "After dinner ";
                                if (girl.has_active_trait(traits::HAS_BOY_FRIEND)) {
                                    ss << "he walks ${name} home, ";
                                    if (rng.percent(20))//mean so decent chance of trying to get some
                                    {
                                        ss << "but before getting her home he decides he deserves a blow job.\n";
                                        if (girl.has_active_trait(traits::MEEK)) {
                                            ss
                                                    << "Scared of what he might do if she doesn't she went along with it and sucked him off.\n";
                                            imagetype = EImagePresets::BLOWJOB;
                                            girl.oralsex(1);
                                        } else if (girl.has_active_trait(traits::SHY)) {
                                            ss
                                                    << "She couldn't belive he would do that and ran off her face red from the thought.\n";
                                            enjoy -= 2;
                                        } else {
                                            if (enjoy > 2) {
                                                ss
                                                        << "Told him not to ruin a good night gave him a kiss on the check and walked home alone after that.\n";
                                            } else {
                                                ss << "Told him not to be an ass and walked home alone after that.\n";
                                            }
                                        }
                                    } else {
                                        ss << "and wishes her a good night.\n";
                                    }
                                } else if (girl.has_active_trait(traits::HAS_GIRL_FRIEND)) {
                                    ss << "she walks ${name} home, ";
                                    if (rng.percent(30))//mean so decent chance of trying to get some
                                    {
                                        ss << "but before getting her home she decides she deserves some pleasure.\n";
                                        if (girl.has_active_trait(traits::MEEK)) {
                                            ss
                                                    << "Scared of what she might do if she doesn't she went along with it.\n";
                                            imagetype = EImagePresets::LESBIAN;
                                            girl.lesbian(1);
                                        } else if (girl.has_active_trait(traits::SHY)) {
                                            ss
                                                    << "She couldn't belive she would do that and ran off her face red from the thought.\n";
                                            enjoy -= 2;
                                        } else {
                                            if (enjoy > 2) {
                                                ss
                                                        << "Told her not to ruin a good night gave her a kiss on the check and walked home alone after that.\n";
                                            } else {
                                                ss << "Told her not to be an ass and walked home alone after that.\n";
                                            }
                                        }
                                    } else {
                                        ss << "and wishes her a good night.\n";
                                    }
                                }
                            } else if (girl.has_status(STATUS_DATING_NICE))// nice
                            {
                                ss << "During dinner her date was very nice and gave her many complamints which she ";
                                if (girl.has_active_trait(traits::SHY)) {
                                    ss << "enjoyed, even if it did embarrass her.\n";
                                    enjoy += 1;
                                } else if (girl.has_active_trait(traits::BIMBO)) {
                                    ss << "didn't seem to notice.\n";
                                } else if (girl.is_fighter()) {
                                    ss << "thought made them seem like a pussy.\n";
                                    enjoy -= 1;
                                } else {
                                    ss << "didn't care for much either way.\n";
                                }
                                ss << "After dinner ";
                                if (girl.has_active_trait(traits::HAS_BOY_FRIEND)) {
                                    ss << "he walks ${name} home, ";
                                    if (rng.percent(10))// nice so lowest chance of trying to get some
                                    {
                                        ss << "but before getting her home he decides to ask for a blow job.\n";
                                    } else {
                                        ss << "and wishes her a good night.\n";
                                    }
                                } else if (girl.has_active_trait(traits::HAS_GIRL_FRIEND)) {
                                    ss << "she walks ${name} home, ";
                                    if (rng.percent(10))// nice so lowest chance of trying to get some
                                    {
                                        ss
                                                << "but before getting her home she decides to ask if she wants some pleasure.\n";
                                    } else {
                                        ss << "and wishes her a good night.\n";
                                    }
                                }
                            }
                        } else if (roll >= 60)//movie
                        {
                            ss << "They go to a movie. ";
                            // TODO missing text
                        } else if (roll >= 40) {
                            ss << ".";
                        } else if (roll >= 20) {
                            ss << ".";
                        } else {
                            ss << ".";
                        }
                    }
                    if (breakup) //For if they break up removes traits and puts npclove back to 0
                    {
                        ss << "Not happy with how the date went she broke up with them.";
                        girl.clear_dating();
                        girl.lose_trait(traits::HAS_BOY_FRIEND, true);
                        girl.lose_trait(traits::HAS_GIRL_FRIEND, true);
                    }
                }
#endif
                    break;    // end FT_GoOnDate

                case FT_VisitKid:
#if 1
                {
                    ss << "${name} decides to go visit her kids";
                }
#endif
                    break;    // end FT_VisitKid

                case FT_HasTraining:
#if 1
                {
                    ss
                            << "${name} is far enough along in her training that she has started to behave that way in her free time.";
                    if (girl.get_training(TRAINING_PUPPY) >= 10) {
                        if (roll >= 60) {
                            ss << "Even your pets get the occasional downtime.\n";
                            /**/
                            ss
                                    << "${name} crawled around the house, searching for something to do, or someone to play with. The guards had taken her playmates, the other hounds, earlier. She was a little sad, and Brutus, the large dog she had been licking, wasn't too thrilled either,";
                            ss
                                    << " but ${name} knew better than to complain. After wandering through the halls only to find girls too busy to play, she followed the sound of a pair of giggling";
                            ss
                                    << " voices. ${name} pushed a door open gently with her paw, and found two girl wrapped up in each other's arms. ${name} barked and whined softly, trying to push";
                            ss
                                    << " her nose up on the bed, pawing at one of the girl's  thighs. The other girl looked up from the cunt she was licking and smiled impishly, stroking the puppy's";
                            ss
                                    << " hair. ${name} whined and licked the girl's fingers, as the girl continued to eat her partner's pussy. ${name} wanted to play too, why couldn't she play?\n";
                            /**/
                            ss
                                    << "After a few minutes, ${name} gave up, and nosed around the room looking for other mischief. She wanted so desperately to play with the other girls, her little moist pussy";
                            ss
                                    << " ached. Suddenly a scent caught her nose, and she began nosing around the small piles of clothing on the floor. The smell was faint, but puppies were trained to have";
                            ss
                                    << " an acute sense of smell. The smell grew stronger as ${name} pushed a soft, sheer gown off the pile to reveal a damn pair of pink panties. ${name} sniffed them for";
                            ss
                                    << " a moment, eyeing the girls on the bed. They ignored her. With a sharp yelp, ${name} grabbed the panties with her teeth, and scampered off with her prize.\n";
                            /**/
                            ss
                                    << "${name} curled up peevishly in her kennel, licking and chewing the panties. Good puppies knew better than to chew up clothing, but she couldn't help herself, wishing she";
                            ss
                                    << " had someone to play with during her free time today. As the day wore on, ${name} fell asleep, taking a nap, her mouth still absently chewing on the panties.";
                        } else if (roll >= 30) {
                            ss << "Even your pets get the occasional downtime\n";
                            /**/
                            ss
                                    << "${name}  was left mostly to her own devices today. She enjoyed sleeping in, and remained curled up in her kennel even after the door was open. Someone must have been feeling generous, because her";
                            ss
                                    << " bowl of kibble wasn't taken away when ${name} missed breakfast with the rest of the household, and there was even a treat in her bowl. Several of the Master's girls";
                            ss
                                    << " smiled an pet ${name} while she slowly ate her dog food, but didn't stop to clip a leash to her collar or bring her along with whatever errands or chores.\n";
                            /**/
                            ss
                                    << "Since ${name} had little else to do, she crawled out her puppy door into the yard. The grass felt good on her skin, and she spent some time romping";
                            ss
                                    << " in the garden with the other dogs while the guards watched, smiling. When the sun was getting higher in";
                            ss
                                    << " the sky, ${name} plopped down in the grass, curling up beside one of the Master's large hounds. All the dogs accepted her as part of the pack, like they did with all the";
                            ss
                                    << " Master's pets, even when the pets themselves had trouble accepting it. Soon her tits rose and fell much slower as she slept in the shade with her pack.";
                        } else {
                            ss << "Even your pets get the occasional downtime\n";
                            ss << "She was let outside ";
                            if (rng.percent(20)) {
                                ss << "and she found a spot to take a nap.";
                            } else {
                                ss << "she seen a cat and chased it up a tree.";
                            }
                        }
                    } else if (girl.get_training(TRAINING_PONY) >= 10) {
                        ss << "";
                    }
                }
#endif
                    break;    // end FT_HasTraining




                case FT_Quest:
                    break;    // end FT_Quest
                case FT_StrollInCity:
                    break;    // end FT_StrollInCity
                case FT_CountrySide:
                    break;    // end FT_CountrySide
                case FT_MakeExtraMoney:
#if 1
                {
                    ss << "${name} decides to make some extra money so she ";
                }
#endif
                    break;    // end FT_MakeExtraMoney




                case FT_Nothing:
                default:
                    // nothing is done so do nothing :p
                    break;
            }

            // `J` end of the line
        }
    }






    // `J` only add a new message if something new was done.
    if (ss.str().length() > 0) girl.AddMessage(ss.str(), imagetype, messagetype);

    if(!girl.is_dead())
      girl.upd_base_stat(STAT_HEALTH, U_Health, false);        // do health first in case she dies
    girl.upd_base_stat(STAT_TIREDNESS, U_Tiredness, false);
    girl.happiness(U_Happiness);

    if (girl.health() <= 0)
    {
        if (girldiedmsg.str().length() == 0)
        {
            girldiedmsg << girl.FullName() << " died ";
            switch (choice)
            {
            case FT_Shopping:                girldiedmsg << "shopping";    break;
            case FT_WindowShopping:            girldiedmsg << "window shopping";    break;
            case FT_BuyDrugs:                girldiedmsg << "buying drugs";    break;
            case FT_Bath:                    girldiedmsg << "in the bath";    break;
            case FT_Bed:                    girldiedmsg << "in bed";    break;
            case FT_Church:                    girldiedmsg << "in church";    break;
            case FT_Salon:                    girldiedmsg << "at the salon";    break;
            case FT_Pool:                    girldiedmsg << "at the pool";    break;
            case FT_Cook:                    girldiedmsg << "while cooking";    break;
            case FT_ClinicCheckup:
            case FT_ClinicVisit:            girldiedmsg << "in the clinic";    break;
            case FT_WorkOut:                girldiedmsg << "working out";    break;
            case FT_HasTraining:            girldiedmsg << "while puppy training";    break;
            case FT_WatchMovie:                girldiedmsg << "at the movie theater";    break;
            case FT_Concert:                girldiedmsg << "at a concert";    break;
            case FT_Picnic:                    girldiedmsg << "at a picnic";    break;
            case FT_VisitBar:                girldiedmsg << "at a bar";    break;
            case FT_Club:                    girldiedmsg << "at a club";    break;
            case FT_Quest:                    girldiedmsg << "on a quest";    break;
            case FT_Hobby:                    girldiedmsg << "working on her hobby";    break;
            case FT_Counseling:                girldiedmsg << "in counseling";    break;
            case FT_WatchFights:            girldiedmsg << "watching fights";    break;
            case FT_StrollInCity:            girldiedmsg << "in the city";    break;
            case FT_Casino:                    girldiedmsg << "in a casino";    break;
            case FT_CountrySide:            girldiedmsg << "in the countryside";    break;
            case FT_GoOnDate:                girldiedmsg << "on a date";    break;
            case FT_VisitKid:                girldiedmsg << "visiting with her children";    break;
            case FT_MakeExtraMoney:            girldiedmsg << "erning extra money";    break;
            default:
                girldiedmsg << "on her free time";
                break;
            }
            girldiedmsg << ".";
        }
        g_Game->push_message(girldiedmsg.str(), COLOR_WARNING);
        return {false, 0, 0, 0};
    }

    // update stats and skills
    girl.mana(U_Mana);
    girl.exp(U_EXP);
    girl.upd_temp_stat(STAT_BEAUTY, U_Beauty);
    girl.morality(U_Morality);

    // update money
    girl.m_Money += U_Money;

    return {false, 0, 0, 0};
}

double JP_Freetime(const sGirl& girl, bool estimate)    // not used
{
    return 0;
}
