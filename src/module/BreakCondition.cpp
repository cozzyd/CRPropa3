#include "crpropa/module/BreakCondition.h"
#include "crpropa/ParticleID.h"
#include "crpropa/Units.h"

#include <sstream>

namespace crpropa {

MaximumTrajectoryLength::MaximumTrajectoryLength(double maxLength) :
		maxLength(maxLength) {
}

void MaximumTrajectoryLength::setMaximumTrajectoryLength(double length) {
	maxLength = length;
}

double MaximumTrajectoryLength::getMaximumTrajectoryLength() const {
	return maxLength;
}

void MaximumTrajectoryLength::addObserverPosition(const Vector3d& position) {
	observerPositions.push_back(position);
}

const std::vector<Vector3d>& MaximumTrajectoryLength::getObserverPositions() const {
	return observerPositions;
}

std::string MaximumTrajectoryLength::getDescription() const {
	std::stringstream s;
	s << "Maximum trajectory length: " << maxLength / Mpc << " Mpc, ";
	s << "Flag: '" << rejectFlagKey << "' -> '" << rejectFlagValue << "', ";
	s << "MakeInactive: " << (makeRejectedInactive ? "yes" : "no");
	if (rejectAction.valid())
		s << ", Action: " << rejectAction->getDescription();
	s << "\n  Observer positions: \n";
	for (size_t i = 0; i < observerPositions.size(); i++)
		s << "    - " << observerPositions[i] / Mpc << " Mpc\n";
	return s.str();
}

void MaximumTrajectoryLength::process(Candidate *c) const {
	double length = c->getTrajectoryLength();
	Vector3d position = c->current.getPosition();

	if(observerPositions.size()) {
		bool inRange = false;
		for (size_t i = 0; i < observerPositions.size(); i++) {
			double distance = position.getDistanceTo(observerPositions[i]);
			if (distance + length < maxLength)
				inRange = true;
		}
		if (!inRange) {
			reject(c);
			return;
		}
	}

	if (length >= maxLength) {
		reject(c);
	} else {
		c->limitNextStep(maxLength - length);
	}
}

//*****************************************************************************
MinimumEnergy::MinimumEnergy(double minEnergy) :
		minEnergy(minEnergy) {
}

void MinimumEnergy::setMinimumEnergy(double energy) {
	minEnergy = energy;
}

double MinimumEnergy::getMinimumEnergy() const {
	return minEnergy;
}

void MinimumEnergy::process(Candidate *c) const {
	if (c->current.getEnergy() > minEnergy)
		return;
	else
		reject(c);
}

std::string MinimumEnergy::getDescription() const {
	std::stringstream s;
	s << "Minimum energy: " << minEnergy / EeV << " EeV, ";
	s << "Flag: '" << rejectFlagKey << "' -> '" << rejectFlagValue << "', ";
	s << "MakeInactive: " << (makeRejectedInactive ? "yes" : "no");
	if (rejectAction.valid())
		s << ", Action: " << rejectAction->getDescription();
	return s.str();
}

//*****************************************************************************
MinimumRigidity::MinimumRigidity(double minRigidity) :
		minRigidity(minRigidity) {
}

void MinimumRigidity::setMinimumRigidity(double minRigidity) {
	this->minRigidity = minRigidity;
}

double MinimumRigidity::getMinimumRigidity() const {
	return minRigidity;
}

void MinimumRigidity::process(Candidate *c) const {
	if (c->current.getRigidity() < minRigidity)
		reject(c);
}

std::string MinimumRigidity::getDescription() const {
	std::stringstream s;
	s << "Minimum rigidity: " << minRigidity / EeV << " EeV, ";
	s << "Flag: '" << rejectFlagKey << "' -> '" << rejectFlagValue << "', ";
	s << "MakeInactive: " << (makeRejectedInactive ? "yes" : "no");
	if (rejectAction.valid())
		s << ", Action: " << rejectAction->getDescription();
	return s.str();
}

//*****************************************************************************
MinimumRedshift::MinimumRedshift(double zmin) :
		zmin(zmin) {
}

void MinimumRedshift::setMinimumRedshift(double z) {
	zmin = z;
}

double MinimumRedshift::getMinimumRedshift() {
	return zmin;
}

void MinimumRedshift::process(Candidate* c) const {
	if (c->getRedshift() > zmin)
		return;
	else
		reject(c);
}

std::string MinimumRedshift::getDescription() const {
	std::stringstream s;
	s << "Minimum redshift: " << zmin << ", ";
	s << "Flag: '" << rejectFlagKey << "' -> '" << rejectFlagValue << "', ";
	s << "MakeInactive: " << (makeRejectedInactive ? "yes" : "no");
	if (rejectAction.valid())
		s << ", Action: " << rejectAction->getDescription();
	return s.str();
}

//*****************************************************************************
MinimumChargeNumber::MinimumChargeNumber(int minChargeNumber) :
		minChargeNumber(minChargeNumber) {
}

void MinimumChargeNumber::setMinimumChargeNumber(int chargeNumber) {
	minChargeNumber = chargeNumber;
}

int MinimumChargeNumber::getMinimumChargeNumber() const {
	return minChargeNumber;
}

void MinimumChargeNumber::process(Candidate *c) const {
	if (chargeNumber(c->current.getId()) > minChargeNumber)
		return;
	else
		reject(c);
}

std::string MinimumChargeNumber::getDescription() const {
	std::stringstream s;
	s << "Minimum charge number: " << minChargeNumber;
	s << "Flag: '" << rejectFlagKey << "' -> '" << rejectFlagValue << "', ";
	s << "MakeInactive: " << (makeRejectedInactive ? "yes" : "no");
	if (rejectAction.valid())
		s << ", Action: " << rejectAction->getDescription();
	return s.str();
}

//*****************************************************************************
MinimumEnergyPerParticleId::MinimumEnergyPerParticleId(double minEnergyOthers) {
	setMinimumEnergyOthers(minEnergyOthers);
}

void MinimumEnergyPerParticleId::add(int id, double energy) {
	particleIds.push_back(id);
	minEnergies.push_back(energy);
}

void MinimumEnergyPerParticleId::setMinimumEnergyOthers(double energy) {
	minEnergyOthers = energy;
}

double MinimumEnergyPerParticleId::getMinimumEnergyOthers() const {
	return minEnergyOthers;
}

void MinimumEnergyPerParticleId::process(Candidate *c) const {
	for (int i = 0; i < particleIds.size(); i++) {
		if (c->current.getId() == particleIds[i]) {
			if (c->current.getEnergy() < minEnergies[i])
				reject(c);
			else
				return;
		}
	}

	if (c->current.getEnergy() < minEnergyOthers)
		reject(c);
	else
		return;
}

std::string MinimumEnergyPerParticleId::getDescription() const {
	std::stringstream s;
	s << "Minimum energy for non-specified particles: " << minEnergyOthers / eV << " eV";
	for (int i = 0; i < minEnergies.size(); i++) {
		s << "  for particle " << particleIds[i] << " : " << minEnergies[i] / eV << " eV";
	}
	s << "Flag: '" << rejectFlagKey << "' -> '" << rejectFlagValue << "', ";
	s << "MakeInactive: " << (makeRejectedInactive ? "yes" : "no");
	if (rejectAction.valid())
		s << ", Action: " << rejectAction->getDescription();
	return s.str();
}

//*****************************************************************************
DetectionLength::DetectionLength(double detLength) :
		detLength(detLength) {
}

void DetectionLength::setDetectionLength(double length) {
	detLength = length;
}

double DetectionLength::getDetectionLength() const {
	return detLength;
}


std::string DetectionLength::getDescription() const {
	std::stringstream s;
	s << "Detection length: " << detLength / kpc << " kpc, ";
	s << "Flag: '" << rejectFlagKey << "' -> '" << rejectFlagValue << "', ";
	s << "MakeInactive: " << (makeRejectedInactive ? "yes" : "no");
	if (rejectAction.valid())
		s << ", Action: " << rejectAction->getDescription();
	return s.str();
}

void DetectionLength::process(Candidate *c) const {
	double length = c->getTrajectoryLength();
	double step = c->getCurrentStep();

	if (length >= detLength && length - step < detLength) {
		reject(c);
	} else {
		c->limitNextStep(detLength - length);
	}
}


} // namespace crpropa
