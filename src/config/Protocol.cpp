#include <Arduino.h>
#include "Protocol.h"

public Root::Root() {
  _id = "root";
  _code = 0;

  children["nova"] = NovaNode("nova", 0);
  children["joystick_absolute"] = ProtocolLeaf("joystick_absolute", 1);
  children["joystick_absolute"] = ProtocolLeaf("joystick_absolute", 2);
  children["external_input"] = ExternalInputNode("external_input", 3);
  children["keep_distance"] = KeepDistanceNode("keep_distance", 4);
  children["track_object"] = TrackObjectNode("track_object", 5);
}
