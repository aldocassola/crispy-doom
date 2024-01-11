#include "vertex.hpp"
#include "doomdata.hpp"
#include "i_swap.h"


vertex_t::vertex_t(const mapvertex_t &ms)
    : x(SHORT(ms.x) << FRACBITS),
    y(SHORT(ms.y) << FRACBITS),
    r_x(x),
    r_y(y),
    moved(false) {}