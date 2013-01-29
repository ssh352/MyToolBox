// ----------------------------------------------------------------------------
// sample_formats.cpp :  example of basic usage of format
// ----------------------------------------------------------------------------

//  Copyright Samuel Krempp 2003. Use, modification, and distribution are
//  subject to the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/format for library home page
// ----------------------------------------------------------------------------

#include <iostream>
#include <iomanip>
#include <cassert>

#include "boost/format.hpp"

// 2 custom namespaces, to bring in a few useful names :

namespace MyNS_ForOutput {
	using std::cout; using std::cerr;
	using std::string;
	using std::endl; using std::flush;

	using boost::format;
	using boost::io::group;
}

namespace MyNS_Manips {
	using std::setfill;
	using std::setw;
	using std::hex ;
	using std::dec ;
	// gcc-2.95 doesnt define the next ones
	//  using std::showbase ;
	//  using std::left ;
	//  using std::right ;
	//  using std::internal ;
}

//int main(){
//	using namespace MyNS_ForOutput;
//	using namespace MyNS_Manips;
//
//	std::cout << format("%|1$1| %|2$3|") % "Hello" % 3 << std::endl;
//
//	// Reordering :
//	cout << format("%1% %2% %3% %2% %1% \n") % "o" % "oo" % "O"; // 'simple' style.
//	//          prints  "o oo O oo o \n"
//	cout << format("(x,y) = (%1$+5d,%2$+5d) \n") % -23 % 35;     // Posix-Printf style
//
//
//	// No reordering :
//	cout << format("writing %s,  x=%s : %d-th step \n") % "toto" % 40.23 % 50; 
//	//          prints  "writing toto,  x=40.23 : 50-th step \n"
//
//	cout << format("(x,y) = (%+5d,%+5d) \n") % -23 % 35;
//	cout << format("(x,y) = (%|+5|,%|+5|) \n") % -23 % 35;
//	cout << format("(x,y) = (%|1$+5|,%|2$+5|) \n") % -23 % 35;
//	//   all those are the same,  it prints  "(x,y) = (  -23,  +35) \n"
//
//
//
//	// Using manipulators, via 'group' :
//	cout << format("%2% %1% %2%\n")  % 1   % group(setfill('X'), hex, setw(4), 16+3) ;
//	// prints "XX13 1 XX13\n"
//
//
//	// printf directives's type-flag can be used to pass formatting options :
//	cout <<  format("_%1$4d_ is : _%1$#4x_, _%1$#4o_, and _%1$s_ by default\n")  % 18;
//	//          prints  "_  18_ is : _0x12_, _ 022_, and _18_ by default\n"
//
//	// Taking the string value :
//	std::string s;
//	s= str( format(" %d %d ") % 11 % 22 );
//	assert( s == " 11 22 ");
//
//
//	// -----------------------------------------------
//	//  %% prints '%'
//
//	cout << format("%%##%#x ") % 20 << endl;
//	//          prints  "%##0x14 "
//
//
//	// -----------------------------------------------
//	//    Enforcing the right number of arguments 
//
//	// Too much arguments will throw an exception when feeding the unwanted argument :
//	try {
//		format(" %1% %1% ") % 101 % 102;
//		// the format-string refers to ONE argument, twice. not 2 arguments.
//		// thus giving 2 arguments is an error
//	}
//	catch (boost::io::too_many_args& exc) { 
//		cerr <<  exc.what() << "\n\t\t***Dont worry, that was planned\n";
//	}
//
//
//	// Too few arguments when requesting the result will also throw an exception :
//	try {
//		cerr << format(" %|3$| ") % 101; 
//		// even if %1$ and %2$ are not used, you should have given 3 arguments
//	}
//	catch (boost::io::too_few_args& exc) { 
//		cerr <<  exc.what() << "\n\t\t***Dont worry, that was planned\n";
//	}
//
//
//	cerr << "\n\nEverything went OK, exiting. \n";
//	return 0;
//}



#define ONLINE_JUDGE

#ifndef ONLINE_JUDGE
#include "stdafx.h"
#endif


#include <iostream>
#include <vector>
#include <streambuf>
#include <algorithm>
typedef std::pair<int,int> PixelPair;


