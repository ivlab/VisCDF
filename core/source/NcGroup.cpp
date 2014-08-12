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

#include "netcdf/NcGroup.h"
#include "netcdf/NcAttributeCollection.h"
#include "netcdf/NcDimension.h"
#include "netcdf/NcVariable.h"
#include <map>
#include "memory/CountedPtr.h"

namespace viscdfcore {

NcGroup::NcGroup(int ncid, std::string name) : _atts(ncid) {
	_ncid = ncid;
	_name = name;

	int res;
	if ((res = nc_inq(_ncid, &_ndims_in, &_nvars_in, &_ngatts_in, &_unlimdimid_in)))
		ERR(res);
	
	int *dimids = new int[NC_MAX_DIMS];

	if ((res = nc_inq_dimids(_ncid, &_ndims_in, dimids, 1)))
			ERR(res);

	for (int f = 0; f < _ndims_in; f++)
	{
		size_t len;
		char name[32];

		if ((res = nc_inq_dim(_ncid, dimids[f], name, &len)))
				ERR(res);

		_dims.push_back(NcDimension(dimids[f], std::string(name), len));
	}
		
	delete [] dimids;

	int *varids = new int[_nvars_in];

	if ((res = nc_inq_varids(_ncid, &_nvars_in, varids)))
			ERR(res);

	for (int f = 0; f < _nvars_in; f++)
	{
		_vars.push_back(new NcVariable(ncid, varids[f], _dims));
	}

	delete varids;
}

NcGroup::~NcGroup() {
}

void NcGroup::addGroup(IVcGroupRef group)
{
	//std::cout << "AddGroup Not implemented." << std::endl;
}

std::vector<NcGroup::IVcGroupRef> NcGroup::getGroups() const
{
	std::vector<NcGroup::IVcGroupRef> groups;
	int res;
	int numgrps;
	int *ncids;

	/* Get a list of ncids for the root group. (That is, find out of
	              there are any groups already defined. */
	if ((res = nc_inq_grps(_ncid, &numgrps, NULL)))
		ERR(res);

	ncids = (int*)malloc(sizeof(int) * numgrps);

	if ((res = nc_inq_grps(_ncid, NULL, ncids)))
		ERR(res);

	for (int f = 0; f < numgrps; f++)
	{
		groups.push_back(new NcGroup(ncids[f]));
	}

	free(ncids);

	return groups;
}

std::string NcGroup::getName()
{
	if (!_name.empty())
	{
		return _name;
	}

	int res;
	char grpName[MAX_NC_NAME+1];
	if ((res = nc_inq_grpname(_ncid, grpName)))
		ERR(res);
	return std::string(grpName);
}

AttributeCollection* NcGroup::getAttributes() {
	return &_atts;
}

std::vector<Dimension> NcGroup::getDimensions() {
	std::vector<Dimension> dims;
	for (int f = 0; f < _dims.size(); f++)
	{
		dims.push_back(_dims[f]);
	}
	return dims;
}

std::vector<VariableRef> NcGroup::getVariables()
{
	return _vars;
}

} /* namespace viscdfcore */
