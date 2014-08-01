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

#include "VcContainerBase.h"

namespace viscdfcore {

VcContainerBase::VcContainerBase(std::string filename) {
	load(filename);
}

VcContainerBase::~VcContainerBase() {
}

bool VcContainerBase::load(std::string filename) {
	_reader = NcFileReaderRef(new NcFileReader(filename.c_str()));
	IVcGroup::IVcGroupRef g = _reader->getGroup();
	return true;
}

IVcGroup::IVcGroupRef VcContainerBase::getGroup() {
	if (_reader != NULL)
	{
		return _reader->getGroup();
	}

	return NULL;
}

} /* namespace viscdfcore */