void  GeneratePairFromEdge(int aWidth,int* apEdgeBuff,std::vector<PixelPair>& lRet)
{
	for(int i=0;i!= aWidth;)
	{
		PixelPair lPair;
		lPair.first = apEdgeBuff[i];
		lPair.second = 0;
		while(lPair.first == apEdgeBuff[i] && i!=aWidth)
		{
			i++;
			lPair.second ++;
		}
		lRet.push_back(lPair);
	}
}

void TrimResultPairVec(std::vector<PixelPair>& lRet)
{
	if(lRet.size() == 1)
	{
		return;
	}

	std::vector<PixelPair>::iterator iter = lRet.begin();
	std::vector<PixelPair>::iterator lPreIter=iter;
	for(iter++;iter!= lRet.end();)
	{
		while( iter!= lRet.end()  && lPreIter->first == iter->first )
		{
			lPreIter->second += iter->second;
			iter = lRet.erase(iter);	
		}

		if(iter!= lRet.end())
		{
			lPreIter = iter;
			iter++;
		}

	}
}


struct LineBuffer
{
	LineBuffer(int LineWidth)
		:	m_LineWidth(LineWidth)
		, m_SameLineCounter(0)
	{
		m_line = new int[LineWidth];
		m_EdgeBuff = new int[LineWidth];
	}
	~LineBuffer()
	{
		delete [] m_line;
		delete [] m_EdgeBuff;
	}

	void swap(LineBuffer& lother)
	{
		if(&lother == this)
			return;
		std::swap(m_line , lother.m_line);
		std::swap(m_EdgeBuff , lother.m_EdgeBuff);
		std::swap(m_SameLineCounter ,lother.m_SameLineCounter);
		std::swap(m_LineWidth , lother.m_LineWidth);
	}




	void ComputerWithNextLine(const LineBuffer& lother, std::vector<PixelPair>& lRet)
	{

		if(m_SameLineCounter>1)
		{
			GeneratePairFromEdge(m_LineWidth,m_EdgeBuff,lRet);
			memset(m_EdgeBuff,0,sizeof(int)*m_LineWidth);

		}
		if(m_SameLineCounter>2)
		{
			lRet.push_back( std::make_pair(0,m_LineWidth * (m_SameLineCounter -2)));
		}





		//todo Check linewidth >= 3
		//do clac unless first and last
		for(int i=1;i<m_LineWidth-1;++i)
		{
			int v_s =  abs(m_line[i] - lother.m_line[i]);
			if(v_s>m_EdgeBuff[i])
			{
				m_EdgeBuff[i] = v_s;
			}
			if(v_s>lother.m_EdgeBuff[i])
			{
				lother.m_EdgeBuff[i] = v_s;
			}

			int v_p =  abs(m_line[i] - lother.m_line[i-1]);
			if(v_p>m_EdgeBuff[i])
			{
				m_EdgeBuff[i] = v_p;
			}
			if(v_p>lother.m_EdgeBuff[i-1])
			{
				lother.m_EdgeBuff[i-1] = v_p;
			}

			int v_n =  abs(m_line[i] - lother.m_line[i+1]);
			if(v_n>m_EdgeBuff[i])
			{
				m_EdgeBuff[i] = v_n;
			}
			if(v_n>lother.m_EdgeBuff[i+1])
			{
				lother.m_EdgeBuff[i+1] = v_n;
			}
		}

		{
			int i = m_LineWidth-1;
			int v_s =  abs(m_line[i] - lother.m_line[i]);
			if(v_s>m_EdgeBuff[i])
			{
				m_EdgeBuff[i] = v_s;
			}
			if(v_s>lother.m_EdgeBuff[i])
			{
				lother.m_EdgeBuff[i] = v_s;
			}

			int v_p =  abs(m_line[i] - lother.m_line[i-1]);
			if(v_p>m_EdgeBuff[i])
			{
				m_EdgeBuff[i] = v_p;
			}
			if(v_p>lother.m_EdgeBuff[i-1])
			{
				lother.m_EdgeBuff[i-1] = v_p;
			}
		}

		{
			int i = 0;
			int v_s =  abs(m_line[i] - lother.m_line[i]);
			if(v_s>m_EdgeBuff[i])
			{
				m_EdgeBuff[i] = v_s;
			}
			if(v_s>lother.m_EdgeBuff[i])
			{
				lother.m_EdgeBuff[i] = v_s;
			}

			int v_n =  abs(m_line[i] - lother.m_line[i+1]);
			if(v_n>m_EdgeBuff[i])
			{
				m_EdgeBuff[i] = v_n;
			}
			if(v_n>lother.m_EdgeBuff[i+1])
			{
				lother.m_EdgeBuff[i+1] = v_n;
			}
		}
		GeneratePairFromEdge(m_LineWidth,m_EdgeBuff,lRet);
		TrimResultPairVec(lRet);
		return;
	}

