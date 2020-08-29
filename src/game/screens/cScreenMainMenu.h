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
#pragma once

#include "interface/cInterfaceWindowXML.h"

class cScreenMainMenu : public cInterfaceWindowXML
{
public:
    void set_ids() override;

    cScreenMainMenu(std::string saves_path);
    void init(bool back) override;
    void process() override {};

private:
    int continue_id;
    int load_id;
    int new_id;
    int settings_id;
    int quit_id;
    int version_id;

    std::string m_SaveGamesPath;
};
