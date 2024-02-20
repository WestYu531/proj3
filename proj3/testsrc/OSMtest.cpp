#include <gtest/gtest.h>
#include "DSVReader.h"
#include "DSVWriter.h"
#include "DataSink.h"
#include "XMLReader.h"
#include "XMLEntity.h"
#include "StreetMap.h"
#include "OpenStreetMap.h"
#include "StringDataSource.h"
#include <iostream>

TEST(OpenStreetMap, ComparePointers){
    
    //Nodes
    auto CDataSource = std::make_shared<CStringDataSource>("<?xml version='1.0' encoding='UTF-8'?>"
                                                        "<osm version=\"0.6\" generator=\"osmconvert 0.8.5\">"
                                                        "<node id=\"62208369\" lat=\"38.5178523\" lon=\"-121.7712408\"/>"
	                                                    "<node id=\"62209104\" lat=\"38.535052\" lon=\"-121.7408606z\"/>"
	                                                    "<node id=\"62224286\" lat=\"38.5302841\" lon=\"-121.7689756\"/>"
	                                                    "<node id=\"62224288\" lat=\"38.5288995\" lon=\"-121.7691145\"/>"
	                                                    "<node id=\"62224290\" lat=\"38.5284834\" lon=\"-121.7691556\"/>"
                                                        "</osm>");
    auto Readr = std::make_shared<CXMLReader>(CDataSource);
    COpenStreetMap osm(Readr);
    EXPECT_EQ(osm.NodeByID(62208369), osm.NodeByIndex(0));

    //Ways

    auto CDataSource2 = std::make_shared<CStringDataSource>("<osm version=\"0.6\" generator=\"osmconvert 0.8.5\">"
                                                            "<way id=\"8699536\">"
		                                                    "<nd ref=\"258592863\"/>"
		                                                    "<nd ref=\"4399281377\"/>"
		                                                    "<nd ref=\"62224286\"/>"
		                                                    "<nd ref=\"62224288\"/>"
		                                                    "<nd ref=\"62224290\"/>"
                                                        "</way>"
                                                          "</osm>");
    
    auto Readr2 = std::make_shared<CXMLReader>(CDataSource2);
    COpenStreetMap osm2(Readr2);
    EXPECT_EQ(osm.WayByID(8699536), osm.WayByIndex(0));

}
TEST(OpenStreetMap, NodeCount_Node){
    
    // //checks where there is only one node
    auto CDataSource = std::make_shared<CStringDataSource>("<?xml version='1.0' encoding='UTF-8'?>"
                                                        "<osm version=\"0.6\" generator=\"osmconvert 0.8.5\">"
                                                        "<node id=\"1\" lat=\"1.0\" lon=\"-1.0\"/>"
                                                        "</osm>");
    auto Readr = std::make_shared<CXMLReader>(CDataSource);
    COpenStreetMap osm(Readr);
    EXPECT_EQ(1, osm.NodeCount());

    //checks where there are no nodes
    auto CDataSource2 = std::make_shared<CStringDataSource>("<?xml version='1.0' encoding='UTF-8'?>"
                                                        "<osm version=\"0.6\" generator=\"osmconvert 0.8.5\">"
                                                        "</osm>");
    auto Readr2 = std::make_shared<CXMLReader>(CDataSource2);
    COpenStreetMap osm2(Readr2);
    EXPECT_EQ(0, osm2.NodeCount());
   
    //checks where there are more than 1 node
    auto CDataSource3 = std::make_shared<CStringDataSource>("<?xml version='1.0' encoding='UTF-8'?>"
                                                        "<osm version=\"0.6\" generator=\"osmconvert 0.8.5\">"
                                                        "<node id=\"62208369\" lat=\"38.5178523\" lon=\"-121.7712408\"/>"
	                                                    "<node id=\"62209104\" lat=\"38.535052\" lon=\"-121.7408606z\"/>"
	                                                    "<node id=\"62224286\" lat=\"38.5302841\" lon=\"-121.7689756\"/>"
	                                                    "<node id=\"62224288\" lat=\"38.5288995\" lon=\"-121.7691145\"/>"
	                                                    "<node id=\"62224290\" lat=\"38.5284834\" lon=\"-121.7691556\"/>"
                                                        "</osm>");
    auto Readr3 = std::make_shared<CXMLReader>(CDataSource3);
    COpenStreetMap osm3(Readr3);
    EXPECT_EQ(5, osm3.NodeCount());
};

