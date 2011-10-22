/*  File: cmatrix.hpp
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

#include <iostream>

#ifndef CMATRIX_H
#define CMATRIX_H

namespace cmc {
  class CMatrix {
	public:
		// type for the dimension of the matrix
		typedef size_t dim;
		// type for the data of the matrix (possibly for templating)
		//typedef float data_t;
  private:
    // data structure related stuff - this could change if i use a different data structure
    float **data;                   // internat data structure
    void allocateData(dim x, dim y);// allocate the data structure of a X x Y matrix
    void deleteData();              // delete the data structure

    // abstract stuff - if i ever change the data structure, these'll remain
    dim             sizeX,  sizeY;          // size of the matrix
    void checkBounds(dim x, dim y) const;   // check if X,Y are vaild elements
    float getElemCon(dim x, dim y) const;   // get element X,Y
    float&   getElem(dim x, dim y);         // get reference to element X,Y
    void     setElem(dim x, dim y, float v);// set element X,Y to V
    void setAllElems(float v);              // set all elements to V

  public:
    // constructors
    CMatrix();                          // default: 2x2 CMatrix
    CMatrix(dim x, dim y);              // new CMatrix with these dims
    CMatrix(dim x, dim y, float v);     // new CMatrix with default value V
    CMatrix(dim x, dim y, float **mat); // new CMatrix with defaults
    CMatrix(const CMatrix&);            // copy constructor
    ~CMatrix();                         // destructor (most important piece of code, lol)

    // public accessors
    float get(dim x, dim y) const;    // get a copy of X,Y
    float get(dim pos     ) const;    // get a copy of pos (like operator[])
    void  set(dim x, dim y, float v); // set X,Y to V
    dim height() const;               // get the height of the matrix (
    dim width() const;                // get the matrix's length
    dim size() const;                 // get the matrix's size (X*Y)

    // matrix related things
    bool square() const;        // return true if this is a square matrix
    float determinant() const;  // get the determinant
    void resize(dim x, dim y);  // resize this matrix to an XxY matrix
    void escape();              // escape this matrix
    void invert();              // invert this matrix
    CMatrix escaped() const;    // return an escaped version of this matrix
    CMatrix inverse() const;    // return an inverted version of this matrix

    // operators
    // element access
    float& operator()(dim x, dim y); 
    float& operator[](dim pos); 

    // comparing
    bool operator==(const CMatrix&) const;
    bool operator!=(const CMatrix&) const;

    // copy / assignment / math
    CMatrix& operator= (const CMatrix&);
    CMatrix& operator+=(const float);
    CMatrix& operator+=(const CMatrix&);
    CMatrix& operator++();
    CMatrix& operator-=(const float);
    CMatrix& operator-=(const CMatrix&);
    CMatrix& operator--();
    CMatrix& operator*=(const float);
    CMatrix& operator*=(const CMatrix&);
    CMatrix& operator/=(const float);

    // math
    CMatrix operator+(const float) const;
    CMatrix operator+(const CMatrix&) const;
    CMatrix operator-(const float) const;
    CMatrix operator-(const CMatrix&) const;
    CMatrix operator*(const float) const;
    CMatrix operator*(const CMatrix&) const;
    CMatrix operator/(const float) const;
    friend CMatrix operator+(const float, const CMatrix&);
    friend CMatrix operator-(const float, const CMatrix&);
    friend CMatrix operator*(const float, const CMatrix&);

    // stream
    friend std::ostream& operator<<(std::ostream&, const CMatrix&);
  };
}

#endif
