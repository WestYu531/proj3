OpenStreetMap:
Data Structure Definitions:
SNode and SWay structs: These represent the nodes and ways in the OSM data. Each node contains a unique identifier (ID), location (latitude and longitude), attributes (such as street names), and key-value pairs (to store arbitrary OSM tags). Each way contains a unique identifier, a list of node IDs (which form the sequence of nodes that make up the way), and similarly structured attributes and key-value pairs.

Parsing XML Data:
SImplementation constructor: This is responsible for populating instances of nodes and ways by reading XML data. It uses a CXMLReader object to iteratively read through entities in the XML (nodes, ways). For each entity read, depending on its type (e.g., "node" or "way" tags), it creates an appropriate SNode or SWay object and populates its data.

Data Storage:
AllNodes and AllWays: These are collections that store all the nodes and ways parsed from the XML, respectively.
MappedNodes and MappedWays: These are mappings from node IDs to node objects and from way IDs to way objects, respectively, allowing for fast lookup of entities by ID.

Interface Functions:
NodeCount, WayCount, NodeByIndex, WayByIndex, NodeByID, and WayByID: These are public interfaces provided by the COpenStreetMap class and SImplementation struct for accessing the number of parsed nodes and ways, and for accessing specific nodes and ways by index or ID.

Error Handling and Exception Management:
When accessing the mappings (like finding a node or way by ID), if the key (ID) does not exist, it catches and handles the std::out_of_range exception, with the corresponding function returning a null pointer to indicate a failed lookup.
6. Construction and Destruction:
COpenStreetMap class includes a constructor and destructor. The constructor initializes an instance of SImplementation to handle the XML data. The destructor is responsible for cleaning up resources.

for DSVreader, DSVwriter, XMLReader,XMLwriter we directly use them from proejct2.