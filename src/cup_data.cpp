//  $Id$
//
//  SuperTuxKart - a fun racing game with go-kart
//  Copyright (C) 2004-2005 Ingo Ruhnke <grumbel@gmx.de>
//  Copyright (C) 2006      Joerg Henrichs, Ingo Ruhnke <grumbel@gmx.de>
//
//  This program is free software; you can redistribute it and/or
//  modify it under the terms of the GNU General Public License
//  as published by the Free Software Foundation; either version 3
//  of the License, or (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

#include <iostream>
#include <stdexcept>
#include "file_manager.hpp"
#include "lisp/parser.hpp"
#include "lisp/lisp.hpp"
#include "cup_data.hpp"
#include "string_utils.hpp"
#include "track_manager.hpp"

CupData::CupData(const std::string filename)
{
    m_filename = filename;
    m_id       = StringUtils::without_extension(filename);
    const lisp::Lisp* lisp = 0;
    try
    {
        lisp::Parser parser;
        lisp = parser.parse(file_manager->getConfigFile(m_filename));

        lisp = lisp->getLisp("supertuxkart-cup");
        if(!lisp)
        {
            throw std::runtime_error("No supertuxkart-cup node");
        }

        lisp->get      ("name",        m_name        );
        lisp->get      ("description", m_description );
        lisp->get      ("herring",     m_herring_style);
        lisp->getVector("tracks",      m_tracks      );
        lisp->getVector("laps",        m_laps        );
    }
    catch(std::exception& err)
    {
        fprintf(stderr, "Error while reading cup file '%s'\n", filename.c_str());
        fprintf(stderr, err.what());
        fprintf(stderr, "\n");
    }

    delete lisp;
}
// ----------------------------------------------------------------------------
bool CupData::checkConsistency()
{
    bool correct=true;
    for(unsigned int i=0; i<m_tracks.size(); i++)
    {
        if(!track_manager->getTrack(m_tracks[i]))
        {
            fprintf(stderr, "Grand Prix '%s': Track '%s' does not exist!",
                    m_name.c_str(), m_tracks[i]);
            correct=false;
        }
    }   // for i
    return correct;
}
/* EOF */
