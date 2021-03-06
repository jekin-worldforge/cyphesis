// Cyphesis Online RPG Server and AI Engine
// Copyright (C) 2011 Alistair Riddoch
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

#ifdef NDEBUG
#undef NDEBUG
#endif
#ifndef DEBUG
#define DEBUG
#endif

#include "physics/Shape.h"

#include <Atlas/Message/Element.h>

#include <wfmath/axisbox.h>
#include <wfmath/point.h>
#include <wfmath/polygon.h>

#include <iostream>

#include <cassert>

using Atlas::Message::ListType;
using Atlas::Message::MapType;

int main()
{
    {
        MapType m;

        Shape * s = Shape::newFromAtlas(m);

        assert(s == 0);
    }

    // The AxisBox is a little different, and is not covered by the
    // name constructor
    {
        Shape * s = new MathShape<WFMath::AxisBox, 2>(WFMath::AxisBox<2>());

        assert(s != 0);
        assert(!s->isValid());

    }

    {
        Shape * s = new MathShape<WFMath::AxisBox, 2>(
              WFMath::AxisBox<2>(WFMath::Point<2>(0,0),
                                 WFMath::Point<2>(1,1)));

        assert(s != 0);
        assert(s->isValid());

    }

    {
        MapType m;

        Shape * s = new MathShape<WFMath::AxisBox, 2>(WFMath::AxisBox<2>());
        s->fromAtlas(m);

        assert(s != 0);
        assert(!s->isValid());

    }

    {
        MapType m;
        m["type"] = "axisbox";
        m["points"] = ListType(2, 1.);

        Shape * s = new MathShape<WFMath::AxisBox, 2>(WFMath::AxisBox<2>());
        s->fromAtlas(m);

        assert(s != 0);
        assert(s->isValid());

    }

    {
        MapType m;
        m["type"] = "axisbox";
        m["points"] = ListType(2, 1.);

        Shape * s = new MathShape<WFMath::AxisBox, 2>(WFMath::AxisBox<2>());
        s->fromAtlas(m);

        assert(s != 0);
        assert(s->isValid());

        MapType dest;
        s->toAtlas(dest);
    }

    {
        MapType m;
        m["type"] = "axisbox";
        m["points"] = ListType(2, 1.);

        Shape * s = new MathShape<WFMath::AxisBox, 2>(WFMath::AxisBox<2>());
        s->fromAtlas(m);

        assert(s != 0);
        assert(s->isValid());

        WFMath::Point<3> low = s->lowCorner();
        WFMath::Point<3> high = s->highCorner();
        assert(low.isValid());
        assert(high.isValid());
    }

    // The Polygon conversion functions throw if there isn't complete valid
    // polygon data
    {
        Shape * s = new MathShape<WFMath::Polygon, 2>(WFMath::Polygon<2>());

        assert(s != 0);
        assert(s->isValid());
    }

    {
        WFMath::Polygon<2> p;
        p.addCorner(0, WFMath::Point<2>(1,1));
        p.addCorner(0, WFMath::Point<2>(1,0));
        p.addCorner(0, WFMath::Point<2>(0,0));
        Shape * s = new MathShape<WFMath::Polygon, 2>(WFMath::Polygon<2>());

        assert(s != 0);
        assert(s->isValid());
    }

    {
        MapType m;
        m["type"] = "polygon";

        Shape * s = Shape::newFromAtlas(m);

        assert(s == 0);
    }

    {
        MapType m;
        m["type"] = "polygon";
        m["points"] = ListType(3, ListType(2, 1.f));

        Shape * s = Shape::newFromAtlas(m);

        assert(s != 0);
        assert(s->isValid());
    }

    {
        MapType m;
        m["type"] = "polygon";
        m["points"] = ListType(3, ListType(2, 1.f));

        Shape * s = Shape::newFromAtlas(m);

        assert(s != 0);
        assert(s->isValid());

        MapType dest;
        s->toAtlas(dest);
    }

    {
        MapType m;
        m["type"] = "polygon";
        ListType points;
        points.push_back(ListType(2, -1.));
        points.push_back(ListType(2, 1.));
        ListType point(1, 1.);
        point.push_back(-1.);
        points.push_back(point);
        m["points"] = points;

        Shape * s = Shape::newFromAtlas(m);

        assert(s != 0);
        assert(s->isValid());
        assert(s->area() > 1.9);
        assert(s->area() < 2.1);
    }

    {
        MapType m;
        m["type"] = "polygon";
        ListType points;
        points.push_back(ListType(2, -1.));
        points.push_back(ListType(2, 1.));
        ListType point(1, 1.);
        point.push_back(-1.);
        points.push_back(point);
        m["points"] = points;

        Shape * s = Shape::newFromAtlas(m);

        assert(s != 0);
        assert(s->isValid());
        double area = s->area();
        s->scale(2);
        assert(s->isValid());
        assert(area < s->area());
    }

    {
        MapType m;
        m["type"] = "polygon";
        ListType points;
        points.push_back(ListType(2, -1.));
        points.push_back(ListType(2, 1.));
        ListType point(1, 1.);
        point.push_back(-1.);
        points.push_back(point);
        m["points"] = points;

        Shape * s = Shape::newFromAtlas(m);

        assert(s != 0);
        assert(s->isValid());
        WFMath::AxisBox<2> rect = s->footprint();
        std::cout << rect << std::endl;
        assert(rect.isValid());
    }

    {
        MapType m;
        m["type"] = "polygon";
        ListType points;
        points.push_back(ListType(2, -1.));
        points.push_back(ListType(2, 1.));
        ListType point(1, 1.);
        point.push_back(-1.);
        points.push_back(point);
        m["points"] = points;

        Shape * s = Shape::newFromAtlas(m);

        assert(s != 0);
        WFMath::Point<3> low = s->lowCorner();
        WFMath::Point<3> high = s->highCorner();
        assert(low.isValid());
        assert(high.isValid());
    }

    // The Ball conversion functions don't seem to require valid Atlas
    // data
    {
        Shape * s = new MathShape<WFMath::Ball, 2>(WFMath::Ball<2>());

        assert(s != 0);
        assert(!s->isValid());
    }

    {
        Shape * s = new MathShape<WFMath::Ball, 2>(
              WFMath::Ball<2>(WFMath::Point<2>(1,1), 23.f));

        assert(s != 0);
        assert(s->isValid());
    }

    {
        MapType m;
        m["type"] = "ball";

        Shape * s = Shape::newFromAtlas(m);

        assert(s != 0);
        assert(!s->isValid());
    }

    {
        MapType m;
        m["type"] = "ball";
        m["radius"] = 23.9;
        m["position"] = ListType(2, 1.f);

        Shape * s = Shape::newFromAtlas(m);

        assert(s != 0);
        assert(s->isValid());
    }

    {
        MapType m;
        m["type"] = "ball";
        m["radius"] = 23.9;
        m["position"] = ListType(2, 1.f);

        Shape * s = Shape::newFromAtlas(m);

        assert(s != 0);

        MapType dest;
        s->toAtlas(dest);
    }

    {
        MapType m;
        m["type"] = "ball";
        m["radius"] = 23.9;
        m["position"] = ListType(2, 1.f);

        Shape * s = Shape::newFromAtlas(m);

        assert(s != 0);
        WFMath::Point<3> low = s->lowCorner();
        WFMath::Point<3> high = s->highCorner();
        assert(low.isValid());
        assert(high.isValid());
    }

    // The RotBox conversion functions throw if there isn't complete valid
    // polygon data
    {
        Shape * s = new MathShape<WFMath::RotBox, 2>(WFMath::RotBox<2>());

        assert(s != 0);
        assert(!s->isValid());
    }

    {
        Shape * s = new MathShape<WFMath::RotBox, 2>(
            WFMath::RotBox<2>(WFMath::Point<2>(0,0),
                              WFMath::Vector<2>(1,1),
                              WFMath::RotMatrix<2>().identity()));

        assert(s != 0);
        assert(s->isValid());
    }

    {
        MapType m;
        m["type"] = "rotbox";

        Shape * s = Shape::newFromAtlas(m);

        assert(s == 0);
    }

    {
        MapType m;
        m["type"] = "rotbox";
        m["point"] = ListType(2, 1.f);
        m["size"] = ListType(2, 1.f);

        Shape * s = Shape::newFromAtlas(m);

        assert(s != 0);
        assert(s->isValid());
    }

    {
        MapType m;
        m["type"] = "rotbox";
        m["point"] = ListType(2, 1.f);
        m["size"] = ListType(2, 1.f);

        Shape * s = Shape::newFromAtlas(m);

        assert(s != 0);
        assert(s->isValid());

        MapType dest;
        s->toAtlas(dest);
    }

    {
        MapType m;
        m["type"] = "rotbox";
        m["point"] = ListType(2, 1.f);
        m["size"] = ListType(2, 1.f);

        Shape * s = Shape::newFromAtlas(m);

        assert(s != 0);
        WFMath::Point<3> low = s->lowCorner();
        WFMath::Point<3> high = s->highCorner();
        assert(low.isValid());
        assert(high.isValid());
    }

    return 0;
}
