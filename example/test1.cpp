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
#include "hello.pb.shm.hpp"
#include <iostream>
#include <google/protobuf/util/json_util.h>

using namespace helloworld;
using namespace shm_proto;
using namespace mmdata;

int main()
{
    //build a PB3 instance
    HelloTestData data;
    data.set_sv("hello,world");
    for (size_t i = 0; i < 3; i++)
    {
        HelloTestItem* item = data.add_items();
        item->set_ruleid(101 + i);
        item->set_testid(9876 + i);
    }
    for (size_t i = 0; i < 5; i++)
    {
        data.add_k(i + 1000);
    }
    data.mutable_tk()->set_ruleid(567);
    data.mutable_tk()->set_testid(234);
    (*data.mutable_tm())[1] = "asdas";
    (*data.mutable_tm())[2] = "asqw";

    std::cout << "####"<< data.GetTypeName()  << std::endl;

    char* buf = (char*)malloc(1024*1024);
    MMData mdata;
    mdata.OpenWrite(buf, 1024*1024);

    //convert from PB to shm instance
    const ShmProtoFunctors* funcs = ShmProtoFactory::GetInstance().GetShmFunctors(data.GetTypeName());
    void* shm = funcs->Create(mdata);
    funcs->Read(shm, &data);
    ShmHelloTestData* shmdata = (ShmHelloTestData*)shm;
    std::cout << "####"<< *shmdata  << std::endl;

    //get field
    const mmdata::SHMString* sv = (const mmdata::SHMString*)funcs->GetField(shm, "sv");
    std::cout << "#### sv = "<< *sv  << std::endl;

    //convert from shm to pb instance
    HelloTestData another;
    funcs->Write(shm, &another);
    std::string json;
    google::protobuf::util::JsonPrintOptions opt;
    opt.add_whitespace = true;
    opt.always_print_primitive_fields = true;
    opt.preserve_proto_field_names = true;
    google::protobuf::util::MessageToJsonString(another, &json, opt);
    std::cout << "####"<< json  << std::endl;

    funcs->Destroy(mdata, shm);
    free(buf);
    return 0;
}

