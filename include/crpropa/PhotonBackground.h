#ifndef CRPROPA_PHOTONBACKGROUND_H
#define CRPROPA_PHOTONBACKGROUND_H

#include "crpropa/Common.h"
#include "crpropa/Referenced.h"

#include <vector>
#include <string>

namespace crpropa {
/**
 * \addtogroup PhotonFields
 * @{
 */

/**
 @class PhotonField
 @brief Abstract base class for photon fields.
 */
class PhotonField: public Referenced {
public:
	PhotonField() {
		this->fieldName = "AbstractPhotonField";
		this->isRedshiftDependent = false;
	}

	/**
	 returns comoving photon density [1/m^3].
	 multiply with (1+z^3) for physical number density.
	 @param ePhoton		photon energy [J]
	 @param z			redshift (if redshift dependent, default = 0.)
	 */
	virtual double getPhotonDensity(double ePhoton, double z = 0.) const = 0;

	/**
	 returns overall comoving scaling factor
	 (cf. CRPropa3-data/calc_scaling.py)
	 @param z		redshift
	 */
	virtual double getRedshiftScaling(double z) const {
		return 1.;
	};

	bool hasRedshiftDependence() const {
		return this->isRedshiftDependent;
	}

	std::string getFieldName() const {
		return this->fieldName;
	}

protected:
	std::string fieldName;
	bool isRedshiftDependent;
};

/**
 @class TabularPhotonField
 @brief Photon field decorator for tabulated photon fields.

 This class reads photon field data from files;
 The first file must be a list of photon energies [J], named fieldName_photonEnergy.txt
 The second file must be a list of comoving photon field densities [1/m^3], named fieldName_photonDensity.txt
 Optionally, a third file contains redshifts, named fieldName_redshift.txt
 */
class TabularPhotonField: public PhotonField {
public:
	TabularPhotonField(const std::string fieldName, const bool isRedshiftDependent = true);
	double getPhotonDensity(double ePhoton, double z = 0.) const;
	double getRedshiftScaling(double z) const;

protected:
	void readPhotonEnergy(std::string filePath);
	void readPhotonDensity(std::string filePath);
	void readRedshift(std::string filePath);
	void initRedshiftScaling();
	void checkInputData() const;