TEST(OpenStreetMap, WayCount){
      auto CDataSource = std::make_shared<CStringDataSource>("<?xml version='1.0' encoding='UTF-8'?>"
                                                           "<osm version=\"0.6\" generator=\"osmconvert 0.8.5\">"
                                                        "<node id=\"62208369\" lat=\"38.5178523\" lon=\"-121.7712408\"/>"
	                                                    "<node id=\"62209104\" lat=\"38.535052\" lon=\"-121.7408606z\"/>"
	                                                    "<node id=\"62224286\" lat=\"38.5302841\" lon=\"-121.7689756\"/>"
	                                                    "<node id=\"62224288\" lat=\"38.5288995\" lon=\"-121.7691145\"/>"
	                                                    "<node id=\"62224290\" lat=\"38.5284834\" lon=\"-121.7691556\"/>"
                                                        "</osm>");
    auto Readr = std::make_shared<CXMLReader>(CDataSource);
    COpenStreetMap osm(Readr);
   
    //where the way count is 0
    EXPECT_EQ(osm.WayCount(),0);

    //where the way count is 1
    auto CDataSource2 = std::make_shared<CStringDataSource>("<osm version=\"0.6\" generator=\"osmconvert 0.8.5\">"
                                                            "<way id=\"8699536\">"
		                                                    "<nd ref=\"258592863\"/>"
		                                                    "<nd ref=\"4399281377\"/>"
		                                                    "<nd ref=\"62224286\"/>"
		                                                    "<nd ref=\"62224288\"/>"
		                                                    "<nd ref=\"62224290\"/>"
                                                        "</way>"
                                                          "</osm>");
    
    auto Readr2 = std::make_shared<CXMLReader>(CDataSource2);
    COpenStreetMap osm2(Readr2);
     EXPECT_EQ(osm2.WayCount(),1);

    //where the way count is more than 1
    auto CDataSource3 = std::make_shared<CStringDataSource>("<osm version=\"0.6\" generator=\"osmconvert 0.8.5\">"
                                                            "<way id=\"8699536\">"
		                                                    "<nd ref=\"258592863\"/>"
		                                                    "<nd ref=\"4399281377\"/>"
		                                                    "<nd ref=\"62224286\"/>"
		                                                    "<nd ref=\"62224288\"/>"
		                                                    "<nd ref=\"62224290\"/>"
                                                        "</way>"
                                                        "<way id=\"8699536\">"
		                                                    "<nd ref=\"258592864\"/>"
		                                                    "<nd ref=\"4399281378\"/>"
		                                                    "<nd ref=\"62224289\"/>"
		                                                    "<nd ref=\"62224280\"/>"
		                                                    "<nd ref=\"62224291\"/>"
                                                        "</way>"
                                                        "</osm>");
    auto Readr3 = std::make_shared<CXMLReader>(CDataSource3);
    COpenStreetMap osm3(Readr3);
    EXPECT_EQ(osm3.WayCount(),2);

}
TEST(OpenStreetMap, NodeByIndex){

//when the index is the first node
    auto CDataSource = std::make_shared<CStringDataSource>("<?xml version='1.0' encoding='UTF-8'?>"
                                                        "<osm version=\"0.6\" generator=\"osmconvert 0.8.5\">"
                                                        "<node id=\"5\" lat=\"5.0\" lon=\"-5.0\"/>"
                                                        "<node id=\"2\" lat=\"2.0\" lon=\"-2.0\"/>"
                                                        "<node id=\"3\" lat=\"3.0\" lon=\"-3.0\"/>"
                                                        "</osm>"); 
    auto Readr = std::make_shared<CXMLReader>(CDataSource);
    COpenStreetMap osm(Readr);
EXPECT_EQ(osm.NodeByIndex(0)->ID(),5);

// //when the index is greater than the number of nodes
    auto CDataSource2 = std::make_shared<CStringDataSource>("<?xml version='1.0' encoding='UTF-8'?>"
                                                        "<osm version=\"0.6\" generator=\"osmconvert 0.8.5\">"
                                                        "<node id=\"5\" lat=\"5.0\" lon=\"-5.0\"/>"
                                                        "<node id=\"2\" lat=\"2.0\" lon=\"-2.0\"/>"
                                                        "<node id=\"3\" lat=\"3.0\" lon=\"-3.0\"/>"
                                                        "</osm>");
    auto Readr2 = std::make_shared<CXMLReader>(CDataSource2);
    COpenStreetMap osm2(Readr2);
EXPECT_EQ(nullptr, osm2.NodeByIndex(5));

//when the index is a negative number
   auto CDataSource3 = std::make_shared<CStringDataSource>("<?xml version='1.0' encoding='UTF-8'?>"
                                                        "<osm version=\"0.6\" generator=\"osmconvert 0.8.5\">"
                                                        "<node id=\"5\" lat=\"5.0\" lon=\"-5.0\"/>"
                                                        "<node id=\"2\" lat=\"2.0\" lon=\"-2.0\"/>"
                                                        "<node id=\"3\" lat=\"3.0\" lon=\"-3.0\"/>"
                                                        "</osm>");
    auto Readr3 = std::make_shared<CXMLReader>(CDataSource3);
    COpenStreetMap osm3(Readr3);
    EXPECT_EQ(nullptr, osm3.NodeByIndex(-2));

// //when the index is equal to the number of nodes
auto CDataSource4 = std::make_shared<CStringDataSource>("<?xml version='1.0' encoding='UTF-8'?>"
                                                        "<osm version=\"0.6\" generator=\"osmconvert 0.8.5\">"
                                                        "<node id=\"5\" lat=\"5.0\" lon=\"-5.0\"/>"
                                                        "<node id=\"2\" lat=\"2.0\" lon=\"-2.0\"/>"
                                                        "<node id=\"3\" lat=\"3.0\" lon=\"-3.0\"/>"
                                                        "</osm>");
    auto Readr4 = std::make_shared<CXMLReader>(CDataSource4);
    COpenStreetMap osm4(Readr4);
    EXPECT_EQ(3, osm4.NodeByIndex(2)->ID());

    auto CDataSource5 = std::make_shared<CStringDataSource>("<?xml version='1.0' encoding='UTF-8'?>"
                                                        "<osm version=\"0.6\" generator=\"osmconvert 0.8.5\">"
                                                        "<node id=\"5\" lat=\"5.0\" lon=\"-5.0\"/>"
                                                        "<node id=\"2\" lat=\"2.0\" lon=\"-2.0\"/>"
                                                        "<node id=\"3\" lat=\"3.0\" lon=\"-3.0\"/>"
                                                        "</osm>");
    auto Readr5 = std::make_shared<CXMLReader>(CDataSource5);
    COpenStreetMap osm5(Readr5);
    EXPECT_EQ(nullptr, osm5.NodeByIndex(3));
    std::pair<double,double> location;
    location.first = 5.0;
    location.second = -5.0;
    EXPECT_EQ(location, osm5.NodeByIndex(0)->Location());
    EXPECT_EQ(3, osm5.NodeByIndex(2)->ID());

}
TEST(OpenStreetMap, NodeByID){
//when the id is in the node
    auto CDataSource = std::make_shared<CStringDataSource>("<?xml version='1.0' encoding='UTF-8'?>"
                                                        "<osm version=\"0.6\" generator=\"osmconvert 0.8.5\">"
                                                        "<node id=\"5\" lat=\"5.0\" lon=\"-5.0\"/>"
                                                        "<node id=\"2\" lat=\"2.0\" lon=\"-2.0\"/>"
                                                        "<node id=\"3\" lat=\"3.0\" lon=\"-3.0\"/>"
                                                        "</osm>");
    auto Readr = std::make_shared<CXMLReader>(CDataSource);
    COpenStreetMap osm(Readr);
    std::pair <double,double> pair(5.0,-5.0);
    std::pair <double,double> pair2(3.0,-3.0);
    EXPECT_EQ(osm.NodeByID(5)->Location(),(pair));
    EXPECT_EQ(osm.NodeByID(3)->Location(),(pair2));


    auto CDataSource2 = std::make_shared<CStringDataSource>("<?xml version='1.0' encoding='UTF-8'?>"
                                                        "<osm version=\"0.6\" generator=\"osmconvert 0.8.5\">"
                                                        "<node id=\"5\" lat=\"5.0\" lon=\"-5.0\"/>"
                                                        "<node id=\"2\" lat=\"2.0\" lon=\"-2.0\"/>"
                                                        "<node id=\"3\" lat=\"3.0\" lon=\"-3.0\"/>"
                                                        "</osm>");
    auto Readr2 = std::make_shared<CXMLReader>(CDataSource2);
    COpenStreetMap osm2(Readr2);
EXPECT_EQ(nullptr, osm2.NodeByID(8));

//when the id is a negative number
   auto CDataSource3 = std::make_shared<CStringDataSource>("<?xml version='1.0' encoding='UTF-8'?>"
                                                        "<osm version=\"0.6\" generator=\"osmconvert 0.8.5\">"
                                                        "<node id=\"5\" lat=\"5.0\" lon=\"-5.0\"/>"
                                                        "<node id=\"2\" lat=\"2.0\" lon=\"-2.0\"/>"
                                                        "<node id=\"3\" lat=\"3.0\" lon=\"-3.0\"/>"
                                                        "</osm>");
    auto Readr3 = std::make_shared<CXMLReader>(CDataSource3);
    COpenStreetMap osm3(Readr3);
    EXPECT_EQ(nullptr, osm3.NodeByID(-2));
    EXPECT_EQ(nullptr, osm3.NodeByID(0));
}

