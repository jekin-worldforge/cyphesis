// Cyphesis Online RPG Server and AI Engine
// Copyright (C) 2004 Alistair Riddoch
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA

// $Id$

#ifndef SERVER_PEER_H
#define SERVER_PEER_H

#include "Account.h"

#include "common/Router.h"

#include <Atlas/Objects/Operation.h>
#include <Atlas/Objects/Anonymous.h>
#include <Atlas/Objects/RootEntity.h>

using Atlas::Objects::Root;
using Atlas::Objects::Entity::RootEntity;

class CommClient;
class ServerRouting;

enum PeerAuthState { PEER_INIT, PEER_AUTHENTICATING, PEER_AUTHENTICATED };

/// \brief Class represening connections from another server that is peered to
/// to this one
///
/// This is the main point of dispatch for any operation from the peer.
class Peer : public Router {
  protected:
    /// \brief Account identifier returned after successful login
    std::string m_accountId;
    /// \brief Account type returned after login
    std::string m_accountType;
    /// The authentication state of the peer object
    PeerAuthState m_state;
    
  public:
    /// The client socket used to connect to the peer.
    CommClient & m_commClient;
    /// The server routing object of this server.
    ServerRouting & m_server;

    Peer(CommClient & client, ServerRouting & svr,
         const std::string & addr, const std::string & id);
    virtual ~Peer();

    void setAuthState(PeerAuthState state);
    PeerAuthState getAuthState();

    virtual void operation(const Operation &, OpVector &);
    
    int verifyCredentials(const Account & account,
                                  const Root & creds) const;
    void LoginOperation(const Operation & op, OpVector & res);
};

#endif // SERVER_PEER_H
