#pragma once
#include "../../havoc.h"

class CLagCompensation {
public:
	void Main( );
private:
	void UpdateRecord( PlayerEntry_t& entry );
};

namespace Features { inline CLagCompensation LagCompensation; };