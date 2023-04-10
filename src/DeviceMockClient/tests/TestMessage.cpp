#include <gtest/gtest.h>
#include "Message.hpp"

using namespace DeviceMockClient;

TEST(TestPacketParser, TestValidPacket)
{
	Message msg(24U);

	msg.getHeader()->m_dataLenght = 8U;
	msg.getHeader()->m_validationID = ApplicationCode;

	EXPECT_TRUE(msg.isValid(24U));
}

TEST(TestPacketParser, TestInvalidPacket)
{
	Message msg1(24U);

	msg1.getHeader()->m_dataLenght = 8U;
	msg1.getHeader()->m_validationID = ApplicationCode;

	EXPECT_FALSE(msg1.isValid(23U));

	Message msg2(24U);

	msg2.getHeader()->m_dataLenght = (MaxBodySize + 1U);
	msg2.getHeader()->m_validationID = ApplicationCode;

	EXPECT_FALSE(msg2.isValid(24U));

	Message msg3(24U);

	msg2.getHeader()->m_dataLenght = 8U;
	msg2.getHeader()->m_validationID = 4321U;

	EXPECT_FALSE(msg3.isValid(24U));
}
