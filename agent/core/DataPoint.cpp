
#include <string>
#include <iostream>

#include <iterator>
#include "DataPoint.hpp"


#include "rapidjson/prettywriter.h" // for stringify JSON

using namespace std;

namespace twogreencows_core
{

    DataPoint::DataPoint(DataPoint::DataPointType type, string ObjectIdentifier, unordered_map<DataPoint::DataPointKey, std::any> data)    {        
        this->ObjectIdentifier = ObjectIdentifier;
        this->Type = type;
        this->DeviceDate = time(NULL);
        char buf[sizeof "2011-10-08T07:07:09Z"];
        strftime(buf, sizeof buf, "%FT%TZ", gmtime(&(this->DeviceDate)));
        this->DeviceDateString = string(buf);

        unordered_map<DataPoint::DataPointKey, any>::iterator it;
        for (it = data.begin(); it != data.end(); ++it) {
            DataPoint::DataPointKey key = (*it).first;
            switch (key) {
                case SUBJECT_NAME:
                     this->Attributes.insert(make_pair("subject_name" , (*it).second));
                     break;
                case OBJECT_UUID:
                     this->Attributes.insert(make_pair("object_uuid" , (*it).second));
                     break;
                case OBJECT_NAME:
                     this->Attributes.insert(make_pair("object_name" , (*it).second));
                     break;
                case VERB:
                     this->Attributes.insert(make_pair("verb" , (*it).second));
                     break;
                case SENSOR_KIND:
                     this->Attributes.insert(make_pair("sensor_kind" , (*it).second));
                    break;
                case SENSOR_BRAND:
                     this->Attributes.insert(make_pair("sensor_brand" , (*it).second));
                    break;
                case SENSOR_MODEL:
                     this->Attributes.insert(make_pair("sensor_model" , (*it).second));
                    break;
                case SENSOR_VALUE:
                     this->Attributes.insert(make_pair("sensor_value" , (*it).second));
                    break;
                case SENSOR_UNIT:
                     this->Attributes.insert(make_pair("sensor_unit" , (*it).second));
                    break;
                case SENSOR_ATTACHMENT:
                     this->Attributes.insert(make_pair("sensor_attachment" , (*it).second));
                    break;
                default:
                    break;
            }
        }
    }



    string DataPoint::GetDeviceDatePrefix()
    {
        return this->DeviceDateString.substr(0,10) ;
    }

   /* template <typename Writer>
    void DataPoint::Serialize(Writer& writer) const {
        // This base class just write out name-value pairs, without wrapping within an object.
        writer.String("type");
#if RAPIDJSON_HAS_STDSTRING
        writer.String("lala");
#else
        writer.String("lala".c_str(), static_cast<SizeType>(4); // Supplying length of string is faster.
#endif
        writer.String("identifier");
        writer.Uint(ObjectIdentifier);
    }
*/

}
