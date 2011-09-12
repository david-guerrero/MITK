/*=========================================================================

Program:   Medical Imaging & Interaction Toolkit
Language:  C++
Date:      $Date$
Version:   $Revision$

Copyright (c) German Cancer Research Center, Division of Medical and
Biological Informatics. All rights reserved.
See MITKCopyright.txt or http://www.mitk.org/copyright.html for details.

This software is distributed WITHOUT ANY WARRANTY; without even
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#include <mitkTestingMacros.h>
#include <mitkVector.h>

int mitkVectorTest(int /*argc*/, char* /*argv*/[])
{
  MITK_TEST_BEGIN("mitkVector");
  // test itk vector equality methods
  itk::Vector<float,3> itkVector_1;
  itkVector_1[0] = 4.6;
  itkVector_1[1] = 9.76543;
  itkVector_1[2] = 746.09;

  itk::Vector<float,3> itkVector_2;
  itk::Vector<float,3> itkVector_3;
  for (int i=0; i<3; i++)
  {
    itkVector_2[i] = itkVector_1[i] - sqrt(mitk::eps/3);
    itkVector_3[i] = itkVector_1[i] - sqrt(mitk::eps/3.1);
  }

  MITK_TEST_CONDITION_REQUIRED(mitk::Equal(itkVector_1,itkVector_1), "Test vector equality using the same vector with mitk::eps");
  MITK_TEST_CONDITION_REQUIRED(!mitk::Equal(itkVector_1,itkVector_2), "Test vector equality using different vectors with an element-wise difference greater than mitk::eps");
  MITK_TEST_CONDITION_REQUIRED(mitk::Equal(itkVector_1,itkVector_3), "Test vector equality using different vectors with an element-wise difference less than mitk::eps");
  // test itk point equality methods
  itk::Point<float,3> itkPoint_1;
  itk::Point<float,3> itkPoint_2;
  itk::Point<float,3> itkPoint_3;
  for (int i=0; i<3; i++)
  {
    itkPoint_1[i] = itkVector_1[i];
    itkPoint_2[i] = itkVector_2[i];
    itkPoint_3[i] = itkVector_3[i];
  }
  MITK_TEST_CONDITION_REQUIRED(mitk::Equal(itkPoint_1,itkPoint_1), "Test point equality using the same point with mitk::eps");
  MITK_TEST_CONDITION_REQUIRED(!mitk::Equal(itkPoint_1,itkPoint_2), "Test point equality using different points with an element-wise difference greater than mitk::eps");
  MITK_TEST_CONDITION_REQUIRED(mitk::Equal(itkPoint_1,itkPoint_3), "Test point equality using different points with an element-wise difference less than mitk::eps");
  // test mitk vnl vector equality methods
  mitk::VnlVector mitk_vnl_vector_1(3);
  mitk::VnlVector mitk_vnl_vector_2(3);
  mitk::VnlVector mitk_vnl_vector_3(3);
  for (int i=0; i<3; i++)
  {
    mitk_vnl_vector_1.put(i,itkVector_1[i]);
    mitk_vnl_vector_2.put(i,itkVector_2[i]);
    mitk_vnl_vector_3.put(i,itkVector_3[i]);
  }
  MITK_TEST_CONDITION_REQUIRED(mitk::Equal(mitk_vnl_vector_1,mitk_vnl_vector_1), "Test mitk vnl vector equality using the same mitk vnl vector with mitk::eps");
  MITK_TEST_CONDITION_REQUIRED(!mitk::Equal(mitk_vnl_vector_1,mitk_vnl_vector_2), "Test mitk vnl vector equality using different mitk vnl vectors with an element-wise difference greater than mitk::eps");
  MITK_TEST_CONDITION_REQUIRED(mitk::Equal(mitk_vnl_vector_1,mitk_vnl_vector_3), "Test mitk vnl vector equality using different mitk vnl vectors with an element-wise difference less than mitk::eps");
  // test vnl_vector equality method
  vnl_vector_fixed<mitk::ScalarType,7> vnlVector_1;
  vnlVector_1[3] = 56.98;
  vnlVector_1[4] = 22.32;
  vnlVector_1[5] = 1.00;
  vnlVector_1[6] = 119.02;
  vnl_vector_fixed<mitk::ScalarType,7> vnlVector_2;
  vnl_vector_fixed<mitk::ScalarType,7> vnlVector_3;
  for (int i=0; i<7; i++)
  {
    if (i<3)
    {
      vnlVector_1.put(i,itkVector_1[i]);
    }
    vnlVector_2[i] = vnlVector_1[i]- sqrt(mitk::eps/6.9);
    vnlVector_3[i] = vnlVector_1[i]- sqrt(mitk::eps/7.1);
  }
  MITK_TEST_CONDITION_REQUIRED(mitk::Equal(vnlVector_1,vnlVector_1), "Test vnl vector equality using the same vnl vector with mitk::eps");
  MITK_TEST_CONDITION_REQUIRED(!mitk::Equal(vnlVector_1,vnlVector_2), "Test vnl vector equality using different vnl vectors with an element-wise difference greater than mitk::eps");
  MITK_TEST_CONDITION_REQUIRED(mitk::Equal(vnlVector_1,vnlVector_3), "Test vnl vector equality using different vnl vectors with an element-wise difference less than mitk::eps");

  // test scalar equality method
  double scalar1 = 0.5689;
  double scalar2 = scalar1 + mitk::eps;
  double scalar3 = scalar1 + mitk::eps*0.95;
  MITK_TEST_CONDITION_REQUIRED(mitk::Equal(scalar1,scalar1), "Test scalar equality using the same scalar with mitk::eps");
  MITK_TEST_CONDITION_REQUIRED(!mitk::Equal(scalar1,scalar2), "Test scalar equality using the different scalars with a difference greater than mitk::eps");
  MITK_TEST_CONDITION_REQUIRED(mitk::Equal(scalar1,scalar3), "Test scalar equality using the different scalars with a difference less than mitk::eps");

  // test matrix equality methods
  vnl_matrix_fixed<mitk::ScalarType,3,3> vnlMatrix3x3_1;
  vnlMatrix3x3_1(0,0) = 1.1;
  vnlMatrix3x3_1(0,1) = 0.4;
  vnlMatrix3x3_1(0,2) = 5.3;
  vnlMatrix3x3_1(1,0) = 2.7;
  vnlMatrix3x3_1(1,1) = 3578.56418;
  vnlMatrix3x3_1(1,2) = 123.56;
  vnlMatrix3x3_1(2,0) = 546.89;
  vnlMatrix3x3_1(2,1) = 0.0001;
  vnlMatrix3x3_1(2,2) = 1.0;
  vnl_matrix_fixed<mitk::ScalarType,3,3> vnlMatrix3x3_2;
  vnlMatrix3x3_2(0,0) = 1.1000009;
  vnlMatrix3x3_2(0,1) = 0.4000009;
  vnlMatrix3x3_2(0,2) = 5.3000009;
  vnlMatrix3x3_2(1,0) = 2.7000009;
  vnlMatrix3x3_2(1,1) = 3578.5641809;
  vnlMatrix3x3_2(1,2) = 123.5600009;
  vnlMatrix3x3_2(2,0) = 546.8900009;
  vnlMatrix3x3_2(2,1) = 0.0001009;
  vnlMatrix3x3_2(2,2) = 1.0000009;

  mitk::ScalarType epsilon = 0.000001;
  MITK_TEST_CONDITION_REQUIRED(mitk::MatrixEqualElementWise(vnlMatrix3x3_1,vnlMatrix3x3_1,0.0),"Test for matrix equality with given epsilon=0.0 and exactly the same matrix elements");
  MITK_TEST_CONDITION_REQUIRED(!mitk::MatrixEqualElementWise(vnlMatrix3x3_1,vnlMatrix3x3_2,0.0),"Test for matrix equality with given epsilon=0.0 and slightly different matrix elements");
  MITK_TEST_CONDITION_REQUIRED(mitk::MatrixEqualElementWise(vnlMatrix3x3_1,vnlMatrix3x3_2,epsilon),"Test for matrix equality with given epsilon and slightly different matrix elements");
  MITK_TEST_CONDITION_REQUIRED(!mitk::MatrixEqualRMS(vnlMatrix3x3_1,vnlMatrix3x3_2,0.0),"Test for matrix equality with given epsilon=0.0 and slightly different matrix elements");
  MITK_TEST_CONDITION_REQUIRED(mitk::MatrixEqualRMS(vnlMatrix3x3_1,vnlMatrix3x3_2,epsilon),"Test for matrix equality with given epsilon and slightly different matrix elements");

  MITK_TEST_END();
}
