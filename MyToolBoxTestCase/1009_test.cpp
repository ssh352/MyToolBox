#include "stdafx.h"

#include <iostream>
#include <vector>
#include <streambuf>
#include <algorithm>
typedef std::pair<int,int> PixelPair;


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

	std::vector<PixelPair> ComputerWithNextLine(const LineBuffer& lother)
	{
		
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

		std::vector<PixelPair> lRet;
		for(int i=0;i!= m_LineWidth;)
		{
			PixelPair lPair;
			lPair.first = m_EdgeBuff[i];
			lPair.second = 0;
			while(lPair.first == m_EdgeBuff[i] && i!=m_LineWidth)
			{
				i++;
				lPair.second ++;
			}
			lRet.push_back(lPair);
		}
		return lRet;
	}

	std::vector<PixelPair> ComputerWithNullLine()
	{
		std::vector<PixelPair> lRet;
		return lRet;
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
};


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
	//aBuff.m_SameLineCounter = 0;

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
}


Image HandleOneImage(const Image& lInputImage)
{
	Image lRet;
	return lRet;
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


	std::vector<PixelPair> lRet = lbuff1.ComputerWithNextLine(lbuff2);
	EXPECT_EQ(2,lRet.size());
	EXPECT_EQ(2,lRet[0].first);
	EXPECT_EQ(9,lRet[0].second);
	EXPECT_EQ(1,lRet[1].first);
	EXPECT_EQ(1,lRet[1].second);
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