	void ComputerWithNullLine(std::vector<PixelPair>& lRet)
	{
		if(m_SameLineCounter>1)
		{
			GeneratePairFromEdge(m_LineWidth,m_EdgeBuff,lRet);
			memset(m_EdgeBuff,0,sizeof(int)*m_LineWidth);

		}
		if(m_SameLineCounter>2)
		{
			lRet.push_back( std::make_pair(0,m_LineWidth * (m_SameLineCounter -2)));
		}


		GeneratePairFromEdge(m_LineWidth,m_EdgeBuff,lRet);
		TrimResultPairVec(lRet);
		return;
	}

	int* m_line;
	int* m_EdgeBuff;
	int	 m_SameLineCounter;
	int m_LineWidth;

	void UpdateinLineEdgeBuff()
	{
		if(m_SameLineCounter >0)
		{
			memset(m_EdgeBuff,0,m_LineWidth*sizeof(int));
		}
		else
		{
			//todo there could be some op
			for(int i=0;i !=  m_LineWidth-1 ;++i)
			{
				m_EdgeBuff[i] = std::max( abs(m_line[i] - m_line[i-1]),abs(m_line[i] - m_line[i+1])) ;
			}
			m_EdgeBuff[m_LineWidth -1] = abs(m_line[m_LineWidth -1] - m_line[m_LineWidth -2]);
			m_EdgeBuff[0] =  abs(m_line[ 0] - m_line[1]);
		}
	}
};




struct Image
{
	Image()
	{
		m_NextFeedIndex = 0;
		m_Width = 0;
	}
	int m_NextFeedIndex;
	int m_Width;
	std::vector<PixelPair> m_ImageContent;
	bool FeedLines(LineBuffer& aBuff);
	std::ostream& operator << (std::ostream& loutput)
	{
		loutput<<m_Width<<'\n';
		for(std::vector<PixelPair>::iterator iter = m_ImageContent.begin();iter!= m_ImageContent.end(); ++iter)
		{
			loutput<<iter->first<<' '<<iter->second<<'\n';
		}
		return loutput;
	}

};

std::ostream& operator << (std::ostream& loutput,Image& aImage)
{
	return aImage.operator<<(loutput);
}

typedef std::vector<Image> ImageVec;

bool GetOneImage(std::istream& aInput,Image& lOutPutImage)
{
	int width_val;

	aInput>>width_val;
	if(width_val == 0)
	{
		return false;
	}
	lOutPutImage.m_Width = width_val;


	while (true)
	{
		PixelPair aPair;
		aInput>>aPair.first >> aPair.second;
		if(aPair.first == 0 && aPair.second==0)
		{
			break;
		}
		lOutPutImage.m_ImageContent.push_back(aPair);
	}
	return true;
}

ImageVec InputParse(std::istream& aInput)
{
	ImageVec lRet;

	Image lCurrentImage;
	while( GetOneImage(aInput,lCurrentImage))
	{
		lRet.push_back(lCurrentImage);
		lCurrentImage.m_ImageContent.clear();
	}
	return lRet;
}

