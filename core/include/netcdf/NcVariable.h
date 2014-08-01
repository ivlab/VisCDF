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

#ifndef NCVARIABLE_H_
#define NCVARIABLE_H_

#include "Variable.h"
#include "NcAttributeCollection.h"
#include <vector>
#include "NcDimension.h"

namespace viscdfcore {

class NcVariable : public Variable {
public:
	NcVariable(int ncid, int varid, const std::vector<NcDimension> &dimensions);
	virtual ~NcVariable();
	virtual AttributeCollection* getAttributes();
	virtual std::string getName();
	std::vector<Dimension> getDimensions() { return _dims; };
	virtual void getData(float *data);
	virtual void getData(int *data);
private:
	int _ncid;
	int _varid;
	NcAttributeCollection _atts;
	std::string _name;
	std::vector<Dimension> _dims;
};

} /* namespace viscdfcore */

#endif /* NCVARIABLE_H_ */
