#ifndef __EOWS_OGC_WCS_CORE_OPERATIONS_HPP__
#define __EOWS_OGC_WCS_CORE_OPERATIONS_HPP__

#include <string>

namespace eows
{
  namespace ogc
  {
    namespace wcs
    {
      namespace core
      {
        class operation
        {
          public:
            operation()
            {

            }

            virtual ~operation()
            {
            }

            virtual const char* content_type() const = 0;

            /**
             * @brief It retrieves the raw operation output
             * @returns string representation of operation
             */
            virtual const std::string to_string() const = 0;
        };
      }
    }
  }
}

#endif // __EOWS_OGC_WCS_CORE_OPERATIONS_HPP__
