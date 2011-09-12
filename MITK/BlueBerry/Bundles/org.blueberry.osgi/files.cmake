# Files which should be processed by Qts moc
SET(MOC_H_FILES
  src/internal/berryCTKPluginActivator.h
  src/internal/berryExtensionPointService.h
)

SET(CACHED_RESOURCE_FILES
  plugin.xml
)

SET(EVENT_CPP_FILES
  berryBundleEvent.cpp
  berryPlatformEvent.cpp
)

SET(INTERNAL_CPP_FILES
  berryBundle.cpp
  berryBundleContext.cpp
  berryBundleDirectory.cpp
  berryBundleManifest.cpp
  berryCodeCache.cpp
  berryCTKPluginActivator.cpp
  berryDefaultActivator.cpp
  berryConfigurationElement.cpp
  berryExtension.cpp
  berryExtensionPoint.cpp
  berryExtensionPointService.cpp
  berryInternalPlatform.cpp
  berryPlatformLogChannel.cpp
  berryProvisioningInfo.cpp
  berrySystemBundle.cpp
  berrySystemBundleActivator.cpp
  berrySystemBundleManifest.cpp
)

SET(SERVICE_CPP_FILES
  berryIExtensionPointService.cpp
  berryService.cpp
  berryServiceRegistry.cpp
)

SET(SRC_CPP_FILES
  berryDebugBreakpointManager.cpp
  berryException.cpp
  berryExtensionType.cpp
  berryIBundleManifest.cpp
  berryIDebugObjectListener.cpp
  berryBundleLoader.cpp
  berryDebugUtil.cpp
  berryObject.cpp
  berryObjects.cpp
  berryPlatform.cpp
  berryPlatformException.cpp
  berryPlugin.cpp
  berryStackTrace.cpp
  
  # application
  application/berryIApplication.cpp
  application/berryStarter.cpp
)

SET(CPP_FILES manifest.cpp)

foreach(file ${SRC_CPP_FILES})
  SET(CPP_FILES ${CPP_FILES} src/${file})
endforeach(file ${SRC_CPP_FILES})

foreach(file ${EVENT_CPP_FILES})
  SET(CPP_FILES ${CPP_FILES} src/event/${file})
endforeach(file ${EVENT_CPP_FILES})

foreach(file ${INTERNAL_CPP_FILES})
  SET(CPP_FILES ${CPP_FILES} src/internal/${file})
endforeach(file ${INTERNAL_CPP_FILES})

foreach(file ${SERVICE_CPP_FILES})
  SET(CPP_FILES ${CPP_FILES} src/service/${file})
endforeach(file ${SERVICE_CPP_FILES})
