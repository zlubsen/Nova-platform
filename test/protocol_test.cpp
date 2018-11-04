#include <unity.h>
#include <vector>
#include <ProtocolLogic.hpp>

void setUp(void) {
}

void tearDown(void) {
}

void test_build_external_input_move_command() {
  // This command can only be received, but provides a nice test for testing the creation of commands
  std::vector<int> expected {3,5,2,1,90};

  NovaProtocolCommandBuilder builder;
  std::vector<int> args {90};
  std::vector<int>* actual = builder.setModule(cmd_external_input)->setAsset(cmd_servo5)->setOperation(cmd_set_degree)->setArgs(args)->build();

  TEST_ASSERT_TRUE(expected == *actual);

  delete actual;
}

void test_build_track_object_ack_coordinates() {
  std::vector<int> expected {5,0,2,0};

  NovaProtocolCommandBuilder builder;
  std::vector<int>* actual = builder.setModule(cmd_track_object)->setOperation(cmd_ack_coordinates)->build();

  TEST_ASSERT_TRUE(expected == *actual);

  delete actual;
}

void test_build_status_publish_servo() {
  std::vector<int> expected {0,1,1,1,90};

  NovaProtocolCommandBuilder builder;
  std::vector<int> args {90};
  std::vector<int>* actual = builder.setModule(cmd_nova)->setAsset(cmd_servo1)->setOperation(cmd_get_degree)->setArgs(args)->build();

  TEST_ASSERT_TRUE(expected == *actual);

  delete actual;
}

void test_build_get_pid_tuning() {
  std::vector<int> expected {4,1,4,3,500,400,0};

  NovaProtocolCommandBuilder builder;
  std::vector<int> args {500,400,0};
  std::vector<int>* actual = builder.setModule(cmd_keep_distance)->setAsset(cmd_pid)->setOperation(cmd_get_tuning)->setArgs(args)->build();

  TEST_ASSERT_TRUE(expected == *actual);

  delete actual;
}

void test_receive_set_mode() {
  std::vector<int> received {0,0,1,1,2};

  NovaProtocolCommand cmd;
  NovaProtocolCommandReader reader;
  reader.readCommand(&received, &cmd);

  TEST_ASSERT_EQUAL_INT(cmd_nova, cmd.module);
  TEST_ASSERT_EQUAL_INT(cmd_module, cmd.asset);
  TEST_ASSERT_EQUAL_INT(cmd_set_mode, cmd.operation);
  TEST_ASSERT_EQUAL_INT(1, cmd.args.size());
  TEST_ASSERT_EQUAL_INT( ( cmd_joystick_relative - 1 ) , cmd.args.at(0));
}

void test_receive_external_input_move_command() {
  std::vector<int> received {3,1,3,1,90};

  NovaProtocolCommand cmd;
  NovaProtocolCommandReader reader;
  reader.readCommand(&received, &cmd);

  TEST_ASSERT_EQUAL_INT(cmd_external_input, cmd.module);
  TEST_ASSERT_EQUAL_INT(cmd_servo1, cmd.asset);
  TEST_ASSERT_EQUAL_INT(cmd_set_degree_steps, cmd.operation);
  TEST_ASSERT_EQUAL_INT(1, cmd.args.size());
  TEST_ASSERT_EQUAL_INT(90, cmd.args.at(0));
}

void test_receive_pid_tuning_command() {
  std::vector<int> received {4,1,3,3,500,400,0};

  NovaProtocolCommand cmd;
  NovaProtocolCommandReader reader;
  reader.readCommand(&received, &cmd);

  TEST_ASSERT_EQUAL_INT(cmd_keep_distance, cmd.module);
  TEST_ASSERT_EQUAL_INT(cmd_pid, cmd.asset);
  TEST_ASSERT_EQUAL_INT(cmd_set_tuning, cmd.operation);
  TEST_ASSERT_EQUAL_INT(3, cmd.args.size());
  TEST_ASSERT_EQUAL_INT(500, cmd.args.at(0));
  TEST_ASSERT_EQUAL_INT(400, cmd.args.at(1));
  TEST_ASSERT_EQUAL_INT(0, cmd.args.at(2));
}

void test_receive_track_object_set_coordinates() {
  std::vector<int> received {5,0,1,2,80,70};

  NovaProtocolCommand cmd;
  NovaProtocolCommandReader reader;
  reader.readCommand(&received, &cmd);

  TEST_ASSERT_EQUAL_INT(cmd_track_object, cmd.module);
  TEST_ASSERT_EQUAL_INT(cmd_module, cmd.asset);
  TEST_ASSERT_EQUAL_INT(cmd_set_coordinates, cmd.operation);
  TEST_ASSERT_EQUAL_INT(2, cmd.args.size());
  TEST_ASSERT_EQUAL_INT(80, cmd.args.at(0));
  TEST_ASSERT_EQUAL_INT(70, cmd.args.at(1));

  TEST_ASSERT(false);
}

//#include <MemoryFree.hpp>
/*void test_lookup_size() {
  Serial.print("free before: ");
  Serial.println(freeMemory());
  NovaProtocolCommandReader reader_size;

  Serial.print("free after: ");
  Serial.println(freeMemory());

  Serial.print("entry costs: ");
  Serial.println(sizeof(LookupEntry));

  TEST_ASSERT(false);
}*/

void setup() {
  delay(2000);
  Serial.begin(9600);

  UNITY_BEGIN();
  RUN_TEST(test_build_external_input_move_command);
  RUN_TEST(test_build_track_object_ack_coordinates);
  RUN_TEST(test_build_status_publish_servo);
  RUN_TEST(test_build_get_pid_tuning);

  RUN_TEST(test_receive_set_mode);
  RUN_TEST(test_receive_external_input_move_command);
  RUN_TEST(test_receive_pid_tuning_command);
  RUN_TEST(test_receive_track_object_set_coordinates);

  //RUN_TEST(test_lookup_size);

  UNITY_END();
}

void loop() {
}
