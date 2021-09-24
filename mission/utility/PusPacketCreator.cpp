#include <fsfw/tmtcpacket/pus/TcPacketBase.h>
#include <fsfw/tmtcpacket/pus/TmPacketBase.h>
#include <fsfw/globalfunctions/arrayprinter.h>
#include <fsfw/serviceinterface/ServiceInterfaceStream.h>
#include <mission/utility/PusPacketCreator.h>
#include <iomanip>

void PusPacketCreator::createPusPacketAndPrint() {
	// TODO: use TC packet stored here instead..

//	uint8_t packetStore[TcPacketBase::TC_PACKET_MIN_SIZE];
//	TcPacketBase packet(packetStore);
//	packet.initSpacePacketHeader(true, true, 0x73, 25);
//	packet.initializeTcPacket(0x73, 25, 0, 17, 1);
//	packet.setPacketDataLength(sizeof(PUSTcDataFieldHeader)
//			+ TcPacketBase::CRC_SIZE-1);
//	packet.setErrorControl();
//	sif::info << "PUS packet created: " << std::endl;
//	arrayprinter::print(packet.getWholeData(), packet.getFullSize());
}
