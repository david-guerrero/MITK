/*=========================================================================
 
Program:   Medical Imaging & Interaction Toolkit
Language:  C++
Date:      $Date$
Version:   $Revision: 1.12 $
 
Copyright (c) German Cancer Research Center, Division of Medical and
Biological Informatics. All rights reserved.
See MITKCopyright.txt or http://www.mitk.org/copyright.html for details.
 
This software is distributed WITHOUT ANY WARRANTY; without even
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
PURPOSE.  See the above copyright notices for more information.
 
=========================================================================*/

#include "mitkSceneReaderV1.h"
#include "mitkSerializerMacros.h"
#include "mitkDataNodeFactory.h"
#include "mitkBaseRenderer.h"
#include "mitkPropertyListDeserializer.h"
#include "Poco/Path.h"

MITK_REGISTER_SERIALIZER(SceneReaderV1)
    
bool mitk::SceneReaderV1::LoadScene( TiXmlDocument& document, const std::string& workingDirectory, DataStorage* storage )
{
  assert(storage);
  bool error(false);

  // TODO prepare to detect errors (such as cycles) from wrongly written or edited xml files

  // iterate all nodes
  // first level nodes should be <node> elements
  for( TiXmlElement* element = document.FirstChildElement("node"); element != NULL; element = element->NextSiblingElement("node") )
  {
    //   1. if there is a <data type="..." file="..."> element,
    //        - construct a name for the appropriate serializer
    //        - try to instantiate this serializer via itk object factory
    //        - if serializer could be created, use it to read the file into a BaseData object
    //        - if successful, call the new node's SetData(..)
    DataNode::Pointer node = LoadBaseDataFromDataTag( element->FirstChildElement("data"), workingDirectory, error );
   
    //   2. check child nodes
    const char* uida = element->Attribute("UID");
    std::string uid("");

    if (uida)
    {
      uid = uida;
      m_NodeForID[uid] = node.GetPointer();
      m_IDForNode[ node.GetPointer() ] = uid;
    }
    else
    {
      MITK_ERROR << "No UID found for current node. Node will have no parents.";
      error = true;
    }

    // remember node for later adding to DataStorage
    m_Nodes.insert( std::make_pair( node, std::list<std::string>() ) );

    //   3. if there are <source> elements, remember parent objects
    for( TiXmlElement* source = element->FirstChildElement("source"); source != NULL; source = source->NextSiblingElement("source") )
    {
      const char* sourceUID = source->Attribute("UID");
      if (sourceUID)
      {
        m_Nodes[node].push_back( std::string(sourceUID) );
      }
    }


    //   5. if there are <properties> nodes, 
    //        - instantiate the appropriate PropertyListDeSerializer
    //        - use them to construct PropertyList objects
    //        - add these properties to the node (if necessary, use renderwindow name)
    bool success = DecorateNodeWithProperties(node, element, workingDirectory);
    if (!success)
    {
      MITK_ERROR << "Could not load properties for node.";
      error = true;
    }
  } // end for all <node>
    
  // remove all unknown parent UIDs
  for (NodesAndParentsMapType::iterator nodesIter = m_Nodes.begin();
       nodesIter != m_Nodes.end();
       ++nodesIter)
  {
    for (std::list<std::string>::iterator parentsIter = nodesIter->second.begin();
         parentsIter != nodesIter->second.end();)
    {
      if (m_NodeForID.find( *parentsIter ) == m_NodeForID.end())
      {
        parentsIter = nodesIter->second.erase( parentsIter );
        MITK_WARN << "Found a DataNode with unknown parents. Will add it to DataStorage without any parent objects.";
        error = true;
      }
      else
      {
        ++parentsIter;
      }
    }
  }

  // repeat
  //   for all created nodes
  unsigned int lastMapSize(0);
  while ( lastMapSize != m_Nodes.size()) // this is to prevent infinite loops; each iteration must at least add one node to DataStorage
  {
    lastMapSize = m_Nodes.size();

    for (NodesAndParentsMapType::iterator nodesIter = m_Nodes.begin();
         nodesIter != m_Nodes.end();
         ++nodesIter)
    {
      bool addNow(true);
      // if any parent node is not yet in DataStorage, skip node for now and check later
      for (std::list<std::string>::iterator parentsIter = nodesIter->second.begin();
           parentsIter != nodesIter->second.end();
           ++parentsIter)
      {
        if ( !storage->Exists( m_NodeForID[ *parentsIter ] ) )
        {
          addNow = false;
          break;
        }
      }
      
      if (addNow)
      {
        DataStorage::SetOfObjects::Pointer parents = DataStorage::SetOfObjects::New();
        for (std::list<std::string>::iterator parentsIter = nodesIter->second.begin();
             parentsIter != nodesIter->second.end();
             ++parentsIter)
        {
          parents->push_back( m_NodeForID[ *parentsIter ] );
        }
   
        // if all parents are found in datastorage (or are unknown), add node to DataStorage
        storage->Add( nodesIter->first, parents );

        // remove this node from m_Nodes
        m_Nodes.erase( nodesIter );

        // break this for loop because iterators are probably invalid
        break;
      }
    }
  }

  // All nodes that are still in m_Nodes at this point are not part of a proper directed graph structure. We'll add such nodes without any parent information.
  for (NodesAndParentsMapType::iterator nodesIter = m_Nodes.begin();
       nodesIter != m_Nodes.end();
       ++nodesIter)
  {
    storage->Add( nodesIter->first );
    MITK_WARN << "Encountered node that is not part of a directed graph structure. Will be added to DataStorage without parents.";
    error = true;
  }

  return !error;
}

