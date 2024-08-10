# CSC10004-K-Dimensional-tree

KD-Tree for Geographic Data Management
This project implements a KD-Tree for efficiently managing and querying geographic locations using the “SimpleMaps World Cities” dataset. The application allows for inserting city data, conducting nearest neighbor searches, and performing range queries to identify cities within a specified rectangular geographic boundary.

Features

1. Dataset Preparation
  - Read CSV Data: Load the SimpleMaps World Cities dataset in a C++ program.
  - Data Extraction: Extract essential details like latitude, longitude, and city names.
  - Data Preprocessing: Clean and filter the data to optimize processing efficiency.

2. KD-Tree Implementation 
  - Insertion: Insert city data into the KD-Tree, alternating between latitude and longitude for node splitting.
  - Range Search: Query all cities within a specified rectangular region based on latitude and longitude.
  - Nearest Neighbor Search: Locate the nearest city to a given set of geographic coordinates using the Haversine formula for accurate distance calculations.

3. User Interface
  - Command-Line Interface:
  - Load cities from a CSV file.
  - Insert new cities individually or from another CSV file.
  - Conduct nearest neighbor searches using latitude and longitude.
  - Perform range queries within specified geographical boundaries.
  - Output Options: Results can be displayed in the command line or exported to a CSV file.

4. Extended Functionality 
  - KD-Tree Serialization:
  - Serialize the KD-Tree structure to a file format (e.g., JSON, XML, or binary) for storage and reloading.
  - Deserialize the tree from a saved file to restore the tree structure.
  - Justification: Discuss the pros and cons of the chosen file format for serialization/deserialization.
