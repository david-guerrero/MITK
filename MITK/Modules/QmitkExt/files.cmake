SET(CPP_FILES

 QmitkApplicationBase/QmitkCommonFunctionality.cpp

 #QmitkModels/QmitkDataStorageListModel.cpp
 #QmitkModels/QmitkPropertiesTableModel.cpp
 #QmitkModels/QmitkDataStorageTreeModel.cpp
 #QmitkModels/QmitkDataStorageTableModel.cpp
 #QmitkModels/QmitkPropertyDelegate.cpp
 #QmitkModels/QmitkPointListModel.cpp

 #QmitkAlgorithmFunctionalityComponent.cpp
 #QmitkBaseAlgorithmComponent.cpp
 QmitkAboutDialog/QmitkAboutDialog.cpp
        #QmitkFunctionalityComponents/QmitkSurfaceCreatorComponent.cpp
              #QmitkFunctionalityComponents/QmitkPixelGreyValueManipulatorComponent.cpp
                #QmitkFunctionalityComponents/QmitkConnectivityFilterComponent.cpp
            #QmitkFunctionalityComponents/QmitkImageCropperComponent.cpp
              #QmitkFunctionalityComponents/QmitkSeedPointSetComponent.cpp
    #QmitkFunctionalityComponents/QmitkSurfaceTransformerComponent.cpp

 QmitkPropertyObservers/QmitkBasePropertyView.cpp
 QmitkPropertyObservers/QmitkBoolPropertyWidget.cpp
 QmitkPropertyObservers/QmitkColorPropertyEditor.cpp
 QmitkPropertyObservers/QmitkColorPropertyView.cpp
 QmitkPropertyObservers/QmitkEnumerationPropertyWidget.cpp
 QmitkPropertyObservers/QmitkNumberPropertyEditor.cpp
 QmitkPropertyObservers/QmitkNumberPropertyView.cpp
 QmitkPropertyObservers/QmitkPropertyViewFactory.cpp
 QmitkPropertyObservers/QmitkStringPropertyEditor.cpp
 QmitkPropertyObservers/QmitkStringPropertyOnDemandEdit.cpp
 QmitkPropertyObservers/QmitkStringPropertyView.cpp
 QmitkPropertyObservers/QmitkNumberPropertySlider.cpp
 QmitkPropertyObservers/QmitkUGCombinedRepresentationPropertyWidget.cpp

 qclickablelabel.cpp
 #QmitkAbortEventFilter.cpp
 # QmitkApplicationCursor.cpp
 QmitkCallbackFromGUIThread.cpp
 QmitkEditPointDialog.cpp
 QmitkExtRegisterClasses.cpp
 QmitkFileChooser.cpp
 # QmitkRenderingManager.cpp
 # QmitkRenderingManagerFactory.cpp
 # QmitkRenderWindow.cpp

 # QmitkEventAdapter.cpp

 QmitkFloatingPointSpanSlider.cpp
 QmitkColorTransferFunctionCanvas.cpp
 QmitkSlicesInterpolator.cpp
 QmitkStandardViews.cpp
 QmitkStepperAdapter.cpp
 # QmitkLineEditLevelWindowWidget.cpp
 # mitkSliderLevelWindowWidget.cpp
 # QmitkLevelWindowWidget.cpp
 # QmitkPointListWidget.cpp
 # QmitkPointListView.cpp
 QmitkPiecewiseFunctionCanvas.cpp
 QmitkSliderNavigatorWidget.cpp
 QmitkTransferFunctionCanvas.cpp
 QmitkCrossWidget.cpp
  #QmitkLevelWindowRangeChangeDialog.cpp

 #QmitkLevelWindowPresetDefinitionDialog.cpp
 # QmitkLevelWindowWidgetContextMenu.cpp
 QmitkSliceWidget.cpp
 # QmitkStdMultiWidget.cpp
 QmitkTransferFunctionWidget.cpp
 QmitkTransferFunctionGeneratorWidget.cpp
 QmitkSelectableGLWidget.cpp
 QmitkToolReferenceDataSelectionBox.cpp
 QmitkToolWorkingDataSelectionBox.cpp
 QmitkToolGUIArea.cpp
 QmitkToolSelectionBox.cpp
 # QmitkPropertyListPopup.cpp
 QmitkToolGUI.cpp
 QmitkNewSegmentationDialog.cpp
 QmitkPaintbrushToolGUI.cpp
 QmitkDrawPaintbrushToolGUI.cpp
 QmitkErasePaintbrushToolGUI.cpp
 QmitkBinaryThresholdToolGUI.cpp
 QmitkCalculateGrayValueStatisticsToolGUI.cpp
 QmitkCopyToClipBoardDialog.cpp
 # QmitkMaterialEditor.cpp
 # QmitkMaterialShowcase.cpp
 # QmitkPropertiesTableEditor.cpp
 QmitkPrimitiveMovieNavigatorWidget.cpp
 # QmitkDataStorageComboBox.cpp

 QmitkHistogram.cpp
 QmitkHistogramWidget.cpp

 QmitkPlotWidget.cpp
 QmitkPlotDialog.cpp
 QmitkPointListModel.cpp
 QmitkPointListView.cpp
 QmitkPointListWidget.cpp
 QmitkPointListViewWidget.cpp
 QmitkCorrespondingPointSetsView.cpp
 QmitkCorrespondingPointSetsModel.cpp
 QmitkCorrespondingPointSetsWidget.cpp
 QmitkVideoBackground.cpp
 QmitkHotkeyLineEdit.cpp
 QmitkErodeToolGUI.cpp
 QmitkDilateToolGUI.cpp
 QmitkMorphologicToolGUI.cpp
 QmitkOpeningToolGUI.cpp
 QmitkClosingToolGUI.cpp
 QmitkBinaryThresholdULToolGUI.cpp
 QmitkPixelManipulationToolGUI.cpp
 QmitkRegionGrow3DToolGUI.cpp
 QmitkToolRoiDataSelectionBox.cpp
 QmitkBoundingObjectWidget.cpp
)

