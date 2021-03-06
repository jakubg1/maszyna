﻿/*
This Source Code Form is subject to the
terms of the Mozilla Public License, v.
2.0. If a copy of the MPL was not
distributed with this file, You can
obtain one at
http://mozilla.org/MPL/2.0/.
*/

#pragma once

#include <vector>

#include "Classes.h"
#include "material.h"
#include "vertex.h"
#include "openglgeometrybank.h"

struct lighting_data {

    glm::vec4 diffuse  { 0.8f, 0.8f, 0.8f, 1.0f };
    glm::vec4 ambient  { 0.2f, 0.2f, 0.2f, 1.0f };
    glm::vec4 specular { 0.0f, 0.0f, 0.0f, 1.0f };

    // stores content of the struct in provided output stream
    void
        serialize( std::ostream &Output ) const;
    // restores content of the struct from provided input stream
    void
        deserialize( std::istream &Input );
};

inline
bool
operator==( lighting_data const &Left, lighting_data const &Right ) {
    return ( ( Left.diffuse  == Right.diffuse )
          && ( Left.ambient  == Right.ambient )
          && ( Left.specular == Right.specular ) );
}

inline
bool
operator!=( lighting_data const &Left, lighting_data const &Right ) {
    return !( Left == Right );
}

namespace scene {

struct bounding_area {

    glm::dvec3 center; // mid point of the rectangle
    float radius { -1.0f }; // radius of the bounding sphere

    bounding_area() = default;
    bounding_area( glm::dvec3 Center, float Radius ) :
        center( Center ),
        radius( Radius )
        {}
    // stores content of the struct in provided output stream
    void
        serialize( std::ostream &Output ) const;
    // restores content of the struct from provided input stream
    void
        deserialize( std::istream &Input );
};

struct node_data {

    double range_min { 0.0 };
    double range_max { std::numeric_limits<double>::max() };
    std::string name;
    std::string type;
};

// holds unique piece of geometry, covered with single material
class shape_node {

    friend class basic_region; // region might want to modify node content when it's being inserted

public:
// types
    struct shapenode_data {
    // members:
        // placement and visibility
        scene::bounding_area area; // bounding area, in world coordinates
        double rangesquared_min { 0.0 }; // visibility range, min
        double rangesquared_max { 0.0 }; // visibility range, max
        bool visible { true }; // visibility flag
        // material data
        bool translucent { false }; // whether opaque or translucent
        material_handle material { null_handle };
        lighting_data lighting;
        // geometry data
        glm::dvec3 origin; // world position of the relative coordinate system origin
        gfx::geometry_handle geometry { 0, 0 }; // relative origin-centered chunk of geometry held by gfx renderer
        std::vector<world_vertex> vertices; // world space source data of the geometry
    // methods:
        // sends content of the struct to provided stream
        void
            serialize( std::ostream &Output ) const;
        // restores content of the struct from provided input stream
        void
            deserialize( std::istream &Input );
    };

// methods
    // sends content of the class to provided stream
    void
        serialize( std::ostream &Output ) const;
    // restores content of the node from provided input stream
    shape_node &
        deserialize( std::istream &Input );
    // restores content of the node from provided input stream
    shape_node &
        deserialize( cParser &Input, scene::node_data const &Nodedata );
    // imports data from provided submodel
    shape_node &
        convert( TSubModel const *Submodel );
    // adds content of provided node to already enclosed geometry. returns: true if merge could be performed
    bool
        merge( shape_node &Shape );
    // generates renderable version of held non-instanced geometry in specified geometry bank
    void
        create_geometry( gfx::geometrybank_handle const &Bank );
    // calculates shape's bounding radius
    void
        compute_radius();
    // set visibility
    void
        visible( bool State );
    // set origin point
    void
        origin( glm::dvec3 Origin );
    // data access
    shapenode_data const &
        data() const;

private:
// members
    std::string m_name;
    shapenode_data m_data;
};

// set visibility
inline
void
shape_node::visible( bool State ) {
    m_data.visible = State;
}
// set origin point
inline
void
shape_node::origin( glm::dvec3 Origin ) {
    m_data.origin = Origin;
}
// data access
inline
shape_node::shapenode_data const &
shape_node::data() const {
    return m_data;
}



// holds a group of untextured lines
class lines_node {

