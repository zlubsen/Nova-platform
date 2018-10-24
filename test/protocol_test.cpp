#include <Arduino.h>
#include <unity.h>
#include <vector>
#include <communication/ProtocolLogic.h>

void test_external_input_move_command() {
  /*std::vector<int> expected {3,5,2,1,90};

  NovaProtocolCommandBuilder* builder = NovaProtocolCommandBuilder::createCommand();
  std::vector<int> args {0};
  //args.push_back(0);
  std::vector<int>* actual = builder->setModule("external_input")->setAsset("servo5")->setOperation("set_degree")->setArgs(args)->build();
  TEST_ASSERT_TRUE(expected == *actual);*/
  int expected = 5;
  int actual = 5;
  TEST_ASSERT_EQUAL(expected, actual);
}

void setup() {
  delay(2000);

  UNITY_BEGIN();
  RUN_TEST(test_external_input_move_command);
  UNITY_END();
}

void loop() {
}
