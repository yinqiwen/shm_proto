# shm_proto

## Features
- Create data strucures in shared memory from proto3 description file.
- Simple reflection support on created data strucures
- Convert from/to standard C++ protobuf instances

## Dependency

- [mmdata](https://github.com/yinqiwen/mmdata)
- golang (used as proto3 compiler plugin to generate source files)


## Example

```cpp
   //build a PB3 instance
    HelloTestData data;
    .....
    
    //Create 
    char* buf = (char*)malloc(1024*1024);
    MMData mdata;
    mdata.OpenWrite(buf, 1024*1024);
    
    //convert from PB to shm instance
    const ShmProtoFunctors* funcs = ShmProtoFactory::GetInstance().GetShmFunctors(data.GetTypeName());
    void* shm = funcs->Create(mdata);
    funcs->Read(shm, &data);
    ShmHelloTestData* shmdata = (ShmHelloTestData*)shm;
    std::cout << "####"<< *shmdata  << std::endl;

    //get field by name
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
```