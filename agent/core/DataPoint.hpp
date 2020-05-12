/* DataPoint are not subclass of base because they are transient */

#pragma once

#include <string>
#include <unordered_map>
#include <iterator>
#include <any>
#include <iostream>

#include "rapidjson/prettywriter.h" // for stringify JSON
#include <cstdio>

using namespace rapidjson;

using namespace std;

namespace twogreencows_core
{
    class DataPoint
    {
        public:
        enum DataPointType {
            GROWBOX_CREATE,
            GROWBOX_START,
            GROWBOX_STOP,
            TIMELINE_ADD,
            TIMELINE_MODIFY,
            TRIGGER_DATAEVENT,
            ERROR
        };

        enum DataPointKey {
            SUBJECT_NAME,
            OBJECT_UUID,
            OBJECT_NAME,
            VERB,
            SENSOR_KIND,
            SENSOR_BRAND,
            SENSOR_MODEL,
            SENSOR_VALUE,
            SENSOR_UNIT,
            SENSOR_ATTACHMENT,
            ERROR_CODE,
            ERROR_TEXT
        };
        

    template <typename Writer>
    void Serialize(Writer& writer) const   {
//        // This base class just write out name-value pairs, without wrapping within an object.
        writer.String("type");
        switch (this->Type) {
            case GROWBOX_CREATE: writer.String("growbox_create"); break;
            case GROWBOX_START: writer.String("growbox_start"); break;
            case GROWBOX_STOP: writer.String("growbox_stop"); break;
            case TIMELINE_ADD: writer.String("timeline_add"); break;
            case TIMELINE_MODIFY: writer.String("timeline_modify"); break;
            case TRIGGER_DATAEVENT: writer.String("trigger_dataevent"); break;
            case ERROR: writer.String("error"); break;
            default: writer.String("unnknown"); break;

        }
        writer.String("identifier");
        writer.String(ObjectIdentifier);
        writer.String("timestamp");
        writer.String(this->DeviceDateString );
        if (this->Attributes.find("subject_name") != this->Attributes.end()) {
            writer.String("name");
            writer.String(std::any_cast<string>(this->Attributes.at("subject_name")));
        }
        writer.String("data");
        writer.StartObject();
         try {
        if (this->Attributes.find("sensor_kind") != this->Attributes.end()) {
            writer.String("sensor_kind");
            writer.String(std::any_cast<string>(this->Attributes.at("sensor_kind")));
        }
         } catch (std::exception& e) {
            std::cout << "Exception: " << e.what() << std::endl;
        } 

        if (this->Attributes.find("sensor_brand") != this->Attributes.end()) {
            writer.String("sensor_brand");
            writer.String(std::any_cast<string>(this->Attributes.at("sensor_brand")));
        }
        if (this->Attributes.find("sensor_model") != this->Attributes.end()) {
            writer.String("sensor_model");
            writer.String(std::any_cast<string>(this->Attributes.at("sensor_model")));
        }
        if (this->Attributes.find("sensor_unit") != this->Attributes.end()) {
            writer.String("sensor_unit");
            writer.String(std::any_cast<string>(this->Attributes.at("sensor_unit")));
        }
        if (this->Attributes.find("sensor_attachment") != this->Attributes.end()) {
            writer.String("sensor_attachement");
            writer.String(std::any_cast<string>(this->Attributes.at("sensor_attachment")));
        }
        writer.EndObject();
    }

        private:
            string ObjectIdentifier;
            DataPoint::DataPointType Type;
            time_t DeviceDate;
            string DeviceDateString;
            unordered_map<string, any> Attributes;

        public:
            DataPoint(DataPoint::DataPointType type, string ObjectIdentifier, unordered_map<DataPoint::DataPointKey, std::any> data);
            string  GetDeviceDatePrefix();
    };
}
