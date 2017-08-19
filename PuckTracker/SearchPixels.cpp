//////Find Black
////for (int y = 0; y < heightOut; y++)
////{
////	for (int x = 0; x < widthOut; x++)
////	{
////		int index = y * stride + 4 * x;
////		if (y > 80 && x>600 && y < 520 && x < 1200)
////		{
////			System::Byte red = pixels[index];
////			System::Byte green = pixels[index + 1];
////			System::Byte blue = pixels[index + 2];
////			System::Byte alpha = pixels[index + 3];
////			if (red < 40 && green < 40 && blue < 40)
////			{
////				pixels[index] = 250;
////				pixels[index + 1] = 0;
////				pixels[index + 2] = 0;
////			}
////			else
////			{
////				//pixels[index] = 100;
////			}
////		}
////	}
////}
////
//////Find Yellow
////for (int y = 0; y < heightOut; y++)
////{
////	for (int x = 0; x < widthOut; x++)
////	{
////		int index = y * stride + 4 * x;
////		if (y > 1 && x>1 && y < 1000 && x < 1950)
////		{
////			System::Byte red = pixels[index];
////			System::Byte green = pixels[index + 1];
////			System::Byte blue = pixels[index + 2];
////			System::Byte alpha = pixels[index + 3];
////			if (red < 65 && green > 75 && blue > 75)
////			{
////				pixels[index] = 0;
////				pixels[index + 1] = 250;
////				pixels[index + 2] = 0;
////			}
////			else
////			{
////				//pixels[index] = 200;
////			}
////		}
////	}
////}
//
//#include <iostream>
//#include <fstream>
//#include <windows.h>
//#include <math.h> 
//#include "Pixel.h"
//#include "MiscFunctions.h"
//#include "SubFrame.h"
//#include "Dshow.h"
//#include <Mfreadwrite.h>
//#include <Mfapi.h>
//#include <Mfidl.h>
//#include <Objbase.h>
//#include <assert.h>
//
//// Direct2D
//#include <D2d1.h>
//#include <D2d1helper.h>
//
//using namespace cli;
//using namespace std;
//using namespace System;
//using namespace System::IO;
//using namespace System::Windows;
//using namespace System::Windows::Media;
//using namespace System::Windows::Media::Imaging;
//using namespace System::Windows::Controls;
//using namespace System::Diagnostics;
//using namespace Collections::Generic;
//
//
//extern "C" void mainCRTStartup();
//
//ID2D1HwndRenderTarget   *g_pRT = NULL;      // Render target for D2D animation
//
//int main()
//{
//
//	int NextLabel = 0;
//	int smallestNeighbor = 0;
//
//	const int NORTHEAST = 0;
//	const int NORTH = 1;
//	const int NORTHWEST = 2;
//	const int WEST = 3;
//	const int SOUTHWEST = 4;
//	const int SOUTH = 5;
//	const int SOUTHEAST = 6;
//	const int EAST = 7;
//
//	SUB_FRAME AnalyseFrame;
//	AnalyseFrame.TopLeft->x = 0;
//	AnalyseFrame.TopLeft->y = 0;
//	AnalyseFrame.BottomRight->x = 1900;  //1960
//	AnalyseFrame.BottomRight->y = 1080; //1080
//
//	// Open a Stream and decode a PNG image
//	Stream^ imageStreamSource = gcnew FileStream("Still001.png", FileMode::Open, FileAccess::Read, FileShare::Read);
//	//Stream^ imageStreamSource = gcnew FileStream("JackPuck.png", FileMode::Open, FileAccess::Read, FileShare::Read);
//	//Stream^ imageStreamSource = gcnew FileStream("Pucks.png", FileMode::Open, FileAccess::Read, FileShare::Read);
//	//Stream^ imageStreamSource = gcnew FileStream("Still124.bmp", FileMode::Open, FileAccess::Read, FileShare::Read);
//	PngBitmapDecoder^ decoder = gcnew PngBitmapDecoder(imageStreamSource, BitmapCreateOptions::PreservePixelFormat, BitmapCacheOption::Default);
//	BitmapSource^ bitmapSource = decoder->Frames[0];
//
//	int stride = bitmapSource->PixelWidth * 4;
//	int size = bitmapSource->PixelHeight * stride;
//
//	//Create pixel array
//	array<Byte>^ pixels = gcnew array<Byte>(size);
//	//bitmapSource->CopyPixels(pixels, stride, 0);
//
//	int widthOut = bitmapSource->PixelWidth;
//	int heightOut = bitmapSource->PixelHeight;
//	int strideOut = stride;
//
//	array<int, 2> ^labels = gcnew array<int, 2>(widthOut, heightOut);
//	array<int> ^neighbors = gcnew array<int>(4);
//	array<int> ^neighborLabels = gcnew array<int>(8);
//
//	//----------------------------------------------------------------------------------------------------------------
//	LPCWSTR wszFileName = L"GOPR0023.MP4";
//	FormatInfo      m_format;
//	IMFSourceReader *m_pReader;
//	IMFAttributes *pAttributes = NULL;
//	HRESULT hr = S_OK;
//	LONGLONG hnsPos = 1000000;
//	IMFMediaType *pType = NULL;
//
//	hr = MFStartup(MF_VERSION);
//	if (SUCCEEDED(hr))
//	{
//		hr = MFCreateMediaType(&pType);
//
//		hr = pType->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Video);
//		hr = pType->SetGUID(MF_MT_SUBTYPE, MFVideoFormat_RGB32);
//
//		UINT32  width = 0, height = 0;
//		LONG lStride = 0;
//		MFRatio par = { 0, 0 };
//		FormatInfo& format = m_format;
//
//		ZeroMemory(&m_format, sizeof(m_format));
//
//		hr = MFCreateAttributes(&pAttributes, 1);
//		hr = pAttributes->SetUINT32(MF_SOURCE_READER_ENABLE_VIDEO_PROCESSING, TRUE);
//
//		hr = MFCreateSourceReaderFromURL(wszFileName, pAttributes, &m_pReader);
//		if (SUCCEEDED(hr))
//		{
//			// Get the media type from the stream.
//			hr = m_pReader->GetCurrentMediaType((DWORD)MF_SOURCE_READER_FIRST_VIDEO_STREAM, &pType);
//			hr = m_pReader->SetCurrentMediaType((DWORD)MF_SOURCE_READER_FIRST_VIDEO_STREAM, NULL, pType);
//			hr = m_pReader->SetStreamSelection((DWORD)MF_SOURCE_READER_FIRST_VIDEO_STREAM, TRUE);
//
//			// Get the width and height
//			hr = MFGetAttributeSize(pType, MF_MT_FRAME_SIZE, &width, &height);
//
//			// Get the stride to find out if the bitmap is top-down or bottom-up.
//			lStride = (LONG)MFGetAttributeUINT32(pType, MF_MT_DEFAULT_STRIDE, 1);
//
//			format.bTopDown = (lStride > 0);
//
//			// Get the pixel aspect ratio. (This value might not be set.)
//			hr = MFGetAttributeRatio(pType, MF_MT_PIXEL_ASPECT_RATIO, (UINT32*)&par.Numerator, (UINT32*)&par.Denominator);
//			SetRect(&format.rcPicture, 0, 0, width, height);
//
//			format.imageWidthPels = width;
//			format.imageHeightPels = height;
//
//			PROPVARIANT var;
//			PropVariantInit(&var);
//			var.vt = VT_I8;
//			var.hVal.QuadPart = hnsPos;
//			hr = m_pReader->SetCurrentPosition(GUID_NULL, var);
//
//
//			m_pReader->SetCurrentPosition(GUID_NULL, var);
//			hr = m_pReader->SetCurrentMediaType((DWORD)MF_SOURCE_READER_FIRST_VIDEO_STREAM, NULL, pType);
//
//			IMFSample *pSampleTmp = NULL;
//			DWORD dwFlags = 0;
//			hr = m_pReader->ReadSample((DWORD)MF_SOURCE_READER_FIRST_VIDEO_STREAM, 0, NULL, &dwFlags, NULL, &pSampleTmp);
//
//			pSampleTmp->AddRef();
//
//			IMFMediaBuffer *pBuffer = 0;
//			IMF2DBuffer *pBuffer2D = 0;
//			hr = pSampleTmp->QueryInterface(&pBuffer);
//			pSampleTmp->AddRef();
//			//hr = pSampleTmp->QueryInterface(IID_IMF2DBuffer, (void**)&pBuffer2D);
//
//			//m_pBuffer->QueryInterface(IID_IMF2DBuffer, (void**)&m_p2DBuffer);
//			//hr = pBuffer2D->ContiguousCopyFrom(pBuffer);
//
//			//copy the data into the array called pBitmapData
//			BYTE  *pBitmapData = NULL;
//			BYTE **pbScanline0 = NULL;
//			LONG *plPitch = 0;
//			DWORD cbBitmapDataLength = 0;       //Size of Bitmap data, in bytes
//			//HRESULT Lock2D([out]  BYTE **pbScanline0,	[out]  LONG *plPitch);
//			//hr = pBuffer2D->Lock2D(pbScanline0, &lStride);
//
//			//loop through the pBitmapData array
//			for (int y = 1; y < heightOut; y++)
//			{
//				for (int x = 1; x < widthOut; x++)
//				{
//					int index = y * stride + 4 * x;
//
//					//Debug::WriteLine("index: " + index + " Value: " + pBitmapData[index]);
//
//
//				}
//			}
//
//			//HRESULT CreateSystemMemoryBuffer(BYTE *pSrc,DWORD cbData,IMFMediaBuffer **ppBuffer)
//
//			HRESULT hr = S_OK;
//			BYTE *pData = NULL;
//
//
//
//			// Create the media buffer.
//			//hr = MFCreateMemoryBuffer(cbData, &pBuffer); // Amount of memory to allocate, in bytes.
//
//			//hr = pBuffer->Lock(&pData, NULL, NULL);
//			//errno_t memcpy_s(void *dest,size_t numberOfElements,const void *src,size_t count);
//			//memcpy_s( pixels, cbData, pBitmapData, cbBitmapDataLength);
//
//			//	hr = pBuffer->SetCurrentLength(cbData);
//
//			//	hr = pBuffer->Unlock();
//
//			//	*ppBuffer = pBuffer;
//			//	(*ppBuffer)->AddRef();
//
//			//when we are finished doing everything we can unlock the buffer and call MFShutdown (media foundation shutdown method)
//			pBuffer->Unlock();
//			MFShutdown();
//
//
//
//			//----------------------------------------------------------------------------------------------------------------
//
//			Dictionary<int, HashSet<int> ^> ^equivalentLabels = gcnew Dictionary<int, HashSet<int> ^>();
//
//			//Step 1	
//			//First Pass to loop through all pixels and assign a Label(int) to every Pixel that meets the criteria
//			//and maintain a list of equivalent pucks
//			BYTE red;
//			BYTE green;
//			BYTE blue;
//			BYTE alpha;
//
//			for (int y = 1; y < heightOut; y++)
//			{
//				for (int x = 1; x < widthOut; x++)
//				{
//					int index = y * stride + 4 * x;
//
//					//if (red < 65 && green > 75 && blue > 75) //find yellow	
//
//					red = pBitmapData[index];
//					green = pBitmapData[index + 1];
//					blue = pBitmapData[index + 2];
//					alpha = pBitmapData[index + 3];
//
//					if (red < 60 && green < 60 && blue < 60) //find black
//
//					{
//						//SetNeighborValues(neighborLabels, labels,x, y)
//
//						//Look for neighbors
//						neighborLabels[NORTHEAST] = 0;
//						neighborLabels[NORTH] = 0;
//						neighborLabels[NORTHWEST] = 0;
//						neighborLabels[WEST] = 0;
//						neighborLabels[SOUTHWEST] = 0;
//						neighborLabels[SOUTH] = 0;
//						neighborLabels[SOUTHEAST] = 0;
//						neighborLabels[EAST] = 0;
//
//						if (x == 0 && y > 0) //only check NorthEast and North for first column of pixels
//						{
//							neighborLabels[NORTHEAST] = labels[x + 1, y - 1];
//							neighborLabels[NORTH] = labels[x, y - 1];
//							neighborLabels[EAST] = labels[x + 1, y];
//							neighborLabels[SOUTHEAST] = labels[x + 1, y + 1];
//							neighborLabels[SOUTH] = labels[x, y + 1];
//						}
//						else if (y == 0 && x > 0) //only check West for first row of pixels
//						{
//							neighborLabels[WEST] = labels[x - 1, y];
//							neighborLabels[EAST] = labels[x + 1, y];
//							neighborLabels[SOUTHEAST] = labels[x + 1, y + 1];
//							neighborLabels[SOUTH] = labels[x, y + 1];
//							neighborLabels[SOUTHWEST] = labels[x - 1, y + 1];
//						}
//						else if (y > 0 && x == widthOut - 1)  // only check North, NorthWest and West for the last column of pixels
//						{
//							neighborLabels[NORTH] = labels[x, y - 1];
//							neighborLabels[NORTHWEST] = labels[x - 1, y - 1];
//							neighborLabels[WEST] = labels[x - 1, y];
//							neighborLabels[SOUTH] = labels[x, y + 1];
//							neighborLabels[SOUTHWEST] = labels[x - 1, y + 1];
//						}
//						else if (y == heightOut - 1 && x > 0)
//						{
//							neighborLabels[NORTHEAST] = labels[x + 1, y - 1];
//							neighborLabels[NORTH] = labels[x, y - 1];
//							neighborLabels[NORTHWEST] = labels[x - 1, y - 1];
//							neighborLabels[WEST] = labels[x - 1, y];
//							neighborLabels[EAST] = labels[x + 1, y];
//						}
//						else
//						{
//							//check all neighbors for all other pixels
//							neighborLabels[NORTHEAST] = labels[x + 1, y - 1];
//							neighborLabels[NORTH] = labels[x, y - 1];
//							neighborLabels[NORTHWEST] = labels[x - 1, y - 1];
//							neighborLabels[WEST] = labels[x - 1, y];
//							neighborLabels[EAST] = labels[x + 1, y];
//							neighborLabels[SOUTHEAST] = labels[x + 1, y + 1];
//							neighborLabels[SOUTH] = labels[x, y + 1];
//							neighborLabels[SOUTHWEST] = labels[x - 1, y + 1];
//						}
//
//						if (neighborLabels[NORTHEAST] + neighborLabels[NORTH] + neighborLabels[NORTHWEST] + neighborLabels[WEST] == 0)
//						{
//							//this pixel is part of a blob but has no neighbors that have already been labeled
//							NextLabel += 1;
//							labels[x, y] = NextLabel;
//
//							equivalentLabels->Add(NextLabel, gcnew HashSet<int>());
//							equivalentLabels[NextLabel]->Add(NextLabel);
//						}
//						else
//						{
//							//set the label of this pixel to the smallest label of neighbor pixels
//							smallestNeighbor = findSmallestNeighbor(neighborLabels);
//							labels[x, y] = smallestNeighbor;
//
//							// now create an entry in the equivalence list if the neighbor has a different label
//							// to be used by the second pass to aggregate multiple blobs
//							int largestNeighbor = findLargestNeighbor(neighborLabels);
//							if (largestNeighbor != smallestNeighbor)
//							{
//								if (!equivalentLabels[largestNeighbor]->Contains(smallestNeighbor))
//								{
//									//if (!equivalentLabels[largestNeighbor]->Contains(smallestNeighbor))
//									//{	add 59	to 62
//									//equivalentLabels[62]->Add(59);
//									if (smallestNeighbor == 843)
//									{
//										int ccc = 1;
//									}
//									//equivalentLabels[largestNeighbor]->Add(smallestNeighbor);
//									//List<int> ^ listsmallestNeighbor = gcnew List<int>(equivalentLabels[smallestNeighbor]);
//									//equivalentLabels[largestNeighbor]->AddRange(equivalentLabels[smallestNeighbor]);
//
//									//the next line is adding duplicate pucks to the list by mistake.  Not a logical problem but makes it hard to debug and 
//									//will have performance ramifications
//									//ROADWORKS
//									/////equivalentLabels[smallestNeighbor]->AddRange(equivalentLabels[largestNeighbor]); //not sure if this is required
//									//Need to add  smallest neighbor to everything currently in the list of the largest neighbour
//									//For(each label in equivalentLabels[largestNeighbor])
//
//
//									//Add the int smallestNeighbor to every int in the list attached to largestNeighbor
//									//put a list of integers relating to the largestNeighbor into a temporary List,int. called list
//
//									//CHANGES BY KEL
//									//modified the declaration of the temp list so that instead of just making it a pointer
//									//to the list equivalentLabels[largestNeighbor] it is now a copy of the list
//									//old line: List<int> ^ list = equivalentLabels[largestNeighbor]; 
//									//new line adds: gcnew List<int> and then passes in the list  equivalentLabels[largestNeighbor to the
//									//list constructor which c++ takes and copies into the new list
//
//									List<int> ^ list = gcnew List<int>(equivalentLabels[largestNeighbor]);
//
//									for each(int puck in list)							// for each int in the list
//									{
//										// add smallestNeighbor to every int in the temporary list
//										for each (int item in equivalentLabels[smallestNeighbor])
//										{
//											equivalentLabels[puck]->Add(item);
//										}
//
//										//I get this error at Runtime on line 152
//										//An unhandled exception of type 'System.InvalidOperationException' occurred in mscorlib.dll
//										//	Additional information : Collection was modified; enumeration operation may not execute.
//									}
//								}
//							}
//						}
//					}
//				} //end of x loop
//			}  //end of y loop
//
//
//
//			//Print list of equivalent pucks
//			ExportEquivalentPucks(equivalentLabels, "EquivalentPucksStep1.csv");
//
//			//ExportLabels(labels, widthOut, heightOut, AnalyseFrame, "InitialLabels.csv");
//
//			Dictionary<int, HashSet<int> ^> ^equivalentLabelsInitial = CopyDictionary(equivalentLabels);
//
//			//Step 2
//			//Update the Labels with the information in the EquivalentLabels dictionary
//			int previousPixelLabel = 0;
//			for (int y = 0; y < heightOut; y++)
//			{
//				for (int x = 0; x < widthOut; x++)
//				{
//					int pixelLabel = labels[x, y];
//					if (pixelLabel > 0 && pixelLabel != previousPixelLabel)
//					{
//						previousPixelLabel = pixelLabel;
//						//get the list of equivalent labels for the region specified in labels[x, y] if the list contains more than itself
//						//first item in the list is always itself
//						if (equivalentLabelsInitial->ContainsKey(pixelLabel) && equivalentLabelsInitial->Count>1)
//						{
//							//create a new Hashset to hold the temporary list of Labels
//							//This list will be added to as we look through the dictionary for equivalent labels
//							HashSet<int>^ labelsList = gcnew HashSet<int>(equivalentLabelsInitial[pixelLabel]);
//
//							//Debug::WriteLine("Next Pixel");
//							//Debug::WriteLine("--------------------------------------------------------------");
//							//Debug::WriteLine("Pixel Label: " + pixelLabel );
//							//Debug::WriteLine("--------------------------------------------------------------");
//							//Debug::Write("Initial Equivalent Labels for Label " + pixelLabel + " : ");
//							//printHashSet(equivalentLabelsInitial[pixelLabel]);
//
//							for each (int item in equivalentLabelsInitial[pixelLabel])
//							{
//								if (item != pixelLabel)
//								{
//									//Debug::Write("Label " + item + " : ");
//									//printHashSet(equivalentLabelsInitial[item]);
//									labelsList->UnionWith(equivalentLabelsInitial[item]);
//
//									//for each (int item1 in equivalentLabelsInitial[item])
//									//{
//									//	if (item1 != item)
//									//	{
//									//		labelsList->UnionWith(equivalentLabelsInitial[item1]);
//									//	}
//									//}
//								}
//							}
//
//							equivalentLabels[pixelLabel]->UnionWith(labelsList);
//
//							//Debug::Write("Aggregated Equivalent Labels for " + pixelLabel + " : ");
//							//printHashSet(labelsList);
//							//Debug::Write("Initial Equivalent Labels for " + pixelLabel + " : ");
//							//printHashSet(equivalentLabelsInitial[pixelLabel]);
//
//							//find the min value of the list and use that as the key to access the pucks dictionary
//							int min = findMinValue(labelsList);
//
//							//Check to see if this Region also has a
//							labels[x, y] = min;
//
//							//Debug::WriteLine("Pixel Label: " + pixelLabel + " Min: " + min);
//							//printHashSet(labelsList);
//						}
//					}
//				}
//			}
//
//			//Step 3
//			//Create the pucks object (containing a list of pixels for each puck) by looping through the updated Labels array
//			Dictionary<int, List<Pixel^> ^> ^pucks = gcnew Dictionary<int, List<Pixel^> ^>();
//			for (int y = 0; y < heightOut; y++)
//			{
//				for (int x = 0; x < widthOut; x++)
//				{
//					int pixelLabel = labels[x, y];
//					if (pixelLabel > 0)
//					{
//						if (!pucks->ContainsKey(pixelLabel))
//							pucks->Add(pixelLabel, gcnew List<Pixel^>());
//
//						//create a new pixel with co-ords x,y and add it to the list of pixels for this region
//						Pixel^ pixel = gcnew Pixel(x, y);
//						pucks[pixelLabel]->Add(pixel);
//					}
//				}
//			}
//
//			//Step 4
//			//Loop through the Pucks dictionary and determine using the Pixel list for each Puck, which pucks are not Pucks
//			//put them in a list calleld pucksToRemove
//			List<int> ^ pucksToRemove = gcnew List<int>();
//			AnalysePucks(pucks, pucksToRemove);
//
//			//Step 5
//			//now remove the pucks we have marked to be culled in pucksToRemove
//			//PurgePucksFromPucks(pucks, pucksToRemove);
//			PurgePucksFromLabels(labels, widthOut, heightOut, pucksToRemove);
//
//			//Step 6
//			// Loop though the Label array and update the Pixel array
//			for (int y = 0; y < heightOut; y++)
//			{
//				for (int x = 0; x < widthOut; x++)
//				{
//					int index = y * stride + 4 * x;
//					if (y >AnalyseFrame.TopLeft->y && x < AnalyseFrame.BottomRight->x && y <AnalyseFrame.BottomRight->y && x > AnalyseFrame.TopLeft->x)
//					{
//						if (labels[x, y]>0)
//						{
//							//Update the Pixel array
//							pixels[index] = 0;
//							pixels[index + 1] = 250;
//							pixels[index + 2] = 0;
//						}
//					}
//				}
//
//			}
//			//draw a border around the region that the pixels are being coloured to represent a selected pixel
//			//for (int y = 0; y < heightOut; y++)
//			//{
//			//	for (int x = 0; x < widthOut; x++)
//			//	{
//			//		index = y * stride + 4 * x;
//			//		if (
//			//			(
//			//			(y == AnalyseFrame.TopLeft->y || y == AnalyseFrame.BottomRight->y) && x >= AnalyseFrame.TopLeft->x && x <= AnalyseFrame.BottomRight->x) 
//			//			|| 
//			//			(
//			//			(x == AnalyseFrame.TopLeft->x || x == AnalyseFrame.BottomRight->x) && y >= AnalyseFrame.TopLeft->y && y <= AnalyseFrame.BottomRight->y)
//			//			)
//			//		{
//			//			pixels[index] = 0;
//			//			pixels[index + 1] = 0;
//			//			pixels[index + 2] = 250;
//			//		}
//			//	}
//			//}
//			//Draw vertical grid lines
//			//for (int y = 0; y < heightOut; y++)
//			//{
//			//	for (int x = 0; x < widthOut; x=x+100)
//			//	{
//			//		int index = y * stride + 4 * x;
//			//			pixels[index] = 0;
//			//			pixels[index + 1] = 0;
//			//			pixels[index + 2] = 250;
//			//	}
//			//}
//			////Draw horizontal grid lines
//			//for (int y = 0; y < heightOut; y=y+100)
//			//{
//			//	for (int x = 0; x < widthOut; x++)
//			//	{
//			//		int index = y * stride + 4 * x;
//			//			pixels[index] = 0;
//			//			pixels[index + 1] = 0;
//			//			pixels[index + 2] = 250;
//			//	}
//			//}
//
//			//printRegionPixels(pucks);
//			ExportPucks(pucks, pucksToRemove, AnalyseFrame);
//
//			ExportLabels(labels, widthOut, heightOut, AnalyseFrame, "FinalLabels.csv");
//
//			// Create new image file
//			BitmapSource^ imageOut = BitmapSource::Create(widthOut,
//				heightOut,
//				96,
//				96,
//				//PixelFormats::Indexed8,
//				//PixelFormat::Bgra32,
//				bitmapSource->Format,
//				bitmapSource->Palette,
//				pixels,
//				stride);
//
//			FileStream^ stream = gcnew FileStream("new.png", FileMode::Create);
//			PngBitmapEncoder^ encoder = gcnew PngBitmapEncoder();
//			TextBlock^ myTextBlock = gcnew TextBlock();
//			myTextBlock->Text = "Codec Author is: " + encoder->CodecInfo->Author->ToString();
//			encoder->Interlace = PngInterlaceOption::On;
//			encoder->Frames->Add(BitmapFrame::Create(imageOut));
//			encoder->Save(stream);
//			return 0;
//		}
//	}
//}
//
//
//// we need a new entry point  so we can set the STAThread attribute on the main thread
//[System::STAThread]
//int mymain()  //the new entry point so we can set STAThread
//{
//	//Initialize the CRT,which also calls our "main" program
//	mainCRTStartup();
//
//	return 0;
//}
//
//


