// network.cc 
//	Routines to simulate a network interface, using UNIX sockets
//	to deliver packets between multiple invocations of nachos.
//
//  DO NOT CHANGE -- part of the machine emulation
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.
// Modifications:
//
//   Date: July, 1995
//   Author: K. Salem
//   Description:  added packet delays so that packet delivery is
//                  not guaranteed to be ordered
//

#include "copyright.h"
#include "system.h"

// Dummy functions because C++ can't call member functions indirectly 
static void NetworkReadPoll(_int arg)
{ Network *net = (Network *)arg; net->CheckPktAvail(); }
static void NetworkSendDone(_int arg)
{ Network *net = (Network *)arg; net->SendDone(); }

// Initialize the network emulation
//   addr is used to generate the socket name
//   reliability says whether we drop packets to emulate unreliable links
//   readAvail, writeDone, callArg -- analogous to console
Network::Network(NetworkAddress addr, double reliability, double orderability,
	VoidFunctionPtr readAvail, VoidFunctionPtr writeDone, _int callArg)
{
    ident = addr;
    if (reliability < 0) chanceToWork = 0;
    else if (reliability > 1) chanceToWork = 1;
    else chanceToWork = reliability;

    if (orderability < 0) chanceToNotDelay = 0;
    else if (orderability > 1) chanceToNotDelay = 1;
    else chanceToNotDelay = orderability;

    // set up the stuff to emulate asynchronous interrupts
    writeHandler = writeDone;
    readHandler = readAvail;
    handlerArg = callArg;
    sendBusy = FALSE;
    inHdr.length = 0;
    delayBufFull = FALSE;
    
    sock = OpenSocket();
    sprintf(sockName, "SOCKET_%d", (int)addr);
    AssignNameToSocket(sockName, sock);		 // Bind socket to a filename 
						 // in the current directory.

    // start polling for incoming packets
    interrupt->Schedule(NetworkReadPoll, (_int)this, NetworkTime, NetworkRecvInt);
}

Network::~Network()
{
    CloseSocket(sock);
    DeAssignNameToSocket(sockName);
}

// if a packet is already buffered, we simply delay reading 
// the incoming packet.  In real life, the incoming 
// packet might be dropped if we can't read it in time.
void
Network::CheckPktAvail()
{
    // schedule the next time to poll for a packet
    interrupt->Schedule(NetworkReadPoll, (_int)this, NetworkTime, NetworkRecvInt);

    if (inHdr.length != 0) 	// do nothing if packet is already buffered
	return;		
    if (!PollSocket(sock)) 	// do nothing if no packet to be read
	return;

    // otherwise, read packet in
    char *buffer = new char[MaxWireSize];
    ReadFromSocket(sock, buffer, MaxWireSize);

    // divide packet into header and data
    inHdr = *(PacketHeader *)buffer;
    ASSERT((inHdr.to == ident) && (inHdr.length <= MaxPacketSize));
    bcopy(buffer + sizeof(PacketHeader), inbox, inHdr.length);
    delete []buffer ;

    DEBUG('n', "Network received packet from %d, length %d...\n",
	  				(int) inHdr.from, inHdr.length);
    stats->numPacketsRecvd++;

    // tell post office that the packet has arrived
    (*readHandler)(handlerArg);	
}

// notify user that another packet can be sent
void
Network::SendDone()
{
    sendBusy = FALSE;
    stats->numPacketsSent++;
    (*writeHandler)(handlerArg);
}

// send a packet by concatenating hdr and data, and schedule
// an interrupt to tell the user when the next packet can be sent 
//
// Note we always pad out a packet to MaxWireSize before putting it into
// the socket, because it's simpler at the receive end.
void
Network::Send(PacketHeader hdr, char* data)
{
    char toName[32];

    ASSERT((sendBusy == FALSE) && (hdr.length > 0) 
		&& (hdr.length <= MaxPacketSize) && (hdr.from == ident));
    DEBUG('n', "Sending to addr %d, %d bytes... ", hdr.to, hdr.length);

    interrupt->Schedule(NetworkSendDone, (_int)this, NetworkTime, NetworkSendInt);

    if (Random() % 100 >= chanceToWork * 100) { // emulate a lost packet
	DEBUG('n', "oops, lost it!\n");
	return;
    }
    if (Random() % 100 >= chanceToNotDelay * 100) { // emulate delay
      // to delay a packet, we simply save it in a buffer
      // it remains there until another packet is delayed, at which
      //  point we send it out
      if (delayBufFull == TRUE) {
	SendToSocket(sock, delayBuf, MaxWireSize, delayToName);
      }
      sprintf(delayToName, "SOCKET_%d", (int)hdr.to);
      *(PacketHeader *)delayBuf = hdr;
      bcopy(data, delayBuf + sizeof(PacketHeader), hdr.length);
      delayBufFull = TRUE;
      return;
    }

    // packet is neither lost nor delayed - send it now

    sprintf(toName, "SOCKET_%d", (int)hdr.to);
    // concatenate hdr and data into a single buffer, and send it out
    char *buffer = new char[MaxWireSize];
    *(PacketHeader *)buffer = hdr;
    bcopy(data, buffer + sizeof(PacketHeader), hdr.length);
    SendToSocket(sock, buffer, MaxWireSize, toName);
    delete []buffer;
}

// read a packet, if one is buffered
PacketHeader
Network::Receive(char* data)
{
    PacketHeader hdr = inHdr;

    inHdr.length = 0;
    if (hdr.length != 0)
    	bcopy(inbox, data, hdr.length);
    return hdr;
}