IF ( NOT ${VTK_MAJOR_VERSION}.${VTK_MINOR_VERSION}.${VTK_BUILD_VERSION} VERSION_LESS 5.4.0 )
  SET(CPP_FILES
    ${CPP_FILES}
    QmitkVtkHistogramWidget.cpp
    QmitkVtkLineProfileWidget.cpp
  )
ENDIF()

IF (NOT APPLE)
SET(CPP_FILES
  ${CPP_FILES}
 QmitkBaseComponent.cpp
 QmitkBaseFunctionalityComponent.cpp
 QmitkFunctionalityComponentContainer.cpp
 QmitkFunctionalityComponents/QmitkThresholdComponent.cpp

 )
ENDIF()

QT4_ADD_RESOURCES(CPP_FILES resources/QmitkResources.qrc)


SET(MOC_H_FILES


 QmitkPropertyObservers/QmitkBasePropertyView.h
 QmitkPropertyObservers/QmitkBoolPropertyWidget.h
 QmitkPropertyObservers/QmitkColorPropertyEditor.h
 QmitkPropertyObservers/QmitkColorPropertyView.h
 QmitkPropertyObservers/QmitkEnumerationPropertyWidget.h
 QmitkPropertyObservers/QmitkNumberPropertyEditor.h
 QmitkPropertyObservers/QmitkNumberPropertyView.h
 QmitkPropertyObservers/QmitkStringPropertyEditor.h
 QmitkPropertyObservers/QmitkStringPropertyOnDemandEdit.h
 QmitkPropertyObservers/QmitkStringPropertyView.h
 QmitkPropertyObservers/QmitkNumberPropertySlider.h
 QmitkPropertyObservers/QmitkUGCombinedRepresentationPropertyWidget.h

              # QmitkFunctionalityComponents/QmitkSurfaceCreatorComponent.h
                      #QmitkFunctionalityComponents/QmitkPixelGreyValueManipulatorComponent.h
                  # QmitkFunctionalityComponents/QmitkConnectivityFilterComponent.h
            # QmitkFunctionalityComponents/QmitkImageCropperComponent.h
                                # QmitkFunctionalityComponents/QmitkSeedPointSetComponent.h
                        # QmitkFunctionalityComponents/QmitkSurfaceTransformerComponent.h

 qclickablelabel.h
 QmitkCallbackFromGUIThread.h
 QmitkEditPointDialog.h
 #QmitkAlgorithmFunctionalityComponent.h
 #QmitkBaseAlgorithmComponent.h
 QmitkStandardViews.h
 QmitkStepperAdapter.h
 QmitkSliderNavigatorWidget.h
 QmitkSliceWidget.h
 QmitkSlicesInterpolator.h
 QmitkColorTransferFunctionCanvas.h
 QmitkPiecewiseFunctionCanvas.h
 QmitkTransferFunctionCanvas.h
 QmitkFloatingPointSpanSlider.h
 QmitkCrossWidget.h
 QmitkTransferFunctionWidget.h
 QmitkTransferFunctionGeneratorWidget.h
 QmitkToolGUIArea.h
 QmitkToolGUI.h
 QmitkToolReferenceDataSelectionBox.h
 QmitkToolWorkingDataSelectionBox.h
 QmitkToolSelectionBox.h
 # QmitkPropertyListPopup.h
 #QmitkSelectableGLWidget.h
 QmitkNewSegmentationDialog.h
 QmitkPaintbrushToolGUI.h
 QmitkDrawPaintbrushToolGUI.h
 QmitkErasePaintbrushToolGUI.h
 QmitkBinaryThresholdToolGUI.h
 QmitkCalculateGrayValueStatisticsToolGUI.h
 QmitkCopyToClipBoardDialog.h
 QmitkPrimitiveMovieNavigatorWidget.h
 QmitkPointListModel.h
 QmitkPointListView.h
 QmitkPointListWidget.h
 QmitkPointListViewWidget.h
 QmitkCorrespondingPointSetsView.h
 QmitkCorrespondingPointSetsModel.h
 QmitkCorrespondingPointSetsWidget.h
 QmitkHistogramWidget.h
 QmitkVideoBackground.h
 QmitkFileChooser.h
 QmitkHotkeyLineEdit.h
 QmitkAboutDialog/QmitkAboutDialog.h
 QmitkErodeToolGUI.h
 QmitkDilateToolGUI.h
 QmitkMorphologicToolGUI.h
 QmitkOpeningToolGUI.h
 QmitkClosingToolGUI.h
 QmitkBinaryThresholdULToolGUI.h
 QmitkPixelManipulationToolGUI.h
 QmitkRegionGrow3DToolGUI.h
 QmitkToolRoiDataSelectionBox.h
 QmitkBoundingObjectWidget.h
)