bool Image::FeedLines( LineBuffer& aBuff )
{
	if(m_ImageContent.size() == m_NextFeedIndex)
	{
		return false;
	}

	PixelPair* lpCurrentPair = &m_ImageContent[m_NextFeedIndex];

	if( lpCurrentPair->second >= m_Width)
	{
		for (int i = 0; i< m_Width;++i)
		{
			aBuff.m_line[i] = lpCurrentPair->first;
		}
		aBuff.m_SameLineCounter = lpCurrentPair->second / m_Width;
		lpCurrentPair->second -= m_Width * aBuff.m_SameLineCounter;
		if(lpCurrentPair->second == 0)
		{
			++m_NextFeedIndex;
		}

	}
	else
	{
		aBuff.m_SameLineCounter = 0;
		int feedCharCount = 0;
		while(feedCharCount < m_Width)
		{
			lpCurrentPair = &m_ImageContent[m_NextFeedIndex];
			if(feedCharCount + lpCurrentPair->second <= m_Width )
			{
				for (int i = 0; i< lpCurrentPair->second;++i)
				{
					aBuff.m_line[feedCharCount+i] = lpCurrentPair->first;
				}
				feedCharCount += lpCurrentPair->second;
				//just for debug
				lpCurrentPair->second = 0;
				++m_NextFeedIndex; 

			}
			else
			{
				for (int i = 0; i< (m_Width- feedCharCount);++i)
				{
					aBuff.m_line[feedCharCount+i] = lpCurrentPair->first;
				}
				lpCurrentPair->second  -= m_Width- feedCharCount ;
				feedCharCount  =  m_Width;
			}
		}
	}
	aBuff.UpdateinLineEdgeBuff();
	return true;
}


Image HandleOneImage(Image& lInputImage)
{
	Image lRet;
	lRet.m_Width = lInputImage.m_Width;
	std::vector<PixelPair>& lContent = lRet.m_ImageContent;


	LineBuffer lbuff1(lInputImage.m_Width);
	LineBuffer lbuff2(lInputImage.m_Width);
	lInputImage.FeedLines(lbuff1);

	while(lInputImage.FeedLines(lbuff2))
	{
		lbuff1.ComputerWithNextLine(lbuff2,lContent);
		lbuff1.swap(lbuff2);
	}
	lbuff1.ComputerWithNullLine(lContent);
	return lRet;
}


void MainProcess(std::istream& inputStream , std::ostream& outputStream)
{
	ImageVec lImageVec = InputParse(inputStream);
	ImageVec lEdgeImageVec;
	for(ImageVec::iterator iter = lImageVec.begin();iter != lImageVec.end();iter++)
	{
		lEdgeImageVec.push_back(HandleOneImage(*iter));
	}

	for(ImageVec::iterator iter = lEdgeImageVec.begin();iter != lEdgeImageVec.end();iter++)
	{
		outputStream << *iter;
		outputStream << "0 0\n";
	}
	outputStream<<'0';
}

//================================ TEST FIELD=====================================

#ifndef ONLINE_JUDGE


TEST(test_1009, imgae_process_test1)
{
	std::stringstream lInputStream;
	lInputStream
		<<7<<' '<<'\n'
		<<15<<' '<<4<<'\n'
		<<100<<' '<<15<<'\n'
		<<25 <<' '<<2<<'\n'
		<<175 <<' '<<2<<'\n'
		<<25<<' '<< 5<<'\n'
		<<175<<' '<< 2<<'\n'
		<<25<<' '<< 5<<'\n'
		<<0 <<' '<<0<<'\n'
		<<0<<'\n';

	Image lOutPutImage;

	bool lret = GetOneImage(lInputStream,lOutPutImage);

	Image EdgeImage = HandleOneImage(lOutPutImage);
	EXPECT_EQ(lOutPutImage.m_Width,EdgeImage.m_Width);
	EXPECT_EQ(9,EdgeImage.m_ImageContent.size());
	EXPECT_EQ( std::make_pair(85,5),EdgeImage.m_ImageContent[0]);
	EXPECT_EQ( std::make_pair(0,2),EdgeImage.m_ImageContent[1]);
	EXPECT_EQ( std::make_pair(85,5),EdgeImage.m_ImageContent[2]);
	EXPECT_EQ( std::make_pair(75,10),EdgeImage.m_ImageContent[3]);
	EXPECT_EQ( std::make_pair(150,2),EdgeImage.m_ImageContent[4]);
	EXPECT_EQ( std::make_pair(75,3),EdgeImage.m_ImageContent[5]);
	EXPECT_EQ( std::make_pair(0,2),EdgeImage.m_ImageContent[6]);
	EXPECT_EQ( std::make_pair(150,2),EdgeImage.m_ImageContent[7]);
	EXPECT_EQ( std::make_pair(0,4),EdgeImage.m_ImageContent[8]);
}