	std::vector<double> photonEnergies;
	std::vector<double> photonDensity;
	std::vector<double> redshifts;
	std::vector<double> redshiftScalings;
};

/**
 @class IRB_Kneiske04
 @brief Extragalactic background light model from Kneiske et al. 2004

 Source info:
 DOI:10.1051/0004-6361:20031542,
 https://www.aanda.org/articles/aa/pdf/2004/03/aa3848.pdf, figure 1 ("Best-fit" model)
 */
class IRB_Kneiske04: public TabularPhotonField {
public:
	IRB_Kneiske04() : TabularPhotonField("IRB_Kneiske04", true) {}
};

/**
 @class IRB_Stecker05
 @brief Extragalactic background light model by Stecker at al. 2005

 Source info:
 DOI:10.1086/506188, astro-ph/0510449
 https://iopscience.iop.org/article/10.1086/506188/pdf
 */
class IRB_Stecker05: public TabularPhotonField {
public:
	IRB_Stecker05() : TabularPhotonField("IRB_Stecker05", true) {}
};

/**
 @class IRB_Franceschini08
 @brief Extragalactic background light model from Franceschini et al. 2008

 Source info:
 DOI:10.1051/0004-6361:200809691
 https://arxiv.org/pdf/0805.1841.pdf, tables 1 and 2
 */
class IRB_Franceschini08: public TabularPhotonField {
public:
	IRB_Franceschini08() : TabularPhotonField("IRB_Franceschini08", true) {}
};

/**
 @class IRB_Finke10
 @brief Extragalactic background light model from Finke et al. 2010

 Source info:
 DOI:10.1088/0004-637X/712/1/238
 https://iopscience.iop.org/article/10.1088/0004-637X/712/1/238/pdf
 */
class IRB_Finke10: public TabularPhotonField {
public:
	IRB_Finke10() : TabularPhotonField("IRB_Finke10", true) {}
};

/**
 @class IRB_Dominguez11
 @brief Extragalactic background light model from Dominguez et al. 2011

 Source info:
 DOI:10.1111/j.1365-2966.2010.17631.x
 https://academic.oup.com/mnras/article/410/4/2556/1008012
 */
class IRB_Dominguez11: public TabularPhotonField {
public:
	IRB_Dominguez11() : TabularPhotonField("IRB_Dominguez11", true) {}
};

/**
 @class IRB_Gilmore12
 @brief Extragalactic background light model from Gilmore et al. 2012

 Source info:
 DOI:10.1111/j.1365-2966.2012.20841.x
 https://academic.oup.com/mnras/article/422/4/3189/1050758
 */
class IRB_Gilmore12: public TabularPhotonField {
public:
	IRB_Gilmore12() : TabularPhotonField("IRB_Gilmore12", true) {}
};

/**
 @class IRB_Stecker16_upper
 @brief Extragalactic background light model from Stecker et al. 2016 (upper-bound model)

 Source info:
 DOI:10.3847/0004-637X/827/1/6
 https://iopscience.iop.org/article/10.3847/0004-637X/827/1/6
 */
class IRB_Stecker16_upper: public TabularPhotonField {
public:
	IRB_Stecker16_upper() : TabularPhotonField("IRB_Stecker16_upper", true) {}
};

/**
 @class IRB_Stecker16_lower
 @brief Extragalactic background light model from Stecker et al. 2016 (lower-bound model)

 Source info:
 DOI:10.3847/0004-637X/827/1/6
 https://iopscience.iop.org/article/10.3847/0004-637X/827/1/6
 */
class IRB_Stecker16_lower: public TabularPhotonField {
public:
	IRB_Stecker16_lower() : TabularPhotonField("IRB_Stecker16_lower", true) {}
};

/**
 @class BlackbodyPhotonField
 @brief Photon field decorator for black body photon fields.
 */
class BlackbodyPhotonField: public PhotonField {
public:
	BlackbodyPhotonField(const std::string fieldName, const double blackbodyTemperature);
	double getPhotonDensity(double ePhoton, double z = 0.) const;

protected:
	double blackbodyTemperature;
};

/**
 @class CMB
 @brief Cosmic mircowave background photon field

 Source info:
 This field is an isotropic blackbody photon field with temperature T = 2.73 K
 */
class CMB: public BlackbodyPhotonField {
public:
	CMB() : BlackbodyPhotonField("CMB", 2.73) {}
};


/**
 @class PhotonFieldSampling
 @brief Reimplementation of SOPHIA photon sampling. Naming and unit conventions are taken from SOPHIA to ease comparisions.
 */
class PhotonFieldSampling {
public:
	PhotonFieldSampling();

	/**
	 Constructor to mimic SOPHIA structure.
	  @param bgFlag		1: CMB | 2: IRB_Kneiske04
	 */
	explicit PhotonFieldSampling(int bgFlag);

	/**
	 SOPHIA's photon sampling method. Returns energy [J] of a photon of the photon field.
	 @param onProton	particle type: proton or neutron
	 @param E_in		energy of incoming nucleon
	 @param z_in		redshift of incoming nucleon
	 */
	double sample_eps(bool onProton, double E_in, double z_in) const;
protected:
	int bgFlag;

	// called by: sample_eps
	// - input: photon energy [eV], redshift
	// - output: photon density per unit energy [#/(eVcm^3)]
	double getPhotonDensity(double eps, double z_in) const;

	// called by: sample_eps
	// - input: s [GeV^2]
	// - output: (s-p^2) * sigma_(nucleon/gamma) [GeV^2 * mubarn]
	double functs(double s, bool onProton) const;

	// called by: sample_eps, gaussInt
	// - input: photon energy eps [eV], E_in [GeV]
	// - output: probability to encounter photon of energy eps
	double prob_eps(double eps, bool onProton, double E_in, double z_in) const;

	// called by: functs
	// - input: photon energy [eV]
	// - output: crossection of nucleon-photon-interaction [mubarn]
	double crossection(double eps, bool onProton) const;

	// called by: crossection
	// - input: photon energy [eV], threshold [eV], max [eV], unknown [no unit]
	// - output: unknown [no unit]
	double Pl(double x, double xth, double xmax, double alpha) const;

	// called by: crossection
	// - input: photon energy [eV], threshold [eV], unknown [eV]
	// - output: unknown [no unit]
	double Ef(double x, double th, double w) const;

	// called by: crossection
	// - input: cross section [µbarn], width [GeV], mass [GeV/c^2], rest frame photon energy [GeV]
	// - output: Breit-Wigner crossection of a resonance of width Gamma
	double breitwigner(double sigma_0, double Gamma, double DMM, double epsPrime, bool onProton) const;
};
/** @}*/

} // namespace crpropa

#endif // CRPROPA_PHOTONBACKGROUND_H
