
#include "Pixel.h"
#include "MiscFunctions.h"

using namespace cli;
using namespace std;
using namespace System;
using namespace System::IO;
using namespace System::Windows;
using namespace System::Windows::Media;
using namespace System::Windows::Media::Imaging;
using namespace System::Windows::Controls;
using namespace System::Diagnostics;
using namespace Collections::Generic;

void printHashSet(HashSet<int>^ hashList)
{
	for each (int item in hashList)
	{
		Debug::Write( item + " ");
	}
	Debug::WriteLine("");
}

void printPuckPixels(Dictionary<int, List<Pixel^> ^> ^pucks)
{
	//loop through each key in the dictionary, the key is an integer and it represents a region
	for each (int key in pucks->Keys)
	{
		//now for the region we get the value which is a list of pixels, we put the value (i.e the list of pixels) into a variable called list
		List<Pixel^> ^ list = pucks[key];
		Debug::WriteLine("Region: " + key.ToString() + " Pixels: " + list->Count);
		//now loop through the list of pixel and print the co-ordinates for each pixel
		//for each (Pixel^ pixel in list)
		//{
		//	Debug::Write("(" + pixel->x.ToString() + "," + pixel->y.ToString() + ") ");
		//}
		//Debug::WriteLine("");
	}
}

void printEquivalentLabels(Dictionary<int, HashSet<int> ^> ^equivalentLabels)
{
	for each (int key in equivalentLabels->Keys)
	{
		HashSet<int> ^ list = equivalentLabels[key];
		Debug::Write("Key: " + key.ToString() + " Values: ");
		for each (int value in list)
		{
			Debug::Write(value.ToString() + ", ");
		}
		Debug::WriteLine("");
	}
}

void ExportPucks(Dictionary<int, List<Pixel^> ^> ^pucks, List<int> ^ regionsToRemove, SUB_FRAME AnalyseFrame)
{
	String^ fileName = "Pucks.csv";
	StreamWriter^ sw = gcnew StreamWriter(fileName);

	//Create Region Labels in row 1.
	sw->Write("Puck");
	for each (int regionKey in pucks->Keys)
	{
		sw->Write("," + regionKey.ToString());
	}
	sw->WriteLine("");

	//Indicate if region is marked to be removed
	sw->Write("Remove");
	for each (int regionKey in pucks->Keys)
	{
		if (FindInList(regionsToRemove, regionKey))
			//if (regionKey > 0)
		{
			sw->Write(", Remove");
		}
		else
		{
			sw->Write(",");
		}
	}
	sw->WriteLine("");


	//Create the Pixel count for each region in row 2.
	sw->Write("Pixel Count");
	for each (int puckKey in pucks->Keys)
	{
		List<Pixel^> ^ list = pucks[puckKey];
		sw->Write("," + list->Count);
	}
	sw->WriteLine("");


	//List the (x,y) for every pixel by Region.
	for (int PixelCounter = 0; PixelCounter < 2000; PixelCounter++)
	{
		sw->Write("Pixel " + PixelCounter.ToString());
		for each (int puckKey in pucks->Keys)
		{
			List<Pixel^> ^ list = pucks[puckKey];
			if (list->Count >= PixelCounter + 1)
			{
				sw->Write(",(" + list[PixelCounter]->x.ToString() + ":" + list[PixelCounter]->y.ToString() + ")");
			}
			else
			{
				sw->Write(",");
			}
		}
		sw->WriteLine("");
	}
	sw->Close();
}

void ExportLabels(array<int, 2> ^labels, int widthOut, int heightOut, SUB_FRAME AnalyseFrame, String^ filename)
{
	//String^ fileName = "Labels.csv";
	StreamWriter^ sw = gcnew StreamWriter(filename);

	sw->Write("x: ");
	for (int x = 1; x < widthOut; x++)
	{
		if (x < AnalyseFrame.BottomRight->x && x > AnalyseFrame.TopLeft->x)
		{
			sw->Write("," + x.ToString());
		}
	}
	sw->WriteLine("");

	for (int y = 1; y < heightOut; y++)
	{
		if (y >AnalyseFrame.TopLeft->y && y <AnalyseFrame.BottomRight->y)
		{
			sw->Write("y: " + y.ToString());

			for (int x = 1; x < widthOut; x++)
			{
				if ( x < AnalyseFrame.BottomRight->x && x > AnalyseFrame.TopLeft->x)
				{
					int Label = labels[x, y];
					if (Label>0)
					{
						sw->Write("," + Label);
					}
					else
					{
						sw->Write(",");
					}
				}
			}
		}
		sw->WriteLine("");
	}
	sw->Close();
}

