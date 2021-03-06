/*
 * TextDataLoader.h
 *
 *  Created on: Aug 7, 2015
 *      Author: nigul
 */

#ifndef TEXTDATALOADER_H_
#define TEXTDATALOADER_H_

#include "DataLoader.h"

class TextDataLoader: public DataLoader {
public:
	TextDataLoader(const string& fileName, unsigned bufferSize,
			const vector<unsigned>& dims,
			const vector<vector<pair<unsigned, unsigned>>>& regions,
			unsigned totalNumVars, const vector<unsigned>& varIndices);
	TextDataLoader(const TextDataLoader& dataLoader);
	virtual ~TextDataLoader();

	virtual bool Next();
	virtual DataLoader* Clone() const;

	virtual bool RowMajor() const {
		return true;
	}
};


#endif /* TEXTDATALOADER_H_ */