IF ( NOT ${VTK_MAJOR_VERSION}.${VTK_MINOR_VERSION}.${VTK_BUILD_VERSION} VERSION_LESS 5.4.0 )
  SET(MOC_H_FILES
    ${MOC_H_FILES}
    QmitkVtkHistogramWidget.h
    QmitkVtkLineProfileWidget.h
  )
ENDIF()

IF (NOT APPLE)
SET(MOC_H_FILES
  ${MOC_H_FILES}
 QmitkBaseComponent.h
 QmitkBaseFunctionalityComponent.h
 QmitkFunctionalityComponentContainer.h
 QmitkFunctionalityComponents/QmitkThresholdComponent.h
)
ENDIF()

SET(UI_FILES
  QmitkSliderNavigator.ui
  # QmitkLevelWindowRangeChange.ui
  # QmitkLevelWindowPresetDefinition.ui
  # QmitkLevelWindowWidget.ui
  QmitkSliceWidget.ui
  QmitkTransferFunctionWidget.ui
  QmitkTransferFunctionGeneratorWidget.ui
  QmitkSelectableGLWidget.ui
  QmitkPrimitiveMovieNavigatorWidget.ui
  QmitkFunctionalityComponentContainerControls.ui
  QmitkFunctionalityComponents/QmitkThresholdComponentControls.ui
  QmitkAboutDialog/QmitkAboutDialogGUI.ui
)

SET(QRC_FILES
  QmitkExt.qrc
)
