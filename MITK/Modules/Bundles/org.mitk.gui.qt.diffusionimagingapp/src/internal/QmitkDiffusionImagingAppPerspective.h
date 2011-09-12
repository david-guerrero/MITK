/*=========================================================================
 
 Program:   BlueBerry Platform
 Language:  C++
 Date:      $Date: 2009-10-23 02:59:36 +0200 (Fr, 23 Okt 2009) $
 Version:   $Revision: 19652 $
 
 Copyright (c) German Cancer Research Center, Division of Medical and
 Biological Informatics. All rights reserved.
 See MITKCopyright.txt or http://www.mitk.org/copyright.html for details.
 
 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE.  See the above copyright notices for more information.
 
 =========================================================================*/


#ifndef QMITKDIFFUSIONIMAGINGAPPPERSPECTIVE_H_
#define QMITKDIFFUSIONIMAGINGAPPPERSPECTIVE_H_

#include <berryIPerspectiveFactory.h>

class QmitkDiffusionImagingAppPerspective : public QObject, public berry::IPerspectiveFactory
{
  Q_OBJECT
  Q_INTERFACES(berry::IPerspectiveFactory)

public:

  QmitkDiffusionImagingAppPerspective() {}
  QmitkDiffusionImagingAppPerspective(const QmitkDiffusionImagingAppPerspective& other)
  {
    Q_UNUSED(other)
    throw std::runtime_error("Copy constructor not implemented");
  }
  ~QmitkDiffusionImagingAppPerspective() {}

  void CreateInitialLayout(berry::IPageLayout::Pointer layout);
};

#endif /* QMITKDIFFUSIONIMAGINGAPPPERSPECTIVE_H_ */
