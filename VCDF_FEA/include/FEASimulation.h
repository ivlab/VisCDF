/*
 * Copyright Regents of the University of Minnesota, 2014.  This software is released under the following license: http://opensource.org/licenses/lgpl-3.0.html.
 * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
 *
 * Funding for this software development was provided in part by the NSF (IIS-1251069) and NIH (1R01EB018205-01).
 * Any opinions, findings, and conclusions or recommendations expressed in this material are those of the author(s) and do not necessarily reflect the views of the National Science Foundation.
 *
 * The content is solely the responsibility of the authors and does not necessarily represent the official views of the National Institutes of Health.
 *
 * Code author(s):
 * 		Dan Orban (dtorban)
 */

#ifndef FEASIMULATION_H_
#define FEASIMULATION_H_

#include "VcContainerBase.h"
#include "FEAPart.h"

namespace vcdf_fea {

/*! @brief FEASimulation is the class that contains all of the information for a simulation stored
 *  in a NetCDF file according to the VCDF_FEA data file specification.
 */
class FEASimulation : public viscdfcore::VcContainerBase {
public:
	/*! @brief Constructor which loads a NetCDF file.*/
	FEASimulation(std::string filename);
	virtual ~FEASimulation();
	/*! @brief The parts that represent separate entities in a simulation.*/
	std::vector<FEAPartRef> getParts();
};

} /* namespace vcdf_fea */

#endif /* FEASIMULATION_H_ */
