// Application Unit Tests
#include "gtest/gtest.h"
#include "XApplication.h"
#include "Logging.h"

using namespace cs349;

TEST(XApplicationTests, Construction) {
  XApplication* app = XApplication::GetInstance();
  EXPECT_TRUE(app != NULL);
  EXPECT_TRUE(app->GetEventQueue() != NULL);
}
