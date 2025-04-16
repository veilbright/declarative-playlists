#include "../../src/util/url.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include <gmock/gmock.h>
#include <gtest/gtest.h>

class UrlTest : public testing::Test {
  protected:
    Url blankUrl_ = Url("");
};

TEST_F(UrlTest, InitiallyEmpty) {
    EXPECT_EQ(blankUrl_.get_url_string(), "");
    EXPECT_THAT(blankUrl_.get_params(), testing::IsEmpty());
}

TEST(UrlConstructorTest, BaseUrlConstructorWorks) {
    EXPECT_EQ(Url("abc").get_url_string(), "abc");
    EXPECT_EQ(Url("kasjdf kljasodfh oihqHIOS HSFIjasfd jfjA KJda jpofjASJKDn csa").get_url_string(),
              "kasjdf kljasodfh oihqHIOS HSFIjasfd jfjA KJda jpofjASJKDn csa");
    EXPECT_EQ(Url("21908").get_url_string(), "21908");
}

TEST_F(UrlTest, SetParamsWorks) {
    EXPECT_THAT(blankUrl_.get_params(), testing::IsEmpty());

    std::map<std::string, std::string> singleParameter = {{"test", "value"}};
    blankUrl_.set_params(singleParameter);
    EXPECT_THAT(blankUrl_.get_params(), testing::ContainerEq(singleParameter));

    std::map<std::string, std::string> multiParameter = {
        {"more", "testing"}, {"ai298", "%^9123#"}, {"71^$Jaa", "asjf~~~"}};
    blankUrl_.set_params(multiParameter);
    EXPECT_THAT(blankUrl_.get_params(), testing::ContainerEq(multiParameter));
}

TEST_F(UrlTest, InsertParamWorks) {
    std::map<std::string, std::string> currentParameters;
    EXPECT_THAT(blankUrl_.get_params(), testing::IsEmpty());

    currentParameters["test"] = "value";
    blankUrl_.InsertParam("test", "value");
    EXPECT_THAT(blankUrl_.get_params(), testing::ContainerEq(currentParameters));

    currentParameters["6731500bcasH&*$$"] = "ABCDEFGHI#4";
    blankUrl_.InsertParam("6731500bcasH&*$$", "ABCDEFGHI#4");
    EXPECT_THAT(blankUrl_.get_params(), testing::ContainerEq(currentParameters));
}

TEST_F(UrlTest, GetUrlStringWorks) {
    EXPECT_EQ(blankUrl_.get_url_string(), "");

    Url url1 = Url("319hn");
    EXPECT_EQ(url1.get_url_string(), "319hn");

    Url url2 = Url("%^**$#$%^(((((((*)))))))");
    EXPECT_EQ(url2.get_url_string(), "%^**$#$%^(((((((*)))))))");
}

TEST_F(UrlTest, GetParamsStringWorks) {
    EXPECT_EQ(blankUrl_.get_params_string(), "");

    blankUrl_.InsertParam("test", "value");
    const std::string param1 = "test=value";
    std::string paramPattern = param1;
    EXPECT_EQ(blankUrl_.get_params_string(), paramPattern);

    blankUrl_.InsertParam("56720432", "6jasdf7912H");
    const std::string param2 = "56720432=6jasdf7912H";
    paramPattern += "|" + param2;
    EXPECT_THAT(blankUrl_.get_params_string(),
                testing::MatchesRegex("(" + paramPattern + ")&(" + paramPattern + ")")); // Verify pattern
    EXPECT_THAT(blankUrl_.to_string(), testing::HasSubstr(param1)); // Verify first parameter is included
    EXPECT_THAT(blankUrl_.to_string(), testing::HasSubstr(param2)); // Verify second parameter is included

    blankUrl_.InsertParam("%^(!@^$(@!#)&)", "^HJLASD()");
    const std::string param3 = "%25%5E%28%21%40%5E%24%28%40%21%23%29%26%29=%5EHJLASD%28%29";
    paramPattern += "|" + param3;
    EXPECT_THAT(blankUrl_.get_params_string(),
                testing::MatchesRegex("(" + paramPattern + ")&(" + paramPattern + ")&(" + paramPattern +
                                      ")"));                        // Verify pattern
    EXPECT_THAT(blankUrl_.to_string(), testing::HasSubstr(param1)); // Verify first parameter is included
    EXPECT_THAT(blankUrl_.to_string(), testing::HasSubstr(param2)); // Verify second parameter is included
    EXPECT_THAT(blankUrl_.to_string(), testing::HasSubstr(param3)); // Verify third parameter is included
}

TEST_F(UrlTest, ToStringWorks) {
    EXPECT_EQ(blankUrl_.to_string(), "");

    Url url = Url("test");
    std::string paramPattern = "";
    EXPECT_EQ(url.to_string(), "test");

    url.InsertParam("test", "value");
    paramPattern = "test=value";
    EXPECT_EQ(url.to_string(), "test?" + paramPattern);

    url.InsertParam("second", "value2");
    paramPattern += "|second=value2";
    EXPECT_THAT(url.to_string(),
                testing::MatchesRegex("test\\?(" + paramPattern + ")&(" + paramPattern + ")")); // Verify pattern
    EXPECT_THAT(url.to_string(), testing::HasSubstr("test=value"));    // Verify test parameter is included
    EXPECT_THAT(url.to_string(), testing::HasSubstr("second=value2")); // Verify second parameter is included

    url.InsertParam("#$#", ")*(^)");
    paramPattern += "|%23%24%23=%29%2A%28%5E%29";
    EXPECT_THAT(url.to_string(),
                testing::MatchesRegex("test\\?(" + paramPattern + ")&(" + paramPattern + ")&(" + paramPattern +
                                      ")"));                           // Verify pattern
    EXPECT_THAT(url.to_string(), testing::HasSubstr("test=value"));    // Verify test parameter is included
    EXPECT_THAT(url.to_string(), testing::HasSubstr("second=value2")); // Verify second parameter is included
    EXPECT_THAT(url.to_string(), testing::HasSubstr("%23%24%23=%29%2A%28%5E%29")); // Verify #?# parameter is included
}
