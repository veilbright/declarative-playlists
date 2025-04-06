#include "../src/crypto/crypto_util.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include <cstdint>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

TEST(GenPkceVerifierTest, HandlesWrongLength) {
    EXPECT_EQ(CryptoUtil::GenPkceVerifier(-1), "");
    EXPECT_EQ(CryptoUtil::GenPkceVerifier(0), "");
    EXPECT_EQ(CryptoUtil::GenPkceVerifier(42), "");
    EXPECT_EQ(CryptoUtil::GenPkceVerifier(129), "");
    EXPECT_EQ(CryptoUtil::GenPkceVerifier(SIZE_MAX), "");
}

TEST(Sha256Test, HandlesEmptyString) {
    EXPECT_THAT(CryptoUtil::Sha256(""),
                testing::ElementsAre(227, 176, 196, 66, 152, 252, 28, 20, 154, 251, 244, 200, 153, 111, 185, 36, 39,
                                     174, 65, 228, 100, 155, 147, 76, 164, 149, 153, 27, 120, 82, 184, 85));
}

TEST(Sha256Test, HandlesString) {
    EXPECT_THAT(CryptoUtil::Sha256("example"),
                testing::ElementsAre(80, 216, 88, 224, 152, 94, 204, 127, 96, 65, 138, 175, 12, 197, 171, 88, 127, 66,
                                     194, 87, 10, 136, 64, 149, 169, 232, 204, 172, 208, 246, 84, 92));
    EXPECT_THAT(CryptoUtil::Sha256("ABCDEFGHIJKLMNOPQRSTUVWXYZ"),
                testing::ElementsAre(214, 236, 104, 152, 222, 135, 221, 172, 110, 91, 54, 17, 112, 138, 122, 161, 194,
                                     210, 152, 41, 51, 73, 204, 26, 108, 41, 154, 29, 183, 20, 157, 56));
}

TEST(Base64EncodeTest, HandlesEmptyVector) {
    EXPECT_THAT(CryptoUtil::Base64Encode({}), testing::IsEmpty());
}

TEST(Base64EncodeTest, HandlesSmallVector) {
    EXPECT_THAT(CryptoUtil::Base64Encode({0}), testing::ElementsAre(65, 65, 61, 61));
    EXPECT_THAT(CryptoUtil::Base64Encode({48}), testing::ElementsAre(77, 65, 61, 61));
    EXPECT_THAT(CryptoUtil::Base64Encode({65}), testing::ElementsAre(81, 81, 61, 61));
    EXPECT_THAT(CryptoUtil::Base64Encode({97}), testing::ElementsAre(89, 81, 61, 61));
    EXPECT_THAT(CryptoUtil::Base64Encode({255}), testing::ElementsAre(47, 119, 61, 61));
    EXPECT_THAT(CryptoUtil::Base64Encode({0, 0}), testing::ElementsAre(65, 65, 65, 61));
    EXPECT_THAT(CryptoUtil::Base64Encode({0, 0, 0}), testing::ElementsAre(65, 65, 65, 65));
    EXPECT_THAT(CryptoUtil::Base64Encode({0, 0, 0, 0}), testing::ElementsAre(65, 65, 65, 65, 65, 65, 61, 61));
    EXPECT_THAT(CryptoUtil::Base64Encode({48, 49}), testing::ElementsAre(77, 68, 69, 61));
    EXPECT_THAT(CryptoUtil::Base64Encode({48, 49, 50}), testing::ElementsAre(77, 68, 69, 121));
    EXPECT_THAT(CryptoUtil::Base64Encode({48, 49, 50, 51}), testing::ElementsAre(77, 68, 69, 121, 77, 119, 61, 61));
}

TEST(Base64EncodeTest, HandlesVector) {
    EXPECT_THAT(CryptoUtil::Base64Encode({49, 50, 51, 52, 53, 54, 55, 56, 57, 48, 49, 50, 51, 52, 53, 54,
                                          55, 56, 57, 48, 49, 50, 51, 52, 53, 55, 56, 57, 48, 49, 50}),
                testing::ElementsAre(77, 84, 73, 122, 78, 68, 85, 50, 78, 122, 103, 53, 77, 68, 69, 121, 77, 122, 81,
                                     49, 78, 106, 99, 52, 79, 84, 65, 120, 77, 106, 77, 48, 78, 84, 99, 52, 79, 84, 65,
                                     120, 77, 103, 61, 61));
    EXPECT_THAT(CryptoUtil::Base64Encode(
                    {32,  107, 97,  115, 106, 100, 102, 107, 106, 97,  115, 100, 32,  102, 106, 97,  107, 115, 32,
                     100, 106, 102, 107, 97,  104, 115, 100, 102, 32,  107, 106, 97,  115, 107, 100, 106, 102, 32,
                     106, 97,  115, 101, 105, 32,  104, 102, 105, 97,  115, 100, 106, 102, 59,  32,  97,  115, 100,
                     102, 32,  10,  97,  32,  115, 100, 102, 107, 106, 97,  32,  115, 100, 106, 102, 105, 97,  106,
                     115, 101, 102, 32,  106, 97,  115, 100, 107, 32,  106, 99,  97,  115, 106, 32,  101, 105, 106}),
                testing::ElementsAre(73, 71, 116, 104, 99, 50, 112, 107, 90, 109, 116, 113, 89, 88, 78, 107, 73, 71, 90,
                                     113, 89, 87, 116, 122, 73, 71, 82, 113, 90, 109, 116, 104, 97, 72, 78, 107, 90,
                                     105, 66, 114, 97, 109, 70, 122, 97, 50, 82, 113, 90, 105, 66, 113, 89, 88, 78, 108,
                                     97, 83, 66, 111, 90, 109, 108, 104, 99, 50, 82, 113, 90, 106, 115, 103, 89, 88, 78,
                                     107, 90, 105, 65, 75, 89, 83, 66, 122, 90, 71, 90, 114, 97, 109, 69, 103, 99, 50,
                                     82, 113, 90, 109, 108, 104, 97, 110, 78, 108, 90, 105, 66, 113, 89, 88, 78, 107,
                                     97, 121, 66, 113, 89, 50, 70, 122, 97, 105, 66, 108, 97, 87, 111, 61));
}
