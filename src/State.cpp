#include "Common.h"
#include "State.h"

namespace PangPang
{
const StateData& State::GetStateData()
{
	return ptr->stateData;
}
}

