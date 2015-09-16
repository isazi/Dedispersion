// Copyright 2014 Alessio Sclocco <a.sclocco@vu.nl>
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

#include <string>
#include <vector>
#include <map>
#include <fstream>

#include <Observation.hpp>
#include <utils.hpp>

#ifndef DEDISPERSION_HPP
#define DEDISPERSION_HPP

namespace PulsarSearch {

class DedispersionConf {
public:
  DedispersionConf();
  ~DedispersionConf();

  // Get
  bool getLocalMem() const;
  unsigned int getNrSamplesPerBlock() const;
  unsigned int getNrSamplesPerThread() const;
  unsigned int getNrDMsPerBlock() const;
  unsigned int getNrDMsPerThread() const;
  unsigned int getUnroll() const;
  // Set
  void setLocalMem(bool local);
  void setNrSamplesPerBlock(unsigned int samples);
  void setNrSamplesPerThread(unsigned int samples);
  void setNrDMsPerBlock(unsigned int dms);
  void setNrDMsPerThread(unsigned int dms);
  void setUnroll(unsigned int unroll);
  // Utils
  std::string print() const;

private:
  bool local;
  unsigned int nrSamplesPerBlock;
  unsigned int nrSamplesPerThread;
  unsigned int nrDMsPerBlock;
  unsigned int nrDMsPerThread;
  unsigned int unroll;
};

typedef std::map< std::string, std::map< unsigned int, PulsarSearch::DedispersionConf > > tunedDedispersionConf;

// Sequential dedispersion
template< typename T > void dedispersion(AstroData::Observation & observation, const std::vector< T > & input, std::vector< T > & output, const std::vector< float > & shifts);
// OpenCL dedispersion algorithm
std::string * getDedispersionOpenCL(const DedispersionConf & conf, const std::string & dataType, const AstroData::Observation & observation, std::vector< float > & shifts);
// Read configuration files
void readTunedDedispersionConf(tunedDedispersionConf & tunedDedispersion, const std::string & dedispersionFilename);


// Implementations
template< typename T > void dedispersion(AstroData::Observation & observation, const std::vector< T > & input, std::vector< T > & output, const std::vector< float > & shifts) {
	for ( unsigned int dm = 0; dm < observation.getNrDMs(); dm++ ) {
		for ( unsigned int sample = 0; sample < observation.getNrSamplesPerSecond(); sample++ ) {
			T dedispersedSample = static_cast< T >(0);

			for ( unsigned int channel = 0; channel < observation.getNrChannels(); channel++ ) {
				unsigned int shift = static_cast< unsigned int >((observation.getFirstDM() + (dm * observation.getDMStep())) * shifts[channel]);

				dedispersedSample += input[(channel * observation.getNrSamplesPerDispersedChannel()) + (sample + shift)];
			}

			output[(dm * observation.getNrSamplesPerPaddedSecond()) + sample] = dedispersedSample;
		}
	}
}

inline bool DedispersionConf::getLocalMem() const {
  return local;
}

inline unsigned int DedispersionConf::getNrSamplesPerBlock() const {
  return nrSamplesPerBlock;
}

inline unsigned int DedispersionConf::getNrSamplesPerThread() const {
  return nrSamplesPerThread;
}

inline unsigned int DedispersionConf::getNrDMsPerBlock() const {
  return nrDMsPerBlock;
}

inline unsigned int DedispersionConf::getNrDMsPerThread() const {
  return nrDMsPerThread;
}

inline unsigned int DedispersionConf::getUnroll() const {
  return unroll;
}

inline void DedispersionConf::setLocalMem(bool local) {
  this->local = local;
}

inline void DedispersionConf::setNrSamplesPerBlock(unsigned int samples) {
  nrSamplesPerBlock = samples;
}

inline void DedispersionConf::setNrSamplesPerThread(unsigned int samples) {
  nrSamplesPerThread = samples;
}

inline void DedispersionConf::setNrDMsPerBlock(unsigned int dms) {
  nrDMsPerBlock = dms;
}

inline void DedispersionConf::setNrDMsPerThread(unsigned int dms) {
  nrDMsPerThread = dms;
}

inline void DedispersionConf::setUnroll(unsigned int unroll) {
  this->unroll = unroll;
}

} // PulsarSearch

#endif // DEDISPERSION_HPP

