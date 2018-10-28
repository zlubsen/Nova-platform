#include <unity.h>
#include <vector>
#include <ProtocolLogic.hpp>

void test_external_input_move_command() {
  std::vector<int> expected {3,5,2,1,90};

  Serial.println("heartbeat...");
  NovaProtocolCommandBuilder* builder = NovaProtocolCommandBuilder::createCommand();
  std::vector<int> args {90};
  std::vector<int>* actual = builder->setModule("external_input")->setAsset("servo5")->setOperation("set_degree")->setArgs(args)->build();

  TEST_ASSERT_TRUE(expected == *actual);
}

void setup() {
  delay(2000);
  Serial.begin(9600);

  UNITY_BEGIN();
  RUN_TEST(test_external_input_move_command);
  UNITY_END();
}

void loop() {
}
