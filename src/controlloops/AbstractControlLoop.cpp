#include <Arduino.h>
#include "AbstractControlLoop.hpp"
#include <MemoryFree.hpp>

#include <sstream>

AbstractControlLoop::AbstractControlLoop() {
  _status_messages.push_back(getFreeMemoryMessage());
}

std::string AbstractControlLoop::getFreeMemoryMessage() {
  std::string str_start = "Free mem:";

  char buffer[4];
  sprintf(buffer, "%d", freeMemory());
  std::string str_value(buffer);

  int text_length = str_start.size() + str_value.size();
  std::string mid_padding(16-text_length, ' ');

  std::stringstream s;
  s << str_start << mid_padding << str_value;

  return s.str();
}

std::string AbstractControlLoop::valueToLCDString(std::string item, double value) {
  char buffer[16];
  sprintf(buffer, "%s: %f", item.c_str(), value);
  std::string str_value(buffer);

  return str_value;
}

void AbstractControlLoop::cycleStatusMessageNext() {
  if(_current_status_message_index >= _status_messages.size() - 1)
    _current_status_message_index = 0;
  else
    _current_status_message_index++;
}

void AbstractControlLoop::cycleStatusMessagePrevious() {
  if(_current_status_message_index == 0)
    _current_status_message_index = _status_messages.size() - 1;
  else
    _current_status_message_index--;
}

std::string AbstractControlLoop::getLCDStatusMessage() {
  updateStatusMessages();
  if(_current_status_message_index < _status_messages.size())
    return _status_messages.at(_current_status_message_index);
  else
    return std::string(16, ' ');
}

void AbstractControlLoop::updateStatusMessages() {
  _status_messages[0] = getFreeMemoryMessage();
}
