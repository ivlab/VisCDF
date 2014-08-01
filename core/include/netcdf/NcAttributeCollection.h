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

#ifndef NCATTRIBUTECOLLECTION_H_
#define NCATTRIBUTECOLLECTION_H_

#include "visnetcdf.h"
#include "AttributeCollection.h"

namespace viscdfcore {

class NcAttributeCollection : public AttributeCollection {
public:
	NcAttributeCollection(int ncid, int varid = NC_GLOBAL);
	virtual ~NcAttributeCollection();
	virtual std::string getText(const std::string& key);
private:
	int _ncid;
	int _varid;
};

} /* namespace viscdfcore */

#endif /* NCATTRIBUTECOLLECTION_H_ */
