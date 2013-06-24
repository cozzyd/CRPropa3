#ifndef _INJECT_H
#define _INJECT_H

#include "dint/cvector.h"
#include "dint/spectrum.h"

void SetInjectionSpectrum(const PARTICLE part, const double InjEnergy,
                          const double HInjEnergy, const double deltaE_hadron,
                          const dCVector* pEnergy,
                          const dCVector* pEnergyWidth, Spectrum* pQ_0);

#endif