TEST(OpenStreetMap, WayByIndex){
    auto CDataSource = std::make_shared<CStringDataSource>( "<osm version=\"0.6\" generator=\"osmconvert 0.8.5\">"
                                                            "<way id=\"8699536\">"
		                                                    "<nd ref=\"258592863\"/>"
		                                                    "<nd ref=\"4399281377\"/>"
		                                                    "<nd ref=\"62224286\"/>"
		                                                    "<nd ref=\"62224288\"/>"
		                                                    "<nd ref=\"62224290\"/>"
                                                        "</way>"
                                                        "<way id=\"8699536\">"
		                                                    "<nd ref=\"258592864\"/>"
		                                                    "<nd ref=\"4399281378\"/>"
		                                                    "<nd ref=\"62224289\"/>"
		                                                    "<nd ref=\"62224280\"/>"
		                                                    "<nd ref=\"62224291\"/>"
                                                        "</way>"
                                                              "</osm>");
    
    auto Readr = std::make_shared<CXMLReader>(CDataSource);
    COpenStreetMap osm(Readr);
    EXPECT_EQ(osm.WayByIndex(0)->ID(),8699536); //index is 0
    EXPECT_EQ(nullptr, osm.WayByIndex(-2)); // index is a negative number
    EXPECT_EQ(nullptr, osm.WayByIndex(2)); // index is larger than the amount of ways
    EXPECT_EQ(osm.WayByIndex(1)->ID(),8699536); 
}
TEST(OpenStreetMap, WayByID){
    
    auto CDataSource2 = std::make_shared<CStringDataSource>("<osm version=\"0.6\" generator=\"osmconvert 0.8.5\">"
                                                            "<way id=\"711\">"
		                                                    "<nd ref=\"258592863\"/>"
		                                                    "<nd ref=\"4399281377\"/>"
		                                                    "<nd ref=\"62224286\"/>"
		                                                    "<nd ref=\"62224288\"/>"
		                                                    "<nd ref=\"62224290\"/>"
                                                        "</way>"
                                                        "<way id=\"99999\">"
		                                                    "<nd ref=\"258592864\"/>"
		                                                    "<nd ref=\"4399281378\"/>"
		                                                    "<nd ref=\"62224289\"/>"
		                                                    "<nd ref=\"62224280\"/>"
		                                                    "<nd ref=\"62224291\"/>"
                                                        "</way>"
                                                              "</osm>");
    auto Readr2 = std::make_shared<CXMLReader>(CDataSource2);
    COpenStreetMap osm2(Readr2);
    //auto pointertest = make_shared<Sway>();
    
    //ID is not a wayid
    EXPECT_EQ(nullptr, osm2.WayByID(2));
    EXPECT_EQ(nullptr, osm2.WayByID(-2));
    EXPECT_EQ(nullptr, osm2.WayByID(5));

    EXPECT_EQ(osm2.WayByID(99999)->ID(),99999);
    EXPECT_EQ(osm2.WayByID(711)->ID(),711);
    //EXPECT_FALSE(osm2.WayByID(711), pointertest);
  
}
TEST(OpenStreetMap, ID){
//not a lot of tests since ID gets tested so much everywhere else
      /////node

    auto CDataSource = std::make_shared<CStringDataSource>("<?xml version='1.0' encoding='UTF-8'?>"
                                                        "<osm version=\"0.6\" generator=\"osmconvert 0.8.5\">"
                                                        "<node id=\"5\" lat=\"5.0\" lon=\"-5.0\"/>"
                                                        "<node id=\"2\" lat=\"2.0\" lon=\"-2.0\"/>"
                                                        "<node id=\"3\" lat=\"3.0\" lon=\"-3.0\"/>"
                                                        "</osm>"); 
    auto Readr = std::make_shared<CXMLReader>(CDataSource);
    COpenStreetMap osm(Readr);
    EXPECT_EQ(osm.NodeByIndex(0)->ID(),5);

     /////ways

    auto CDataSource2 = std::make_shared<CStringDataSource>("<osm version=\"0.6\" generator=\"osmconvert 0.8.5\">"
                                                            "<way id=\"8699536\">"
		                                                    "<nd ref=\"258592863\"/>"
		                                                    "<nd ref=\"4399281377\"/>"
		                                                    "<nd ref=\"62224286\"/>"
		                                                    "<nd ref=\"62224288\"/>"
		                                                    "<nd ref=\"62224290\"/>"
                                                        "</way>"
                                                        "<way id=\"8699536\">"
		                                                    "<nd ref=\"258592864\"/>"
		                                                    "<nd ref=\"4399281378\"/>"
		                                                    "<nd ref=\"62224289\"/>"
		                                                    "<nd ref=\"62224280\"/>"
		                                                    "<nd ref=\"62224291\"/>"
                                                        "</way>"
                                                              "</osm>");
    
    auto Readr2 = std::make_shared<CXMLReader>(CDataSource2);
    COpenStreetMap osm2(Readr2);
    EXPECT_EQ(osm2.WayByIndex(0)->ID(),8699536);


}
TEST(OpenStreetMap, GetNodeID){
    auto CDataSource3 = std::make_shared<CStringDataSource>("<osm version=\"0.6\" generator=\"osmconvert 0.8.5\">"
                                                            "<way id=\"8699536\">"
		                                                    "<nd ref=\"258592863\"/>"
		                                                    "<nd ref=\"4399281377\"/>"
		                                                    "<nd ref=\"62224286\"/>"
		                                                    "<nd ref=\"62224288\"/>"
		                                                    "<nd ref=\"62224290\"/>"
                                                        "</way>"
                                                        "<way id=\"8699536\">"
		                                                    "<nd ref=\"258592864\"/>"
		                                                    "<nd ref=\"4399281378\"/>"
		                                                    "<nd ref=\"62224289\"/>"
		                                                    "<nd ref=\"62224280\"/>"
		                                                    "<nd ref=\"62224291\"/>"
                                                        "</way>"
                                                              "</osm>");
    auto Readr3 = std::make_shared<CXMLReader>(CDataSource3);
    COpenStreetMap osm3(Readr3);
    EXPECT_EQ(osm3.WayByIndex(0)->GetNodeID(7), int(CStreetMap::InvalidNodeID));
    EXPECT_EQ(osm3.WayByIndex(0)->GetNodeID(5), int(CStreetMap::InvalidNodeID));
    EXPECT_EQ(osm3.WayByIndex(0)->GetNodeID(-2), int(CStreetMap::InvalidNodeID));

    EXPECT_EQ(osm3.WayByIndex(0)->GetNodeID(0), 258592863);
    EXPECT_EQ(osm3.WayByIndex(0)->GetNodeID(1), 4399281377);
    EXPECT_EQ(osm3.WayByIndex(0)->GetNodeID(2), 62224286);
    EXPECT_EQ(osm3.WayByIndex(0)->GetNodeID(3), 62224288);
    EXPECT_EQ(osm3.WayByIndex(0)->GetNodeID(4), 62224290);
        //std::cout<<osm3.WayByIndex(0)->NodeCount();
   EXPECT_EQ(osm3.WayByIndex(1)->GetNodeID(0), 258592864);
   EXPECT_EQ(osm3.WayByIndex(1)->GetNodeID(1), 4399281378);
   EXPECT_EQ(osm3.WayByIndex(1)->GetNodeID(3), 62224280);
   EXPECT_EQ(osm3.WayByIndex(1)->GetNodeID(4), 62224291);
}

