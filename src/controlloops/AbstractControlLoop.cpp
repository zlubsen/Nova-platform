#include <Arduino.h>
#include "AbstractControlLoop.hpp"
#include <MemoryFree.hpp>

#include <sstream>

AbstractControlLoop::AbstractControlLoop() {
  _status_messages.push_back(getFreeMemoryMessage());
}

std::string AbstractControlLoop::getFreeMemoryMessage() {
  char buffer[LCD_SIZE];
  sprintf(buffer, "Free mem:%7d", freeMemory());
  std::string str_message(buffer);

  return str_message;
}

std::string AbstractControlLoop::valueToLCDString(std::string item, double value) {
  item = item + ":";
  int len = LCD_SIZE-item.size();
  char buffer[len];
  sprintf(buffer, "%6f", value);
  std::string str_value(buffer);
  return item + str_value;
  /*char buffer[5];
  sprintf(buffer, "%f", value);
  std::string str_value(buffer);

  int text_length = item.size() + str_value.size() + 1;
  std::string mid_padding(LCD_SIZE-text_length, ' ');

  std::stringstream s;
  s << item << ":" << mid_padding << str_value;

  return s.str();*/
}

void AbstractControlLoop::cycleStatusMessageNext() {
  if(_current_status_message_index == _status_messages.size() - 1)
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
    return std::string(LCD_SIZE, ' ');
}

void AbstractControlLoop::updateStatusMessages() {
  _status_messages[0] = getFreeMemoryMessage();
}