TEST(test_1009,imgae_process_test2)
{
	std::stringstream lInputStream;
	lInputStream
		<<10<<' '<<'\n'
		<<35<<' '<<500000000<<'\n'
		<<200 <<' '<<500000000<<'\n'
		<<0 <<' '<<0<<'\n'
		<<0<<'\n';
	Image lOutPutImage;
	bool lret = GetOneImage(lInputStream,lOutPutImage);

	Image EdgeImage = HandleOneImage(lOutPutImage);
}

void CheckResultSize(const std::vector<PixelPair>& lRet,int Expect_size)
{
	int OutPutSize = 0;
	for(std::size_t i =0;i<lRet.size();i++)
	{
		OutPutSize += lRet[i].second;
	}
	EXPECT_EQ(Expect_size,OutPutSize);
}

TEST(test_1009,lineBuffCompterNextLine)
{

	LineBuffer lbuff1(10);
	LineBuffer lbuff2(10);

	for(int i = 0;i<10;++i)
	{
		lbuff1.m_line[i] = i;
		lbuff1.UpdateinLineEdgeBuff();
		lbuff1.m_SameLineCounter = 0;

		lbuff2.m_line[i] = i+1;
		lbuff2.UpdateinLineEdgeBuff();
		lbuff2.m_SameLineCounter = 0;

	}


	std::vector<PixelPair> lRet ;
	lbuff1.ComputerWithNextLine(lbuff2,lRet);
	lbuff2.ComputerWithNullLine(lRet);
	EXPECT_EQ(3,lRet.size());

	EXPECT_EQ(std::make_pair(2,9),lRet[0]);
	EXPECT_EQ(std::make_pair(1,2),lRet[1]);
	EXPECT_EQ(std::make_pair(2,9),lRet[2]);

	CheckResultSize(lRet,20);
}

TEST(test_1009,twoline_CompterNextLine)
{

	LineBuffer lbuff1(10);
	LineBuffer lbuff2(10);

	for(int i = 0;i<10;++i)
	{
		lbuff1.m_line[i] = i;
		lbuff1.UpdateinLineEdgeBuff();
		lbuff1.m_SameLineCounter = 2;

		lbuff2.m_line[i] = i+1;
		lbuff2.UpdateinLineEdgeBuff();
		lbuff2.m_SameLineCounter = 0;
	}


	std::vector<PixelPair> lRet ;
	lbuff1.ComputerWithNextLine(lbuff2,lRet);
	lbuff2.ComputerWithNullLine(lRet);
	EXPECT_EQ(4,lRet.size());

	EXPECT_EQ(std::make_pair(0,10),lRet[0]);
	EXPECT_EQ(std::make_pair(2,9),lRet[1]);
	EXPECT_EQ(std::make_pair(1,2),lRet[2]);
	EXPECT_EQ(std::make_pair(2,9),lRet[3]);

	CheckResultSize(lRet,30);
}

TEST(test_1009,multiline_CompterNextLine)
{

	LineBuffer lbuff1(10);
	LineBuffer lbuff2(10);

	for(int i = 0;i<10;++i)
	{
		lbuff1.m_line[i] = i;
		lbuff1.UpdateinLineEdgeBuff();
		lbuff1.m_SameLineCounter = 10;

		lbuff2.m_line[i] = i+1;
		lbuff2.UpdateinLineEdgeBuff();
		lbuff2.m_SameLineCounter = 0;
	}


	std::vector<PixelPair> lRet;
	lbuff1.ComputerWithNextLine(lbuff2,lRet);
	EXPECT_EQ(3,lRet.size());

	EXPECT_EQ(std::make_pair(0,90),lRet[0]);
	EXPECT_EQ(std::make_pair(2,9),lRet[1]);
	EXPECT_EQ(std::make_pair(1,1),lRet[2]);


	CheckResultSize(lRet,100);
}




