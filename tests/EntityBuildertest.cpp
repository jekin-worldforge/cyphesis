// Cyphesis Online RPG Server and AI Engine
// Copyright (C) 2005 Alistair Riddoch
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

#include "TestWorld.h"

#include "rulesets/World.h"
#include "rulesets/Python_API.h"

#include "server/EntityBuilder.h"
#include "server/EntityFactory.h"

#include "common/Inheritance.h"
#include "common/TypeNode.h"

#include <Atlas/Objects/Anonymous.h>

#include <cassert>

using Atlas::Message::MapType;
using Atlas::Objects::Entity::Anonymous;
using Atlas::Objects::Root;

class ExposedEntityBuilder : public EntityBuilder {
  public:
    explicit ExposedEntityBuilder(BaseWorld & w) : EntityBuilder(w) { }

    EntityKit * getNewFactory(const std::string & clss) {
        return EntityBuilder::getNewFactory(clss);
    }

    const FactoryDict & factoryDict() const { return m_entityFactories; }

};

int main(int argc, char ** argv)
{
    loadConfig(argc, argv);

    init_python_api();

    {
        World e("1", 1);
        TestWorld test_world(e);
        Anonymous attributes;

        EntityBuilder::init(test_world);

        assert(EntityBuilder::instance() != 0);

        assert(EntityBuilder::instance()->newEntity("1", 1, "world", attributes) == 0);
        assert(EntityBuilder::instance()->newEntity("1", 1, "nonexistant", attributes) == 0);
        assert(EntityBuilder::instance()->newEntity("1", 1, "thing", attributes) != 0);

        EntityBuilder::del();
        assert(EntityBuilder::instance() == 0);
        Inheritance::clear();
    }

    {
        World e("1", 1);
        TestWorld test_world(e);
        Anonymous attributes;
        Atlas::Message::Element val;

        EntityBuilder::init(test_world);

        assert(EntityBuilder::instance() != 0);

        Entity * test_ent = EntityBuilder::instance()->newEntity("1", 1, "thing", attributes);
        assert(test_ent != 0);
        assert(!test_ent->getAttr("funky", val));
        assert(val.isNone());

        attributes->setAttr("funky", "true");

        test_ent = EntityBuilder::instance()->newEntity("1", 1, "thing", attributes);
        assert(test_ent != 0);
        assert(test_ent->getAttr("funky", val));
        assert(val.isString());
        assert(val.String() == "true");

        EntityBuilder::del();
        assert(EntityBuilder::instance() == 0);
        Inheritance::clear();
    }

    {
        // Create a test world.
        World e("1", 1);
        TestWorld test_world(e);
        Atlas::Message::Element val;

        // Instance of EntityBuilder with all protected methods exposed
        // for testing
        ExposedEntityBuilder entity_factory(test_world);

        // Attributes for test entities being created
        Anonymous attributes;

        // Create an entity which is an instance of one of the core classes
        Entity * test_ent = entity_factory.newEntity("1", 1, "thing", attributes);
        assert(test_ent != 0);
        // Check the created entity does not have the attribute values we
        // will be testing later
        assert(!test_ent->getAttr("funky", val));
        assert(val.isNone());

        // Set a test attribute
        attributes->setAttr("funky", "true");

        // Create another entity, and check that it has picked up the new
        // attribute value
        test_ent = entity_factory.newEntity("1", 1, "thing", attributes);
        assert(test_ent != 0);
        assert(test_ent->getAttr("funky", val));
        assert(val.isString());
        assert(val.String() == "true");

        // Check that creating an entity of a type we know we have not yet
        // installed results in a null pointer.
        assert(entity_factory.newEntity("1", 1, "custom_type", attributes) == 0);

        // Get a reference to the internal dictionary of entity factories.
        const FactoryDict & factory_dict = entity_factory.factoryDict();

        // Make sure it has some factories in it already.
        assert(!factory_dict.empty());

        // Assert the dictionary does not contain the factory we know we have
        // have not yet installed.
        assert(factory_dict.find("custom_type") == factory_dict.end());

        // Set up a type description for a new type, and install it
        EntityKit * custom_type_factory = new EntityFactory<Thing>();
        custom_type_factory->m_attributes["test_custom_type_attr"] =
              "test_value";
        {
            Anonymous custom_type_desc;
            custom_type_desc->setId("custom_type");
            custom_type_desc->setParents(std::list<std::string>(1, "thing"));
            entity_factory.installFactory("custom_type", "thing",
                                          custom_type_factory, custom_type_desc);
        }

        custom_type_factory->m_type->defaults()["test_custom_type_attr"] = 
              new Property<std::string>; 
        custom_type_factory->m_type->defaults()["test_custom_type_attr"]->set("test_value");

        // Check that the factory dictionary now contains a factory for
        // the custom type we just installed.
        FactoryDict::const_iterator I = factory_dict.find("custom_type");
        assert(I != factory_dict.end());
        assert(custom_type_factory == I->second);

        MapType::const_iterator J;
        // Check the factory has the attributes we described on the custom
        // type.
        J = custom_type_factory->m_attributes.find("test_custom_type_attr");
        assert(J != custom_type_factory->m_attributes.end());
        assert(J->second.isString());
        assert(J->second.String() == "test_value");

        // Create an instance of our custom type, ensuring that it works.
        test_ent = entity_factory.newEntity("1", 1, "custom_type", attributes);
        assert(test_ent != 0);

        // Reset val.
        val = Atlas::Message::Element();
        assert(val.isNone());

        // Check the custom type has the attribute we passed in when creating
        // the instance.
        assert(test_ent->getAttr("funky", val));
        assert(val.isString());
        assert(val.String() == "true");

        assert(test_ent->getType() == custom_type_factory->m_type);

        // Reset val.
        val = Atlas::Message::Element();
        assert(val.isNone());

        // Check the custom type has the attribute described when the
        // custom type was installed.
        assert(test_ent->getAttr("test_custom_type_attr", val));
        assert(val.isString());
        assert(val.String() == "test_value");

        // Check that creating an entity of a type we know we have not yet
        // installed results in a null pointer.
        assert(entity_factory.newEntity("1", 1, "custom_inherited_type", attributes) == 0);

        // Assert the dictionary does not contain the factory we know we have
        // have not yet installed.
        assert(factory_dict.find("custom_inherited_type") == factory_dict.end());

        // Check that the factory dictionary does contain the factory for
        // the second newly installed type
        I = factory_dict.find("custom_inherited_type");
        assert(I != factory_dict.end());
        EntityKit * custom_inherited_type_factory = I->second;
        assert(custom_inherited_type_factory != 0);

        // Check that the factory has inherited the attributes from the
        // first custom type
        J = custom_inherited_type_factory->m_attributes.find("test_custom_type_attr");
        assert(J != custom_inherited_type_factory->m_attributes.end());
        assert(J->second.isString());
        assert(J->second.String() == "test_value");

        // Check that the factory has the attributes specified when installing
        // it
        J = custom_inherited_type_factory->m_attributes.find("test_custom_inherited_type_attr");
        assert(J != custom_inherited_type_factory->m_attributes.end());
        assert(J->second.isString());
        assert(J->second.String() == "test_inherited_value");

        // Creat an instance of the second custom type, ensuring it works.
        test_ent = entity_factory.newEntity("1", 1, "custom_inherited_type", attributes);
        assert(test_ent != 0);

        // Reset val.
        val = Atlas::Message::Element();
        assert(val.isNone());

        // Check the custom type has the attribute we passed in when creating
        // the instance.
        assert(test_ent->getAttr("funky", val));
        assert(val.isString());
        assert(val.String() == "true");

        // Reset val.
        val = Atlas::Message::Element();
        assert(val.isNone());

        // Check the instance of the second custom type has the attribute
        // described when the first custom type was installed.
        assert(test_ent->getAttr("test_custom_type_attr", val));
        assert(val.isString());
        assert(val.String() == "test_value");

        // Reset val.
        val = Atlas::Message::Element();
        assert(val.isNone());

        // Check the custom type has the attribute described when the
        // second custom type was installed
        assert(test_ent->getAttr("test_custom_inherited_type_attr", val));
        assert(val.isString());
        assert(val.String() == "test_inherited_value");

        // Check that the factory dictionary does contain the factory for
        // the second newly installed type
        I = factory_dict.find("custom_inherited_type");
        assert(I != factory_dict.end());
        custom_inherited_type_factory = I->second;
        assert(custom_inherited_type_factory != 0);

        // Check that the factory has inherited the attributes from the
        // first custom type
        J = custom_inherited_type_factory->m_attributes.find("test_custom_type_attr");
        assert(J != custom_inherited_type_factory->m_attributes.end());
        assert(J->second.isString());
        assert(J->second.String() == "test_value");

        // Check that the factory no longer has the attributes we removed
        J = custom_inherited_type_factory->m_attributes.find("test_custom_inherited_type_attr");
        assert(J == custom_inherited_type_factory->m_attributes.end());

        // Creat an instance of the second custom type, ensuring it works.
        test_ent = entity_factory.newEntity("1", 1, "custom_inherited_type", attributes);
        assert(test_ent != 0);

        // Reset val.
        val = Atlas::Message::Element();
        assert(val.isNone());

        // Make sure test nonexistant attribute isn't present
        assert(!test_ent->getAttr("nonexistant", val));
        // Make sure nonexistant attribute isn't present
        assert(!test_ent->getAttr("nonexistant_attribute", val));

        // Reset val.
        val = Atlas::Message::Element();
        assert(val.isNone());

        // Check the custom type has the attribute we passed in when creating
        // the instance.
        assert(test_ent->getAttr("funky", val));
        assert(val.isString());
        assert(val.String() == "true");

        // Reset val.
        val = Atlas::Message::Element();
        assert(val.isNone());

        // Check the instance of the second custom type has the attribute
        // described when the first custom type was installed.
        assert(test_ent->getAttr("test_custom_type_attr", val));
        assert(val.isString());
        assert(val.String() == "test_value");

        // Reset val.
        val = Atlas::Message::Element();
        assert(val.isNone());

        // Check the custom type has the attribute described when the
        // second custom type was installed
        assert(!test_ent->getAttr("test_custom_inherited_type_attr", val));

        // Check that the factory dictionary now contains a factory for
        // the custom type we just installed.
        I = factory_dict.find("custom_type");
        assert(I != factory_dict.end());
        custom_type_factory = I->second;

        // Check the factory has the attributes we described on the custom
        // type.
        J = custom_type_factory->m_attributes.find("test_custom_type_attr");
        assert(J == custom_type_factory->m_attributes.end());

        // Create an instance of our custom type, ensuring that it works.
        test_ent = entity_factory.newEntity("1", 1, "custom_type", attributes);
        assert(test_ent != 0);

        // Reset val.
        val = Atlas::Message::Element();
        assert(val.isNone());

        // Check the custom type has the attribute we passed in when creating
        // the instance.
        assert(test_ent->getAttr("funky", val));
        assert(val.isString());
        assert(val.String() == "true");

        // Reset val.
        val = Atlas::Message::Element();
        assert(val.isNone());

        // Check the custom type no longer has the custom attribute
        assert(!test_ent->getAttr("test_custom_type_attr", val));

        // Check that the factory dictionary does contain the factory for
        // the second newly installed type
        I = factory_dict.find("custom_inherited_type");
        assert(I != factory_dict.end());
        custom_inherited_type_factory = I->second;
        assert(custom_inherited_type_factory != 0);

        // Check that the factory no longer has inherited the attributes
        // from the first custom type which we removed
        J = custom_inherited_type_factory->m_attributes.find("test_custom_type_attr");
        assert(J == custom_inherited_type_factory->m_attributes.end());

        // Check that the factory no longer has the attributes we removed
        J = custom_inherited_type_factory->m_attributes.find("test_custom_inherited_type_attr");
        assert(J == custom_inherited_type_factory->m_attributes.end());

        // Creat an instance of the second custom type, ensuring it works.
        test_ent = entity_factory.newEntity("1", 1, "custom_inherited_type", attributes);
        assert(test_ent != 0);

        // Reset val.
        val = Atlas::Message::Element();
        assert(val.isNone());

        // Make sure test nonexistant attribute isn't present
        assert(!test_ent->getAttr("nonexistant", val));
        // Make sure nonexistant attribute isn't present
        assert(!test_ent->getAttr("nonexistant_attribute", val));

        // Reset val.
        val = Atlas::Message::Element();
        assert(val.isNone());

        // Check the custom type has the attribute we passed in when creating
        // the instance.
        assert(test_ent->getAttr("funky", val));
        assert(val.isString());
        assert(val.String() == "true");

        // Reset val.
        val = Atlas::Message::Element();
        assert(val.isNone());

        // Check the instance of the second custom type has the attribute
        // described when the first custom type was installed.
        assert(!test_ent->getAttr("test_custom_type_attr", val));

        // Reset val.
        val = Atlas::Message::Element();
        assert(val.isNone());

        // Check the custom type has the attribute described when the
        // second custom type was installed
        assert(!test_ent->getAttr("test_custom_inherited_type_attr", val));

        // Check that the factory dictionary now contains a factory for
        // the custom type we just installed.
        I = factory_dict.find("custom_type");
        assert(I != factory_dict.end());
        custom_type_factory = I->second;

        // Check the factory has the attributes we described on the custom
        // type.
        J = custom_type_factory->m_attributes.find("test_custom_type_attr");
        assert(J != custom_type_factory->m_attributes.end());
        assert(J->second.isString());
        assert(J->second.String() == "test_value");

        J = custom_type_factory->m_attributes.find("new_custom_type_attr");
        assert(J != custom_type_factory->m_attributes.end());
        assert(J->second.isString());
        assert(J->second.String() == "new_value");

        // Create an instance of our custom type, ensuring that it works.
        test_ent = entity_factory.newEntity("1", 1, "custom_type", attributes);
        assert(test_ent != 0);

        // Reset val.
        val = Atlas::Message::Element();
        assert(val.isNone());

        // Check the custom type has the attribute we passed in when creating
        // the instance.
        assert(test_ent->getAttr("funky", val));
        assert(val.isString());
        assert(val.String() == "true");

        // Reset val.
        val = Atlas::Message::Element();
        assert(val.isNone());

        // Check the custom type now has the custom attributes
        assert(test_ent->getAttr("test_custom_type_attr", val));
        assert(val.isString());
        assert(val.String() == "test_value");

        assert(test_ent->getAttr("new_custom_type_attr", val));
        assert(val.isString());
        assert(val.String() == "new_value");

        // Check that the factory dictionary does contain the factory for
        // the second newly installed type
        I = factory_dict.find("custom_inherited_type");
        assert(I != factory_dict.end());
        custom_inherited_type_factory = I->second;
        assert(custom_inherited_type_factory != 0);

        // Check that the factory now has inherited the attributes
        // from the first custom type
        J = custom_inherited_type_factory->m_attributes.find("test_custom_type_attr");
        assert(J != custom_inherited_type_factory->m_attributes.end());
        assert(J->second.isString());
        assert(J->second.String() == "test_value");

        J = custom_inherited_type_factory->m_attributes.find("new_custom_type_attr");
        assert(J != custom_inherited_type_factory->m_attributes.end());
        assert(J->second.isString());
        assert(J->second.String() == "new_value");

        // Check that the factory no longer has the attributes we removed
        J = custom_inherited_type_factory->m_attributes.find("test_custom_inherited_type_attr");
        assert(J == custom_inherited_type_factory->m_attributes.end());

        // Creat an instance of the second custom type, ensuring it works.
        test_ent = entity_factory.newEntity("1", 1, "custom_inherited_type", attributes);
        assert(test_ent != 0);

        // Reset val.
        val = Atlas::Message::Element();
        assert(val.isNone());

        // Make sure test nonexistant attribute isn't present
        assert(!test_ent->getAttr("nonexistant", val));
        // Make sure nonexistant attribute isn't present
        assert(!test_ent->getAttr("nonexistant_attribute", val));

        // Reset val.
        val = Atlas::Message::Element();
        assert(val.isNone());

        // Check the custom type has the attribute we passed in when creating
        // the instance.
        assert(test_ent->getAttr("funky", val));
        assert(val.isString());
        assert(val.String() == "true");

        // Reset val.
        val = Atlas::Message::Element();
        assert(val.isNone());

        // Check the instance of the second custom type has the attribute
        // described when the first custom type was installed.
        assert(test_ent->getAttr("test_custom_type_attr", val));
        assert(val.isString());
        assert(val.String() == "test_value");

        assert(test_ent->getAttr("new_custom_type_attr", val));
        assert(val.isString());
        assert(val.String() == "new_value");

        // Reset val.
        val = Atlas::Message::Element();
        assert(val.isNone());

        // Check the custom type no longer has the attribute described when the
        // second custom type was installed
        assert(!test_ent->getAttr("test_custom_inherited_type_attr", val));

    }
}
