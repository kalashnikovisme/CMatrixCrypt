/*  File: cmatrix.cpp
 *  
 *  Copyright (C) 2011, Patrick M. Elsen
 *
 *  This file is part of CMatrixCrypt (http://github.com/xfbs/CMatrixCrypt)
 *  Author: Patrick M. Elsen <pelsen.vn (a) gmail.com>
 *
 *  All rights reserved.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include "cmatrix.hpp"

namespace cme {
  /* DATA STRUCTURE STUFF
   * this code might change if the data structure used is changed. 
   * this is why it's not public but private, it shoudln't be called
   * by anyone but internal functions!
   */

  // allocate the internal data structure of size XxY
  void CMatrix::allocateData(int x, int y) {
    if((x<=0) or (y<=0))
      throw "illegal size for matrix specified";

    // set the size variables
    sizeX = x;
    sizeY = y;

    // data should be an array of pointers
    data = new float*[sizeX];

    // now, we add another dimension to the whole thing
    for(int x = 0; x < sizeX; x++) {
      data[x] = new float[sizeY];
    }
  }

  // delete the internal data structure
  void CMatrix::deleteData() {
    // loop thru all pointers and delete them
    for(int x = 0; x < sizeX; x++) {
      delete[] data[x];
    }

    // finally, delete the array of pointers
    delete[] data;
    
    // now, to invalidate everything
    sizeX = 0;
    sizeY = 0;
  }

  /* INTERNAL ACCESSORS
   * this is the only place where the internal data structure is
   * directrly accessed. this is why this code may change and is private. 
   * this code is expected to do bounds checking because it is used
   * by all internal functions. 
   */

  // method that checks the bounds - throws if x or y are not part of the matrix. 
  void CMatrix::checkBounds(int x, int y) const {
    // check if x is in bounds
    if( (x<0) or !(x<sizeX) )
      throw "x index out of bounds: "+x;
    
    // check if y is in bounds
    if( (y<0) or !(y<sizeY) )
      throw "y index out of bounds: "+y;
  }

  // element accessor - this one returns a copy of the data so it's a const
  float CMatrix::getElemCon(int x, int y) const {
    checkBounds(x, y);
    return(data[x][y]); // return a copy
  }

  // elemnt accessor - returns a reference so it's not const.
  float& CMatrix::getElem(int x, int y) {
    checkBounds(x, y);
    return(data[x][y]); // return by reference
  }

  // set element X,Y to V. 
  void CMatrix::setElem(int x, int y, float v) {
    checkBounds(x, y);
    data[x][y] = v;
  }

  // set all elements to V
  void CMatrix::setAllElems(float v) {
    for(int x = 0; x < sizeX; x++) {
      for(int y = 0; y < sizeY; y++) {
        data[x][y] = v;
      }
    }
  }

  /* CONSTRUCTORS
   * these are the constructors, which are public. public functions
   * may not directly access the internet data structure. 
   * The following functions may touch sizeX and sizeY. 
   */

  // standard constructor - initializes all elems to 0
  CMatrix::CMatrix() {
    allocateData(2, 2);   // allocates the data structure
    setAllElems(0.0); // initialize all data structure elements to 0.0
  }

  // this constructor specifies the size of the matrix wanted
  CMatrix::CMatrix(int x, int y) {
    allocateData(x, y);
    setAllElems(0.0);
  }

  // make a matrix that is of size XxY with default values V
  CMatrix::CMatrix(int x, int y, float v) {
    allocateData(x, y);
    setAllElems(v);
  }

  // make a new matrix of size XxY which has the same elements as mat (a 2d array)
  CMatrix::CMatrix(int x, int y, float **mat) {
    allocateData(x, y);
    for(int i = 0; i < sizeX; i++) {
      for(int j = 0; j < sizeY; j++) {
        setElem(x, y, mat[x][y]);
      }
    }
  }

  // copy constructor - make a new matrix just like the rhs one. 
  CMatrix::CMatrix(const CMatrix& rhs) {
    allocateData(rhs.height(), rhs.width());  // make a new matrix of same size
    for(int x = 0; x < sizeX; x++) {
      for(int y = 0; y < sizeY; y++) {
        setElem(x, y, rhs.get(x, y));         // copy elements over
      }
    }
  }

  // destructor - cause we're using heap variables
  CMatrix::~CMatrix() {
    deleteData();
  }

  /* PUBLIC ELEMENT ACCESSORS
   * these may not, in any way, interact with the data structure, 
   * only with the private element accessors. apart from that there
   * are not restrictions.
   */

  // return a copy of element X,Y
  float CMatrix::get(int x, int y) const {
    return(getElemCon(x, y));
  }

  float CMatrix::get(int pos) const {
    return(getElemCon(pos/sizeY, pos%sizeY));
  }

  // set X,Y to V
  void CMatrix::set(int x, int y, float v) {
    setElem(x, y, v);
  }

  // get the width (the y size)
  int CMatrix::width() const {
    return(sizeY);
  }

  // get the height (the x size)
  int CMatrix::height() const {
    return(sizeX);
  }

  int CMatrix::size() const {
    return(sizeX*sizeY);
  }

  /* MATRIX RELATED FUNCTIONS
   * these can be used to work with matrices
   */

  // return true if this is a square matrix
  bool CMatrix::square() const {
    return(sizeX==sizeY);
  }

  // get the determinant of a matrix
  float CMatrix::determinant() const {
    // make sure this matrix is a square one
    if(!this->square())
      throw "only square matrices have a determinant";

    // FIXME: add support for bigger matrices!
    if(sizeX!=2)
      throw "currently only the determinants of 2x2 matrices can be found";

    float det = 0;
    det =  getElemCon(0,0) * getElemCon(1,1);
    det -= getElemCon(1,0) * getElemCon(0,1);

    return(det);
  }


  // resize matrix
  void CMatrix::resize(int x, int y) {
    CMatrix old(*this); // make a copy of the old matrix
    deleteData();       // delete all old data
    allocateData(x,y);  // allocate new data
    setAllElems(0);     // set all to 0
    
    // try to copy as many elems as possible over to the new matrix
    for(int x = 0; (x < old.height()) and (x < this->height()); x++) {
      for(int y = 0; (y < old.width()) and (y < this->width()); y++) {
        this->setElem(x, y, old.get(x, y));
      }
    }
  }

  // escape this matrix
  void CMatrix::escape() {
    // check that this is a square matrix
    if(!this->square())
      throw "only square matrices can be escaped";

    // escape only if needed (if determinant is 0)
    if(this->determinant() == 0) {
      // add one to the absolute value of each diagonal element
      for(int xy = 0; xy < sizeX; xy++) {
        if(getElem(xy, xy) < 0) {
          getElem(xy, xy)--;
        } else {
          getElem(xy, xy)++;
        }
      }
    }
  }

  // invert this matrix
  void CMatrix::invert() {
    // make sure this matrix is a square one
    if(!this->square())
      throw "only square matrices have an inverse";

    // FIXME: add support for bigger matrices!
    if(sizeX!=2)
      throw "currently only the inverses of 2x2 matrices can be found";
    
    // make a temporary matrix
    CMatrix temp(this->height(), this->width(), 0.0);
    
    // store the inverted matrix in temp
    temp(0,0) += getElem(1,1);
    temp(1,1) += getElem(0,0);
    temp(0,1) -= getElem(0,1);
    temp(1,0) -= getElem(1,0);
    temp      /= this->determinant(); // divide by determinant

    // copy the converted matrix over
    *this = temp;
  }

  // return the inverse of this matrix
  CMatrix CMatrix::inverse() const {
    // make sure this matrix is a square one
    if(!this->square())
      throw "only square matrices have an inverse";

    // FIXME: add support for bigger matrices!
    if(sizeX!=2)
      throw "currently only the inverses of 2x2 matrices can be found";
    
    // make a temporary matrix
    CMatrix temp(this->height(), this->width(), 0.0);
    
    // invert
    temp(0,0) += getElemCon(1,1);
    temp(1,1) += getElemCon(0,0);
    temp(0,1) -= getElemCon(0,1);
    temp(1,0) -= getElemCon(1,0);
    temp      /= this->determinant(); // divide by determinant

    // return the temporary matrix
    return(temp);
  }

  // return the escaped version of this matrix
  CMatrix CMatrix::escaped() const {
    // make a temporary matrix
    CMatrix temp(*this);
    // escape the temporary matrix
    temp.escape();
    // return the temporary matrix
    return(temp);
  }

  /* OPERATORS
   * these are used for interacting and working with matrices. 
   * they may not directly work with the internal data structure. 
   * they usually accept either CMatrices or floats as arguments.
   */

  // return X,Y by reference
  float& CMatrix::operator()(int x, int y) {
    return(getElem(x, y));
  }

  float& CMatrix::operator[](int pos) {
    pos = pos % (sizeY * sizeX);
    return(getElem(pos/sizeY, pos%sizeY));
  }

  /* EQUALITY TESTS */
  // compare matrices
  bool CMatrix::operator==(const CMatrix& rhs) const {
    if((this->height() != rhs.height()) or (this->width() != rhs.width()))
      return false;

    for(int x = 0; x < sizeX; x++) {
      for(int y = 0; y < sizeY; y++) {
        if(getElemCon(x, y) != rhs.get(x, y))
          return false;
      }
    }

    return true;  // if false wasn't returned above, return true
  }

  // test for inequality
  bool CMatrix::operator!=(const CMatrix& rhs) const {
    return(!(*this == rhs));
  }

  /* COPYING */
  // copy operator - excuse the messyness. this operation is a bit dirty (just a bit tho)
  CMatrix& CMatrix::operator=(const CMatrix& rhs) {
    // we don't wanna copy ourself
    if(this == &rhs) {
      return(*this);
    }

    // if the dimensions are different, we have to allocate a new data structure
    if( (this->height() != rhs.height()) or (this->width() != rhs.width())) {
      deleteData();
      allocateData(rhs.height(), rhs.width());
    }

    // once the matrix sizes are equal, we can just copy the values over. 
    for(int x = 0; x < sizeX; x++) {
      for(int y = 0; y < sizeY; y++) {
        setElem(x, y, rhs.get(x, y)); // and copying the values over.
      }
    }

    return(*this);  // return a reference to this matrix
  }

  /* ADDITION */
  // add v to every element in the matrix
  CMatrix& CMatrix::operator+=(const float v) {
    for(int x = 0; x < sizeX; x++) {
      for(int y = 0; y < sizeY; y++) {
        getElem(x, y) += v; // getElem returns a reference, which is why this code works. 
      }
    }
    return(*this);  // return self for chained command calls.
  }

  // add two matrices
  CMatrix& CMatrix::operator+=(const CMatrix& rhs) {
    // the dimensions must match. 
    if((this->height() != rhs.height()) or (this->width() != rhs.width()))
      throw "matrix dimension mismatch";

    // do all the addition shit
    for(int x = 0; x < sizeX; x++) {
      for(int y = 0; y < sizeY; y++) {
        getElem(x, y) += rhs.get(x, y);
      }
    }
    return(*this); // return self
  }

  // add one to matrix
  CMatrix& CMatrix::operator++() {
    *this += 1;   // add one
    return(*this);// return matrix
  }

  // add a float to a matrix
  CMatrix CMatrix::operator+(const float v) const {
    CMatrix temp(*this);  // make a copy of this matrix
    temp += v;            // add v using operator+=
    return(temp);         // return temp
  }

  CMatrix CMatrix::operator+(const CMatrix& rhs) const {
    CMatrix temp(*this);
    temp += rhs;
    return(temp);
  }

  /* SUBTRACTION */
  // subtract v from every element in the matrix
  CMatrix& CMatrix::operator-=(const float v) {
    for(int x = 0; x < sizeX; x++) {
      for(int y = 0; y < sizeY; y++) {
        getElem(x, y) -= v; // getElem returns a reference, which is why this code works. 
      }
    }
    return(*this);  // return self for chained command calls.
  }

  // subtract two matrices
  CMatrix& CMatrix::operator-=(const CMatrix& rhs) {
    // the dimensions must match. 
    if((this->height() != rhs.height()) or (this->width() != rhs.width()))
      throw "matrix dimension mismatch";

    // do all the subtraction shit
    for(int x = 0; x < sizeX; x++) {
      for(int y = 0; y < sizeY; y++) {
        getElem(x, y) -= rhs.get(x, y);
      }
    }
    return(*this); // return self
  }

  // subtract one from matrix
  CMatrix& CMatrix::operator--() {
    *this -= 1;     // subtract one from this
    return(*this);  // return reference to this
  }

  // subtract from a matrix
  CMatrix CMatrix::operator-(const float v) const {
    CMatrix temp(*this);  // make a temporary matrix
    temp -= v;            // subtract form temp. mat
    return(temp);         // return the temporary matrix
  }

  /* MULTIPLYING */
  // multiply every element in the matrix
  CMatrix& CMatrix::operator*=(const float v) {
    for(int x = 0; x < sizeX; x++) {
      for(int y = 0; y < sizeY; y++) {
        getElem(x, y) *= v; // getElem returns a reference, which is why this code works. 
      }
    }
    return(*this);  // return self for chained command calls.
  }

  // multiply two matrices
  CMatrix& CMatrix::operator*=(const CMatrix& rhs) {
    // check for dimensions
    if(this->width() != rhs.height())
      throw "matrix dimension mismatch";

    // temporary matrix for the result
    CMatrix temp(sizeX, rhs.width());

    for(int x = 0; x < temp.height(); x++) {
      for(int y = 0; y < temp.width(); y++) {
        for(int k = 0; k < sizeY; k++) {
          temp(x, y) += getElem(x, k) * rhs.get(k, y);
        }
      }
    }

    *this = temp;
    return(*this);  // return self for chaining
  }

  // multiply a matrix with a float
  CMatrix CMatrix::operator*(const float v) const {
    CMatrix temp(*this);  // make a copy of this
    temp *= v;            // multiply copy of this
    return(temp);         // return copy
  }

  CMatrix CMatrix::operator*(const CMatrix& rhs) const {
    // check for dimensions
    if(this->width() != rhs.height())
      throw "matrix dimension mismatch";

    // temporary matrix for the result
    CMatrix temp(sizeX, rhs.width());

    for(int x = 0; x < temp.height(); x++) {
      for(int y = 0; y < temp.width(); y++) {
        for(int k = 0; k < sizeY; k++) {
          temp(x, y) += getElemCon(x, k) * rhs.get(k, y);
        }
      }
    }

    return(temp);  // return self for chaining
  }

  /* DIVISION OPERATIONS */
  // divide every element in the matrix
  CMatrix& CMatrix::operator/=(const float v) {
    for(int x = 0; x < sizeX; x++) {
      for(int y = 0; y < sizeY; y++) {
        getElem(x, y) /= v; // getElem returns a reference, which is why this code works. 
      }
    }
    return(*this);  // return self for chained command calls.
  }

  // divide a matrix by a float
  CMatrix CMatrix::operator/(const float v) const {
    CMatrix temp(*this);  // make a copy of this
    temp /= v;            // divide copy of this
    return(temp);         // return copy
  }

  /* STREAM OPERATIONS */
  // this nifty code allows you to write cout << cmatrix << endl;
  std::ostream& operator<<(std::ostream& out, const CMatrix& rhs) {
    // go thru all 'lines' of the matrix 
    for(int x = 0; x < rhs.height(); x++) {
      // print an opening square brace if this is the first line to be printed
      if(x==0) {
        out << "[";
      } else {
        out << " ";
      }

      // print an opening square bracket for the line
      out << "[";
      for(int y = 0; y < rhs.width(); y++) {
        out << rhs.get(x, y);    // print the curent matrix member
        if(y!=(rhs.width()-1)) { // if this isn't the last element, print a separator
          out << ", ";
        }
      }
      // print a closing square bracket
      out << "]";

      // print a newline if needed
      if(x!=(rhs.height()-1)) {
        out << ",\n";
      } else {
        out << "]";
      }
    }

    return(out); // return the out stream for chaining
  }
}