TEST(test_1009,FeedLines_test)
{
	std::stringstream lInputStream;
	lInputStream
		<<7<<' '<<'\n'
		<<15<<' '<<4<<'\n'
		<<100<<' '<<15<<'\n'
		<<25 <<' '<<2<<'\n'
		<<175 <<' '<<2<<'\n'
		<<25<<' '<< 5<<'\n'
		<<175<<' '<< 2<<'\n'
		<<25<<' '<< 5<<'\n'
		<<0 <<' '<<0<<'\n'
		<<0<<'\n';

	Image lOutPutImage;

	bool lret = GetOneImage(lInputStream,lOutPutImage);
	LineBuffer lbuff(lOutPutImage.m_Width);
	{
		lOutPutImage.FeedLines(lbuff);
		int line1[] = {15,15,15,15,100,100,100};
		EXPECT_EQ(0,memcmp((void*)lbuff.m_line,(void*)line1,7*sizeof(int)));
		int edge1[] = {0,0,0,85,85,0,0};
		EXPECT_EQ(0,memcmp((void*)lbuff.m_EdgeBuff,(void*)edge1,7*sizeof(int)));
		EXPECT_EQ(0,lbuff.m_SameLineCounter);
	}

	{
		lOutPutImage.FeedLines(lbuff);
		int line1[] = {100,100,100,100,100,100,100};
		EXPECT_EQ(0,memcmp((void*)lbuff.m_line,(void*)line1,7*sizeof(int)));
		int edge1[] = {0,0,0,0,0,0,0};
		EXPECT_EQ(0,memcmp((void*)lbuff.m_EdgeBuff,(void*)edge1,7*sizeof(int)));
		EXPECT_EQ(1,lbuff.m_SameLineCounter);
	}

	{
		lOutPutImage.FeedLines(lbuff);
		int line1[] = {100,100,100,100,100,25,25};
		EXPECT_EQ(0,memcmp((void*)lbuff.m_line,(void*)line1,7*sizeof(int)));
		int edge1[] = {0,0,0,0,75,75,0};
		EXPECT_EQ(0,memcmp((void*)lbuff.m_EdgeBuff,(void*)edge1,7*sizeof(int)));
		EXPECT_EQ(0,lbuff.m_SameLineCounter);
	}

	{
		lOutPutImage.FeedLines(lbuff);
		int line1[] = {175,175,25,25,25,25,25};
		EXPECT_EQ(0,memcmp((void*)lbuff.m_line,(void*)line1,7*sizeof(int)));
		int edge1[] = {0,150,150,0,0,0,0};
		EXPECT_EQ(0,memcmp((void*)lbuff.m_EdgeBuff,(void*)edge1,7*sizeof(int)));
		EXPECT_EQ(0,lbuff.m_SameLineCounter);
	}

	{
		lOutPutImage.FeedLines(lbuff);
		int line1[] = {175,175,25,25,25,25,25};
		EXPECT_EQ(0,memcmp((void*)lbuff.m_line,(void*)line1,7*sizeof(int)));
		int edge1[] = {0,150,150,0,0,0,0};
		EXPECT_EQ(0,memcmp((void*)lbuff.m_EdgeBuff,(void*)edge1,7*sizeof(int)));
		EXPECT_EQ(0,lbuff.m_SameLineCounter);
	}

	EXPECT_EQ(false,lOutPutImage.FeedLines(lbuff));
}


TEST(test_1009,input_one_Image)
{
	std::stringstream lInputStream;
	lInputStream
		<<7<<' '<<'\n'
		<<15<<' '<<4<<'\n'
		<<100<<' '<<15<<'\n'
		<<25 <<' '<<2<<'\n'
		<<175 <<' '<<2<<'\n'
		<<25<<' '<< 5<<'\n'
		<<175<<' '<< 2<<'\n'
		<<25<<' '<< 5<<'\n'
		<<0 <<' '<<0<<'\n'
		<<0<<'\n';

	Image lOutPutImage;
	bool lret = GetOneImage(lInputStream,lOutPutImage);
	EXPECT_EQ(true,lret);
	EXPECT_EQ(7,lOutPutImage.m_Width);
	EXPECT_EQ(7,lOutPutImage.m_ImageContent.size());
	EXPECT_EQ( std::make_pair(15,4),lOutPutImage.m_ImageContent[0]);
	EXPECT_EQ( std::make_pair(100,15),lOutPutImage.m_ImageContent[1]);
	EXPECT_EQ( std::make_pair(25,2),lOutPutImage.m_ImageContent[2]);
	EXPECT_EQ( std::make_pair(175,2),lOutPutImage.m_ImageContent[3]);
	EXPECT_EQ( std::make_pair(25,5),lOutPutImage.m_ImageContent[4]);
	EXPECT_EQ( std::make_pair(175,2),lOutPutImage.m_ImageContent[5]);
	EXPECT_EQ( std::make_pair(25,5),lOutPutImage.m_ImageContent[6]);


	lret = GetOneImage(lInputStream,lOutPutImage);
	EXPECT_EQ(false,lret);
}

