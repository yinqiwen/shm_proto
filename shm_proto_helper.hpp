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

#ifndef SHM_PROTO_HELPER_HPP_
#define SHM_PROTO_HELPER_HPP_
#include <stdint.h>
#include <string>
#include <vector>
#include <google/protobuf/message.h>
#include <google/protobuf/map.h>
#include "mmdata.hpp"

namespace shm_proto
{
    template<typename Left, typename Right>
    void ReadFrom(Left& left, const Right& right)
    {
        left.ReadFrom(right);
    }
    template<>
    inline void ReadFrom(float& left, const float& right)
    {
        left = right;
    }
    template<>
    inline void ReadFrom(double& left, const double& right)
    {
        left = right;
    }
    template<>
    inline void ReadFrom(int64_t& left, const int64_t& right)
    {
        left = right;
    }
    template<>
    inline void ReadFrom(uint64_t& left, const uint64_t& right)
    {
        left = right;
    }
    template<>
    inline void ReadFrom(uint32_t& left, const uint32_t& right)
    {
        left = right;
    }
    template<>
    inline void ReadFrom(int32_t& left, const int32_t& right)
    {
        left = right;
    }
    template<>
    inline void ReadFrom(int16_t& left, const uint16_t& right)
    {
        left = right;
    }
    template<>
    inline void ReadFrom(uint16_t& left, const int16_t& right)
    {
        left = right;
    }
    template<>
    inline void ReadFrom(int8_t& left, const int8_t& right)
    {
        left = right;
    }
    template<>
    inline void ReadFrom(uint8_t& left, const uint8_t& right)
    {
        left = right;
    }
    template<>
    inline void ReadFrom(bool& left, const bool& right)
    {
        left = right;
    }

    template<>
    inline void ReadFrom(mmdata::SHMString& left, const std::string& right)
    {
        left.assign(right.c_str(), right.size());
    }

    template<typename T, typename R>
    inline void ReadFrom(typename boost::container::vector<T, mmdata::Allocator<T> >& left,
            const ::google::protobuf::RepeatedField<R>& right)
    {
        for (size_t i = 0; i < right.size(); i++)
        {
            mmdata::SHMConstructor<T> value(left.get_allocator());
            ReadFrom(value.value, right[i]);
            left.push_back(value.value);
        }
    }
    template<typename T, typename R>
    inline void ReadFrom(typename boost::container::vector<T, mmdata::Allocator<T> >& left,
            const ::google::protobuf::RepeatedPtrField<R>& right)
    {
        for (size_t i = 0; i < right.size(); i++)
        {
            mmdata::SHMConstructor<T> value(left.get_allocator());
            ReadFrom(value.value, right[i]);
            left.push_back(value.value);
        }
    }
    template<typename LK, typename LV, typename RK, typename RV>
    inline void ReadFrom(typename boost::container::map<LK, LV,std::less<LK>,mmdata::Allocator<std::pair<const LK, LV> > >& left, const ::google::protobuf::Map<RK, RV>& right)
    {
        typedef typename boost::container::map<LK, LV,std::less<LK>,mmdata::Allocator<std::pair<const LK, LV> > > LocalMapType;
        typename ::google::protobuf::Map<RK, RV>::const_iterator cit = right.begin();
        while (cit != right.end())
        {
            typename mmdata::SHMConstructor<LK> key(left.get_allocator());
            typename mmdata::SHMConstructor<LV> value(left.get_allocator());
            ReadFrom(key.value, cit->first);
            ReadFrom(value.value, cit->second);
            left.insert(typename LocalMapType::value_type(key.value, value.value));
            cit++;
        }
    }

    template<typename Left, typename Right>
    void WriteTo(const Left& left, Right& right)
    {
        left.WriteTo(right);
    }
    template<>
    inline void WriteTo(const float& left, float& right)
    {
        right = left;
    }
    template<>
    inline void WriteTo(const double& left, double& right)
    {
        right = left;
    }
    template<>
    inline void WriteTo(const int64_t& left, int64_t& right)
    {
        right = left;
    }
    template<>
    inline void WriteTo(const uint64_t& left, uint64_t& right)
    {
        right = left;
    }
    template<>
    inline void WriteTo(const uint32_t& left, uint32_t& right)
    {
        right = left;
    }
    template<>
    inline void WriteTo(const int32_t& left, int32_t& right)
    {
        right = left;
    }
    template<>
    inline void WriteTo(const int16_t& left, uint16_t& right)
    {
        right = left;
    }
    template<>
    inline void WriteTo(const uint16_t& left, int16_t& right)
    {
        right = left;
    }
    template<>
    inline void WriteTo(const int8_t& left, int8_t& right)
    {
        right = left;
    }
    template<>
    inline void WriteTo(const uint8_t& left, uint8_t& right)
    {
        right = left;
    }
    template<>
    inline void WriteTo(const bool& left, bool& right)
    {
        right = left;
    }

    template<>
    inline void WriteTo(const mmdata::SHMString& left, std::string& right)
    {
        right.assign(left.c_str(), left.size());
    }

    template<typename T, typename R>
    inline void WriteTo(const typename boost::container::vector<T, mmdata::Allocator<T> >& left,
            ::google::protobuf::RepeatedField<R>& right)
    {
        for (size_t i = 0; i < left.size(); i++)
        {
            R* r = right.Add();
            WriteTo(left[i], *r);
        }
    }
    template<typename T, typename R>
    inline void WriteTo(const typename boost::container::vector<T, mmdata::Allocator<T> >& left,
            ::google::protobuf::RepeatedPtrField<R>& right)
    {
        for (size_t i = 0; i < left.size(); i++)
        {
            R* r = right.Add();
            WriteTo(left[i], *r);
        }
    }
    template<typename LK, typename LV, typename RK, typename RV>
    inline void WriteTo(const typename boost::container::map<LK, LV,std::less<LK>,mmdata::Allocator<std::pair<const LK, LV> > >& left, ::google::protobuf::Map<RK, RV>& right)
    {
        typedef typename ::google::protobuf::Map<RK, RV> LocalMapType;
        typename boost::container::map<LK, LV,std::less<LK>,mmdata::Allocator<std::pair<const LK, LV> > >::const_iterator cit = left.begin();
        while (cit != left.end())
        {
            const LK& lkey = cit->first;
            const LV& lvalue = cit->second;
            RK rkey;
            RV rvalue;
            WriteTo(lkey, rkey);
            WriteTo(lvalue, rvalue);
            right.insert(typename LocalMapType::value_type(rkey, rvalue));
            cit++;
        }
    }
}

#endif /* SHM_PROTO_HELPER_HPP_ */
