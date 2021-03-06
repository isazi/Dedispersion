// Copyright 2017 Netherlands Institute for Radio Astronomy (ASTRON)
// Copyright 2017 Netherlands eScience Center
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <vector>
#include <cmath>

#include <Observation.hpp>


#pragma once

namespace Dedispersion {

std::vector<float> * getShifts(AstroData::Observation & observation, const unsigned int padding);
std::vector<float> * getShiftsStepTwo(AstroData::Observation & observation, const unsigned int padding);

} // Dedispersion