TEST(test_1009,input_parse)
{
	std::stringstream lInputStream;
	lInputStream
		<<7<<' '<<'\n'
		<<15<<' '<<4<<'\n'
		<<100<<' '<<15<<'\n'
		<<25 <<' '<<2<<'\n'
		<<175 <<' '<<2<<'\n'
		<<25<<' '<< 5<<'\n'
		<<175<<' '<< 2<<'\n'
		<<25<<' '<< 5<<'\n'
		<<0 <<' '<<0<<'\n'
		<<10<<'\n'
		<<35<<' '<< 500000000<<'\n'
		<<200<<' '<< 500000000<<'\n'
		<<0<<' '<< 0<<'\n'
		<<3<<'\n'
		<<255 <<' '<<1<<'\n'
		<<10<<' '<< 1<<'\n'
		<<255<<' '<< 2<<'\n'
		<<10 <<' '<<1<<'\n'
		<<255<<' '<< 2<<'\n'
		<<10<<' '<< 1<<'\n'
		<<255 <<' '<<1<<'\n'
		<<0<<' '<< 0<<'\n'
		<<0;


	ImageVec lRet = InputParse(lInputStream);
	EXPECT_EQ(3,lRet.size());
	EXPECT_EQ(7,lRet[0].m_Width);
	EXPECT_EQ(7,lRet[0].m_ImageContent.size());

	EXPECT_EQ(10,lRet[1].m_Width);
	EXPECT_EQ(2,lRet[1].m_ImageContent.size());

	EXPECT_EQ(3,lRet[2].m_Width);
	EXPECT_EQ(7,lRet[2].m_ImageContent.size());
}


TEST(test_1009,accept_test)
{
	std::stringstream lInputStream;
	lInputStream
		<<7<<' '<<'\n'
		<<15<<' '<<4<<'\n'
		<<100<<' '<<15<<'\n'
		<<25 <<' '<<2<<'\n'
		<<175 <<' '<<2<<'\n'
		<<25<<' '<< 5<<'\n'
		<<175<<' '<< 2<<'\n'
		<<25<<' '<< 5<<'\n'
		<<0 <<' '<<0<<'\n'
		<<10<<'\n'
		<<35<<' '<< 500000000<<'\n'
		<<200<<' '<< 500000000<<'\n'
		<<0<<' '<< 0<<'\n'
		<<3<<'\n'
		<<255 <<' '<<1<<'\n'
		<<10<<' '<< 1<<'\n'
		<<255<<' '<< 2<<'\n'
		<<10 <<' '<<1<<'\n'
		<<255<<' '<< 2<<'\n'
		<<10<<' '<< 1<<'\n'
		<<255 <<' '<<1<<'\n'
		<<0<<' '<< 0<<'\n'
		<<0;


	std::stringstream loutputStream;
	MainProcess(lInputStream,loutputStream);

	std::stringstream lExpectStream;

	lExpectStream
		<<7<<'\n'
		<<	85<<' '<<5<<'\n'
		<<	0 <<' '<<2<<'\n'
		<<	85 <<' '<<5<<'\n'
		<<	75<<' '<< 10<<'\n'
		<<	150<<' '<< 2<<'\n'
		<<	75<<' '<< 3<<'\n'
		<<	0<<' '<< 2<<'\n'
		<<	150 <<' '<<2<<'\n'
		<<	0 <<' '<<4<<'\n'
		<<	0 <<' '<<0<<'\n'
		<<	10<<'\n'
		<<	0<<' '<< 499999990<<'\n'
		<<	165<<' '<< 20<<'\n'
		<<	0<<' '<< 499999990<<'\n'
		<<	0<<' '<< 0<<'\n'
		<<	3<<'\n'
		<<	245<<' '<< 9<<'\n'
		<<	0 <<' '<<0<<'\n'
		<<	0;
	EXPECT_EQ(lExpectStream.str(),loutputStream.str());


}


#else  //ON_Line_main
int main(int argc,char** argv)
{

	std::istream& lInput = std::cin;
	std::ostream& lOutpt =std::cout;
	MainProcess(lInput,lOutpt);
}
#endif
