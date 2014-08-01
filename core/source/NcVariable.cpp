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

#include "netcdf/NcVariable.h"

namespace viscdfcore {

NcVariable::NcVariable(int ncid, int varid, const std::vector<NcDimension> &dimensions) : _ncid(ncid), _varid(varid), _atts(ncid, varid) {
	int res;
	nc_type type;
	int numDims, numAtts;
	int dimids[NC_MAX_VAR_DIMS];
	char name[NC_MAX_NAME+1];
	if ((res = nc_inq_var(_ncid, _varid, name, &type, &numDims, dimids, &numAtts)))
		ERR(res);
	_name = std::string(name);

	for (int i = 0; i < dimensions.size(); i++)
	{
		for (int f = 0; f < numDims; f++)
		{
			if (dimensions[i].getDimID() == dimids[f])
			{
				_dims.push_back(dimensions[i]);
				break;
			}
		}
	}
}

NcVariable::~NcVariable() {
}

AttributeCollection* NcVariable::getAttributes() {
	return &_atts;
}

std::string NcVariable::getName() {
	return _name;
}

void NcVariable::getData(float* data) {
	int res;
	if ((res = nc_get_var_float(_ncid, _varid, data)))
		ERR(res);
}

void NcVariable::getData(int* data) {
	int res;
	if ((res = nc_get_var_int(_ncid, _varid, data)))
		ERR(res);
}

}
/* namespace viscdfcore */