TEST(OpenStreetMap, NodeCount_Way){
auto CDataSource3 = std::make_shared<CStringDataSource>("<osm version=\"0.6\" generator=\"osmconvert 0.8.5\">"
                                                            "<way id=\"8699536\">"
		                                                    "<nd ref=\"258592863\"/>"
		                                                    "<nd ref=\"4399281377\"/>"
		                                                    "<nd ref=\"62224286\"/>"
		                                                    "<nd ref=\"62224288\"/>"
		                                                    "<nd ref=\"62224290\"/>"
                                                        "</way>"
                                                        "<way id=\"8699536\">"
		                                                    "<nd ref=\"258592864\"/>"
		                                                    "<nd ref=\"4399281378\"/>"
		                                                    "<nd ref=\"62224289\"/>"
		                                                    "<nd ref=\"62224280\"/>"
		                                                    "<nd ref=\"62224291\"/>"
                                                        "</way>"
                                                              "</osm>");
    auto Readr3 = std::make_shared<CXMLReader>(CDataSource3);
    COpenStreetMap osm3(Readr3);
    EXPECT_EQ(osm3.WayByIndex(0)->NodeCount(), 5);
    EXPECT_EQ(osm3.WayByIndex(1)->NodeCount(), 5);
}

TEST(OpenStreetMap, Location){

  /////node

 auto CDataSource = std::make_shared<CStringDataSource>("<?xml version='1.0' encoding='UTF-8'?>"
                                                        "<osm version=\"0.6\" generator=\"osmconvert 0.8.5\">"
                                                        "<node id=\"5\" lat=\"5.0\" lon=\"-5.0\"/>"
                                                        "<node id=\"2\" lat=\"2.0\" lon=\"-2.0\"/>"
                                                        "<node id=\"3\" lat=\"3.0\" lon=\"-3.0\"/>"
                                                        "</osm>");
    auto Readr = std::make_shared<CXMLReader>(CDataSource);
    COpenStreetMap osm(Readr);
    std::pair <double,double> pair(5.0,-5.0);
    std::pair <double,double> pair2(2.0,-2.0);
    EXPECT_EQ(osm.NodeByIndex(0)->Location(),(pair));
    EXPECT_EQ(osm.NodeByIndex(1)->Location(),(pair2));
}   
TEST(OpenStreetMap, AttributeCount){

     /////node

    auto CDataSource = std::make_shared<CStringDataSource>("<?xml version='1.0' encoding='UTF-8'?>"
                                                        "<osm version=\"0.6\" generator=\"osmconvert 0.8.5\">"
                                                        "<node id=\"5\" lat=\"5.0\" lon=\"-5.0\"/>"
                                                        "<node id=\"2\" lat=\"2.0\" lon=\"-2.0\"/>"
                                                        "<node id=\"3\" lon=\"-3.0\"/>"
                                                        "</osm>");
    auto Readr = std::make_shared<CXMLReader>(CDataSource);
    COpenStreetMap osm(Readr);
    EXPECT_EQ(osm.NodeByIndex(0)->AttributeCount(), 3);
    EXPECT_EQ(osm.NodeByIndex(1)->AttributeCount(), 3);
    EXPECT_EQ(osm.NodeByIndex(2)->AttributeCount(), 2);


    /////ways

    auto CDataSource2 = std::make_shared<CStringDataSource>("<osm version=\"0.6\" generator=\"osmconvert 0.8.5\">"
                                                            "<way id=\"8699536\">"
		                                                    "<nd ref=\"258592863\"/>"
		                                                    "<nd ref=\"4399281377\"/>"
		                                                    "<nd ref=\"62224286\"/>"
		                                                    "<nd ref=\"62224288\"/>"
		                                                    "<nd ref=\"62224290\"/>"
                                                        "</way>"
                                                        "<way id=\"8699536\">"
		                                                    "<nd ref=\"258592864\"/>"
		                                                    "<nd ref=\"4399281378\"/>"
		                                                    "<nd ref=\"62224289\"/>"
		                                                    "<nd ref=\"62224280\"/>"
		                                                    "<nd ref=\"62224291\"/>"
                                                        "</way>"
                                                              "</osm>");
    auto Readr2 = std::make_shared<CXMLReader>(CDataSource2);
    COpenStreetMap osm2(Readr2);

    EXPECT_EQ(osm2.WayByIndex(0)->AttributeCount(), 1);
    EXPECT_EQ(osm2.WayByIndex(1)->AttributeCount(), 1);


}
TEST(OpenStreetMap, GetAttributeKey){
    /////node




auto CDataSource = std::make_shared<CStringDataSource>("<?xml version='1.0' encoding='UTF-8'?>"
                                                        "<osm version=\"0.6\" generator=\"osmconvert 0.8.5\">"
                                                        "<node id=\"5\" lat=\"5.0\" lon=\"-5.0\"/>"
                                                        "<node id=\"2\" lat=\"2.0\" lon=\"-2.0\"/>"
                                                        "<node id=\"3\" lat=\"3.0\" lon=\"-3.0\"/>"
                                                        "</osm>");
    auto Readr = std::make_shared<CXMLReader>(CDataSource);
    COpenStreetMap osm(Readr);
    EXPECT_EQ(osm.NodeByIndex(0)->GetAttributeKey(0), "id");
    EXPECT_EQ(osm.NodeByIndex(0)->GetAttributeKey(1), "lat");
    EXPECT_EQ(osm.NodeByIndex(0)->GetAttributeKey(4), "");
    EXPECT_EQ(osm.NodeByIndex(0)->GetAttributeKey(3), "");
    EXPECT_EQ(osm.NodeByIndex(0)->GetAttributeKey(2), "lon");
    EXPECT_EQ(osm.NodeByIndex(0)->GetAttributeKey(-1), "");

        /////ways

    auto CDataSource2 = std::make_shared<CStringDataSource>("<osm version=\"0.6\" generator=\"osmconvert 0.8.5\">"
                                                            "<way id=\"8699536\">"
		                                                    "<nd ref=\"258592863\"/>"
		                                                    "<nd ref=\"4399281377\"/>"
		                                                    "<nd ref=\"62224286\"/>"
		                                                    "<nd ref=\"62224288\"/>"
		                                                    "<nd ref=\"62224290\"/>"
                                                        "</way>"
                                                        "<way id=\"8699536\">"
		                                                    "<nd ref=\"258592864\"/>"
		                                                    "<nd ref=\"4399281378\"/>"
		                                                    "<nd ref=\"62224289\"/>"
		                                                    "<nd ref=\"62224280\"/>"
		                                                    "<nd ref=\"62224291\"/>"
                                                        "</way>"
                                                              "</osm>");
    auto Readr2 = std::make_shared<CXMLReader>(CDataSource2);
    COpenStreetMap osm2(Readr2);

    EXPECT_EQ(osm2.WayByIndex(0)->GetAttributeKey(0), "id");
    EXPECT_EQ(osm2.WayByIndex(0)->GetAttributeKey(1), "");
    EXPECT_EQ(osm2.WayByIndex(0)->GetAttributeKey(-1), "");
    EXPECT_EQ(osm2.WayByIndex(1)->GetAttributeKey(0), "id");
}
TEST(OpenStreetMap, HasAttribute){
     
    /////node

    auto CDataSource = std::make_shared<CStringDataSource>("<?xml version='1.0' encoding='UTF-8'?>"
                                                        "<osm version=\"0.6\" generator=\"osmconvert 0.8.5\">"
                                                        "<node id=\"5\" lat=\"5.0\" lon=\"-5.0\"/>"
                                                        "<node id=\"2\" lat=\"2.0\" lon=\"-2.0\"/>"
                                                        "<node id=\"3\" lon=\"-3.0\"/>"
                                                        "</osm>");
    auto Readr = std::make_shared<CXMLReader>(CDataSource);
    COpenStreetMap osm(Readr);
    EXPECT_EQ(osm.NodeByIndex(0)->HasAttribute("id"), true); 
    EXPECT_EQ(osm.NodeByIndex(1)->HasAttribute("lat"), true); 
    EXPECT_EQ(osm.NodeByIndex(2)->HasAttribute("lat"), false); // is this how this works?

    /////ways

    auto CDataSource2 = std::make_shared<CStringDataSource>("<osm version=\"0.6\" generator=\"osmconvert 0.8.5\">"
                                                            "<way id=\"8699536\">"
		                                                    "<nd ref=\"258592863\"/>"
		                                                    "<nd ref=\"4399281377\"/>"
		                                                    "<nd ref=\"62224286\"/>"
		                                                    "<nd ref=\"62224288\"/>"
		                                                    "<nd ref=\"62224290\"/>"
                                                        "</way>"
                                                        "<way id=\"8699536\">"
		                                                    "<nd ref=\"258592864\"/>"
		                                                    "<nd ref=\"4399281378\"/>"
		                                                    "<nd ref=\"62224289\"/>"
		                                                    "<nd ref=\"62224280\"/>"
		                                                    "<nd ref=\"62224291\"/>"
                                                        "</way>"
                                                              "</osm>");
    auto Readr2 = std::make_shared<CXMLReader>(CDataSource2);
    COpenStreetMap osm2(Readr2);

    EXPECT_EQ(osm2.WayByIndex(0)->HasAttribute("id"), true);
    EXPECT_EQ(osm2.WayByIndex(1)->HasAttribute("id"), true);
    EXPECT_EQ(osm2.WayByID(8699536)->HasAttribute("lat"), false);
}

