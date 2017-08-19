#pragma once


private ref struct SUB_FRAME {
	SUB_FRAME()
	{
		// default constructor
	}

	SUB_FRAME(const SUB_FRAME% frame)
	{
		// copy constructor
		TopLeft = frame.TopLeft;
		BottomRight = frame.BottomRight;
	}

	Pixel ^  TopLeft = gcnew Pixel();
	Pixel ^ BottomRight = gcnew Pixel();

};

