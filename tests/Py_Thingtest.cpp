// Cyphesis Online RPG Server and AI Engine
// Copyright (C) 2009 Alistair Riddoch
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

#include <Python.h>

#include "TestWorld.h"
#include "TestPropertyManager.h"

#include "rulesets/Python_API.h"
#include "rulesets/Py_Thing.h"
#include "rulesets/Entity.h"
#include "rulesets/Character.h"

#include <cassert>

void check_union()
{
    PyEntity ent;

    Entity * e;
    LocatedEntity * le;
    Character * c;

    e = new Entity("1", 1);
    ent.m_entity.e = e;

    le = ent.m_entity.e;
    assert(le == ent.m_entity.l);
    assert(le == ent.m_entity.e);

    e = new Entity("1", 1);
    ent.m_entity.l = e;

    le = ent.m_entity.e;
    assert(le == ent.m_entity.l);
    assert(le == ent.m_entity.e);

    c = new Character("1", 1);
    ent.m_entity.c = c;

    le = ent.m_entity.e;
    assert(le == ent.m_entity.l);
    assert(le == ent.m_entity.e);
    assert(c == ent.m_entity.e);
    assert(c == ent.m_entity.l);

    ent.m_entity.e = new Entity("1", 1);
}

int main()
{
    check_union();

    new TestPropertyManager;

    init_python_api();

    PyEntity * ent = newPyLocatedEntity();
    assert(ent != 0);
    Py_DECREF(ent);
    ent = newPyEntity();
    assert(ent != 0);
    Py_DECREF(ent);
    ent = newPyCharacter();
    assert(ent != 0);
    Py_DECREF(ent);

    Entity * e = new Entity("1", 1);
    Entity * wrld = new Entity("0", 0);
    e->m_location.m_loc = wrld;
    e->m_location.m_loc->makeContainer();
    assert(e->m_location.m_loc->m_contains != 0);
    e->m_location.m_loc->m_contains->insert(e);
    TestWorld test_world(*wrld);

    assert(PyRun_SimpleString("from server import *") == 0);
    assert(PyRun_SimpleString("from atlas import Operation") == 0);
    assert(PyRun_SimpleString("from atlas import Oplist") == 0);

    assert(PyRun_SimpleString("LocatedEntity()") == -1);
    assert(PyRun_SimpleString("LocatedEntity('s')") == -1);
    assert(PyRun_SimpleString("le=LocatedEntity('1')") == 0);
    assert(PyRun_SimpleString("le.as_entity()") == 0);
    assert(PyRun_SimpleString("le.send_world(Operation('get'))") == -1);
    assert(PyRun_SimpleString("le==LocatedEntity('2')") == 0);
    assert(PyRun_SimpleString("print le.get_task()") == -1);
    assert(PyRun_SimpleString("print le.type") == -1);
    assert(PyRun_SimpleString("print le.foo_operation") == -1);
    assert(PyRun_SimpleString("print le.location") == 0);
    assert(PyRun_SimpleString("print le.contains") == 0);
    assert(PyRun_SimpleString("le.type") == -1);
    assert(PyRun_SimpleString("le.type='non_exist'") == -1);
    assert(PyRun_SimpleString("le.type=1") == -1);
    assert(PyRun_SimpleString("le.type='game_entity'") == 0);
    assert(PyRun_SimpleString("le.type='game_entity'") == -1);
    assert(PyRun_SimpleString("le.type") == 0);
    assert(PyRun_SimpleString("le.map=1") == -1);
    assert(PyRun_SimpleString("le.map_attr={'1': 2}") == 0);
    assert(PyRun_SimpleString("le.map_attr") == 0);
    assert(PyRun_SimpleString("le.list_attr=[1,2]") == 0);
    assert(PyRun_SimpleString("le.list_attr") == 0);
    assert(PyRun_SimpleString("le.non_atlas=set([1,2])") == 0);
    assert(PyRun_SimpleString("le.non_atlas") == 0);
    // assert(PyRun_SimpleString("le.foo=1") == 0);
    // assert(PyRun_SimpleString("le.foo='1'") == 0);
    // assert(PyRun_SimpleString("le.foo=[1]") == 0);
    // assert(PyRun_SimpleString("le.foo=['1']") == 0);
    // assert(PyRun_SimpleString("le.foo={'foo': 1, 'bar': '1'}") == 0);

    assert(PyRun_SimpleString("Thing()") == -1);
    assert(PyRun_SimpleString("Thing('s')") == -1);
    assert(PyRun_SimpleString("Thing(1)") == -1);
    assert(PyRun_SimpleString("t=Thing('1')") == 0);
    assert(PyRun_SimpleString("Thing(t)") == 0);
    assert(PyRun_SimpleString("t.as_entity()") == 0);
    assert(PyRun_SimpleString("t.send_world(Operation('get'))") == 0);
    assert(PyRun_SimpleString("t.send_world('get')") == -1);
    assert(PyRun_SimpleString("print t.get_task()") == -1);
    assert(PyRun_SimpleString("print t.type") == -1);
    assert(PyRun_SimpleString("print t.foo_operation") == -1);
    assert(PyRun_SimpleString("print t.location") == 0);
    assert(PyRun_SimpleString("print t.contains") == 0);

    assert(PyRun_SimpleString("Character()") == -1);
    assert(PyRun_SimpleString("Character('s')") == -1);
    assert(PyRun_SimpleString("Character(1)") == -1);
    assert(PyRun_SimpleString("c=Character('1')") == 0);
    assert(PyRun_SimpleString("Character(c)") == 0);
    assert(PyRun_SimpleString("c.as_entity()") == 0);
    assert(PyRun_SimpleString("c.send_world(Operation('get'))") == 0);
    assert(PyRun_SimpleString("c.get_task()") == 0);
    assert(PyRun_SimpleString("c.start_task()") == -1);
    assert(PyRun_SimpleString("c.start_task(Task(c))") == -1);
    assert(PyRun_SimpleString("c.start_task(1,Operation('cut'),Oplist())") == -1);
    assert(PyRun_SimpleString("c.start_task(Task(c),1,Oplist())") == -1);
    assert(PyRun_SimpleString("c.start_task(Task(c),Operation('cut'),1)") == -1);
    assert(PyRun_SimpleString("c.start_task(Task(c),Operation('cut'),Oplist())") == 0);
    assert(PyRun_SimpleString("c.get_task()") == 0);
    assert(PyRun_SimpleString("c.clear_task()") == 0);
    assert(PyRun_SimpleString("c.mind2body(1)") == -1);
    assert(PyRun_SimpleString("c.mind2body(Operation('update'))") == 0);
    assert(PyRun_SimpleString("c.mind2body(Operation('get'))") == 0);
    assert(PyRun_SimpleString("print c.type") == -1);
    assert(PyRun_SimpleString("print c.foo_operation") == -1);
    assert(PyRun_SimpleString("print c.location") == 0);
    assert(PyRun_SimpleString("print c.contains") == 0);

    shutdown_python_api();
    return 0;
}
