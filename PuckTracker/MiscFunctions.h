
#include <iostream>
#include <fstream>
#include <windows.h>
#include <math.h> 
#include <Mfapi.h>
#include <Mfidl.h>
#include <Mfreadwrite.h>

#include "Pixel.h"
#include "SubFrame.h"

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


template <class T> void SafeRelease(T **ppT)
{
	if (*ppT)
	{
		(*ppT)->Release();
		*ppT = NULL;
	}
}

void printHashSet(HashSet<int>^ hashList);
void printPuckPixels(Dictionary<int, List<Pixel^> ^> ^pucks);
void printEquivalentLabels(Dictionary<int, HashSet<int> ^> ^equivalentLabels);

int findSmallestNeighbor(array<int> ^neighborLabels);
int findLargestNeighbor(array<int> ^neighborLabels);
//bool findBlack(array<Byte> ^pixels, int index);
bool findBlack(array<BYTE> ^pixels, int index);
int findMinValue(HashSet<int> ^list);

bool FindInList(List<String ^> ^list, String ^ key);
bool FindInList(List<int> ^list, int key);
Dictionary<int, HashSet<int> ^> ^CopyDictionary(Dictionary<int, HashSet<int> ^> ^originalDictionary);

void ExportPucks(Dictionary<int, List<Pixel^> ^> ^pucks, List<int> ^ pucksToRemove, SUB_FRAME AnalyseFrame);
void PurgePucksFromPucks(Dictionary<int, List<Pixel^> ^> ^pucks, List<int> ^ pucksToRemove);
void ExportEquivalentPucks(Dictionary<int, HashSet<int> ^> ^equivalentPucks);
void ExportLabels(array<int, 2> ^labels, int widthOut, int heightOut, SUB_FRAME AnalyseFrame, String^ filename);
void ExportEquivalentPucks(Dictionary<int, HashSet<int> ^> ^equivalentLabels, String^ filename);
void PurgePucksFromLabels(array<int, 2> ^labels, int widthOut, int heightOut, List<int> ^ pucksToRemove);
void AnalysePucks(Dictionary<int, List<Pixel^> ^> ^pucks, List<int> ^ pucksToRemove);

bool puckSameY(List<Pixel^> ^ list);
bool puckSameX(List<Pixel^> ^ list);
int puckHeight(List<Pixel^> ^ list);
int puckWidth(List<Pixel^> ^ list);

void MediaFoundationTest(WCHAR * fileName);
struct FormatInfo
{
	UINT32          imageWidthPels;
	UINT32          imageHeightPels;
	BOOL            bTopDown;
	RECT            rcPicture;    // Corrected for pixel aspect ratio

	FormatInfo() : imageWidthPels(0), imageHeightPels(0), bTopDown(FALSE)
	{
		SetRectEmpty(&rcPicture);
	}
};