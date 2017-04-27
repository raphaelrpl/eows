#ifndef __EOWS_OGC_OWS_DATA_TYPES_HPP__
#define __EOWS_OGC_OWS_DATA_TYPES_HPP__

#include <string>

namespace eows
{
  namespace ogc
  {
    namespace ows {
      struct contact_info_t
      {
        std::string address_type;
        std::string address;
        std::string city;
        std::string state;
        std::string code;
        std::string country;
      };

      struct contact_t
      {
        std::string name;
        std::string position;
        contact_info_t info;
      };

      struct provider_t
      {
        std::string name;
        std::string site;
        contact_t contact;
      };
    }
  }
}

#endif // __EOWS_OGC_OWS_DATA_TYPES_HPP__
