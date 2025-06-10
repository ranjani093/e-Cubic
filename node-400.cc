// ****************************************************************************************
//Self Organizing Networking Group (SONG) @ Kongu Engineering College (KEC) 
//kecsong2013@gmail.com		 kecsong@gmail.com
//*****************************************************************************************
// Program V : Creating TCP ON-OFF Application for VANET scenario (Mobility Model Included)

#include "ns3/ocb-wifi-mac.h"
#include "ns3/wifi-80211p-helper.h"
#include "ns3/wave-mac-helper.h"
#include "ns3/aodv-module.h"
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/mobility-module.h"
#include "ns3/ns2-mobility-helper.h"
#include "ns3/point-to-point-module.h"
#include "ns3/wifi-module.h" 
#include "ns3/yans-wifi-helper.h"
#include "ns3/internet-stack-helper.h"
#include "ns3/ipv4-address-helper.h"
#include "ns3/ipv4-interface-container.h"
#include "ns3/network-module.h"
#include "ns3/applications-module.h"
#include "ns3/config-store-module.h"
#include "ns3/internet-module.h"
#include "ns3/tcp-header.h"
#include "ns3/flow-monitor-helper.h"
#include "ns3/netanim-module.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>


using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("v2v");

int 
main (int argc, char *argv[])
{
  bool verbose = true;
 // bool tracing = true;
  bool enableFlowMonitor = true;
  std::string phyMode ("OfdmRate27MbpsBW10MHz");
  std::string traceFile = "scratch/node-400.ns_movements";
  uint32_t maxBytes = 0;

  CommandLine cmd;

  cmd.AddValue ("phyMode", "Wifi Phy mode", phyMode);
  cmd.AddValue ("EnableMonitor", "Enable Flow Monitor", enableFlowMonitor);
  cmd.AddValue ("maxBytes",
                "Total number of bytes for application to send", maxBytes);
  cmd.AddValue ("verbose", "turn on all WifiNetDevice log components", verbose);
 // cmd.AddValue ("tracing", "Enable pcap tracing", tracing);
  cmd.Parse (argc,argv);
 Config::SetDefault ("ns3::TcpL4Protocol::SocketType", StringValue ("ns3::TcpCubic"));



 // if (verbose)
 //   {
  //    LogComponentEnable ("TcpCubic", LOG_LEVEL_INFO);
//    }
  

  Ns2MobilityHelper ns2mobility = Ns2MobilityHelper (traceFile);
  NodeContainer vanetnodes;
  vanetnodes.Create (400);
  ns2mobility.Install ();


  // The below set of helpers will help us to put together the wifi NICs we want
  //YansWifiPhyHelper wifiPhy =  YansWifiPhyHelper::Default ();
  YansWifiPhyHelper wifiPhy;
  wifiPhy.SetErrorRateModel ("ns3::NistErrorRateModel");
  //YansWifiChannelHelper wifiChannel = YansWifiChannelHelper::Default ();
  
  //wifiPhy.SetPcapDataLinkType (WifiPhyHelper::DLT_IEEE802_11_RADIO);
  YansWifiChannelHelper wifiChannel;
  wifiChannel.SetPropagationDelay ("ns3::ConstantSpeedPropagationDelayModel");
  wifiChannel.AddPropagationLoss ("ns3::NakagamiPropagationLossModel");

  //wifiPhy.SetChannel (wifiChannel.Create ());
  
  
  Ptr<YansWifiChannel> channel = wifiChannel.Create ();
  wifiPhy.SetChannel (channel);
  // ns-3 supports generate a pcap trace
  wifiPhy.SetPcapDataLinkType (YansWifiPhyHelper::DLT_IEEE802_11);
  NqosWaveMacHelper wifi80211pMac = NqosWaveMacHelper::Default ();
  Wifi80211pHelper wifi80211p = Wifi80211pHelper::Default ();
   wifi80211p.SetRemoteStationManager ("ns3::ConstantRateWifiManager",
                                      "DataMode",StringValue (phyMode),
                                      "ControlMode",StringValue (phyMode));
  NetDeviceContainer vanetdevices = wifi80211p.Install (wifiPhy, wifi80211pMac, vanetnodes);


 // if (verbose)
    //{
    // wifi80211p.EnableLogComponents ();      // Turn on all Wifi 802.11p logging
    //}

  // Enable AODV
  AodvHelper aodv;
  InternetStackHelper stack;
  stack.SetRoutingHelper (aodv);
  stack.Install (vanetnodes);
     

  Ipv4AddressHelper address;
  address.SetBase ("10.1.0.0", "255.255.0.0");
  Ipv4InterfaceContainer interfaces = address.Assign (vanetdevices);

 // Choosing Source and Destination Node for tcp traffic Flow 

  NS_LOG_INFO ("Create Applications.");
 // uint16_t port = 9;   
 
// 8 traffic
//1

  Ptr<Node> appSource1 = NodeList::GetNode (1);
  Ptr<Node> appSink1 = NodeList::GetNode (84);
  Ipv4Address remoteAddr1 = appSink1->GetObject<Ipv4> ()->GetAddress(1,0).GetLocal ();

  BulkSendHelper source1 ("ns3::TcpSocketFactory", InetSocketAddress (remoteAddr1, 1));
  source1.SetAttribute ("MaxBytes", UintegerValue (maxBytes));
  ApplicationContainer apps1 = source1.Install (vanetnodes.Get  (1));
  apps1.Start (Seconds (1));
  apps1.Stop (Seconds (500));
  PacketSinkHelper sink1 ("ns3::TcpSocketFactory", InetSocketAddress(Ipv4Address::GetAny (), 1));
  apps1 = sink1.Install (vanetnodes.Get (84));
  
//2
  Ptr<Node> appSource2 = NodeList::GetNode (12);
  Ptr<Node> appSink2 = NodeList::GetNode (45);
  Ipv4Address remoteAddr2 = appSink2->GetObject<Ipv4> ()->GetAddress(1,0).GetLocal ();
  BulkSendHelper source2 ("ns3::TcpSocketFactory", InetSocketAddress (remoteAddr2, 1));
  source2.SetAttribute ("MaxBytes", UintegerValue (maxBytes));
  ApplicationContainer apps2 = source2.Install (vanetnodes.Get  (12));
  apps2.Start (Seconds (1));
  apps2.Stop (Seconds (500));
  PacketSinkHelper sink2 ("ns3::TcpSocketFactory", InetSocketAddress(Ipv4Address::GetAny (), 1));
  apps2 = sink2.Install (vanetnodes.Get (45));


 //3
  Ptr<Node> appSource3 = NodeList::GetNode (32);
  Ptr<Node> appSink3 = NodeList::GetNode (57);
  Ipv4Address remoteAddr3 = appSink3->GetObject<Ipv4> ()->GetAddress(1,0).GetLocal ();
  BulkSendHelper source3 ("ns3::TcpSocketFactory", InetSocketAddress (remoteAddr3, 1));
  source3.SetAttribute ("MaxBytes", UintegerValue (maxBytes));
  ApplicationContainer apps3 = source3.Install (vanetnodes.Get  (32));
  apps3.Start (Seconds (1));
  apps3.Stop (Seconds (500));
  PacketSinkHelper sink3 ("ns3::TcpSocketFactory", InetSocketAddress(Ipv4Address::GetAny (), 1));
  apps3 = sink3.Install (vanetnodes.Get (57));
  
//4
  Ptr<Node> appSource4 = NodeList::GetNode (82);
  Ptr<Node> appSink4 = NodeList::GetNode (71);
  Ipv4Address remoteAddr4 = appSink4->GetObject<Ipv4> ()->GetAddress(1,0).GetLocal ();
  BulkSendHelper source4 ("ns3::TcpSocketFactory", InetSocketAddress (remoteAddr4, 1));
  source4.SetAttribute ("MaxBytes", UintegerValue (maxBytes));
  ApplicationContainer apps4 = source4.Install (vanetnodes.Get  (82));
  apps4.Start (Seconds (1));
  apps4.Stop (Seconds (500));
  PacketSinkHelper sink4 ("ns3::TcpSocketFactory", InetSocketAddress(Ipv4Address::GetAny (), 1));
  apps4 = sink4.Install (vanetnodes.Get (71));
  
  
//   AsciiTraceHelper ascii;
//   wifiPhy.EnableAsciiAll (ascii.CreateFileStream("vanet-base.tr"));

  
//  AnimationInterface anim("node-400.xml");
// Installing Flow Monitor
  FlowMonitorHelper flowmonHelper;
 if (enableFlowMonitor)
    {
      flowmonHelper.InstallAll ();
    }
  Simulator::Stop (Seconds (500));
  Simulator::Run ();

// Enabling the XML Flow Monitor
 if (enableFlowMonitor)
    {
      flowmonHelper.SerializeToXmlFile ("node-400.flowmon", false, false);
    }
  Simulator::Destroy ();
  return 0;
}