    friend class basic_region; // region might want to modify node content when it's being inserted

public:
// types
    struct linesnode_data {
    // members:
        // placement and visibility
        scene::bounding_area area; // bounding area, in world coordinates
        double rangesquared_min { 0.0 }; // visibility range, min
        double rangesquared_max { 0.0 }; // visibility range, max
        bool visible { true }; // visibility flag
        // material data
        float line_width { 1.f }; // thickness of stored lines
        lighting_data lighting;
        // geometry data
        glm::dvec3 origin; // world position of the relative coordinate system origin
        gfx::geometry_handle geometry { 0, 0 }; // relative origin-centered chunk of geometry held by gfx renderer
        std::vector<world_vertex> vertices; // world space source data of the geometry
    // methods:
        // sends content of the struct to provided stream
        void
            serialize( std::ostream &Output ) const;
        // restores content of the struct from provided input stream
        void
            deserialize( std::istream &Input );
    };

// methods
    // sends content of the class to provided stream
    void
        serialize( std::ostream &Output ) const;
    // restores content of the node from provided input stream
    lines_node &
        deserialize( std::istream &Input );
    // restores content of the node from provided input stream
    lines_node &
        deserialize( cParser &Input, scene::node_data const &Nodedata );
    // adds content of provided node to already enclosed geometry. returns: true if merge could be performed
    bool
        merge( lines_node &Lines );
    // generates renderable version of held non-instanced geometry in specified geometry bank
    void
        create_geometry( gfx::geometrybank_handle const &Bank );
    // calculates shape's bounding radius
    void
        compute_radius();
    // set visibility
    void
        visible( bool State );
    // set origin point
    void
        origin( glm::dvec3 Origin );
    // data access
    linesnode_data const &
        data() const;

private:
// members
    std::string m_name;
    linesnode_data m_data;
};

// set visibility
inline
void
lines_node::visible( bool State ) {
    m_data.visible = State;
}
// set origin point
inline
void
lines_node::origin( glm::dvec3 Origin ) {
    m_data.origin = Origin;
}
// data access
inline
lines_node::linesnode_data const &
lines_node::data() const {
    return m_data;
}


/*
// holds geometry for specific piece of track/road/waterway
class path_node {

    friend class basic_region; // region might want to modify node content when it's being inserted

public:
// types
    // TODO: enable after track class refactoring
    struct pathnode_data {
        // placement and visibility
        bounding_area area; // bounding area, in world coordinates
        bool visible { true }; // visibility flag
        // material data
        material_handle material_1 { 0 };
        material_handle material_2 { 0 };
        lighting_data lighting;
        TEnvironmentType environment { e_flat };
        // geometry data
        std::vector<world_vertex> vertices; // world space source data of the geometry
        glm::dvec3 origin; // world position of the relative coordinate system origin
        using geometryhandle_sequence = std::vector<geometry_handle>;
        geometryhandle_sequence geometry_1; // geometry chunks textured with texture 1
        geometryhandle_sequence geometry_2; // geometry chunks textured with texture 2
    };
// methods
    // restores content of the node from provded input stream
    // TODO: implement
    path_node &
        deserialize( cParser &Input, node_data const &Nodedata );
    // binds specified track to the node
    // TODO: remove after track class refactoring
    void
        path( TTrack *Path ) {
            m_path = Path; }
    TTrack *
        path() {
            return m_path; }

private:
// members

//    // TODO: enable after track class refactoring
//    pathnode_data m_data;

    TTrack * m_path;
};
*/
} // scene



namespace editor {

// base interface for nodes which can be actvated in scenario editor
struct basic_node {

public:
// constructor
    explicit basic_node( scene::node_data const &Nodedata );
// destructor
    virtual ~basic_node() = default;
// methods
    std::string const &
        name() const;
    void
        location( glm::dvec3 const Location );
    glm::dvec3 const &
        location() const;
    float const &
        radius();
    void
        visible( bool const Visible );
    bool
        visible() const;

protected:
// methods
    // radius() subclass details, calculates node's bounding radius
    virtual void radius_();
// members
    scene::bounding_area m_area;
    bool m_visible { true };
    double m_rangesquaredmin { 0.0 }; // visibility range, min
    double m_rangesquaredmax { 0.0 }; // visibility range, max
    std::string m_name;
};

inline
std::string const &
basic_node::name() const {
    return m_name;
}

inline
void
basic_node::location( glm::dvec3 const Location ) {
    m_area.center = Location;
}

inline
glm::dvec3 const &
basic_node::location() const {
    return m_area.center;
}

inline
void
basic_node::visible( bool const Visible ) {
    m_visible = Visible;
}

inline
bool
basic_node::visible() const {
    return m_visible;
}

} // editor

//---------------------------------------------------------------------------
