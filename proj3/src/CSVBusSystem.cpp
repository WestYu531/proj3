#include "CSVBusSystem.h"
#include <vector>
#include <memory>

// CCSVBusSystem的内部实现结构，隐藏具体实现细节
struct CCSVBusSystem::SImplementation {
    std::vector<std::shared_ptr<SStop>> Stops;
    // 假设Routes和其他必要的数据也在这里定义
};

// 构造函数
CCSVBusSystem::CCSVBusSystem(std::shared_ptr<CDSVReader> stopsrc, std::shared_ptr<CDSVReader> routesrc)
    : DImplementation(std::make_unique<SImplementation>()) {
    // 使用stopsrc和routesrc读取数据并初始化Stops和Routes
    // 这里的实现将依赖于CDSVReader的具体功能和数据格式
    // 例如，可以循环读取每一行，创建SStop对象，并添加到Stops向量中
}

// 析构函数
CCSVBusSystem::~CCSVBusSystem() {
    // 由于使用了智能指针，这里不需要显式释放资源
}

// 返回系统中的停靠点数量
std::size_t CCSVBusSystem::StopCount() const noexcept {
    return DImplementation->Stops.size();
}

// 返回系统中的路线数量
std::size_t CCSVBusSystem::RouteCount() const noexcept {
    // 这里需要返回Routes向量的大小，假设它在SImplementation中定义
    // return DImplementation->Routes.size();
}

// 根据索引返回指定的停靠点，如果索引超出范围，则返回nullptr
std::shared_ptr<CBusSystem::SStop> CCSVBusSystem::StopByIndex(std::size_t index) const noexcept {
    if (index < DImplementation->Stops.size()) {
        return DImplementation->Stops[index];
    } else {
        return nullptr; // 索引超出范围
    }
}
// Returns the SStop specified by the stop id, nullptr is returned if id is not in the stops
std::shared_ptr<CBusSystem::SStop> CCSVBusSystem::StopByID(TStopID id) const noexcept {
    // 遍历存储所有站点的容器，查找具有指定ID的站点
    for (const auto& stop : DImplementation->Stops) {
        if (stop->ID() == id) {
            return stop; // 如果找到，返回对应的智能指针
        }
    }
    return nullptr; // 如果未找到，返回nullptr
}

// Returns the SRoute specified by the index, nullptr is returned if index is greater than equal to RouteCount()
std::shared_ptr<CBusSystem::SRoute> CCSVBusSystem::RouteByIndex(std::size_t index) const noexcept {
    // 检查索引是否在有效范围内
    if (index < DImplementation->Routes.size()) {
        return DImplementation->Routes[index]; // 如果在范围内，返回对应索引的路线
    }
    return nullptr; // 如果索引无效，返回nullptr
}

// Returns the SRoute specified by the name, nullptr is returned if name is not in the routes
std::shared_ptr<CBusSystem::SRoute> CCSVBusSystem::RouteByName(const std::string &name) const noexcept {
    // 遍历存储所有路线的容器，查找具有指定名称的路线
    for (const auto& route : DImplementation->Routes) {
        if (route->Name() == name) {
            return route; // 如果找到，返回对应的智能指针
        }
    }
    return nullptr; // 如果未找到，返回nullptr
}
class CStopImpl : public CBusSystem::SStop {
private:
    TStopID StopID;
    CStreetMap::TNodeID NodeID;

public:
    CStopImpl(TStopID id, CStreetMap::TNodeID nodeID) : StopID(id), NodeID(nodeID) {}

    // 返回公交站点的ID
    TStopID ID() const noexcept override {
        return StopID;
    }

    // 返回公交站点对应的地图节点ID
    CStreetMap::TNodeID NodeID() const noexcept override {
        return NodeID;
    }
};

class CRouteImpl : public CBusSystem::SRoute {
private:
    std::string RouteName;
    std::vector<TStopID> StopIDs;

public:
    CRouteImpl(const std::string& name, const std::vector<TStopID>& stops) : RouteName(name), StopIDs(stops) {}

    // 返回路线的名称
    std::string Name() const noexcept override {
        return RouteName;
    }

    // 返回路线上的站点数量
    std::size_t StopCount() const noexcept override {
        return StopIDs.size();
    }

    // 根据索引返回站点ID，如果索引超出范围，则返回InvalidStopID
    TStopID GetStopID(std::size_t index) const noexcept override {
        if (index >= StopCount()) {
            return CBusSystem::InvalidStopID;
        }
        return StopIDs[index];
    }
};