#include "../irs-engine/irs_engine.h"

#define EXPECT_IRS_TRUE(expression) if (expression == IRS_FALSE) g_error("Expect IRS_TRUE, but gotten IRS_FALSE")

#define TEST(name) void name()

TEST(irs_engine_init_shut)
{
  irs_engine * engine;
  EXPECT_IRS_TRUE(irs_engine_initialize(&engine, "langs/") == IRS_ENGINE_OK);
}

irs_int32 main()
{
  irs_engine_init_shut();
}
