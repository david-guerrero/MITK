/*=========================================================================

Program:   Medical Imaging & Interaction Toolkit
Language:  C++
Date:      $Date$
Version:   $Revision: 1.0 $

Copyright (c) German Cancer Research Center, Division of Medical and
Biological Informatics. All rights reserved.
See MITKCopyright.txt or http://www.mitk.org/copyright.html for details.

This software is distributed WITHOUT ANY WARRANTY; without even
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef QmitkErasePaintbrushToolGUI_h_Included
#define QmitkErasePaintbrushToolGUI_h_Included

#include "QmitkPaintbrushToolGUI.h"
#include "QmitkExtExports.h"

/**
  \ingroup org_mitk_gui_qt_interactivesegmentation_internal
  \brief GUI for mitk::PaintbrushTool.

  This GUI shows a slider to change the tool's threshold and an OK button to accept a preview for actual thresholding.

  Last contributor: $Author$
*/

class QmitkExt_EXPORT QmitkErasePaintbrushToolGUI : public QmitkPaintbrushToolGUI
{
  Q_OBJECT

  public:
    
    mitkClassMacro(QmitkErasePaintbrushToolGUI, QmitkPaintbrushToolGUI);
    itkNewMacro(QmitkErasePaintbrushToolGUI);

  signals:

  public slots:

  protected slots:

  protected:

    QmitkErasePaintbrushToolGUI();
    virtual ~QmitkErasePaintbrushToolGUI();
};

#endif

