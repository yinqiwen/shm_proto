/*
 *Copyright (c) 2018-2018, yinqiwen <yinqiwen@gmail.com>
 *All rights reserved.
 *
 *Redistribution and use in source and binary forms, with or without
 *modification, are permitted provided that the following conditions are met:
 *
 *  * Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *  * Neither the name of Redis nor the names of its contributors may be used
 *    to endorse or promote products derived from this software without
 *    specific prior written permission.
 *
 *THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 *AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 *IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 *ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS
 *BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 *CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 *SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 *INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 *CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 *THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef SHM_PROTO_HPP_
#define SHM_PROTO_HPP_
#include <string>
#include <google/protobuf/message.h>
#include <functional>
#include <map>
#include "mmdata.hpp"

namespace shm_proto
{
    typedef std::function<void(void*, const void*)> ProtoReader;
    typedef std::function<void(const void*, void*)> ProtoWriter;
    typedef void* InstanceConstructor(mmdata::MMData&);
    typedef void InstanceDestructor(mmdata::MMData&, void*);
    typedef std::function<const void*(const void*, const std::string& name)> FieldReader;
    struct ShmProtoFunctors
    {
            InstanceConstructor* Create;
            InstanceDestructor* Destroy;
            ProtoReader Read;
            ProtoWriter Write;
            FieldReader GetField;
            ShmProtoFunctors()
                    : Create(NULL), Destroy(NULL)
            {
            }
    };
    class ShmProtoFactory
    {
        private:
            typedef std::map<std::string, ShmProtoFunctors> ShmProtoFunctorTable;
            ShmProtoFunctorTable functors;
            ShmProtoFactory()
            {
            }
        public:
            const ShmProtoFunctors* GetShmFunctors(const std::string& name) const
            {
                ShmProtoFunctorTable::const_iterator found = functors.find(name);
                if (found != functors.end())
                {
                    return &(found->second);
                }
                return NULL;
            }
            void Registe(const std::string& name, const ShmProtoFunctors& funcs)
            {
                functors[name] = funcs;
            }
            static ShmProtoFactory& GetInstance();
    };

    template<typename T>
    class ShmProtoRegister
    {
        public:
            ShmProtoRegister()
            {
                const char* name = T::GetTypeName();
                ShmProtoFunctors funcs;
                funcs.Create = T::New;
                funcs.Destroy = T::Destroy;
                funcs.Read = T::ReadFromPB;
                funcs.Write = T::WriteToPB;
                funcs.GetField = T::GetField;
                ShmProtoFactory::GetInstance().Registe(name, funcs);
            }
    };

}

#endif /* SHM_PROTO_HPP_ */
