#include "trace.h"
#include "../../core/displacement.h"

TraceFilter_t::TraceFilter_t( std::uintptr_t mask, const void* skip, int layer ) noexcept {
	Displacement::TraceFilterInit( this, skip, mask, layer, 7 );
}

void CreateTrace( TraceData_t* const trace, const Vector start, const Vector end, const TraceFilter_t& filter, const int penetrationCount ) {
	Displacement::CreateTrace( trace, start, end, filter, nullptr, nullptr, nullptr, nullptr, penetrationCount );
}

void InitTraceInfo( GameTrace_t* const hit ) {
	Displacement::InitTraceInfo( hit );
}

void GetTraceInfo( TraceData_t* const trace, GameTrace_t* const hit, const float unknownFloat, void* unknown ) {
	Displacement::GetTraceInfo( trace, hit, unknownFloat, unknown );
}

bool HandleBulletPenetration( TraceData_t* const trace, HandleBulletPenetrationData_t* stats, UpdateValue_t* const modValue, const bool draw_showimpacts ) {
	return Displacement::HandleBulletPenetration( trace, stats, modValue, nullptr, nullptr, nullptr, nullptr, nullptr, draw_showimpacts );
}