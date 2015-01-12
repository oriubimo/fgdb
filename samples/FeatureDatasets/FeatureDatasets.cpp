//
// Sample: FeatureDatasets
//
// Demonstrates how to create a new geodatabase, create a feature dataset, then
// create a table in the feature dataset.

// Copyright 2012 ESRI
// 
// All rights reserved under the copyright laws of the United States
// and applicable international laws, treaties, and conventions.
// 
// You may freely redistribute and use this sample code, with or
// without modification, provided you include the original copyright
// notice and use restrictions.
// 
// See the use restrictions at <your File Geodatabase API install location>/license/userestrictions.txt.
// 

#include <string>
#include <iostream>
#include <fstream>

#include <FileGDBAPI.h>

using namespace std;
using namespace FileGDBAPI;

int main()
{
  // Delete the geodatabase in the current directory if it's already there.
  fgdbError hr;
  wstring errorText;
  Geodatabase geodatabase;
  hr = DeleteGeodatabase(L"../FeatureDatasets/FeatureDatasetDemo.gdb");
  if (hr == S_OK)
  {
    wcout << "The geodatabase has been deleted" << endl;
  }
  else if (hr == -2147024893)
  {
    wcout << "The geodatabase does not exist, no need to delete" << endl;
  }
  else
  {
    wcout << "An error occurred while deleting the geodatabase." << endl;
    ErrorInfo::GetErrorDescription(hr, errorText);
    wcout << errorText << "(" << hr << ")." << endl;
    return -1;
  }

  // Create a new geodatabase in the current directory.
  if ((hr = CreateGeodatabase(L"../FeatureDatasets/FeatureDatasetDemo.gdb", geodatabase)) != S_OK)
  {
    wcout << "An error occurred while creating the geodatabase." << endl;
    ErrorInfo::GetErrorDescription(hr, errorText);
    wcout << errorText << "(" << hr << ")." << endl;
    return -1;
  }
  wcout << "The geodatabase has been created." << endl;

  // Load a feature dataset XML definition into a string. An example of a data
  // element is provided in the sample as "TransitFD.xml".
  string featureDatasetDef;
  string fdDefLine;
  ifstream fdDefFile("../FeatureDatasets/TransitFD.xml");
  while (getline(fdDefFile, fdDefLine))
    featureDatasetDef.append(fdDefLine + "\n");
  fdDefFile.close();

  // Create the feature dataset.
  if ((hr = geodatabase.CreateFeatureDataset(featureDatasetDef)) != S_OK)
  {
    wcout << "An error occurred while creating the feature dataset." << endl;
    ErrorInfo::GetErrorDescription(hr, errorText);
    wcout << errorText << "(" << hr << ")." << endl;
    return -1;
  }
  wcout << "The feature dataset has been created." << endl;

  // Load a table XML definition into a string. There are several differences
  // between this example and adding a table at the root level of the geodatabase:
  //  - The table must have a shape field
  //  - The spatial reference must match that of the feature dataset
  //  - The definition's CatalogPath must include the feature dataset
  string tableDef;
  string tableDefLine;
  ifstream tableDefFile("../FeatureDatasets/BusStopsTable.xml");
  while (getline(tableDefFile, tableDefLine))
    tableDef.append(tableDefLine + "\n");
  tableDefFile.close();

  // Create the table.
  Table table;
  if ((hr = geodatabase.CreateTable(tableDef, L"\\Transit", table)) != S_OK)
  {
    wcout << "An error occurred while creating the table." << endl;
    ErrorInfo::GetErrorDescription(hr, errorText);
    wcout << errorText << "(" << hr << ")." << endl;
    return -1;
  }
  wcout << "The table has been created." << endl;

  // Close the table.
  if ((hr = geodatabase.CloseTable(table) != S_OK))
  {
    wcout << "An error occurred while closing Cities." << endl;
    ErrorInfo::GetErrorDescription(hr, errorText);
    wcout << errorText << "(" << hr << ")." << endl;
    return -1;
  }

  // Close the geodatabase
  if ((hr = CloseGeodatabase(geodatabase)) != S_OK)
  {
    wcout << "An error occurred while closing the geodatabase." << endl;
    ErrorInfo::GetErrorDescription(hr, errorText);
    wcout << errorText << "(" << hr << ")." << endl;
    return -1;
  }

  return 0;
}
