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


#ifndef PandaPluginView_h
#define PandaPluginView_h

#include <berryISelectionListener.h>

#include <QmitkFunctionality.h>

#include "ui_PandaPluginViewControls.h"
#include <QPushButton>


/*!
  \brief PandaPluginView

  \warning  This class is not yet documented. Use "git blame" and ask the author to provide basic documentation.

  \sa QmitkFunctionality
  \ingroup ${plugin_target}_internal
*/
class PandaPluginView : public QmitkFunctionality
{  
  // this is needed for all Qt objects that should have a Qt meta-object
  // (everything that derives from QObject and wants to have signal/slots)
  Q_OBJECT
  
  public:  

    static const std::string VIEW_ID;

    PandaPluginView();
    virtual ~PandaPluginView();

    virtual void CreateQtPartControl(QWidget *parent);

  protected slots:
  
    /// \brief Called when the user clicks the GUI button
    void DoImageProcessing();

		void ToggleFrames(bool on);

		void GenerateInitialUI();

		void CreateCommandBox();

  protected:

    /// \brief called by QmitkFunctionality when DataManager's selection has changed
    virtual void OnSelectionChanged( std::vector<mitk::DataNode*> nodes );
    Ui::PandaPluginViewControls m_Controls;
		QString m_FileName;
		QPushButton* m_LoadFileButton;

};

#endif // PandaPluginView_h