TEST(OpenStreetMap, GetAttribute){

    /////nodes 
    
    auto CDataSource = std::make_shared<CStringDataSource>("<?xml version='1.0' encoding='UTF-8'?>"
                                                        "<osm version=\"0.6\" generator=\"osmconvert 0.8.5\">"
                                                        "<node id=\"5\" lat=\"5.0\" lon=\"-5.0\"/>"
                                                        "<node id=\"2\" lat=\"2.0\" lon=\"-2.0\"/>"
                                                        "<node id=\"3\" lon=\"-3.0\"/>"
                                                        "</osm>");
    auto Readr = std::make_shared<CXMLReader>(CDataSource);
    COpenStreetMap osm(Readr);
    EXPECT_EQ(osm.NodeByIndex(0)->GetAttribute("id"), "5"); 
    EXPECT_EQ(osm.NodeByIndex(1)->GetAttribute("lat"), "2.0");
    EXPECT_EQ(osm.NodeByIndex(2)->GetAttribute("lat"), "");

    /////ways

    auto CDataSource2 = std::make_shared<CStringDataSource>("<osm version=\"0.6\" generator=\"osmconvert 0.8.5\">"
                                                            "<way id=\"8699536\">"
		                                                    "<nd ref=\"258592863\"/>"
		                                                    "<nd ref=\"4399281377\"/>"
		                                                    "<nd ref=\"62224286\"/>"
		                                                    "<nd ref=\"62224288\"/>"
		                                                    "<nd ref=\"62224290\"/>"
                                                        "</way>"
                                                        "<way id=\"8699537\">"
		                                                    "<nd ref=\"258592864\"/>"
		                                                    "<nd ref=\"4399281378\"/>"
		                                                    "<nd ref=\"62224289\"/>"
		                                                    "<nd ref=\"62224280\"/>"
		                                                    "<nd ref=\"62224291\"/>"
                                                        "</way>"
                                                              "</osm>");
    auto Readr2 = std::make_shared<CXMLReader>(CDataSource2);
    COpenStreetMap osm2(Readr2);

    EXPECT_EQ(osm2.WayByIndex(0)->GetAttribute("id"), "8699536");
    EXPECT_EQ(osm2.WayByIndex(1)->GetAttribute("id"), "8699537");
    EXPECT_EQ(osm2.WayByIndex(1)->GetAttribute("lat"), ""); 
}