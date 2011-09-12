/*=========================================================================

Program:   Medical Imaging & Interaction Toolkit
Language:  C++
Date:      $Date: 2009-08-11 15:15:02 +0200 (Di, 11 Aug 2009) $
Version:   $Revision $

Copyright (c) German Cancer Research Center, Division of Medical and
Biological Informatics. All rights reserved.
See MITKCopyright.txt or http://www.mitk.org/copyright.html for details.

This software is distributed WITHOUT ANY WARRANTY; without even
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef NAVIGATIONTOOL_H_INCLUDED
#define NAVIGATIONTOOL_H_INCLUDED

//itk headers
#include <itkObjectFactory.h>
#include <itkSpatialObject.h>

//mitk headers
#include <mitkCommon.h>
#include <mitkDataNode.h>
#include <mitkTrackingTool.h>
#include <mitkTrackingTypes.h>
#include <mitkSurface.h>
#include <MitkIGTExports.h>

namespace mitk {
  /**Documentation
  * \brief An object of this class represents a navigation tool in the view of the software.
  *        A few informations like an identifier, a toolname, a surface and a itk spatial
  *        object are stored in such an object. The classes NavigationToolReader and
  *        are availiable to write/read tools to/from the harddisc. If you need a collection
  *        of navigation tools the class NavigationToolStorage could be used.
  *
  * \ingroup IGT
  */
  class MitkIGT_EXPORT NavigationTool : public itk::Object
  {
  public:

    mitkClassMacro(NavigationTool,itk::Object);
    itkNewMacro(Self);

    enum NavigationToolType {Instrument, Fiducial, Skinmarker, Unknown};

    //## getter and setter ##
    //NavigationToolType:
    itkGetMacro(Type,NavigationToolType);
    itkSetMacro(Type,NavigationToolType);
    //Identifier:
    itkGetMacro(Identifier,std::string);
    itkSetMacro(Identifier,std::string);
    //Datatreenode:
    itkGetMacro(DataNode,mitk::DataNode::Pointer);
    itkSetMacro(DataNode,mitk::DataNode::Pointer);
    //SpatialObject:
    itkGetMacro(SpatialObject,itk::SpatialObject<3>::Pointer);
    itkSetMacro(SpatialObject,itk::SpatialObject<3>::Pointer);
    //TrackingTool:
    itkGetMacro(TrackingTool,mitk::TrackingTool::Pointer);
    itkSetMacro(TrackingTool,mitk::TrackingTool::Pointer);
    //CalibrationFile:
    itkGetMacro(CalibrationFile,std::string);
    void SetCalibrationFile(const std::string filename); //itkSetMacro(CalibrationFile,std::string);
    //SerialNumber:
    itkGetMacro(SerialNumber,std::string);
    itkSetMacro(SerialNumber,std::string);
    //TrackingDeviceType:
    itkGetMacro(TrackingDeviceType,mitk::TrackingDeviceType);
    itkSetMacro(TrackingDeviceType,mitk::TrackingDeviceType);
    //ToolName (only getter):
    /** @return Returns the name of this navigation tool. Returns an empty string if there is
     *          no name (for example because the data node has not been set yet).
     *
     *          Note: There is no setter for the name,
     *          because the name of the corresponding data node is used as tool name. So if you
     *          want to modify the name of this navigation tool only get the data node and modify
     *          its name.
     */
    std::string GetToolName();
    //ToolSurface (only getter):
    /** @return Returns the surface of this navigation tool. Returns NULL if there is
     *          no surface (for example because the data node has not been set yet).
     *
     *          Note: There is no setter for the surface,
     *          because the surface is the data of the corresponding data node. So if you
     *          want to set a new surface only get the data node and modify its data.
     */
    mitk::Surface::Pointer GetToolSurface();
    //#######################

  protected:

    NavigationTool();
    ~NavigationTool();

    //## data structure of a navigation tool object ##
    std::string m_Identifier;
    NavigationToolType m_Type;
    /** @brief This DataNode holds a toolname and a tool surface */
    mitk::DataNode::Pointer m_DataNode;
    /** @brief This member variable holds a mathamatical description of the tool */
    itk::SpatialObject<3>::Pointer m_SpatialObject;
    /** @brief This member variable holds a pointer to the corresponding tracking tool in the hardware. */
    mitk::TrackingTool::Pointer m_TrackingTool;
    /** @brief The path to the calibration file of the tool. */
    std::string m_CalibrationFile;
    /** @brief A unique serial number of the tool which is needed to identify the tool correctly. This is very important
      *        in case of the NDI Aurora System. */
    std::string m_SerialNumber;
    /** @brief   This member holds the tracking device type of the tool. */
    mitk::TrackingDeviceType m_TrackingDeviceType;
    //#################################################

  };
} // namespace mitk
#endif //NAVIGATIONTOOL