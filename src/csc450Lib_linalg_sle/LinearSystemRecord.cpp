//
//  LinearSystemRecord.cpp
//
//
//  Created by Christopher Glasz on 3/29/15.
//
//

#include "LinearSystemRecord.h"
using namespace csc450Lib_linalg_sle;
using namespace csc450Lib_linalg_base;

LinearSystemRecord::LinearSystemRecord(const LinearSystemStatus theStatus,
                                       const Matrix *theSol) {
    this->theStatus = theStatus;
    this->theSol = theSol;
}

const Matrix* LinearSystemRecord::getSolution() const {
    return theSol;
}

const LinearSystemStatus LinearSystemRecord::getStatus() const {
    return theStatus;
}