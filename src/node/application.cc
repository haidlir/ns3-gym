/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2006 Georgia Tech Research Corporation
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Author: George F. Riley<riley@ece.gatech.edu>
 */

// Implementation for ns3 Application base class.
// George F. Riley, Georgia Tech, Fall 2006

#include "application.h"
#include "ns3/node.h"
#include "ns3/nstime.h"
#include "ns3/random-variable.h"
#include "ns3/simulator.h"

using namespace std;

namespace ns3 {

// Application Methods

// \brief Application Constructor
Application::Application(Ptr<Node> n) 
    : m_node (n),
      m_startVar(0), m_stopVar(0),
      m_start(false), m_stop(false)
{
  m_node->AddApplication (this);
}
  
// \brief Application Destructor
Application::~Application()
{}

void
Application::DoDispose (void)
{
  m_node = 0;
  if (m_start) 
    {
      Simulator::Cancel(m_startEvent);
      m_start = false;
    }
  if (m_stop) 
    {
      Simulator::Cancel(m_stopEvent);
      m_stop = false;
    }
  delete m_startVar;
  m_startVar = 0;
  delete m_stopVar;
  m_stopVar = 0;
}  
   
// \brief Specify application start time
// The virtual method STartApp will be called at the time
// specified by startTime.
// \param Time to start application (absolute time, from start of simulation)
void Application::Start(const Time& startTime)
{
  delete m_startVar;
  m_startVar = new ConstantVariable(startTime.GetSeconds());
  ScheduleStart();
}

void Application::Start(const RandomVariable& startVar)
{ // Start at random time
  delete m_startVar;
  m_startVar = startVar.Copy();
  ScheduleStart();
}

   
// \brief Specify application stop time
// The virtual method StopApp will be called at the time
// specified by stopTime.
// \param Time to stop application (absolute time, from start of simulation)
void Application::Stop(const Time& stopTime)
{
  delete m_stopVar;
  m_stopVar = new ConstantVariable(stopTime.GetSeconds());
  ScheduleStop();
}

void Application::Stop(const RandomVariable& stopVar)
{ // Stop at random time
  delete m_stopVar;
  m_stopVar = stopVar.Copy();
  ScheduleStop();
}
  
Ptr<Node> Application::GetNode() const
{
  return m_node;
}

// Protected methods
// StartApp and StopApp will likely be overridden by application subclasses
void Application::StartApplication()
{ // Provide null functionality in case subclass is not interested
}

void Application::StopApplication()
{ // Provide null functionality in case subclass is not interested
}


// Private helpers
void Application::ScheduleStart()
{
  m_startEvent = Simulator::Schedule(Seconds(m_startVar->GetValue()) -
                                     Simulator::Now(),
                                     &Application::StartApplication, this);
  m_start = true;
}

void Application::ScheduleStop()
{
  m_stopEvent = Simulator::Schedule(Seconds(m_stopVar->GetValue()) -
                                     Simulator::Now(),
                                     &Application::StopApplication, this);
  m_stop = true;
}

} //namespace ns3
      
  
