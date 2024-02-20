#include "OpenStreetMap.h"
#include <unordered_map>
#include <XMLEntity.h>
#include <XMLReader.h>
#include <expat.h>
#include <iostream>
#include <unordered_map>
struct COpenStreetMap::SImplementation{

    struct SNode : public CStreetMap::SNode{
        ~SNode(){};
        TNodeID id;
        TLocation location;
        std::vector< std::string > Keys;
        std::unordered_map<std::string,std::string> Attributes;
        TNodeID ID() const noexcept{
            return id;
        }

        TLocation Location() const noexcept{
            return location;
        }

        std::size_t AttributeCount() const noexcept{
            return Attributes.size();
        }

        std::string GetAttributeKey(std::size_t index) const noexcept{
            return index < Keys.size() ? Keys[index] : std::string();
        }

        bool HasAttribute(const std::string &key) const noexcept{
            return Attributes.find(key) != Attributes.end();
            
        }

        std::string GetAttribute(const std::string &key) const noexcept{
             auto Search = Attributes.find(key);
            if(Search == Attributes.end()){
                return std::string();
            }
            return Search->second;
        }

    };

    struct SWay : public CStreetMap::SWay{
        std::vector< TNodeID > NodesOfTheWay;
        TWayID ThisWayID;
        std::unordered_map<std::string,std::string> Attributes;
        std::vector< std::string > Keys;

        ~SWay(){};
        TWayID ID() const noexcept{
            return ThisWayID;
        }

        std::size_t NodeCount() const noexcept{
            return NodesOfTheWay.size();
        }

        TNodeID GetNodeID(std::size_t index) const noexcept{
            if(index < NodesOfTheWay.size()){
                return NodesOfTheWay[index];
            }

            else if(index >= NodesOfTheWay.size()  || index < 0){
                
                return CStreetMap::InvalidNodeID;
            }
            return 0;
            
            
        }

        std::size_t AttributeCount() const noexcept{
            return Attributes.size();
        }

        std::string GetAttributeKey(std::size_t index) const noexcept{
            if(index < Keys.size()){
                return Keys[index];
            }
            else{
                return "";
            }
        }

        bool HasAttribute(const std::string &key) const noexcept{
            return Attributes.find(key) != Attributes.end();
        }

        std::string GetAttribute(const std::string &key) const noexcept{
            auto Search = Attributes.find(key);
            if(Search == Attributes.end()){
                return std::string();
            }
            return Search->second;
        }

    };

    std::vector< std::shared_ptr< SWay > > AllWays;
    std::vector< std::shared_ptr< SNode > > AllNodes;
    std::shared_ptr<SNode> NewNode;
    std::shared_ptr<SNode> WayNode;
    // std::pair<std::string, std::string> location;
    //std::unordered_map<TNodeID, std::pair> Mappedloc;
    std::unordered_map< TWayID, std::shared_ptr< SWay > > MappedWays;
    std::unordered_map< TNodeID, std::shared_ptr< SNode > > MappedNodes;
    double lat;
    double lon;

