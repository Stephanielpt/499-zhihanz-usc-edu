#include<gtest/gtest.h>
#include <grpcpp/grpcpp.h>
#include<iostream>
#include<string>
#include<vector>
#include "kvstore_client.h"
#include "../kvstore/cchash.h"
#include"kvstore.pb.h"

namespace UnitTest {
using namespace std;
// This is a client implemented for unittest
class UnitTestKVClient {
 public:
  UnitTestKVClient() : table_{} {}
  bool PutOrUpdate(std::string key, std::string value) {
    table_.AddOrUpdate(key, value);
    return true;
  }
  bool Put(std::string key, std::string value) {
    if (table_.Has(key)) return false;
    table_.Add(key, value);
    return true;
  }
  string GetValue(const std::string& key) {
    string response = table_.GetValue(key);
    return response;
  }
  bool Has(std::string key) { return table_.Has(key); }
  bool Delete(std::string key) {
    table_.DeleteKey(key);
    return true;
  }

  ConcurrentHashTable<string, string> table_;
};

}  // namespace UnitTest
using namespace UnitTest;
using chirp::GetReply;
using namespace std;
ConcurrentHashTable<string, string> table{};
UnitTestKVClient client{};
TEST(test, Put){
  auto b1 = client.Put("test1", "val1");
  auto b2 = client.Put("test1", "val2");
  EXPECT_EQ(true, b1);
  EXPECT_EQ(false, b2);
  auto val = client.GetValue("test1");
  EXPECT_EQ("val1", val);
}

TEST(test, Update){
  auto b1 = client.PutOrUpdate("test1", "val2");
  auto b2 = client.PutOrUpdate("test1", "val3");
  EXPECT_EQ(true, b1);
  EXPECT_EQ(true, b2);
  auto val = client.GetValue("test1");
  EXPECT_EQ("val3", val);
}
TEST(test, Delete){
  auto b1 = client.Delete("test1");
  EXPECT_EQ(true, b1);
  auto val = client.Has("test1");
  EXPECT_EQ(false, val);
}
int main(int argc, char* argv[]){
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
