#include <boost\integer.hpp>
using boost::uint8_t;

namespace MY_TOOL_BOX
{
	void dtoh(char* hex,uint8_t n)
	{
		static const char HEX[] = "0123456789ABCDEF";

		hex[0] = HEX[n/16];
		hex[1] = HEX[n%16];
	}

	void dump_block(char* in,char* out,int len)
	{
		for(int cur=0; cur < len; cur++)
		{
			dtoh(out+2*cur,in[cur]);
		}
	}

}
