// Cyphesis Online RPG Server and AI Engine
// Copyright (C) 2006 Alistair Riddoch
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA

// $Id$

#ifndef COMMON_PROPERTY_MANAGER_H
#define COMMON_PROPERTY_MANAGER_H

#include <string>

class PropertyBase;

/// \brief Base class for classes that handle creating Entity properties.
class PropertyManager {
  protected:
    /// \brief Singleton instance pointer for any subclass
    static PropertyManager * m_instance;

    PropertyManager();

  public:
    virtual ~PropertyManager();

    /// \brief Add a new named property to an Entity
    ///
    /// @param name a string giving the name of the property.
    virtual PropertyBase * addProperty(const std::string & name,
                                       int type) = 0;

    /// \brief Return the registered singleton instance of any subclass
    static PropertyManager * instance() {
        return m_instance;
    }
};

#endif // COMMON_PROPERTY_MANAGER_H
