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

#ifndef DIMENSION_H_
#define DIMENSION_H_

#include <string>

namespace viscdfcore {

class Dimension {
public:
	Dimension(std::string name, int len);
	~Dimension();
	const std::string& getName() const { return _name; }
	const int& getLength() const { return _len; }
private:
	std::string _name;
	int _len;
};

} /* namespace viscdfcore */

#endif /* DIMENSION_H_ */
