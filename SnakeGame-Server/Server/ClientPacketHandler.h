#pragma once

class Session;	

bool Process_HANDLE_INVALID_PACKET(const std::shared_ptr<Session>&, const char* const);
	
bool Process_C2S_LOGIN_PACKET(const std::shared_ptr<Session>&, const C2S_LOGIN_PACKET&);
bool Process_C2S_RESTART_PACKET(const std::shared_ptr<Session>&, const C2S_RESTART_PACKET&);
bool Process_C2S_MOVE_PACKET(const std::shared_ptr<Session>&, const C2S_MOVE_PACKET&);

using PacketHandlerFunc = bool(*)(const std::shared_ptr<Session>&, const char* const);
extern inline constinit std::array<PacketHandlerFunc, std::numeric_limits<uint16>::max() + 1> PacketHandlerFuncs{};
	
class ClientPacketHandler {
private:
	ClientPacketHandler() = delete;
	~ClientPacketHandler() = delete;
	ClientPacketHandler(const ClientPacketHandler&) = delete;
	ClientPacketHandler& operator= (const ClientPacketHandler&) = delete;
	ClientPacketHandler(ClientPacketHandler&&) noexcept = delete;
	ClientPacketHandler& operator= (ClientPacketHandler&&) noexcept = delete;

public:
	template<typename PacketType, typename HandleFunc>
	static bool HandlePacket(HandleFunc func, const std::shared_ptr<Session>& session, const char* const buffer) noexcept
	{
		return func(session, *reinterpret_cast<const PacketType*>(buffer));
	}

	static void Init() noexcept
	{
		for(auto& func : PacketHandlerFuncs) {
			func = Process_HANDLE_INVALID_PACKET;

			PacketHandlerFuncs[static_cast<uint8>(PACKET_ID::C2S_LOGIN)] = [](const std::shared_ptr<Session>& session, const char* const buffer) -> bool { return HandlePacket<C2S_LOGIN_PACKET>(Process_C2S_LOGIN_PACKET, session, buffer); };
			PacketHandlerFuncs[static_cast<uint8>(PACKET_ID::C2S_RESTART)] = [](const std::shared_ptr<Session>& session, const char* const buffer) -> bool { return HandlePacket<C2S_RESTART_PACKET>(Process_C2S_RESTART_PACKET, session, buffer); };
			PacketHandlerFuncs[static_cast<uint8>(PACKET_ID::C2S_MOVE)] = [](const std::shared_ptr<Session>& session, const char* const buffer) -> bool { return HandlePacket<C2S_MOVE_PACKET>(Process_C2S_MOVE_PACKET, session, buffer); };
		}
	}

	static inline bool HandlePacket(const std::shared_ptr<Session>& session, const char* const buffer)
	{
		const PacketHeader* const packetHeader = reinterpret_cast<const PacketHeader*>(buffer);
		return std::invoke(PacketHandlerFuncs[static_cast<uint8>(packetHeader->packetID)], session, buffer);
	}
};