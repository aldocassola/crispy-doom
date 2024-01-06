#include "subsector.hpp"
#include "doomdata.hpp"
#include "i_swap.hpp"

subsector_t::subsector_t(const mapsubsector_t &ms)
    : sector(nullptr),
    numlines(static_cast<unsigned short>(SHORT(ms.numsegs))),
    firstline(static_cast<unsigned short>(SHORT(ms.firstseg)))
    {}