void ExportEquivalentPucks(Dictionary<int, HashSet<int> ^> ^equivalentLabels, String^ filename)
{
	//Dictionary<int, List<int> ^> ^equivalentLabels = gcnew Dictionary<int, List<int> ^>();
	
	StreamWriter^ sw = gcnew StreamWriter(filename);

	//Create Puck Labels in row 1.
	sw->Write("Region");
	for each (int puckKey in equivalentLabels->Keys)
	{
		sw->Write("," + puckKey.ToString());
	}
	sw->WriteLine("");

	//Create the Region count for each region in row 2.
	sw->Write("Count");
	for each (int puckKey in equivalentLabels->Keys)
	{
		HashSet<int> ^ list = equivalentLabels[puckKey];
		sw->Write("," + list->Count);
	}
	sw->WriteLine("");

	//Equivalent Regions by Region.
	//ROADWORKS: need to rewrite debug output for HashSet
	for (int puckCounter = 0; puckCounter < 2000; puckCounter++)
	{
		sw->Write("Region " + puckCounter.ToString());
		for each (int puckKey in equivalentLabels->Keys)
		{	
			array<int> ^ list = gcnew array<int>(equivalentLabels[puckKey]->Count);
			equivalentLabels[puckKey]->CopyTo(list);

			if (list->Length > puckCounter ) //+ 1)
			{			
				sw->Write("," + list[puckCounter].ToString());
			}
			else
			{
				sw->Write(",");
			}
		}
		sw->WriteLine("");
	}
	sw->Close();
	//Console::WriteLine("a new file ('{0}') has been written", fileName);

}

//copy all the keys and the value list for each key from the original dictionary into a new dictionary
//which is returned in the function return value
Dictionary<int, HashSet<int> ^> ^CopyDictionary(Dictionary<int, HashSet<int> ^> ^originalDictionary)
{
	Dictionary<int, HashSet<int> ^> ^copyDictionary = gcnew Dictionary<int, HashSet<int> ^>();
	for each (int key in originalDictionary->Keys)
	{
		HashSet<int>^ copyList = gcnew HashSet<int>(originalDictionary[key]);
		copyDictionary->Add(key, copyList);
	}

	return copyDictionary;
}

bool FindInList(List<String ^> ^list, String ^ key)
{
	bool result = false;
	for each (String^ item in list)
	{
		if (item == key)
		{
			result = true;
		}
	}
	return result;
}

bool FindInList(List<int> ^list, int key)
{
	bool result = false;
	for each (int item in list)
	{
		if (item == key)
		{
			result = true;
		}
	}
	return result;
}

void PurgePucksFromPucks(Dictionary<int, List<Pixel^> ^> ^pucks, List<int> ^ pucksToRemove)
{
	for each (int regionKey in pucksToRemove)
	{
		pucks->Remove(regionKey);
	}
	pucksToRemove->Clear();


}

void PurgePucksFromLabels(array<int, 2> ^labels, int widthOut, int heightOut, List<int> ^ pucksToRemove)
{
	for (int y = 0; y < heightOut; y++)
	{
		for (int x = 0; x < widthOut; x++)
		{//equivalentLabels[largestNeighbor]->Contains(smallestNeighbor)

			if (pucksToRemove->Contains(labels[x, y]) )
			{
				labels[x, y] = 0;
			}
		}
	}
}

bool findBlack(array<Byte> ^pixels, int index)
//bool findBlack(BYTE ^pixels, int index)

{
	Byte red = pixels[index];
	Byte green = pixels[index + 1];
	Byte blue = pixels[index + 2];
	Byte alpha = pixels[index + 3];

	if (red < 60 && green < 60 && blue < 60) //find black
		return true;
	else
		return false;
}

int findSmallestNeighbor(array<int> ^neighborLabels)
{//Finds the smallest non-zero neighbor
	int result = 10000;
	for (int x = 0; x < 4; x++)
	{
		if (neighborLabels[x]>0 && neighborLabels[x] < result)
		{
			result = neighborLabels[x];
		}
	}

	return result;
}
int findLargestNeighbor(array<int> ^neighborLabels)
{
	int result = 0;
	for (int x = 0; x < 4; x++)
	{
		if (neighborLabels[x] > result)
		{
			result = neighborLabels[x];
		}
	}
	return result;
}

//Find the min value in a list
int findMinValue(HashSet<int> ^list)
{
	int result = INT_MAX;
	for each (int item in list)
	{
		if (item < result)
		{
			result = item;
		}
	}
	return result;
}

