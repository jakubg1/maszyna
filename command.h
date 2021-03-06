/*
This Source Code Form is subject to the
terms of the Mozilla Public License, v.
2.0. If a copy of the MPL was not
distributed with this file, You can
obtain one at
http://mozilla.org/MPL/2.0/.
*/

#pragma once

#include <unordered_map>
#include <queue>
#include <unordered_set>

enum class user_command {

    aidriverenable,
    aidriverdisable,
    mastercontrollerincrease,
    mastercontrollerincreasefast,
    mastercontrollerdecrease,
    mastercontrollerdecreasefast,
    mastercontrollerset,
    secondcontrollerincrease,
    secondcontrollerincreasefast,
    secondcontrollerdecrease,
    secondcontrollerdecreasefast,
    secondcontrollerset,
    mucurrentindicatorothersourceactivate,
    independentbrakeincrease,
    independentbrakeincreasefast,
    independentbrakedecrease,
    independentbrakedecreasefast,
    independentbrakeset,
    independentbrakebailoff,
    trainbrakeincrease,
    trainbrakedecrease,
    trainbrakeset,
    trainbrakecharging,
    trainbrakerelease,
    trainbrakefirstservice,
    trainbrakeservice,
    trainbrakefullservice,
    trainbrakehandleoff,
    trainbrakeemergency,
    trainbrakebasepressureincrease,
    trainbrakebasepressuredecrease,
    trainbrakebasepressurereset,
    trainbrakeoperationtoggle,
    manualbrakeincrease,
    manualbrakedecrease,
    alarmchaintoggle,
    wheelspinbrakeactivate,
    sandboxactivate,
    reverserincrease,
    reverserdecrease,
    linebreakertoggle,
    linebreakeropen,
    linebreakerclose,
    convertertoggle,
    converterenable,
    converterdisable,
    convertertogglelocal,
    converteroverloadrelayreset,
    compressortoggle,
    compressorenable,
    compressordisable,
    compressortogglelocal,
    motoroverloadrelaythresholdtoggle,
    motoroverloadrelaythresholdsetlow,
    motoroverloadrelaythresholdsethigh,
    motoroverloadrelayreset,
    notchingrelaytoggle,
    epbrakecontroltoggle,
    brakeactingspeedincrease,
    brakeactingspeeddecrease,
    brakeactingspeedsetcargo,
    brakeactingspeedsetpassenger,
    brakeactingspeedsetrapid,
    brakeloadcompensationincrease,
    brakeloadcompensationdecrease,
    mubrakingindicatortoggle,
    alerteracknowledge,
    hornlowactivate,
    hornhighactivate,
    radiotoggle,
    radiochannelincrease,
    radiochanneldecrease,
    radiostopsend,
    radiostoptest,
    cabchangeforward,
    cabchangebackward,

    viewturn,
    movehorizontal,
    movehorizontalfast,
    movevertical,
    moveverticalfast,
    moveleft,
    moveright,
    moveforward,
    moveback,
    moveup,
    movedown,

    carcouplingincrease,
    carcouplingdisconnect,
    doortoggleleft,
    doortoggleright,
    departureannounce,
    doorlocktoggle,
    pantographcompressorvalvetoggle,
    pantographcompressoractivate,
    pantographtogglefront,
    pantographtogglerear,
    pantographraisefront,
    pantographraiserear,
    pantographlowerfront,
    pantographlowerrear,
    pantographlowerall,
    heatingtoggle,
    heatingenable,
    heatingdisable,
    lightspresetactivatenext,
    lightspresetactivateprevious,
    headlighttoggleleft,
    headlightenableleft,
    headlightdisableleft,
    headlighttoggleright,
    headlightenableright,
    headlightdisableright,
    headlighttoggleupper,
    headlightenableupper,
    headlightdisableupper,
    redmarkertoggleleft,
    redmarkerenableleft,
    redmarkerdisableleft,
    redmarkertoggleright,
    redmarkerenableright,
    redmarkerdisableright,
    headlighttogglerearleft,
    headlighttogglerearright,
    headlighttogglerearupper,
    redmarkertogglerearleft,
    redmarkertogglerearright,
    redmarkerstoggle,
    endsignalstoggle,
    headlightsdimtoggle,
    headlightsdimenable,
    headlightsdimdisable,
    motorconnectorsopen,
    motorconnectorsclose,
    motordisconnect,
    interiorlighttoggle,
    interiorlightenable,
    interiorlightdisable,
    interiorlightdimtoggle,
    interiorlightdimenable,
    interiorlightdimdisable,
    instrumentlighttoggle,
    instrumentlightenable,
    instrumentlightdisable,
    generictoggle0,
    generictoggle1,
    generictoggle2,
    generictoggle3,
    generictoggle4,
    generictoggle5,
    generictoggle6,
    generictoggle7,
    generictoggle8,
    generictoggle9,
    batterytoggle,
    batteryenable,
    batterydisable,

    none = -1
};

enum class command_target {

    userinterface,
    simulation,
/*
    // NOTE: there's no need for consist- and unit-specific commands at this point, but it's a possibility.
    // since command targets are mutually exclusive these don't reduce ranges for individual vehicles etc
    consist = 0x4000,
    unit    = 0x8000,
*/
    // values are combined with object id. 0xffff objects of each type should be quite enough ("for everyone")
    vehicle = 0x10000,
    signal  = 0x20000,
    entity  = 0x40000
};

enum class command_mode {
	oneoff,
	continuous
};

struct command_description {
    std::string name;
    command_target target;
	command_mode mode = command_mode::oneoff;
};

struct command_data {

    user_command command;
    int action; // press, repeat or release
    std::uint64_t param1;
    std::uint64_t param2;
    double time_delta;
};

// command_queues: collects and holds commands from input sources, for processing by their intended recipients
// NOTE: this won't scale well.
// TODO: turn it into a dispatcher and build individual command sequences into the items, where they can be examined without lookups

class command_queue {

public:
// methods
    // posts specified command for specified recipient
    void
        push( command_data const &Command, std::size_t const Recipient );
    // retrieves oldest posted command for specified recipient, if any. returns: true on retrieval, false if there's nothing to retrieve
    bool
        pop( command_data &Command, std::size_t const Recipient );
	void update();

private:
// types
    typedef std::queue<command_data> commanddata_sequence;
    typedef std::unordered_map<std::size_t, commanddata_sequence> commanddatasequence_map;
// members
    commanddatasequence_map m_commands;

	// TODO: this set should contain more than just user_command
	// also, maybe that and all continuous input logic should be in command_relay?
	std::unordered_set<user_command> m_active_continuous;
};

// NOTE: simulation should be a (light) wrapper rather than namespace so we could potentially instance it,
//       but realistically it's not like we're going to run more than one simulation at a time
namespace simulation {

typedef std::vector<command_description> commanddescription_sequence;

extern command_queue Commands;
// TODO: add name to command map, and wrap these two into helper object
extern commanddescription_sequence Commands_descriptions;

}

// command_relay: composite class component, passes specified command to appropriate command stack

class command_relay {

public:
// constructors
// methods
    // posts specified command for the specified recipient
    // TODO: replace uint16_t with recipient handle, based on item id
    void
        post( user_command const Command, std::uint64_t const Param1, std::uint64_t const Param2,
            int const Action, std::uint16_t const Recipient ) const;
private:
// types
// members
};

//---------------------------------------------------------------------------
