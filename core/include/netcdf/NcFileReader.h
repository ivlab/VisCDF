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

#ifndef NCFILEREADER_H_
#define NCFILEREADER_H_

#include "visnetcdf.h"
#include "NcGroup.h"
#include <iostream>

namespace viscdfcore {

class NcFileReader {
public:
	NcFileReader() : _ncid(0), _isOpen(false) { std::cout << "blah!" << std::endl;}
	NcFileReader(const char* filename);
	virtual ~NcFileReader();

	IVcGroup::IVcGroupRef getGroup();
	void close();

private:
	int _ncid;
	bool _isOpen;
};

typedef CountedPtr<NcFileReader> NcFileReaderRef;

} /* namespace viscdfcore */

#endif /* NCFILEREADER_H_ */
