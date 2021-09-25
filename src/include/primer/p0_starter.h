//===----------------------------------------------------------------------===//
//
//                         BusTub
//
// p0_starter.h
//
// Identification: src/include/primer/p0_starter.h
//
// Copyright (c) 2015-2020, Carnegie Mellon University Database Group
//
//===----------------------------------------------------------------------===//

#pragma once

#include <memory>
#include <stdexcept>
#include <vector>

#include "common/exception.h"

namespace bustub {

/**
 * The Matrix type defines a common
 * interface for matrix operations.
 */
template <typename T>
class Matrix {
 protected:
  // TODO(P0): Add implementation
  Matrix(int r, int c) : rows(r), cols(c)  {
    linear = new T[rows*cols];
  }
  // # of rows in the matrix
  int rows;
  // # of Columns in the matrix
  int cols;
  // Flattened array containing the elements of the matrix
  // TODO(P0) : Allocate the array in the constructor. Don't forget to free up
  // the array in the destructor.
  T *linear;

 public:
  // Return the # of rows in the matrix
  virtual int GetRows() = 0;

  // Return the # of columns in the matrix
  virtual int GetColumns() = 0;

  // Return the (i,j)th  matrix element
  virtual T GetElem(int i, int j) = 0;

  // Sets the (i,j)th  matrix element to val
  virtual void SetElem(int i, int j, T val) = 0;

  // Sets the matrix elements based on the array arr
  virtual void MatImport(T *arr) = 0;

  // TODO(P0): Add implementation
  virtual ~Matrix() { delete [] linear; }
};

/**
 * The RowMatrix type is a concrete matrix implementation.
 * It implements the interface defined by the Matrix type.
 */
template <typename T>
class RowMatrix : public Matrix<T> {
 public:
  // using data from base
  using Matrix<T>::rows;
  using Matrix<T>::cols;
  using Matrix<T>::linear;
  // TODO(P0): Add implementation
  RowMatrix(int r, int c) : Matrix<T>(r, c) {
    // allocate the data array
    data_ = new T*[rows];
    for (int i = 0; i < rows; i++) {
      data_[i] = linear + i*cols;
    }
  }

  // TODO(P0): Add implementation
  int GetRows() override { return rows; }

  // TODO(P0): Add implementation
  int GetColumns() override { return cols; }

  // TODO(P0): Add implementation
  T GetElem(int i, int j) override { return data_[i][j]; }

  // TODO(P0): Add implementation
  void SetElem(int i, int j, T val) override { data_[i][j] = val; }

  // TODO(P0): Add implementation
  void MatImport(T *arr) override {
    for (int i = 0; i < rows*cols; i++) {
      linear[i] = arr[i];
    }
  }

  // TODO(P0): Add implementation
  ~RowMatrix() override { 
    delete [] data_;
  };

 private:
  /**
   * A 2D array containing the elements of the matrix in row-major format.
   *
   * TODO(P0):
   * - Allocate the array of row pointers in the constructor.
   * - Use these pointers to point to corresponding elements of the `linear` array.
   * - Don't forget to deallocate the array in the destructor.
   */
  T **data_;
};

/**
 * The RowMatrixOperations class defines operations
 * that may be performed on instances of `RowMatrix`.
 */
template <typename T>
class RowMatrixOperations {
 public:
  // Compute (mat1 + mat2) and return the result.
  // Return nullptr if dimensions mismatch for input matrices.
  static std::unique_ptr<RowMatrix<T>> AddMatrices(std::unique_ptr<RowMatrix<T>> mat1,
                                                   std::unique_ptr<RowMatrix<T>> mat2) {
    // TODO(P0): Add code
    int rows1 = mat1->GetRows();
    int cols1 = mat1->GetColumns();
    int rows2 = mat2->GetRows();
    int cols2 = mat2->GetColumns();
    if (rows1 != rows2 || cols1 != cols2)
      return std::unique_ptr<RowMatrix<T>>(nullptr);
    RowMatrix<T> *ret = new RowMatrix<T>(rows1, cols1);
    for (int i = 0; i < rows1; ++i) {
      for (int j = 0; j < cols1; ++j) {
        ret->SetElem(i,j,mat1->GetElem(i,j) + mat2->GetElem(i,j)); 
      }
    }
    return std::unique_ptr<RowMatrix<T>>(ret);
  }

  // Compute matrix multiplication (mat1 * mat2) and return the result.
  // Return nullptr if dimensions mismatch for input matrices.
  static std::unique_ptr<RowMatrix<T>> MultiplyMatrices(std::unique_ptr<RowMatrix<T>> mat1,
                                                        std::unique_ptr<RowMatrix<T>> mat2) {
    // TODO(P0): Add code
    int rows1 = mat1->GetRows();
    int cols1 = mat1->GetColumns();
    int rows2 = mat2->GetRows();
    int cols2 = mat2->GetColumns();
    if (cols1 != rows2)
      return std::unique_ptr<RowMatrix<T>>(nullptr);
    RowMatrix<T> *ret = new RowMatrix<T>(rows1, cols2);
    for (int i = 0; i < rows1; ++i) {
      for (int j = 0; j < cols2; ++j) {
        T val = 0;
        for (int k = 0; k < cols1; ++k) {
          val += mat1->GetElem(i,k) * mat2->GetElem(k,j);
        }
        ret->SetElem(i,j,val);
      }
    }
    return std::unique_ptr<RowMatrix<T>>(ret);
  }

  // Simplified GEMM (general matrix multiply) operation
  // Compute (matA * matB + matC). Return nullptr if dimensions mismatch for input matrices
  static std::unique_ptr<RowMatrix<T>> GemmMatrices(std::unique_ptr<RowMatrix<T>> matA,
                                                    std::unique_ptr<RowMatrix<T>> matB,
                                                    std::unique_ptr<RowMatrix<T>> matC) {
    // TODO(P0): Add code
    auto tmp = MultiplyMatrices(matA, matB);
    if (!tmp) return std::unique_ptr<RowMatrix<T>>(nullptr);
    return AddMatrices(tmp,matC);
  }
};
}  // namespace bustub