mitk::DataNode::Pointer mitk::SceneReaderV1::LoadBaseDataFromDataTag( TiXmlElement* dataElement, const std::string& workingDirectory, bool& error )
{
  DataNode::Pointer node;

  if (dataElement) 
  {
    const char* filename( dataElement->Attribute("file") );
    if ( filename )
    {
      DataNodeFactory::Pointer factory = DataNodeFactory::New();
      factory->SetFileName( workingDirectory + Poco::Path::separator() + filename );
      
      try
      {
        factory->Update();
        node = factory->GetOutput();
      }
      catch (std::exception& e)
      {
        MITK_ERROR << "Error during attempt to read '" << filename << "'. Exception says: " << e.what();
        error = true;
      }

      if (node.IsNull())
      {
        MITK_ERROR << "Error during attempt to read '" << filename << "'. Factory returned NULL object.";
        error = true;
      }
    }
  }

  // in case there was no <data> element we create a new empty node (for appending a propertylist later)
  if (node.IsNull())
  {
    node = DataNode::New();
  }

  return node;
}

bool mitk::SceneReaderV1::DecorateNodeWithProperties(DataNode* node, TiXmlElement* nodeElement, const std::string& workingDirectory)
{
  assert(node);
  assert(nodeElement);
  bool error(false);

  for( TiXmlElement* properties = nodeElement->FirstChildElement("properties"); properties != NULL; properties = properties->NextSiblingElement("properties") )
  {
    const char* propertiesfilea( properties->Attribute("file") );
    std::string propertiesfile( propertiesfilea ? propertiesfilea : "" );
    
    const char* renderwindowa( properties->Attribute("renderwindow") );
    std::string renderwindow( renderwindowa ? renderwindowa : "" );

    BaseRenderer* renderer = BaseRenderer::GetByName( renderwindow );
    if (renderer || renderwindow.empty())
    {
      PropertyList::Pointer propertyList = node->GetPropertyList(renderer); // DataNode implementation always returns a propertylist
      // clear all properties from node that might be set by DataNodeFactory during loading 
      propertyList->Clear();

      // use deserializer to construct new properties
      PropertyListDeserializer::Pointer deserializer = PropertyListDeserializer::New();
      
      deserializer->SetFilename(workingDirectory + Poco::Path::separator() + propertiesfile);
      bool success = deserializer->Deserialize();
      error |= !success;
      PropertyList::Pointer readProperties = deserializer->GetOutput();

      if (readProperties.IsNotNull())
      {
        propertyList->ConcatenatePropertyList( readProperties, true ); // true = replace
      }
      else
      {
        MITK_ERROR << "Property list reader did not return a property list. This is an implementation error. Please tell your developer.";
        error = true;
      }
    }
    else
    {
      MITK_ERROR << "Found properties for renderer " << renderwindow << " but there is no such renderer in current application. Ignoring those properties";
      error = true;
    }
  }

  return !error;
}

