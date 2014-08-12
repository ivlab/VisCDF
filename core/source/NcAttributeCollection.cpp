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

#include "netcdf/NcAttributeCollection.h"

namespace viscdfcore {

NcAttributeCollection::NcAttributeCollection(int ncid, int varid) {
	_ncid = ncid;
	_varid = varid;
}

NcAttributeCollection::~NcAttributeCollection() {
	// TODO Auto-generated destructor stub
}

std::string NcAttributeCollection::getText(const std::string& key) {
	int res;
	size_t len;
	nc_type type;
	if ((res = nc_inq_att(_ncid, _varid, key.c_str(), &type, &len)))
		ERR(res);

	char *val = new char[len+1];

	if ((res = nc_get_att_text(_ncid, _varid, key.c_str(), val)))
	        ERR(res);

	val[len] = '\0';
	std::string str = std::string(val);
	delete [] val;
	return str;
}

} /* namespace viscdfcore */
