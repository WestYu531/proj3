#include "CSVBusSystem.h"  // 包含被测试类CSVBusSystem的头文件
#include <gtest/gtest.h>   // 包含Google Test框架的头文件
#include <memory>          // 包含智能指针相关的头文件

// 为CDSVReader类创建一个模拟类
class MockDSVReader : public CDSVReader {
public:
    MockDSVReader() : CDSVReader(nullptr, ',') {}  // 使用逗号作为分隔符初始化模拟的CDSVReader对象

    // 模拟ReadRow方法以模拟读取CSV数据的行为
    bool ReadRow(std::vector<std::string>& row) override {
        static int callCount = 0;  // 用于跟踪ReadRow方法被调用的次数
        if (callCount == 0) {
            row = {"1", "Stop A", "100"};  // 第一次调用时返回的数据
            callCount++;
            return true;
        } else if (callCount == 1) {
            row = {"2", "Stop B", "200"};  // 第二次调用时返回的数据
            callCount++;
            return true;
        }
        return false;  // 在后续调用中表示没有更多数据
    }
};

// 为CSVBusSystem测试定义一个测试夹具
class CSVBusSystemTest : public ::testing::Test {
protected:
    std::shared_ptr<MockDSVReader> stopSrc;  // 用于停靠点数据的模拟DSVReader
    std::shared_ptr<MockDSVReader> routeSrc;  // 用于路线数据的模拟DSVReader
    std::unique_ptr<CCSVBusSystem> busSystem;  // 被测试的CSVBusSystem对象

    // 在每个测试用例开始前执行的设置代码
    void SetUp() override {
        stopSrc = std::make_shared<MockDSVReader>();  // 初始化模拟的停靠点数据源
        routeSrc = std::make_shared<MockDSVReader>();  // 初始化模拟的路线数据源
        busSystem = std::make_unique<CCSVBusSystem>(stopSrc, routeSrc);  // 创建CSVBusSystem实例
    }
};

// 测试CSVBusSystem的构造函数和基本数据加载功能
TEST_F(CSVBusSystemTest, LoadData) {
    EXPECT_EQ(busSystem->StopCount(), 2);  // 假设加载了2个停靠点，验证StopCount()的返回值
    // 根据需要添加更多的断言来验证加载的数据
}

// 测试通过索引查找停靠点的功能
TEST_F(CSVBusSystemTest, FindStopByIndex) {
    auto stop = busSystem->StopByIndex(0);  // 尝试通过索引0获取停靠点
    ASSERT_NE(stop, nullptr);  // 验证返回的停靠点不是nullptr
    EXPECT_EQ(stop->ID(), 1);  // 假设第一个停靠点的ID为1，验证ID是否匹配

    // 测试索引超出范围的情况
    EXPECT_EQ(busSystem->StopByIndex(10), nullptr);  // 使用超出范围的索引，期望返回nullptr
}

// 测试通过ID查找停靠点的功能
TEST_F(CSVBusSystemTest, FindStopByID) {
    auto stop = busSystem->StopByID(2);  // 尝试通过ID 2获取停靠点
    ASSERT_NE(stop, nullptr);  // 验证返回的停靠点不是nullptr
    EXPECT_EQ(stop->ID(), 2);  // 验证返回的停靠点ID是否为2

    // 测试查找不存在的ID
    EXPECT_EQ(busSystem->StopByID(99), nullptr);  // 使用不存在的ID，期望返回nullptr
}

// 根据需要添加更多的测试用例来覆盖其他功能和边界条件