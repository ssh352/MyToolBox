#pragma once



#if (_MSC_VER <= 1600)
#include  <boost\foreach.hpp>
#define  MYFOREACH(VAL,CONTANER)\
	BOOST_FOREACH(auto VAL , CONTANER )
#else
#define  MYFOREACH(VAL,CONTANER )\
	for(auto VAL : CONTANER )
#endif