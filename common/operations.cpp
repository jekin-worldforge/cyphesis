// This file may be redistributed and modified only under the terms of
// the GNU General Public License (See COPYING for details).
// Copyright (C) 2000-2005 Alistair Riddoch

#include "Add.h"
#include "Chop.h"
#include "Cut.h"
#include "Eat.h"
#include "Burn.h"
#include "Delve.h"
#include "Dig.h"
#include "Generic.h"
#include "Mow.h"
#include "Nourish.h"
#include "Setup.h"
#include "Tick.h"
#include "Unseen.h"
#include "Update.h"
#include "Use.h"
#include "Wield.h"

namespace Atlas { namespace Objects { namespace Operation {

/// \brief Constructor for Add operation instances
Add::Add() : Set("", "add")
{
}

/// \brief Constructor for operation instances that inherit from Add
Add::Add(const char * id, const char * parent) : Set(id, parent)
{
}

/// \brief Destructor for Add operations
Add::~Add()
{
}

/// \brief Return an object containing the op_definition for Add operation
Add Add::Class()
{
    Add value("add", "set");
    value.setObjtype(std::string("op_definition"));
    return value;
}

/// \brief Constructor for Add operation instances
Affect::Affect() : Set("", "affect")
{
}

/// \brief Constructor for operation instances that inherit from Affect
Affect::Affect(const char * id, const char * parent) : Set(id, parent)
{
}

/// \brief Destructor for Affect operations
Affect::~Affect()
{
}

/// \brief Return an object containing the op_definition for Affect operation
Affect Affect::Class()
{
    Affect value("affect", "set");
    value.setObjtype(std::string("op_definition"));
    return value;
}

/// \brief Constructor for Chop operation instances
Chop::Chop() : Affect("", "chop")
{
}

/// \brief Constructor for operation instances that inherit from Chop
Chop::Chop(const char * id, const char * parent) : Affect(id, parent)
{
}

/// \brief Destructor for Chop operations
Chop::~Chop()
{
}

/// \brief Return an object containing the op_definition for Chop operation
Chop Chop::Class()
{
    Chop value("chop", "action");
    value.setObjtype(std::string("op_definition"));
    return value;
}

/// \brief Constructor for Cut operation instances
Cut::Cut() : Action("", "cut")
{
}

/// \brief Constructor for operation instances that inherit from Cut
Cut::Cut(const char * id, const char * parent) : Action(id, parent)
{
}

/// \brief Destructor for Cut operations
Cut::~Cut() { }

/// \brief Return an object containing the op_definition for Cut operation
Cut Cut::Class()
{
    Cut value("cut", "action");
    value.setObjtype(std::string("op_definition"));
    return value;
}

/// \brief Constructor for Delve operation instances
Delve::Delve() : Affect("", "delve")
{
}

/// \brief Constructor for operation instances that inherit from Delve
Delve::Delve(const char * id, const char * parent) : Affect(id, parent)
{
}

/// \brief Destructor for Delve operations
Delve::~Delve()
{
}

/// \brief Return an object containing the op_definition for Delve operation
Delve Delve::Class()
{
    Delve value("delve", "action");
    value.setObjtype(std::string("op_definition"));
    return value;
}

/// \brief Constructor for Dig operation instances
Dig::Dig() : Affect("", "dig")
{
}

/// \brief Constructor for operation instances that inherit from Dig
Dig::Dig(const char * id, const char * parent) : Affect(id, parent)
{
}

/// \brief Destructor for Dig operations
Dig::~Dig()
{
}

/// \brief Return an object containing the op_definition for Dig operation
Dig Dig::Class()
{
    Dig value("dig", "action");
    value.setObjtype(std::string("op_definition"));
    return value;
}

/// \brief Constructor for Eat operation instances
Eat::Eat() : Action("", "eat")
{
}

/// \brief Constructor for operation instances that inherit from Eat
Eat::Eat(const char * id, const char * parent) : Action(id, parent)
{
}

/// \brief Destructor for Eat operations
Eat::~Eat()
{
}

/// \brief Return an object containing the op_definition for Eat operation
Eat Eat::Class()
{
    Eat value("eat", "action");
    value.setObjtype(std::string("op_definition"));
    return value;
}

/// \brief Constructor for Burn operation instances
Burn::Burn() : Action("", "burn")
{
}

/// \brief Constructor for operation instances that inherit from Burn
Burn::Burn(const char * id, const char * parent) : Action(id, parent)
{
}

/// \brief Destructor for Burn operations
Burn::~Burn()
{
}

/// \brief Return an object containing the op_definition for Burn operation
Burn Burn::Class()
{
    Burn value("burn", "action");
    value.setObjtype(std::string("op_definition"));
    return value;
}

/// \brief Constructor for Generic operation instances
Generic::Generic(const std::string & p) : RootOperation("", p.c_str())
{
}

/// \brief Destructor for Generic operations
Generic::~Generic()
{
}

/// \brief Return an object containing the op_definition for Generic operations
Generic Generic::Class(const std::string & p)
{
    Generic value(p);
    value.setObjtype(std::string("op_definition"));
    return value;
}

/// \brief Constructor for Mow operation instances
Mow::Mow() : Affect("", "mow")
{
}

/// \brief Constructor for operation instances that inherit from Mow
Mow::Mow(const char * id, const char * parent) : Affect(id, parent)
{
}

/// \brief Destructor for Mow operations
Mow::~Mow()
{
}

/// \brief Return an object containing the op_definition for Mow operation
Mow Mow::Class()
{
    Mow value("mow", "action");
    value.setObjtype(std::string("op_definition"));
    return value;
}

/// \brief Constructor for Nourish operation instances
Nourish::Nourish() : Action("", "nourish")
{
}

/// \brief Constructor for operation instances that inherit from Nourish
Nourish::Nourish(const char * id, const char * parent) : Action(id, parent)
{
}

/// \brief Destructor for Nourish operations
Nourish::~Nourish()
{
}

/// \brief Return an object containing the op_definition for Nourish operation
Nourish Nourish::Class()
{
    Nourish value("nourish", "action");
    value.setObjtype(std::string("op_definition"));
    return value;
}

/// \brief Constructor for Setup operation instances
Setup::Setup() : RootOperation("", "setup")
{
}

/// \brief Constructor for operation instances that inherit from Setup
Setup::Setup(const char * id, const char * parent) : RootOperation(id, parent)
{
}

/// \brief Destructor for Setup operations
Setup::~Setup()
{
}

/// \brief Return an object containing the op_definition for Setup operation
Setup Setup::Class()
{
    Setup value("setup", "root_operation");
    value.setObjtype(std::string("op_definition"));
    return value;
}

/// \brief Constructor for Tick operation instances
Tick::Tick() : RootOperation("", "tick")
{
}

/// \brief Constructor for operation instances that inherit from Tick
Tick::Tick(const char * id, const char * parent) : RootOperation(id, parent)
{
}

/// \brief Destructor for Tick operations
Tick::~Tick()
{
}

/// \brief Return an object containing the op_definition for Tick operation
Tick Tick::Class()
{
    Tick value("tick", "root_operation");
    value.setObjtype(std::string("op_definition"));
    return value;
}

/// \brief Constructor for Unseen operation instances
Unseen::Unseen() : Perception("", "unseen")
{
}

/// \brief Constructor for operation instances that inherit from Unseen
Unseen::Unseen(const char * id, const char * parent) : Perception(id, parent)
{
}

/// \brief Destructor for Unseen operations
Unseen::~Unseen()
{
}

/// \brief Return an object containing the op_definition for Unseen operation
Unseen Unseen::Class()
{
    Unseen value("unseen", "perception");
    value.setObjtype(std::string("op_definition"));
    return value;
}

/// \brief Constructor for Update operation instances
Update::Update() : Tick("", "update")
{
}

/// \brief Constructor for operation instances that inherit from Update
Update::Update(const char * id, const char * parent) : Tick(id, parent)
{
}

/// \breif Destructor for Update operations
Update::~Update()
{
}

/// \brief Return an object containing the op_definition for Update operation
Update Update::Class()
{
    Update value("update", "tick");
    value.setObjtype(std::string("op_definition"));
    return value;
}

/// \brief Constructor for Use operation instances
Use::Use() : Action("", "use")
{
}

/// \brief Constructor for operation instances that inherit from Use
Use::Use(const char * id, const char * parent) : Action(id, parent)
{
}

/// \brief Destructor for Use operations
Use::~Use()
{
}

/// \brief Return an object containing the op_definition for Use operation
Use Use::Class()
{
    Use value("use", "action");
    value.setObjtype(std::string("op_definition"));
    return value;
}

/// \brief Constructor for Wield operation instances
Wield::Wield() : Set("", "wield")
{
}

/// \brief Constructor for operation instances that inherit from Wield
Wield::Wield(const char * id, const char * parent) : Set(id, parent)
{
}

/// \brief Destructor for Wield operations
Wield::~Wield()
{
}

/// \brief Return an object containing the op_definition for Wield operation
Wield Wield::Class()
{
    Wield value("wield", "set");
    value.setObjtype(std::string("op_definition"));
    return value;
}

} } }
