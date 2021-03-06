#ifndef NephilimPacket_h__
#define NephilimPacket_h__

#include <Nephilim/Platform.h>
#include <Nephilim/Foundation/String.h>
#include <Nephilim/Foundation/Vector.h>

#include <vector>

NEPHILIM_NS_BEGIN

/**
	\class Packet
	\brief Allows to pack and unpack data for network transfer
*/
class Packet
{
public:
	/// Start the empty packet
	Packet();

	/// Clear the contents
	void clear();

	/// Appends data to the buffer
	void append(const void* data, std::size_t dataSize);

	/// Get the size in bytes of the buffer
	std::size_t getDataSize() const;

	/// Get the data raw pointer
	const void* getData() const;

	virtual const void* onSend(std::size_t& size);

	virtual void onReceive(const void* data, std::size_t size);
	
	/// Appends a char to the buffer
	Packet& operator<<(char data);

	/// Appends a float to the buffer
	Packet& operator<<(float data);

	/// Appends a Vec2f to the buffer
	Packet& operator<<(const Vec2f& data);

	/// Appends a Int16 to the buffer
	Packet& operator<<(Int16 data);

	/// Appends a Uint32 to the buffer
	Packet& operator<<(Uint32 data);

	/// Appends a String to the buffer
	Packet& operator<<(const String& data);




	/// Reads a char from the buffer current position
	Packet& operator>>(char& data);

	/// Reads a float from the buffer current position
	Packet& operator>>(float& data);

	/// Reads a Vec2f from the buffer current position
	Packet& operator>>(Vec2f& data);

	/// Reads a Int8 from the buffer current position
	Packet& operator>>(Int8& data);

	/// Reads a Int16 from the buffer current position
	Packet& operator>>(Int16& data);

	/// Reads a Int32 from the buffer current position
	Packet& operator>>(Int32& data);

	/// Reads a Uint8 from the buffer current position
	Packet& operator>>(Uint8& data);

	/// Reads a Uint16 from the buffer current position
	Packet& operator>>(Uint16& data);

	/// Reads a Uint32 from the buffer current position
	Packet& operator>>(Uint32& data);

	/// Reads a String from the buffer current position
	Packet& operator>>(String& data);


private:
	/// The data buffer of the packet
	std::vector<char> m_data;
	/// The reading offset
	mutable std::size_t m_readPos;
};

NEPHILIM_NS_END

#endif // NephilimPacket_h__