//draw a border around the region that the pixels are being coloured to represent a selected pixel
//for (int y = 0; y < heightOut; y++)
//{
//	for (int x = 0; x < widthOut; x++)
//	{
//		index = y * stride + 4 * x;
//		if (
//			(
//			(y == AnalyseFrame.TopLeft->y || y == AnalyseFrame.BottomRight->y) && x >= AnalyseFrame.TopLeft->x && x <= AnalyseFrame.BottomRight->x) 
//			|| 
//			(
//			(x == AnalyseFrame.TopLeft->x || x == AnalyseFrame.BottomRight->x) && y >= AnalyseFrame.TopLeft->y && y <= AnalyseFrame.BottomRight->y)
//			)
//		{
//			pixels[index] = 0;
//			pixels[index + 1] = 0;
//			pixels[index + 2] = 250;
//		}
//	}
//}
//Draw vertical grid lines
//for (int y = 0; y < heightOut; y++)
//{
//	for (int x = 0; x < widthOut; x=x+100)
//	{
//		int index = y * stride + 4 * x;
//			pixels[index] = 0;
//			pixels[index + 1] = 0;
//			pixels[index + 2] = 250;
//	}
//}
////Draw horizontal grid lines
//for (int y = 0; y < heightOut; y=y+100)
//{
//	for (int x = 0; x < widthOut; x++)
//	{
//		int index = y * stride + 4 * x;
//			pixels[index] = 0;
//			pixels[index + 1] = 0;
//			pixels[index + 2] = 250;
//	}
//}

//if (x == 737 && y == 542)
//{
//	pixels[index + 1] = 250;
//}
//
//if (x == 1200 && y == 665)
//{
//	pixels[index + 1] = 250;
//}
//
//if (x == 1225 && y == 665)
//{
//	pixels[index + 1] = 250;
//}
//if (x == 1250 && y == 665)
//{
//	pixels[index + 1] = 250;
//}
//
//if (x == 1275 && y == 665)
//{
//	pixels[index + 1] = 250;
//}
//
//if (x == 1300 && y == 665)
//{
//	pixels[index + 1] = 250;
//}
//if (x == 1325 && y == 665)
//{
//	pixels[index + 1] = 250;
//}
//if (x == 1350 && y == 665)
//{
//	pixels[index + 1] = 250;
//}
//if (x == 1375 && y == 665)
//{
//	pixels[index + 1] = 250;
//}
//if (x == 1400 && y == 665)
//{
//	pixels[index + 1] = 250;
//}