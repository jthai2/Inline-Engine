#pragma once

#include "TcpClient.hpp"

namespace inl::net::servers
{
	using namespace sockets;

	class ServerConnection
	{
	public:
		ServerConnection(TcpClient *client)
			: m_client(client)
		{
		}

		TcpClient * GetClient() { return m_client; }
		uint32_t GetID() { return m_id; }
		void SetID(uint32_t id) { m_id = id; }

	private:
		TcpClient * m_client;
		uint32_t m_id;
	};
}