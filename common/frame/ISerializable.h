#ifndef I_SERIALIZABLE_H
#define I_SERIALIZABLE_H

#define OUT

namespace common 
{
	class ISerializable
	{
	public:
		virtual void Deserialize(std::vector<uint8_t>& buffer) = 0;	
		virtual	void Serialize(std::vector<uint8_t>& OUT buffer) const = 0;
	};
}

#endif // ISERIALIZABLE_H