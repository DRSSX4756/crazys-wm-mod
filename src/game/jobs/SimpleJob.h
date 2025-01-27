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


#ifndef WM_SIMPLEJOB_H
#define WM_SIMPLEJOB_H

#include "BasicJob.h"
#include "utils/piecewise_linear.h"
#include "images/sImageSpec.h"

struct sSimpleJobData {
    Action_Types Action;
    int BaseWages = 0;
    sImagePreset DefaultImage = EImageBaseType::PROFILE;
    bool IsCombatJob = false;
};


class cSimpleJob : public cBasicJob {
public:
    cSimpleJob(JOBS job, const char* xml, sSimpleJobData data);
    sWorkJobResult DoWork(sGirl& girl, bool is_night) override;
    /// Run the job processing, and return whether the girl refused.
    virtual bool JobProcessing(sGirl& girl, IBuilding& brothel, bool is_night) = 0;

protected:
    eCheckWorkResult CheckWork(sGirl& girl, bool is_night) override;

    /// This function should check if `girl` is, in principle, available to do this job.
    /// It should return false if she could not work even if she wanted to, due to external circumstances.
    /// In that case a message should be attached to girl.
    virtual bool CheckCanWork(sGirl& girl, bool is_night) { return true; };

    void InitWork() override;
    void HandleGains(sGirl& girl, int fame);
    sSimpleJobData m_Data;

    PiecewiseLinearFunction m_PerformanceToEarnings;

    void load_from_xml_callback(const tinyxml2::XMLElement& job_element) override;
    void shift_enjoyment();

    /// Returns a number between 0 and 5 (inclusive) that classifies the given performance, from worst (0) to perfect (5)
    static int get_performance_class(int performance);

    template<class T>
    T performance_based_lookup(T worst, T bad, T ok, T good, T great, T perfect) {
        switch(get_performance_class(m_Performance)) {
            case 0: return worst;
            case 1: return bad;
            case 2: return ok;
            case 3: return good;
            case 4: return great;
            case 5: return perfect;
            default: __builtin_unreachable();
        }
    }

    int m_Enjoyment;
    sImagePreset m_ImageType;
};


#endif //WM_SIMPLEJOB_H
