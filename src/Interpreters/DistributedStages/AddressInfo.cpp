/*
 * Copyright (2022) Bytedance Ltd. and/or its affiliates
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <IO/ReadHelpers.h>
#include <Interpreters/DistributedStages/AddressInfo.h>
#include <Protos/plan_node_utils.pb.h>


namespace DB
{
AddressInfo::AddressInfo(const String &host_name_, UInt16 port_, const String &user_, const String &password_)
        : host_name(host_name_), port(port_), user(user_), password(password_) {}

AddressInfo::AddressInfo(const String &host_name_, UInt16 port_, const String &user_, const String &password_, UInt16 exchange_port_)
        : host_name(host_name_), port(port_), user(user_), password(password_), exchange_port(exchange_port_) {}

AddressInfo::AddressInfo(const String &host_name_, UInt16 port_, const String &user_, const String &password_, UInt16 exchange_port_, UInt16 exchange_status_port_)
        : host_name(host_name_), port(port_), user(user_), password(password_), exchange_port(exchange_port_), exchange_status_port(exchange_status_port_) {}

void AddressInfo::serialize(WriteBuffer &buf) const
{
    // TODO: remove this when PlanSegment Protobuf is ready
    writeBinary(host_name, buf);
    writeBinary(port, buf);
    writeBinary(user, buf);
    writeBinary(password, buf);
    writeBinary(exchange_port, buf);
    writeBinary(exchange_status_port, buf);
}

void AddressInfo::deserialize(ReadBuffer &buf)
{
    // TODO: remove this when PlanSegment Protobuf is ready
    readBinary(host_name, buf);
    readBinary(port, buf);
    readBinary(user, buf);
    readBinary(password, buf);
    readBinary(exchange_port, buf);
    readBinary(exchange_status_port, buf);
}

void AddressInfo::toProto(Protos::AddressInfo & proto) const
{
    proto.set_host_name(host_name);
    proto.set_port(port);
    proto.set_user(user);
    proto.set_password(password);
    proto.set_exchange_port(exchange_port);
    proto.set_exchange_status_port(exchange_status_port);
}

void AddressInfo::fillFromProto(const Protos::AddressInfo & proto)
{
    host_name = proto.host_name();
    port = proto.port();
    user = proto.user();
    password = proto.password();
    exchange_port = proto.exchange_port();
    exchange_status_port = proto.exchange_status_port();
}

std::vector<String> extractHostPorts(const AddressInfos &addresses)
{
    std::vector<String> ret;
    for (const auto & address : addresses)
        ret.emplace_back(createHostPortString(address.getHostName(), toString(address.getPort())));
    return ret;
}

std::vector<String> extractExchangeHostPorts(const AddressInfos & addresses)
{
    std::vector<String> ret;
    for (const auto & address : addresses)
        ret.emplace_back(createHostPortString(address.getHostName(), toString(address.getExchangePort())));
    return ret;
}

std::vector<String> extractExchangeStatusHostPorts(const AddressInfos & addresses)
{
    std::vector<String> ret;
    for (const auto & address : addresses)
        ret.emplace_back(createHostPortString(address.getHostName(), toString(address.getExchangeStatusPort())));
    return ret;
}


}


