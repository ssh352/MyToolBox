#pragma once
#include <string>
class OrderRefGenerator
{
public:
	OrderRefGenerator(void);
	~OrderRefGenerator(void);

	std::string GenerateOrderRef();
	void SetMaxOrderRef(int orderRef);

private:
	int m_MaxRef;
};

