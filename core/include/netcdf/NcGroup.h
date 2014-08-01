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

#ifndef NCGROUP_H_
#define NCGROUP_H_

#include "VcGroup.h"
#include "visnetcdf.h"
#include "NcAttributeCollection.h"
#include "NcVariable.h"

namespace viscdfcore {

class NcGroup: public IVcGroup {
public:
	NcGroup(int ncid, std::string name = "");
	virtual ~NcGroup();
	virtual void addGroup(IVcGroupRef group);
	virtual std::vector<IVcGroupRef> getGroups() const;
	virtual std::string getName();
	virtual AttributeCollection* getAttributes();
	virtual std::vector<Dimension> getDimensions();
	virtual std::vector<VariableRef> getVariables();
private:
	int _ncid;
	std::string _name;
	int _ndims_in, _nvars_in, _ngatts_in, _unlimdimid_in;
	NcAttributeCollection _atts;
	std::vector<NcDimension> _dims;
	std::vector<VariableRef> _vars;
};

} /* namespace viscdfcore */

#endif /* NCGROUP_H_ */
