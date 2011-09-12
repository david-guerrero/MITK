#ifndef BERRYPARTSERVICE_H_
#define BERRYPARTSERVICE_H_

#include "../berryIPartService.h"

#include <string>

namespace berry
{

class PartService : public IPartService
{

private:

  //PartListenerList listeners = new PartListenerList();
  IPartListener::Events partEvents;

  IWorkbenchPartReference::WeakPtr activePart;

  std::string debugListenersKey;
  std::string debugListeners2Key;

  /**
   * @param ref
   */
  void FirePartActivated(IWorkbenchPartReference::Pointer ref);

  /**
   * @param ref
   */
  void FirePartDeactivated(IWorkbenchPartReference::Pointer ref);

public:

  PartService(const std::string& debugListenersKey,
      const std::string& debugListeners2Key);

  /*
   * (non-Javadoc)
   * Method declared on IPartService
   */
  void AddPartListener(IPartListener::Pointer l);

  /*
   * (non-Javadoc)
   * Method declared on IPartService
   */
  void RemovePartListener(IPartListener::Pointer l);

  /**
   * @param ref
   */
  void FirePartBroughtToTop(IWorkbenchPartReference::Pointer ref);

  /**
   * @param ref
   */
  void FirePartClosed(IWorkbenchPartReference::Pointer ref);

  void FirePartVisible(IWorkbenchPartReference::Pointer ref);

  void FirePartHidden(IWorkbenchPartReference::Pointer ref);

  void FirePartInputChanged(IWorkbenchPartReference::Pointer ref);

  /**
   * @param ref
   */
  void FirePartOpened(IWorkbenchPartReference::Pointer ref);

  IWorkbenchPart::Pointer GetActivePart();

  IWorkbenchPartReference::Pointer GetActivePartReference();

  void SetActivePart(IWorkbenchPartReference::Pointer ref);

};

}

#endif /*BERRYPARTSERVICE_H_*/