void AnalysePucks(Dictionary<int, List<Pixel^> ^> ^pucks, List<int> ^ pucksToRemove)
{
	int minPixelCount = 30;
	int maxPixelCount = 125;
	for each (int key in pucks->Keys)
	{
		//if (key == 2388)
		//{
		//	int aaa = 1;
		//}
		List<Pixel^> ^ list = pucks[key];

		if (list->Count <= minPixelCount || list->Count >= maxPixelCount)
		{
			pucksToRemove->Add(key);
		}
		else if (puckSameY(list) || puckSameX(list))
		{
			pucksToRemove->Add(key);
		}
		else if (puckWidth(list)>18 || puckHeight(list)>18)
		{
			pucksToRemove->Add(key);
		}
		else if ((float)list->Count/(float)puckWidth(list)/(float)puckHeight(list)<.65)
		{
			pucksToRemove->Add(key);
		}
		else if (((float)puckWidth(list) / (float)puckHeight(list)) < .3 || ((float)puckHeight(list) / (float)puckWidth(list)) < .3)
		{
			pucksToRemove->Add(key);
		}
	}
}

bool puckSameY(List<Pixel^> ^ list)
{
	int minY =10000;
	int maxY = 0;
	for each (Pixel^ item in list)
	{
		if (item->y < minY)
		{
			minY = item->y;
		}
		if (item->y > maxY)
		{
			maxY = item->y;
		}
	}
	if (abs(minY - maxY) < 1)
	{
		return true;
	}
	else
	{
		return false;
	}
}
bool puckSameX(List<Pixel^> ^ list)
{
	int minX = 10000;
	int maxX = 0;
	for each (Pixel^ item in list)
	{
		if (item->x < minX)
		{
			minX = item->x;
		}
		if (item->x > maxX)
		{
			maxX = item->x;
		}
	}
	if (abs(minX - maxX) < 1)
	{
		return true;
	}
	else
	{
		return false;
	}
}
int puckWidth(List<Pixel^> ^ list)
{
	int minX = 10000;
	int maxX = 0;
	for each (Pixel^ item in list)
	{
		if (item->x < minX)
		{
			minX = item->x;
		}
		if (item->x > maxX)
		{
			maxX = item->x;
		}
	}
	return abs(minX - maxX);
}
int puckHeight(List<Pixel^> ^ list)
{
	int minY = 10000;
	int maxY = 0;
	for each (Pixel^ item in list)
	
	return abs(minY - maxY);
}




void MediaFoundationTest(WCHAR *fileName)
{
	const WCHAR *pszURL = fileName;

	// Initialize the COM runtime.
	HRESULT hr = CoInitialize(0);

	if (SUCCEEDED(hr))
	{
		// Initialize the Media Foundation platform.
		hr = MFStartup(MF_VERSION);
		if (SUCCEEDED(hr))
		{
			// Create the source reader.
			IMFSourceReader *pReader;
 
			hr = MFCreateSourceReaderFromURL(pszURL, NULL, &pReader);
			if (SUCCEEDED(hr))
			{
				//ReadMediaFile(pReader);
				//pReader->ReadSample((DWORD)MF_SOURCE_READER_FIRST_VIDEO_STREAM,,0,NULL,
				pReader->Release();
			}
			// Shut down Media Foundation.
			MFShutdown();
		}
		CoUninitialize();
	} 
}

//hr = pAttributes->SetUINT32(MF_SOURCE_READER_ENABLE_VIDEO_PROCESSING, TRUE);
//hr = MFCreateSourceReaderFromURL(wszFileName, pAttributes, &m_pReader);
//
//
//Code:
//hr = m_pReader->ReadSample(
//	(DWORD)MF_SOURCE_READER_FIRST_VIDEO_STREAM,
//	0,
//	NULL,
//	&dwFlagss,
//	&time,
//	&pSampleTmp
//	);
//-See more at : http ://en.efreedom.net/Question/1-14538619/Media-Foundation-IMFSourceReader-Consuming-Excessive-Memory#sthash.CDCoyucI.dpuf
//IMFSample *pSample = NULL;

//hr = pReader->ReadSample(
//	MF_SOURCE_READER_ANY_STREAM,    // Stream index.
//	0,                              // Flags.
//	&streamIndex,                   // Receives the actual stream index. 
//	&flags,                         // Receives status flags.
//	&llTimeStamp,                   // Receives the time stamp.
//	&pSample                        // Receives the sample or NULL.
//	); -See more at : http ://en.efreedom.net/Question/1-10191897/Save-RGB24-Sample-Bitmap#sthash.6XJrRl5l.dpuf
//

//HRESULT EnumerateTypesForStream(IMFSourceReader *pReader, DWORD dwStreamIndex)
//{
//	HRESULT hr = S_OK;
//	DWORD dwMediaTypeIndex = 0;
//
//	while (SUCCEEDED(hr))
//	{
//		IMFMediaType *pType = NULL;
//		hr = pReader->GetNativeMediaType(dwStreamIndex, dwMediaTypeIndex, &pType);
//		if (hr == MF_E_NO_MORE_TYPES)
//		{
//			hr = S_OK;
//			break;
//		}
//		else if (SUCCEEDED(hr))
//		{
//			// Examine the media type. (Not shown.)
//
//			pType->Release();
//		}
//		++dwMediaTypeIndex;
//	}
//	return hr;
//}
