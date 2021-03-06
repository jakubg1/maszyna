/*
This Source Code Form is subject to the
terms of the Mozilla Public License, v.
2.0. If a copy of the MPL was not
distributed with this file, You can
obtain one at
http://mozilla.org/MPL/2.0/.
*/

#include "stdafx.h"
#include "Button.h"
#include "parser.h"
#include "Model3d.h"
#include "Console.h"
#include "Logs.h"
#include "renderer.h"

void TButton::Clear(int i)
{
    pModelOn = nullptr;
    pModelOff = nullptr;
    m_state = false;
    if (i >= 0)
        FeedbackBitSet(i);
    Update(); // kasowanie bitu Feedback, o ile jakiś ustawiony
};

void TButton::Init( std::string const &asName, TModel3d *pModel, bool bNewOn ) {

    if( pModel == nullptr ) { return; }

    pModelOn = pModel->GetFromName( asName + "_on" );
    pModelOff = pModel->GetFromName( asName + "_off" );
    m_state = bNewOn;
    Update();
};

void TButton::Load( cParser &Parser, TDynamicObject const *Owner, TModel3d *pModel1, TModel3d *pModel2 ) {

    std::string submodelname;

    Parser.getTokens();
    if( Parser.peek() != "{" ) {
        // old fixed size config
        Parser >> submodelname;
    }
    else {
        // new, block type config
        // TODO: rework the base part into yaml-compatible flow style mapping
        submodelname = Parser.getToken<std::string>( false );
        while( true == Load_mapping( Parser ) ) {
            ; // all work done by while()
        }
    }

    // bind defined sounds with the button owner
    m_soundfxincrease.owner( Owner );
    m_soundfxdecrease.owner( Owner );

    if( pModel1 ) {
        // poszukiwanie submodeli w modelu
        Init( submodelname, pModel1, false );
    }
    if( ( pModelOn  == nullptr )
     && ( pModelOff == nullptr )
     && ( pModel2   != nullptr ) ) {
        // poszukiwanie submodeli w modelu
        Init( submodelname, pModel2, false );
    }

    if( ( pModelOn  == nullptr )
     && ( pModelOff == nullptr ) ) {
        // if we failed to locate even one state submodel, cry
        ErrorLog( "Bad model: failed to locate sub-model \"" + submodelname + "\" in 3d model \"" + pModel1->NameGet() + "\"", logtype::model );
    }

    // pass submodel location to defined sounds
    auto const offset { model_offset() };
    m_soundfxincrease.offset( offset );
    m_soundfxdecrease.offset( offset );
}

bool
TButton::Load_mapping( cParser &Input ) {

    // token can be a key or block end
    std::string const key { Input.getToken<std::string>( true, "\n\r\t  ,;" ) };
    if( ( true == key.empty() ) || ( key == "}" ) ) { return false; }
    // if not block end then the key is followed by assigned value or sub-block
         if( key == "soundinc:" ) { m_soundfxincrease.deserialize( Input, sound_type::single ); }
    else if( key == "sounddec:" ) { m_soundfxdecrease.deserialize( Input, sound_type::single ); }

    return true; // return value marks a key: value pair was extracted, nothing about whether it's recognized
}

// returns offset of submodel associated with the button from the model centre
glm::vec3
TButton::model_offset() const {

    auto const
        submodel { (
            pModelOn ? pModelOn :
            pModelOff ? pModelOff :
            nullptr ) };

    return (
        submodel != nullptr ?
            submodel->offset( std::numeric_limits<float>::max() ) :
            glm::vec3() );
}

void
TButton::Turn( bool const State ) {

    if( State != m_state ) {

        m_state = State;
        play();
        Update();
    }
}

void TButton::Update() {

    if( (  bData != nullptr )
     && ( *bData != m_state ) ) {

        m_state = ( *bData );
        play();
    }

    if( pModelOn  != nullptr ) { pModelOn->iVisible  =   m_state; }
    if( pModelOff != nullptr ) { pModelOff->iVisible = (!m_state); }

#ifdef _WIN32
    if (iFeedbackBit) {
        // jeżeli generuje informację zwrotną
        if (m_state) // zapalenie
            Console::BitsSet(iFeedbackBit);
        else
            Console::BitsClear(iFeedbackBit);
    }
#endif
};

void TButton::AssignBool(bool const *bValue) {

    bData = bValue;
}

void
TButton::play() {
    if( m_state == true ) { m_soundfxincrease.play(); }
    else                  { m_soundfxdecrease.play(); }
}