    SImplementation(std::shared_ptr<CXMLReader> src){
        SXMLEntity Entity;
        bool inway = false;
        auto NewWay = std::shared_ptr<SWay>();
       // auto NewNode = std::make_shared<SNode>();

        // check osm start first
        while(src->ReadEntity(Entity)){
            //std::cout<<Entity.DNameData<<std::endl;
            if(Entity.DNameData == "osm" && Entity.DType == SXMLEntity::EType::EndElement){
                break;
            }
            if(Entity.DNameData == "node" && Entity.DType == SXMLEntity::EType::StartElement){
                auto NewNode = std::make_shared<SNode>();
                if(Entity.AttributeExists("id")){
                    NewNode->id = stoi(Entity.AttributeValue("id"));
                    NewNode->Attributes.insert(std::make_pair<std::string,std::string>("id", Entity.AttributeValue("id")));
                    MappedNodes[NewNode->ID()] = NewNode;
                    NewNode->Keys.push_back("id");
                    
                }
                if(Entity.AttributeExists("lat")){
                    lat = stod(Entity.AttributeValue("lat"));
                    NewNode->Attributes.insert(std::make_pair<std::string,std::string>("lat", Entity.AttributeValue("lat")));
                    NewNode->Keys.push_back("lat");
                }
                if(Entity.AttributeExists("lon")){
                    lon = stod(Entity.AttributeValue("lon"));
                    NewNode->Attributes.insert(std::make_pair<std::string,std::string>("lon", Entity.AttributeValue("lon")));
                    NewNode->Keys.push_back("lon");
                }
                if(lat && lon){
                    NewNode->location = std::make_pair(lat, lon);
                }
                AllNodes.push_back(NewNode);
             
            
                //std::cout<<NewNode->ID();
                
               // Mappedloc[NewNode->location()] = NewNode;
               //break;
                //create nother one of these for location?
                //<SNode, SNode, SNode>
            }
            if(Entity.DNameData == "way" || Entity.DNameData == "nd"){
                // Parse way
                if(Entity.DNameData == "way" && Entity.DType == SXMLEntity::EType::StartElement){
                    // std::cout<<"made a newway"<<std::endl;
                    NewWay = std::make_shared<SWay>();
                    // std::cout<<"Way Node addresss is: "<<NewWay<<std::endl;


                }
                // std::cout<<"Dnamedata is: "<<Entity.DNameData<<std::endl;
                if(Entity.AttributeExists("id")){
                    NewWay->ThisWayID = stol(Entity.AttributeValue("id"));
                    NewWay->Attributes.insert(std::make_pair<std::string,std::string>("id", Entity.AttributeValue("id")));
                    // std::cout<<"id tag for mappedways:"<<NewWay->ID()<<std::endl;
                    // std::cout<<"Way Node addresss is: "<<NewWay<<std::endl;
                    MappedWays.insert({NewWay->ID(), NewWay});
                     //std::cout<<"size:"<<MappedWays.at(NewWay->ID())<<std::endl;
                    NewWay->Keys.push_back("id");
                }
                if(Entity.DNameData == "nd" && Entity.AttributeExists("ref")){
                    
                    // AllNodes[0]->id = stoi(Entity.AttributeValue("ref"));
                    NewWay->NodesOfTheWay.push_back(stol(Entity.AttributeValue("ref")));
                    // NewWay->Keys.push_back("ref");
                }
                if(Entity.DNameData == "way" && Entity.DType == SXMLEntity::EType::EndElement){
                    AllWays.push_back(NewWay);  
                   //MappedWays[NewWay->ID()] = NewWay;
                }

            }

        }
        

    }

    std::size_t NodeCount() const noexcept{
        return AllNodes.size();
    }

    std::size_t WayCount() const noexcept{
       return AllWays.size();
    }

    std::shared_ptr<CStreetMap::SNode> NodeByIndex(std::size_t index) const noexcept{
       if(index >= AllNodes.size() || index < 0){
            return nullptr;
       }
        return AllNodes[index];
    }

    std::shared_ptr<CStreetMap::SNode> NodeByID(TNodeID id) const noexcept{
        //working
        try {
            MappedNodes.at(id);      // vector::at throws an out-of-range
        }
        catch (const std::out_of_range& mapnode) {
             return nullptr;
        }
        
        return MappedNodes.at(id);
        }


    std::shared_ptr<CStreetMap::SWay> WayByIndex(std::size_t index) const noexcept{
        if(index >= AllWays.size() || index < 0){
            return nullptr;
       }
        return AllWays[index];
    }

    std::shared_ptr<CStreetMap::SWay> WayByID(TWayID id) const noexcept{
       
        std::cout<<"MappedWays is size: "<<MappedWays.size()<<std::endl;
        std::cout<<"Mappedways[0] = "<<MappedWays[0]<<std::endl;
        if(MappedWays.find(8699537) == MappedWays.end())
            std::cout<< "Element Not Present"<<std::endl;
        else{
            std::cout<< "Element is present"<<std::endl;
        }

        try {
            std::cout<<"TRYING"<<std::endl;
            std::cout<<"ID is: "<<id<<std::endl;
            MappedWays.at(id); 
            std::cout<<"NODE is: "<<MappedWays.at(id)<<std::endl;
            vector::at throws an out-of-range
        }
        catch (const std::out_of_range& mapnode) {
            std::cout<<"OUT OF RANGE"<<std::endl;;
            return nullptr;
        }
    
        return MappedWays.at(id);
    }
};

COpenStreetMap::COpenStreetMap(std::shared_ptr<CXMLReader> src){
    DImplementation = std::make_unique<SImplementation>(src);
}

COpenStreetMap::~COpenStreetMap(){

}

std::size_t COpenStreetMap::NodeCount() const noexcept{
    return DImplementation->NodeCount();
}

std::size_t COpenStreetMap::WayCount() const noexcept{
    return DImplementation->WayCount();
}

std::shared_ptr<CStreetMap::SNode> COpenStreetMap::NodeByIndex(std::size_t index) const noexcept{
    return DImplementation->NodeByIndex(index);
}

std::shared_ptr<CStreetMap::SNode> COpenStreetMap::NodeByID(TNodeID id) const noexcept{
    return DImplementation->NodeByID(id);
}

std::shared_ptr<CStreetMap::SWay> COpenStreetMap::WayByIndex(std::size_t index) const noexcept{
    return DImplementation->WayByIndex(index);
}

std::shared_ptr<CStreetMap::SWay> COpenStreetMap::WayByID(TWayID id) const noexcept{
    return DImplementation->WayByID(id);
}