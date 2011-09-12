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

#ifndef MITK_THRESHOLDCOMPONENT_H
#define MITK_THRESHOLDCOMPONENT_H

#include "QmitkFunctionalityComponentContainer.h"
#include "QmitkExtExports.h"
#include <itkImage.h>
#include "ui_QmitkThresholdComponentControls.h"
class QmitkStdMultiWidget;

#include "qcheckbox.h"

/**
* \brief ComponentClass to find an adequate threshold for a selected image
* \ingroup QmitkFunctionalityComponent
* 
* This class inherits from
* - \ref QmitkFunctionalityComponentContainer

* 
* \section QmitkThresholdComponent Overview
* 
* The ThresholdComponent is a class to easy find an adequate threshold. On two ways the user can insert 
* a possible threshold: as a number in a textfield or by moving a scrollbar. The selected threshold will 
* be shown on the image as an new binary layer that contains those areas above the theshold.
* Like all other componentes the QmitkThresholdComponent inherits from QmitkFunctionalityComponentContainer.
* 
*/


class QmitkExt_EXPORT QmitkThresholdComponent : public QmitkFunctionalityComponentContainer
{

  Q_OBJECT

public:
  /***************       CONSTRUCTOR      ***************/
  /** \brief Constructor. */
  QmitkThresholdComponent(QObject *parent=0, const char *parentName=0, bool updateSelector = true, bool showSelector = true, QmitkStdMultiWidget *mitkStdMultiWidget = NULL);

  /***************        DESTRUCTOR      ***************/
  /** \brief Destructor. */
  virtual ~QmitkThresholdComponent();

  /***************        CREATE          ***************/

  /** \brief Method to create the GUI for the component from the .ui-File. This Method is obligatory */
  void CreateQtPartControl(QWidget *parent, mitk::DataStorage::Pointer dataStorage);

  /** \brief Method to create the connections for the component. This Method is obligatory even if no connections is needed*/
  virtual void CreateConnections();


  /***************      SET AND GET       ***************/

  /** \brief Method to set the Image Selector visible or invisible */
  virtual void SetSelectorVisibility(bool visibility);

  /** \brief Method to return the TreeNodeSelector-QmitkDataStorageComboBox */
  virtual QmitkDataStorageComboBox* GetTreeNodeSelector();

  /** \brief Method to set the DataStorage*/
  virtual void SetDataStorage(mitk::DataStorage::Pointer dataStorage/*, mitk::DataNode::Pointer dtn = NULL*/);

  /** \brief Method to get the DataStorage*/
  virtual mitk::DataStorage::Pointer GetDataStorage();
  
    /** \brief Method to return the ComboBox that includes all GUI-elements instead of the outermost checkable CheckBox and that can be set visible or not*/
  virtual QGroupBox * GetContentContainer();

    /** \brief Method to return the outermost checkable ComboBox that is to decide whether the content shall be shown or not */
  virtual QGroupBox * GetMainCheckBoxContainer();

/** \brief Method to return the group-box that contains the tree-node-selector */
virtual QGroupBox* GetImageContent();

  /***************      (DE)ACTIVATED     ***************/

  ///** \brief Method to set m_Activated to true */
  virtual void Activated();

  ///** \brief Method to set m_Activated to false */
  virtual void Deactivated();

public slots:  
  /***************      OHTER METHODS     ***************/

  virtual void DataStorageChanged(mitk::DataStorage::Pointer ds);

  /** \brief Slot method that will be called if TreeNodeSelector widget was activated to select the current image. */
	void ImageSelected(const mitk::DataNode* item);

  /** \brief Slot method that will be called if the CheckBox at the Threshold-Group-Box was toggled to show the threshold image or not. */ 
  void ShowThreshold(bool show = true);

  /** \brief Slot method that will be called if the CheckBox at the Threshold-Group-Box was toggled to show the threshold image or not. */ 
  void ShowThresholdFinderContent(bool show = true);

  /** \brief Slot method that will be called if the CheckBox at the TreeNodeSelector-Group-Box was toggled to show the TreeNodeSelector or not. */ 
  void ShowImageContent(bool show = true);

  /** \brief Slot method that will be called if the Thresholdslider was moved to update the shown image and the ThresholdLineEdit. */ 
  void ThresholdSliderChanged(int moved = 0);

  /** \brief Slot method that will be called if the ThresholdLineEdit was changed to update the shown image and the ThresholdSlider. */
  void ThresholdValueChanged( );

  /** \brief Slot method that will be called if the "Create Segmentation"-Button was pressed to create a new segmentation image. Everything over the threshold will have the value 1, all other 0. */
  void CreateThresholdSegmentation();


protected:

  /*!
  Method to create the node for the thresholdbased segmentation
  */
  mitk::DataNode::Pointer CreateSegmentationNode( mitk::Image* image);

  /** \brief Method that controls everything what happend after an image was selected */
  void DataObjectSelected();

  /***************        ATTRIBUTES      ***************/

  /** \brief Item on the actual selected Image in the TreeNodeSelector */
  mitk::DataStorage::Pointer m_DataStorage;

  /*!
  a reference to the node with the thresholdImage and adjusted preferences to show the threshold
  */
  mitk::DataNode::Pointer m_ThresholdImageNode;


private:

  /** \brief Method to create an Node that will hold the ThresholdImage and to set that preferences*/
  void CreateThresholdImageNode();

  /** \brief Method to set the Range of the ThresholdSlider*/
  void SetSliderRange();

  /** \brief Method to to delete ThresholdNode if Component is deactivated*/
  void DeleteThresholdNode();

 // /*!
 // \brief template to create thresholdSegmentation
 // */
 //template < typename TPixel, unsigned int VImageDimension >
 //void ThresholdSegmentation(itk::Image< TPixel, VImageDimension >* itkImage, mitk::Image* segmentation, QmitkThresholdComponent * /*thresholdComponent*/);

  template <typename TPixel, unsigned int VImageDimension>
  void ITKThresholding( itk::Image<TPixel, VImageDimension>* originalImage, mitk::Image* segmentation, unsigned int timeStep );

  /***************        ATTRIBUTES      ***************/

  /** \brief The created GUI from the .ui-File. This Attribute is  obligatory*/
 Ui::QmitkThresholdComponentControls * m_ThresholdComponentGUI;

  /** \brief This Attribute holds the information if a thresholdnode is already existing or not*/
  bool m_ThresholdNodeExisting;

    /*!
  * Segmentation made with thresholdSegmentation 
  */
  mitk::Image::Pointer m_ThresholdSegmentationImage;

  /*******GUI ELEMENTS******/
  QGroupBox* m_ThresholdFinder;
  QGroupBox* m_ThresholdSelectDataGroupBox;
  QGroupBox* m_ImageContent;
  QGroupBox* m_ContainerContent;
  QGroupBox* m_ShowThresholdGroupBox;
  QLineEdit* m_ThresholdInputNumber;
  QSlider*   m_ThresholdInputSlider;
  QGroupBox* m_ThresholdValueContent;
  QPushButton* m_CreateSegmentationButton;
  QCheckBox* m_DeleateImageIfDeactivatedCheckBox;
  QmitkDataStorageComboBox* m_TreeNodeSelector;

  typedef unsigned char DefaultSegmentationDataType;

};

#endif

